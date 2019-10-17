;
; FTOL.ASM
;
; (c) Copyright 1993, Borland Inc.
; (c) Copyright 2006, vbmacher
;

emul            ; generate emulated '87 code
.model compact
.code

  temp1   equ     [bp-2]
  temp2   equ     [bp-10]

  _twoBytes       STRUC
    BY0     db      ?
    BY1     db      ?
  _twoBytes       ENDS

  _fourWords      STRUC
    W0      dw      ?
    W1      dw      ?
    W2      dw      ?
    W3      dw      ?
  _fourWords      ENDS

        public  FIDRQQ          ; wait, esc
        public  FIARQQ          ; wait, DS:
        public  FICRQQ          ; wait, CS:
        public  FIERQQ          ; wait, ES:
        public  FISRQQ          ; wait, SS:
        public  FIWRQQ          ; nop, wait
        public  FJARQQ          ; Esc nn -> DS:nn
        public  FJCRQQ          ; Esc nn -> CS:nn
        public  FJSRQQ          ; Esc nn -> ES:nn

        FIDRQQ  equ     00000h
        FIARQQ  equ     00000h
        FICRQQ  equ     00000h
        FIERQQ  equ     00000h
        FISRQQ  equ     00000h
        FIWRQQ  equ     00000h
        FJARQQ  equ     00000h
        FJCRQQ  equ     00000h
        FJSRQQ  equ     00000h

public N_FTOL@
N_FTOL@ proc PASCAL
        push    bp
        mov     bp, sp
        sub     sp, 10
        fstcw   temp1                   ; save the control word
        fwait
        mov     al, temp1.BY1
        or      temp1.BY1, 0Ch
        fldcw   temp1
        fistp   qword ptr temp2         ; convert to 64-bit integer
        mov     temp1.BY1, al
        fldcw   temp1                   ; restore the control word
        mov     ax, temp2.W0            ; return LS 32 bits
        mov     dx, temp2.W1
        mov     sp, bp
        pop     bp
        ret
N_FTOL@ endp

end
