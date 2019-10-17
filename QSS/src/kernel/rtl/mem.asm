;
; MEM.ASM
;
; (c) Copyright 2005, vbmacher
;
; Part of RTL lib

.model compact
.386
.data
  mbuffer db 512 dup(?)
.code

;void memcpy (void *dest, void *str, U16B size);
                public  _memcpy
_memcpy         proc  
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds
                les     di,[bp + 4]
                lds     si,[bp + 8]
                mov     cx,[bp + 12]
                shr     cx,1
                cld
                rep     movsw
                jnc     MemCpyDone
                movsb

MemCpyDone:     pop     ds
                pop     di
                pop     si
                pop     bp
                ret
_memcpy         endp


;void memmove (void *dest, void *str, U16B size);
                public  _memmove
_memmove        proc  
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds
		mov di, offset mbuffer
                lds     si,[bp + 8]
                mov     cx,[bp + 12]
                shr     cx,1
                cld
                rep     movsw
                jnc     MemMovNext
                movsb
		jmp short MemMovDone
MemMovNext:	les di,[bp+4]
		mov si, offset mbuffer
                mov     cx,[bp + 12]
                shr     cx,1
                cld
                rep     movsw
                jnc     MemMovDone
		movsb
MemMovDone:     pop     ds
                pop     di
                pop     si
                pop     bp
                ret
_memmove        endp

;void memset(void far *dest, U8B value, U16B count);
                public  _memset
_memset         proc    
                push    bp
                mov     bp,sp
                push    di
                les     di,[bp + 4]
                mov     ax,[bp + 8]
                mov     ah,al
                mov     cx,[bp + 10]
                shr     cx,1
                cld
                rep     stosw
                jnc     MemSetDone
                stosb

MemSetDone:     pop     di
                pop     bp
                ret
_memset         endp

;int memcmp(const void far *s1, const void far *s2, U16B count); 
                public  _memcmp
_memcmp         proc  
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                xor     ax,ax
                les     di,[bp + 4]
                lds     si,[bp + 8]
                mov     cx,[bp + 12]
                cld
                rep     cmpsb
                setne   al
                pop     ds
                pop     di
                pop     si
                pop     bp
                ret
_memcmp         endp

end