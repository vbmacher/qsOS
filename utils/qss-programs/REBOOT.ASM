;/*
; * REBOOT.C
; *
; * (c) Copyright 2005, vbmacher
; *
; */

.model tiny
.code
.startup
    cli
    xor ax, ax
    mov es, ax
    mov bx, 472h
    mov word ptr es:[bx], 1234h
    db 0EAh
    dw 0
    dw 0FFFFh

end