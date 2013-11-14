;
; STRING.ASM
;
; (c) Copyright 2005, vbmacher
;
; Part of RTL lib

.model compact
.386
.data
  digits db '0123456789ABCDEF'

.code

;int strlen(char far *str);
                public  _strlen
_strlen         proc   
                
                push    bp
                mov     bp,sp
                push    di
                les     di,[bp + 4]
                mov     cx,0ffffh
                xor     al,al
                cld
                repne   scasb
                mov     ax,0fffeh
                sub     ax,cx
                pop     di
                pop     bp
                ret
_strlen         endp

;char far *strcpy(char far *dest, char far *src)
                public  _strcpy
_strcpy         proc    
                push    bp
                mov     bp,sp
                push    ds
                push    si
                push    di
                lds     si,[bp + 8]
                les     di,[bp + 4]
                cld

SCCopy:         lodsb
                stosb
                or      al,al
                jnz     SCCopy
                mov     ax,[bp + 4]
                mov     dx,[bp + 6]
                pop     di
                pop     si
                pop     ds
                pop     bp
                ret
_strcpy         endp

;char far *strcat(char far *dest, char far *src)
                public  _strcat
_strcat         proc    
                push    bp
                mov     bp,sp

                push    dword ptr [bp + 4]
                call    _strlen
                pop     ecx
                mov     ecx,[bp + 4]
                add     cx,ax
                push    dword ptr [bp + 8]
                push    ecx
                call    _strcpy
                add     sp,8
                mov     ax,[bp + 4]
                mov     dx,[bp + 6]
                pop     bp
                ret
_strcat         endp

;char far *strchr(char far *str, int c);
                public  _strchr
_strchr        proc     
                push    bp
                mov     bp,sp
                push    si
                les     si,[bp + 4]
                mov     ah,[bp + 8]
                jmp     SC2

SC1:            cmp     al,ah
                je      SCFound
                inc     si
SC2:            mov     al,es:[si]
                or      al,al
                jnz     SC1
                xor     ax,ax
                xor     dx,dx
                jmp     SCEnd

SCFound:        mov     ax,si
                mov     dx,es

SCEnd:          pop     si
                pop     bp
                ret
_strchr        endp

;char far *itoa(long value, char far *str, int base);
;                       4       8               12
                public  _itoa
_itoa           proc   
                enter   33,0
                push    si
                push    di
                mov     si,-33
                les     di,[bp + 8]            ; ES:DI = string begin
                mov     eax,[bp + 4]           ; hodnota
                movzx   ecx,word ptr [bp + 12] ; zaklad

                or      eax,eax                ; je eax < 0 ?
                jnl     EAXPos                 ; nie - skok
                push    word ptr '-'
                neg     eax
                jmp     LDigits

EAXPos:         push    word ptr 0

LDigits:        xor     edx,edx
                div     ecx
                mov     bx,dx
                mov     bl,[bx + Digits]
                mov     [bp + si],bl
                inc     si
                or      eax,eax
                jnz     LDigits

                cld
                pop     ax
                or      ax,ax
                jz      LInvert
                mov     [bp + si],ax
                inc     si

LInvert:        dec     si
                mov     al,[bp + si]
                stosb
                cmp     si,-33
                jne     LInvert
                mov     es:byte ptr [di],0
                mov     ax,[bp + 8]
                mov     dx,[bp + 10]
                pop     di
                pop     si
                leave
                ret
_itoa           endp

;int strcmp(const char far *s1, const char far *s2);
                public  _strcmp
_strcmp         proc    
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                cld
                xor     ax,ax
                xor     bx,bx
                les     di,[bp + 8]
                push    di
                mov     cx,0ffffh
                repne   scasb
                not     cx
                pop     di
                lds     si,[bp + 4]
                rep     cmpsb
                mov     al,[si - 1]
                mov     bl,[di - 1]
                sub     ax,bx
                pop     ds
                pop     di
                pop     si
                pop     bp
                ret
_strcmp         endp

;int stricmp(const char far *s1, const char far *s2);
                public  _stricmp
_stricmp        proc   
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                cld
                les     si,[bp + 4]
                les     di,[bp + 8]
                xor     ax,ax
                xor     bx,bx
                mov     cx,617ah

SICCompLoop:    lodsb
                mov     bl,es:[di]
                or      al,al
                jz      SICEnd
                scasb
                je      SICCompLoop
                cmp     al,ch
                jb      SICCheckCap
                cmp     al,cl
                ja      SICEnd
                sub     al,20h
                jmp     SICncComp

SICCheckCap:    cmp     bl,ch
                jb      SICEnd
                cmp     bl,cl
                ja      SICEnd
                sub     bl,20h

SICncComp:      cmp     al,bl
                je      SICCompLoop

SICEnd:         sub     ax,bx
                pop     ds
                pop     di
                pop     si
                pop     bp
                ret
_stricmp        endp

;long atol(const char far *str);
                public  _atol
_atol           proc    
                push    bp
                mov     bp,sp
                push    si

                cld
                les     si,[bp + 4]
                xor     edx,edx
                xor     eax,eax
                jmp     ALGetChar

ALConvert:      sub     al,'0'
                imul    edx,10
                add     edx,eax

ALGetChar:      lods    es:byte ptr [si]
                or      al,al
                jnz     ALConvert

                mov     ax,dx
                shr     edx,16

                pop     si
                pop     bp
                ret
_atol           endp

end