.model tiny
.data
	UART_BASE equ 3F8h
	UART_BPS equ 12 ;9600 bps

.code
.startup

start:
	call init_UART
	mov al, 'P'
	mov ah, 'T'
	call UART_send
	mov dx, UART_BASE+5
	mov cx, 500
qus_wait:
	dec cx
	cmp cx, 0
	jz qqq
	in al, dx
	test al, 1
	jz qus_wait

	mov ah, 0eh
	mov al, 'S'
	int 10h
	call UART_get
	mov ah, 0eh
	int 10h
qqq:
	mov al, 'Q'
	int 10h
	jmp @@ex

UART_get proc near
	push dx
	mov dx, UART_BASE+5
	in al, dx
	xchg al, ah
	and ax, 9F00h
	test al, 1
	jz ugc_nochar
	mov dx, UART_BASE
	in al, dx
ugc_nochar:
	pop dx
	ret
UART_get endp

UART_send proc near
	push dx
	push ax
	mov dx, UART_BASE+5
us_wait:
	in al, dx
	test al, 20h
	jz us_wait
	pop ax
	mov dx, UART_BASE
	out dx, al
	pop dx
	ret
UART_send endp

init_UART proc near
	push ax
	push dx
	mov dx, UART_BASE+3
	mov al, 80h
	out dx, al
	mov dx, UART_BASE
	mov ax, UART_BPS
	out dx, ax
	mov dx, UART_BASE+3
	mov al, 3 ;8n1
	out dx, al
	mov dx, UART_BASE+4
	mov al, 00010000b  ;set loopback
	out dx, al
	nop
	nop
	nop
	in al, dx
	and al, 00010000b
	jnz @@ppp
	mov ah, 0eh
	mov al, 'R'
	int 10h
@@ppp:
	pop dx
	pop ax
	ret
init_UART endp

@@ex:

.exit
end