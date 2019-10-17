.model tiny
.code
.startup

mov ah, 0eh
mov al, 'A'
int 10h
mov al, 'h'
int 10h
mov al, 'o'
int 10h
mov al, 'j'
int 10h
mov al, '!'
int 10h

xor ah, ah
int 16h

int 21h

end