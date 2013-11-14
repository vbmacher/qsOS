;
; START.ASM
;
; (c) Copyright 2005, vbmacher
;
; Entry point for kernel
;


masm
.model tiny
.486p
.data

  digits db '0123456789ABCDEF',0
  msg_testing db 'Testing CPU...',0
  msg_notIA32 db 10,13,'    QSS: Error, not IA32 processor',0
  msg_halting db 'Terminating...',0

  public _mem
  public _print

  _mem dd ?  ;extern do Ccka
  x db ?
  y db ?

  segKernel equ 0FF0h

.code
.startup
  extrn _mauINIT:near
  extrn _mauDESTROY:near
  extrn _cpuTEST:near
  extrn _serINIT:near
  extrn _keybSET_INT:near
  extrn _comINIT:near
  extrn _conSYSTEM:near
  extrn _conCHATT:near
  extrn _diskINITPHY:near
  extrn _diskINITLOG:near
  extrn _diskINITAPP:near
  extrn _diskDESTROYPHY:near
  extrn _diskDESTROYLOG:near
  extrn _diskDESTROYAPP:near
  extrn _procSET_INT:near

  extrn _conACTIVE

start:
  header db 'QSS'

  db 0EAh      ; skok na vyrovnanie CS:EIP
  dw offset Go
  dw segKernel

Go:
  cld
  mov ax, cs
  mov ds, ax
  mov es, ax
  xor ax, ax
  mov ss, ax
  mov sp, 0FFF0h
;********************* Central Processing Unit (CPU) ****************

  ;najprv - kontrola procesora
  mov ah, 0eh
  mov al, 10
  int 10h
  mov al, 13
  int 10h
  mov si, offset msg_testing
  mov bl, 0Eh
  call _print

  ;ci procak podporuje cpuid
  pushf
  pushf
  pop eax
  mov ebx, eax
  xor eax, 200000h
  push eax
  popf
  pushf
  pop eax  ;<<< TOTO ROBI BRUTALL PROBLEMY PRI VOLANI INDE !!!!!
  popf
  and eax, 200000h
  and ebx, 200000h
  cmp eax, ebx
  jnz get_mem

  ;chyba: procesor je <= 486
  mov si, offset msg_notIA32
  mov bl, 4
  call _print

  cli ;zastavenie procaku
  cli
  hlt

  ;zisti velkost pamate (hlavnej - do 16/64MB)
  ;Alokacia pamate aj tak moze pracovat len do 16MB
  ;v realom mode, aj to musi povolit linku A20

  ; ES = DS = CS = 02F0h

;********************* Memory Allocation Unit (MAU) *****************
get_mem:
  call _cpuTEST ; CPU info
  xor eax, eax
  mov ah, 88h
  int 15h       ; pocet suvislych 1kb blokov od 1MB do ax
  jc _halt      ; nevyhoda: max. 16/64MB. Viac nezisti.
  add eax, 1024 ; od adresy 100000h, cize od 1MB
  mov dword ptr [_mem], eax

  call _mauINIT
  xor ax,ax

;************************ Disk ************************************

  call _diskINITPHY
  call _diskINITLOG
  call _diskINITAPP

;********************* Process **************************************

  call _procSET_INT

;********************* Serial Communication *************************

  call _serINIT
  call _comINIT

;********************* Keyboard *************************************

; Toto prerusenie len indikuje, ci nebola stlacena klavesa
; F1 alebo F2 (zmena konzoly), ci ESC (koniec)

  call _keybSET_INT
  xor ax, ax

;************************ MAIN LOOP *******************************

  mov byte ptr [_conACTIVE], 0
public _main_loop
_main_loop:
  cmp byte ptr [_conACTIVE], 0
  jne testCHATT
  call [_conSYSTEM]
  jmp short _main_loop
testCHATT:
  cmp byte ptr [_conACTIVE], 1
  jne testHALT
  call [_conCHATT]
  jmp short _main_loop
testHALT:
  
  xor ax, ax

;*********************** Halt *******************************

public _halt
_halt:
  mov ah, 0eh
  mov al, 10
  int 10h
  mov al, 13
  int 10h
  mov si, offset msg_halting
  mov bl, 0Eh
  call _print

  call _diskDESTROYAPP
  call _diskDESTROYLOG
  call _diskDESTROYPHY
  call _mauDESTROY

  cli
  cli
  hlt
  hlt

;DS:SI zdroj udajov
;BL = farba textu (atribut znaku)
_print proc near
  push cx
  xor bh, bh
  cld
  mov ah, 03h
  xor bh, bh
  int 10h
  mov byte ptr [x], dl
  mov byte ptr [y], dh
prt:
  lodsb
  cmp al, 0
  je ret_print
  mov cx, 1
  mov ah, 09h
  int 10h
  inc byte ptr [x]
  mov dl, byte ptr [x]
  mov dh, byte ptr [y]
  mov ah, 02h
  int 10h
  jmp short prt
ret_print:
  pop cx
  ret
_print endp

end
