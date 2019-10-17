;
; BOOT.ASM
;
; Zaciatok BootLoadera pre Kristoph Operating System.
;
; (c) Copyright 2001, P. Jakubco ml.
;
; Najprv sa BootSector premiestni na "bezpecne" miesto - 0x90000000, kde
; potom aj skoci. Je to koli tomu, aby nezavadzal kernelu, ktory moze byt
; hocijak velky a potom moze prekryt BootSector (Kernel je na 0x10000000 :).
; BootSector ma okrem svojho sektora aj 2 dalsie, preto ich potom
; nacita rovno za sebou a nakoniec sa pokusi nacitat subor KRISTOPH.SYS -
; jadro (kernel) OS.
;
; Relase notes
; ^^^^^^^^^^^^
; 2001 -      v0.1 - Initial relase, not functing
; 30.1.2002 - v0.6 - Bugs fixed, functing
;



;	Nastavenia compilera
MASM
.MODEL TINY
;	Data programu
.DATA
	segStack equ 5000h
	movSetup equ 9000h
	segSetup equ 8FF0h
        segKernel equ 1000h
	segBoot equ 07B0h
        movBoot equ 07C0h
.DATA
  msgError db 10,13,'Error reading...',0
;	Kod programu
.CODE

        ;Funkcia main, ktora urobi nasledujuce nacitanie KERNEL.SYS
	extrn _main:near
.STARTUP
        ;Najprv sa premiestnime na 0x90000000, aby sme v buducnosti
        ;nezavadzali kernelu.
        mov ax, movBoot
        mov ds, ax
        mov ax, movSetup
        mov es, ax
        xor si, si
        xor di, di
        mov cx, 256
        cld
        rep movsw

        ;Skok, ktory pokryje aj simulaciu ORG 100h.
        db 0EAh
        dw offset Go
        dw segSetup

	;zinicializujeme seg.registre
Go:
	;zasobnik
	mov ax, segStack
	mov ss, ax
	xor sp, sp 

	;data
	mov ax, segSetup
	mov ds, ax
	mov es, ax


	;kedze ma BootLoader 3 sektory, musime si zvysne 2 nacitat.
	mov si, 3
read:   cmp si, 0
	je Rerror
	dec si
	xor ah, ah
	xor dl, dl
	int 13h ;Reset disku
	mov bx, 300h ;Tu konci BootSector (na 8FF0:0300 !!!, nie 9000:0200)
	xor dx, dx
	xor ch, ch
	mov cl, 2
	mov al, 2
	mov ah, 2
	int 13h
	jc read
	jmp Ccall
Rerror:
        mov si, offset msgError
	xor bx, bx
	lodsb
	cmp al, 0
	je h
	mov ah, 0eh
	int 10h
h:	sti
	hlt

	;A az teraz spustame Ceckovsku funkciu, ktora loaduje Kernel.
Ccall:
	xor ax, ax
	call _main

;Zistime, ci funkcia vratila 0.
;Ak nie, halting the system.
	cmp ax, 1
	jne kernel_jmp

	cli
	hlt ;stopka

; Po vykonani tejto funkcie sa urobi skok na KERNEL.
;
kernel_jmp:

  db 0EAh      ; A tu je uz skok.
  dd 10000003h

; Tu sa uz nikdy nevratime.

END
