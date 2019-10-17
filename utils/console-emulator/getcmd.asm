.MODEL SMALL
.data
	buff db 100h dup(?),'$'
.CODE
S: JMP F
GETCMD PROC NEAR
	mov si, di ;hranica
	XOR AH, AH
	INT 16H
	CMP AL, 27 ;ESC
	JNE GETC00
	STC
	RET
GETC00:
	CMP AL, 13 ;ENTER
	JNE GETC10
	XOR AL, AL
	STOSB
	RET
GETC10:
	CMP AL, 08 ;BACKSPACE
	JNE GETC20
	;PAMAT
	cmp di, si
	jl r
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
	jmp GETCMD
r:	mov di, si
	jmp GETCMD
GETC20:
	MOV AH, 0EH
	XOR BH, BH
	INT 10H
	STOSB
	JMP GETCMD
GETCMD ENDP

F:
MOV AX, @data
MOV ES, AX
MOV DI, offset buff
CALL GETCMD
mov ah, 00
mov al, 03
int 10h


mov ax, @data
mov ds, ax
mov dx, offset buff
mov ah,09h
int 21h

MOV AX, 4C00H
INT 21H


END S