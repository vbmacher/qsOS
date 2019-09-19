.model tiny
.486p
.data

  rom_msg db 10,13,'Rom bios date: '
  rom db 8 dup(?),'$'
.code
.startup

mov ax, ds
mov es, ax
mov ax, 0F000h
mov ds, ax
mov si, 0FFF5h
mov di, offset rom
cld
mov cx,2
rep movsd

mov ax, es
mov ds, ax
mov dx, offset rom_msg
mov ah, 09h
int 21h

mov ah, 0c0h
int 15h
jc @@ex

mov ax, es
mov ds, ax
mov si, bx
add si, 0ah
cld
mov ah, 0eh
xor bh, bh
rd_next:
lodsb
cmp al, '$'
je @@ex
int 10h
jmp short rd_next


@@ex:
.exit
end