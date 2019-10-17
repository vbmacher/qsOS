.model small
.data
	text db 10,13,'Your processor is: $'
	n86 db '8086',10,13,'$'
	n286 db '80286',10,13,'$'
	n386 db '80386',10,13,'$'
	n486 db '80486',10,13,'$'
	n686 db '6x86',10,13,'$'
	nPentium db 'Pentium',10,13,'$'
	nPentiumII db 'Pentium II',10,13,'$'
	nAMD db 'AMD',10,13,'$'
.code
s:
mov ax, @data
mov ds, ax
mov dx, offset text
mov ah, 09h
int 21h


mov ax, 1686h
int 2Fh

cmp cl, 0
jne n2Check
mov dx, offset n86
jmp Print
n2Check:
cmp cl, 02h
jne n3Check
mov dx, offset n286
jmp Print
n3Check:
cmp cl, 03h
jne n4Check
mov dx, offset n386
jmp Print
n4Check:
cmp cl, 04h
jne n5Check
mov dx, offset n486
jmp Print
n5Check:
cmp cl, 05h
jne n6Check
mov dx, offset n686
jmp Print
n6Check:
cmp cl, 06h
jne n7Check
mov dx, offset nPentium
jmp Print
n7Check:
cmp cl, 07h
jne n8Check
mov dx, offset nPentiumII
jmp Print
n8Check:
cmp cl, 0ffh
jne X
mov dx, offset nAMD


Print:
mov ah, 09h
int 21h

X:
mov ax, 4c00h
int 21h

end s