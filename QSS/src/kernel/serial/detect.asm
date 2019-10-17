;
; SERIAL.ASM
;
; (c) Copyright 2005, vbmacher
;
; Tento modul patri ku urovni 1
;
.model compact
.386
.data
	irq dw ?
	imr db ?
	ier db ?
	lcr db ?
	mcr db ?

.code
  extrn _PORT: word

;int detectUART()
;POZOR: musi byt nastaveny PORT
public _detectUART
_detectUART proc near
	push dx
	mov dx, word ptr [_PORT]
	add dx, 3
	mov al, 1Bh
	out dx, al
	nop
	nop
	nop
	in al, dx
	cmp al, 1Bh
	je @1
	xor ax, ax       ; unknown chip
	jmp short @uret
@1:	mov al, 3
	out dx, al
	nop
	nop
	nop
	in al, dx
	cmp al, 3
	je @2
	xor ax, ax       ; unknown chip
	jmp short @uret
@2:	;SCR ?
	add dx, 4
	mov al, 55h
	out dx, al
	nop
	nop
	nop
	in al, dx
	cmp al, 55h
	je @3
	mov ax, 1        ; 8250 chip
	jmp short @uret
@3:	mov al, 0AAh
	out dx, al
	nop
	nop
	nop
	in al, dx
	cmp al, 0AAh
	je @4
	mov ax, 1	 ; 8250 chip
	jmp short @uret
@4:	;FIFO ?
	sub dx, 5
	mov al, 1
	out dx, al
	nop
	nop
	nop
	in al, dx
	test al, 80h
	jnz @5
	mov ax, 2	 ; 16450/82450 chip
	jmp short @uret
@5:	test al, 40h
	jnz @6
	mov ax, 3	 ; 16550 chip
	jmp short @uret
@6:	xor al, al
	out dx, al
	mov ax, 4	 ; 16550A chip
@uret:	pop dx
	ret
_detectUART endp

;int detectIRQ()
;POZOR: PORT musi byt nastaveny pred volanim!!
public _detectIRQ
_detectIRQ proc near
	push dx
	push bx
	cli
	mov dx, word ptr [_PORT]
	add dx, 3
	in al, dx ;read LCR
	mov byte ptr [lcr], al
	and al, 7Fh
	out dx, al
	sub dx, 2 ;ier (base + 1)
	in al, dx
	mov byte ptr [ier], al
	xor al, al
	out dx, al ; zakaze vsetky prerusenia UARTu
	add dx, 3 ;mcr (base + 4)
	in al, dx
	mov byte ptr [mcr], al
	and al, 0Fh
	or al, 0Ch
	out dx, al ;povolit prerusenia UARTu
	mov dx, 21h
	in al, dx
	mov byte ptr [imr], al ;interrupt mask register (imr)
	dec dx
	mov al, 0Ah
	out dx, al ; prepare to read IRR
	mov bl, 0FCh ;maska IRQ2-7
	mov dx, word ptr [_PORT]
	inc dx
	mov al, 2  ; enable 'transmitter empty' interrupt
	out dx, al
	mov dx, 20h
	in al, dx
	and bl, al ; select risen interrupt request
	mov dx, word ptr [_PORT]
	inc dx
	xor al, al
	out dx, al ; disable 'transmitter empty' interrupt
	mov dx, 20h
	in al, dx
	neg al
	and bl, al ; select fallen interrupt request
	mov dx, word ptr [_PORT]
	inc dx
	mov al, 2
	out dx, al ; enable 'trans emp' int
	mov dx, 20h
	in al, dx
	and bl, al ; select risen interrupt request
	inc dx
	neg bl
	mov al, bl
	out dx, al  ; unmask only this interrupt(s)
	dec dx
	mov al, 0Ch
	out dx, al  ; enter the poll mode
	mov dx, 20h
	in al, dx
	xor ah, ah
	mov word ptr [irq], ax
	mov dx, word ptr [_PORT]
	add dx, 5
	in al, dx
	sub dx, 5
	in al, dx
	add dx, 2
	in al, dx
	add dx, 4
	in al, dx
	sub dx, 5
	mov al, byte ptr [ier]
	out dx, al
	add dx, 2
	mov al, byte ptr [lcr]
	out dx, al
	inc dx
	mov al, byte ptr [mcr]
	out dx, al
	mov dx, 20h
	mov ax, dx
	out dx, al ; end of interrupt mode
	inc dx
	mov al, byte ptr [imr]
	out dx, al
	sti
	mov ax, word ptr [irq]
	and ax, 80h
	cmp ax, 0
	jnz @good
	mov ax, -1
	jmp short @gret
@good:	mov ax, word ptr [irq]
	and ax, 7
@gret:	sti
	pop bx
	pop dx
	ret
_detectIRQ endp


end