;
; CTEXT.ASM
;
; (c) Copyright 2005, vbmacher
;
; Part of RTL lib
; console i/o

.model compact
.386
.data
 public _attribute
 _attribute db 7

 x db ?
 y db ?

.code

 public _gotoxy
 public _wherexy
 public _cputs
 public _cputch
 public _textattr

;void gotoxy(int x, int y)
_gotoxy proc c
                arg     @@x: word, @@y: word

                mov     dl,byte ptr @@x
                mov     dh,byte ptr @@y
                mov     ah,2
                xor     bh,bh
                int     10h
                ret
                endp

;void wherexy(int *x, int *y)
_wherexy proc c
                arg     @@x:  dword, @@y: dword

                mov     ah,3
                xor     bh,bh
                int     10h
                les     bx,@@x
                movzx   ax,dl
                mov     es:[bx],ax
                les     bx,@@y
                mov     al,dh
                mov     es:[bx],ax
                ret
                endp


;void cputs(const char *str);
_cputs proc c
                arg     @@str: dword

		mov ah, 03h ;pozicia kurzoru
		xor bh, bh
		int 10h
		mov [x], dl
		mov [y], dh
		mov cx, 1
                push    si
                les     si,@@str
                cld
		mov	bl, [_attribute]
                jmp     cPSLods
cTeletype:      cmp al, 7      ; bell
		je nor
		cmp al, 8      ; bs
		je nor
		cmp al, 10     ; lf
		je nor
		cmp al, 13     ; cr
		jne gra
nor:		mov ah, 0eh
		int 10h
		mov ah, 03h ;pozicia kurzoru
		int 10h
		mov [x], dl
		mov [y], dh
		jmp short cPSLods
gra:		mov     ah,09h
		inc byte ptr [x]
		int     10h
		mov dh, [y]  ;nastavenie polohy kurzora
		mov dl, [x]
		mov ah, 02
		int 10h
cPSLods:        lods    es:byte ptr [si]
                or      al,al
                jnz     cTeletype
                pop     si
                ret
                endp


;void cputch(int ch);
_cputch proc c
                arg     @@ch: word
		;zistit poziciu kurzoru
		mov ah, 03h
		xor bh, bh
		int 10h
		mov [x], dl
		mov [y], dh
                mov     al,byte ptr @@ch
		cmp al, 07h
		je xnor
		cmp al, 08h
		je xnor
		cmp al, 0ah
		je xnor
		cmp al, 0dh
		jne xgra
xnor:		mov ah,0eh
		int 10h
		ret
xgra:		mov ah, 09h
		mov cx, 1
                mov     bl, [_attribute]
		inc byte ptr [x]
xintr:          int     10h
		mov dh, [y] ;nastavenie polohy kurzora
		mov dl, [x]
		mov ah, 02h
		int 10h
                ret
                endp

;void textattr(unsigned char att);
_textattr proc c
                arg     @@att: byte

                mov     al,byte ptr @@att
                mov	[_attribute], al
                ret
                endp


end
