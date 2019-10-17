;
; TEXT.ASM
;
; (c) Copyright 2001, Peter Jakubco ml.
;
; Zakladne funkcie na pisanie znakov na obrazovku
;
;

.model  compact
.386p
.code
	public  _puts

;void puts(char *Str);
_puts           proc    
                push    bp
		mov     bp,sp
                push    si
                les     si,[bp + 4]
                cld
                jmp     PSLods

Teletype:       mov     ah,0eh
                mov     bx,7
                int     10h
PSLods:         lods    es:byte ptr [si]
                or      al,al
                jnz     Teletype
                pop     si
                pop     bp
                ret
_puts           endp

end
