.model small
.data
        buff db 64 dup(?)
.code
s: jmp start_of_setup

KEYB proc
;console: ES:DI - where the text give
;	  CX - count chars
;	  BL - color
;	  BH - 00 - do not use X & Y
;	       01 - use X & Y
;	  DH - row
;	  DL - column
;	  AH - 00h

	push ax bx cx dx si di
	push cx
console:cmp ah, 00h
;	jne XXX
	jne @retX
	cmp BH, 01
	jne e
	mov ah, 02h
	xor bh, bh
	int 10h
e:	xor dx, dx
	mov si, di ;hranica
	pop cx
_getcmd:
	XOR AH, AH
	INT 16H
	CMP AL, 27 ;ESC
	JNE GETC00
	STC
	jmp @retX
GETC00:
	CMP AL, 13 ;ENTER
	JNE GETC10
	XOR AL, AL
	STOSB
	jmp @retX
GETC10:
	CMP AL, 08 ;BACKSPACE
	JNE GETC20
	;PAMAT
	cmp di, si
	jle r
	DEC DI
	mov al, 0
	stosb
	dec di
	;OBRAZOVKA
	mov al,08
	MOV AH, 0EH
	XOR BH, BH
	INT 10H
	MOV AL, 32
	INT 10H
	MOV AL, 08
	INT 10H
	dec dx
	jmp _getcmd
r:	mov di, si
	jmp _getcmd
GETC20:
	cmp dx, cx
	je k
	inc dx
	call _print
	STOSB
	JMP _getcmd

k:	mov al, 07h
	mov ah, 0eh
	xor bh, bh
	int 10h
	jmp _getcmd
_print:	push ax
	push bx
	push cx
	push dx

	mov ah, 09h
	xor bh, bh
	mov cx, 1
	int 10h
	mov ah, 03h
	int 10h
	inc dl
	mov ah, 02h
	int 10h

	pop dx
	pop cx
	pop bx
	pop ax
	ret

@retX:	pop di si dx cx bx ax
	iret
KEYB ENDP


start_of_setup:

	;load the keyboard driver on int 47h
	xor ax, ax
	mov es, ax
	mov di, 47h * 4
	mov ax, offset KEYB
	stosw
	mov ax, @code
	stosw

	;load console
        mov ax, @data 
        mov es, ax
        mov di, offset buff
        
	xor ah, ah
        mov bh, 01

	mov dh, 12
	mov dl, 40

        mov bl, 07h
        mov cx, 5
;	call KEYB
	int 47h

mov ax, 4c00h
int 21h

end s
