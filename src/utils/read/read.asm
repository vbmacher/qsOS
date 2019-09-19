.model large
.stack 100h
.data
	nBuffer db 2048 dup (0) ;fat table
.code
	xor ax, ax
	xor dl, dl ;reset
	int 13h

	mov ax, @data
	mov es, ax
	mov ds, ax
	mov bx, offset nBuffer
	xor dx, dx
	xor ch, ch
	mov cl, 3
	mov ah, 02h
	mov al, 4 ;4 sektorov
	int 13h

	mov cx, 4*512
	mov si, offset nBuffer
	cld
	mov ah, 02h
l:	lodsb
	mov dl, al
	int 21h
	loop l

mov ax, 4c00h
int 21h

end