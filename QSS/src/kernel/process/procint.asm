;
; PROCINT.ASM
;
; (c) Copyright 2005, vbmacher
;

;
; Kazdy proces po svojom ukonceni by mal zavolat toto prerusenie
; inak sa system zblazni...
; Zvolil som prerusenie napr. 21h (take iste ma dos hehe), aby som
; mal istotu ze funguje a nie je priradene nejakej inej veci
;

.model compact
.386
.data
        XoldINT dd ?
.code
  extrn _main_loop:near
  extrn _freePROCESS: near

_procINT proc
	call _freePROCESS
        ; navrat do OS - pouzijem fintu - skok na mainloop
        ; tak sa nic nestane ani keby som toto prerusenie
        ; volal z hocikade aj len tak bez priciny...
	pop ax ;cs
	pop ax ;ip
	pop ax ;flags
	clc
        jmp _main_loop
_procINT endp

public _procSET_INT
_procSET_INT proc
        push es
        xor     ax,ax
        mov     es,ax
        mov     bx, 21h ;store old int
	shl	bx, 2
	cli
	mov	ax, es:[bx]
        mov     word ptr [XoldINT], ax
	mov	ax, es:[bx+2]
        mov     word ptr [XoldINT+2], ax
        mov     bx, 21h ;nothing yet
        shl     bx,2
        mov     ax, offset _procINT
        mov     es:[bx], ax
        mov     ax, cs
        mov     es:[bx+2], ax
        sti
        pop     es
	ret
_procSET_INT endp


end
