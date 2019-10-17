;
; CONSOLE.ASM
;
; (c) Copyright 2001, P. Jakubco
;
; Funkcie konzoly napisane v Assembleri.
; -Zobrazenie (PUTCH,PUTS,GETCH,WHEREXY,GOTOXY)
; -Pamat (MEMCPY,MEMSET)
;

MASM
.model compact
.386p
.data
   Digits db '0123456789abcdef'
.code

   public  @gotoxy$qii, @wherexy$qnit1
   public  @puts$qnxzc,  @putch$qi
   public  @GetDigit$qi
   public  _strlen,_strcat,_strcpy,_strchr,_strcmp,_stricmp,_atol

@GetDigit$qi    proc    c
                arg     @@Digit: word

                mov     bx,offset Digits
                add     bx,@@Digit
                mov     al,[bx]

                ret
                endp

;void gotoxy(int x, int y)
@gotoxy$qii     proc    c
                arg     @@x: word, @@y: word

                mov     dl,byte ptr @@x
                mov     dh,byte ptr @@y
                mov     ah,2
                xor     bh,bh
                int     10h
                ret
                endp

;void wherexy(int *x, int *y)
@wherexy$qnit1  proc    c
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

;void puts(const char *str);
@puts$qnxzc     proc    c
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
@putch$qi       proc    c
                arg     @@ch: word

                mov     ah,0eh
                mov     al,byte ptr @@ch
                mov     bx,7
                int     10h
                ret
                endp

;int strlen(char far *str);
_strlen         proc
                
                push    bp
                mov     bp,sp
                push    di
                les     di,[bp + 4] ; [bp + 6]
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

;char *strcpy(char far *dest, char far *src)
_strcpy         proc   
                push    bp
                mov     bp,sp
                push    ds
                push    si
                push    di
                lds     si,[bp + 8] ; odkial [bp + 10]
                les     di,[bp + 4]  ; kam   [bp + 6]
                cld

SCCopy:         lodsb
                stosb
                or      al,al
                jnz     SCCopy
                mov     ax,[bp + 4] ; [bp + 6]
                mov     dx,[bp + 6] ; [bp + 8]
                pop     di
                pop     si
                pop     ds
                pop     bp
                ret
_strcpy         endp

;char *strcat(char far *dest, char far *src)
_strcat         proc
                push    bp
                mov     bp,sp

                push    dword ptr [bp + 4] ; [bp + 6]
                call    _strlen
                pop     ecx
                mov     ecx,[bp + 4] ; [bp + 6]
                add     cx,ax
                push    dword ptr [bp + 8]  ;[bp + 10]
                push    ecx
                call    _strcpy
                add     sp,8
                mov     ax,[bp + 4] ;[bp + 6]
                mov     dx,[bp + 6] ;[bp + 8]
                pop     bp
                retf
_strcat         endp

;char *strchr(char far *str, int c);
_strchr        proc    
                push    bp
                mov     bp,sp
                push    si
                les     si,[bp + 4]  ;[bp + 6]
                mov     ah,[bp + 8] ;[bp + 10]
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

;int strcmp(const char far *s1, const char far *s2);
_strcmp         proc   
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                cld
                xor     ax,ax
                xor     bx,bx
                les     di,[bp + 8] ;[bp + 10]
                push    di
                mov     cx,0ffffh
                repne   scasb
                not     cx
                pop     di
                lds     si,[bp + 4] ;[bp + 6]
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
_stricmp        proc   
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                cld
                les     si,[bp + 4] ;[bp + 6]
                les     di,[bp + 8] ;[bp + 10]        
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
_atol           proc   
                push    bp
                mov     bp,sp
                push    si

                cld
                les     si,[bp + 4] ; [bp + 6]
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
