;
; ENTRY.ASM
;
; (c) Copyright 2001, P. Jakubco ml.
;
; Zaciatok - ENtry Point - Kernelu.
;
; Kernel vykona celu instalaciu systemu.
; Tento subor je vlastne startovacim kodom. Nezalezi na velkosti.
;


;	Nastavenia compilera
MASM
.386p
.model tiny
.data
 segSystem equ 0FF0h
 segStack equ 5000h
 segSetup equ 8FF0h

.code
        ;Funkcia main, ktora urobi nasledujuce instalacie...
        extrn _main:near
.STARTUP
	header db 'KOS'

        db 0eah
        dd 0FF00108h ;Skok na 1000:0008
                     
;1000:0008:
	;Najprv nastavime segmentove registre
        mov ax, segSystem
        mov ds, ax
        mov es, ax

	;Zavolame MAIN.
	call _main

        cli
        hlt
end