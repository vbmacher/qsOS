.model small
.code
 mov ah, 03h
 xor bx, bx
 mov al, 05h
 int 16h

 mov ax, 4c00h
 int 21h
end