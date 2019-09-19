.model small
.code
start:	mov ax, 0003h
	int 10h

mov ax, 4c00h
int 21h
end start