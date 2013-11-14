;
; KEYBINT.ASM
;
; (c) Copyright 2005, vbmacher
;
; len nainstaluje prerusenie INT 16h, ktore bude LEN indikovat zmenu
; konzoly

.model compact
.386
.data
	oldINT dd ?
.code
  public _conACTIVE
  _conACTIVE db 0

  extrn _conSCANCODE
  extrn _conCHR
  extrn _haveto_draw


;interrupt - BIOS 15h
_keybINT proc
	pusha
	cmp ah, 4FH  ; 4FH - funkcia BIOSu - zmena kodu klavesy
	jne het_chod

	;v AL scan kod znaku
	cmp al, 3Bh ;F1
	jne testF2
	;system
	mov byte ptr [_conACTIVE], 0
        mov word ptr [_haveto_draw], 0 ; ze ked stlacim opakovane F1 nemusim stale vykreslovat uvodny text
        jmp short het_chod
testF2: cmp al, 3Ch ;F2
	jne test_ESC
	;chatt
	mov byte ptr [_conACTIVE], 1
        jmp short het_chod
test_ESC:
	cmp al, 1 ;ESC
	jne het_chod
	;halt
	mov byte ptr [_conACTIVE], 2
	popa
	clc
	iret
het_chod:
	popa
	stc
	jmp cs:[oldINT]
_keybINT endp

; nastavi keyb prerusenie na bios 15h
public _keybSET_INT
_keybSET_INT proc
        push es
        xor     ax,ax
        mov     es,ax
	mov	bx, 15h ;store old int
	shl	bx, 2
	cli
	mov	ax, es:[bx]
	mov	word ptr [oldINT], ax
	mov	ax, es:[bx+2]
	mov	word ptr [oldINT+2], ax
        mov     bx, 15h ;bios extended
        shl     bx,2
        mov     ax, offset _keybINT
        mov     es:[bx], ax
        mov     ax, cs
        mov     es:[bx+2], ax
        sti
        pop     es
	ret
_keybSET_INT endp

end