;
; TEXT.ASM
;
; (c) Copyright 2005, vbmacher
;
; Part of RTL lib

.model compact
.386
.code

 public _puts
 public _putch
 public _getchar

;void puts(const char *str);
_puts proc c
                arg     @@str: dword

                push    si
                les     si,@@str
                cld
                jmp     PSLods

Teletype:       mov     ah,0eh
                mov     bx,7
                int     10h
PSLods:         lods    es:byte ptr [si]
                or      al,al
                jnz     Teletype
                pop     si
                ret
                endp

;void putch(int ch);
_putch proc c
                arg     @@ch: word

                mov     ah,0eh
                mov     al,byte ptr @@ch
                mov     bx,7
                int     10h
                ret
                endp

_getchar proc
	xor ah, ah
	int 16h
	xor ah, ah
	ret
	endp

end
