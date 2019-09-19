.model tiny
.486p
.data
  msg_mem db 10,13,'Memory: '
  mem dd ?,'$'

;mem table
 tbl dw ?
     loc_mem16 dd ?
     loc_mem dd ?
     dd 6 dup (?)
     dw 2 dup (?)
     reserved dd ?
     

.code
.startup

start:

  mov ah, 88h
  int 15h     ; pocet suvislych 1kb blokov od 1MB do ax
  jc @@ex

  cmp ax, 0
  je @@ex

  mov bx, ds
  mov es, bx
  mov di, offset mem
  cld
  stosd
  mov di, offset mem
  add dword ptr es:[di], '0000'

  mov dx, offset msg_mem
  mov ah, 09h
  int 21h

@@ex:

  mov si, offset tbl
  cld
  mov ah, 0c7h
  int 15h
  jc @@@ex

  mov bx, ds
  mov es, bx
  mov di, offset mem
  mov eax, dword ptr [loc_mem]
  stosd

  mov dx, offset msg_mem
  mov ah, 09h
  int 21h

@@@ex:

.exit
end