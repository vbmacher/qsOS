model small
dosseg

INTNUM      equ 0Ch          ; COM1; COM2: 0Bh
OFFMASK     equ 00010000b    ; COM1; COM2: 00001000b
ONMASK      equ not OFFMASK
UART_BASE   equ 3F8h         ; COM1; COM2: 2F8h
UART_RATE   equ 12           ; 9600 bps, see table in this file
UART_PARAMS equ 00000011b    ; 8n1, see tables
RXFIFOSIZE  equ 8096         ; set this to your needs
TXFIFOSIZE  equ 8096         ; dito.
                             ; the fifos must be large on slow computers
                             ; and can be small on fast ones
                             ; These have nothing to do with the 16550A's
                             ; built-in FIFOs!

.data
long_text  db  0dh
    db  "This is a very long test string. It serves the purpose of",0dh
    db  "demonstrating that our interrupt-driven routines are capable",0dh
    db  "of coping with pressure situations like the one we provoke",0dh
    db  "by sending large bunches of characters in each direction at",0dh
    db  "the same time. Run this test by pressing F1 at a low data",0dh
    db  "rate and a high data rate to see why serial transmission and",0dh
    db  "reception should be programmed interrupt-driven. You won't lose",0dh
    db  "a single character as long as you don't overload the fifos, no",0dh
    db  "matter how hard you try!",0dh,0

ds_dgroup  macro
  mov  ax,DGROUP
  mov  ds,ax
  assume  ds:DGROUP
endm

ds_text  macro
  push  cs
  pop   ds
  assume  ds:_TEXT
endm

rx_checkwrap  macro
  local rx_nowrap
  cmp  si,offset rxfifo+RXFIFOSIZE
  jb  rx_nowrap
  lea  si,rxfifo
rx_nowrap:
endm

tx_checkwrap  macro
  local tx_nowrap
  cmp  si,offset txfifo+TXFIFOSIZE
  jb  tx_nowrap
  lea  si,txfifo
tx_nowrap:
endm

.stack 256

.data?
old_intptr  dd  ?
rxhead      dw  ?
rxtail      dw  ?
txhead      dw  ?
txtail      dw  ?
bitxfifo    dw  1  ; size of built-in TX fifo (1 if no fifo)
rxfifo      db  RXFIFOSIZE dup (?)
txfifo      db  TXFIFOSIZE dup (?)

.code
start  proc far
  call  install_interrupt_handler
  call  clear_fifos
  call  clear_screen
  call  init_UART
continue:
  call  read_RX_fifo
  call  read_keyboard
  jnc  continue
  call  clean_up
  mov  ax,4c00h
  int  21h  ; return to DOS
start  endp

interrupt_handler  proc far
  assume  ds:nothing,es:nothing,ss:nothing,cs:_text
  push  ax
  push  cx
  push  dx  ; first save the regs we need to change
  push  ds
  push  si
  in  al,21h
  or  al,OFFMASK   ; disarm the interrupt
  out 21h,al
  mov  al,20h  ; acknowledge interrupt
  out  20h,al

ih_continue:
  mov  dx,UART_BASE+2
  xor  ax,ax
  in  al,dx  ; get interrupt cause
  test  al,1  ; did the UART generate the int?
  jne  ih_sep  ; no, then it's somebody else's problem
  and  al,6  ; mask bits not needed
  mov  si,ax  ; make a pointer out of it
  call  interrupt_table[si]  ; serve this int
  jmp  ih_continue  ; and look for more things to be done
ih_sep:

  in  al,21h
  and al,ONMASK  ; rearm the interrupt
  out 21h,al

  pop  si
  pop  ds
  pop  dx  ; restore regs
  pop  cx
  pop  ax
  iret
interrupt_table  dw  int_modem,int_tx,int_rx,int_status
interrupt_handler  endp

int_modem  proc near
  ; just clear modem status, we are not interested in it
  mov  dx,UART_BASE+6
  in  al,dx
  ret
int_modem  endp

int_tx  proc near
  ds_dgroup
  ; check if there's something to be sent
  mov  si,txtail
  mov  cx,bitxfifo
itx_more:
  cmp  si,txhead
  je  itx_nothing
  cld
  lodsb
  mov  dx,UART_BASE
  out  dx,al  ; write it to the THR
  ; check for wrap-around in our fifo
  tx_checkwrap
  ; send as much bytes as the chip can take when available
  loop itx_more
  jmp  itx_dontstop
itx_nothing:
  ; no more data in the fifo, so inhibit TX interrupts
  mov  dx,UART_BASE+1
  mov  al,00000001b
  out  dx,al
itx_dontstop:
  mov  txtail,si
  ret
int_tx  endp

int_rx  proc near
  ds_dgroup
  mov  si,rxhead
irx_more:
  mov  dx,UART_BASE
  in  al,dx
  mov  byte ptr [si],al
  inc  si
  ; check for wrap-around
  rx_checkwrap
  ; see if there are more bytes to be read
  mov  dx,UART_BASE+5
  in  al,dx
  test  al,1
  jne  irx_more
  mov  rxhead,si
  test  al,40h  ; Sometimes when sending and receiving at the
  jne  int_tx   ; same time, TX ints get lost. This is a cure.
  ret
int_rx  endp

int_status  proc near
  ; just clear the status ("this trivial task is left as an exercise
  ; to the student")
  mov  dx,UART_BASE+5
  in  al,dx
  ret
int_status  endp

read_RX_fifo  proc near
  ; see if there are bytes to be read from the fifo
  ; we read a maximum of 16 bytes, then return in order
  ; not to break keyboard control
  ds_dgroup
  cld
  mov  cx,16
  mov  si,rxtail
rx_more:
  cmp  si,rxhead
  je  rx_nodata
  lodsb
  call  output_char
  ; check for wrap-around
  rx_checkwrap
  loop  rx_more
rx_nodata:
  mov  rxtail,si
  ret
read_RX_fifo  endp

read_keyboard  proc near
  ds_dgroup
  ; check for keys pressed
  mov  ah,1
  int  16h
  je  rk_nokey
  xor  ax,ax
  int  16h
  cmp  ax,2d18h  ; is it Ctrl-X?
  stc
  je  rk_ctrlx
  cmp  ax,3b00h  ; is it F1?
  jne  rk_nf1
  lea  si,long_text  ; send a very long test string
  call  send_string
  jmp  rk_nokey
rk_nf1:
  ; echo the character to the screen
  call  output_char

  call  send_char
rk_nokey:
  clc
rk_ctrlx:
  ret
read_keyboard  endp


install_interrupt_handler  proc near
  ds_dgroup
  ; install interrupt handler first
  mov  ax,3500h+INTNUM
  int  21h
  mov  word ptr old_intptr,bx
  mov  word ptr old_intptr+2,es
  mov  ax,2500h+INTNUM
  ds_text
  lea  dx,interrupt_handler
  int  21h
  ret
install_interrupt_handler  endp

clear_fifos  proc near
  ds_dgroup
  ; clear fifos (not those in the 16550A, but ours)
  lea  ax,rxfifo
  mov  rxhead,ax
  mov  rxtail,ax
  lea  ax,txfifo
  mov  txhead,ax
  mov  txtail,ax
  ret
clear_fifos  endp

init_UART  proc near
  ; initialize the UART
  mov  dx,UART_BASE+3
  mov  al,80h
  out  dx,al  ; make DL register accessible
  mov  dx,UART_BASE
  mov  ax,UART_RATE
  out  dx,ax  ; write bps rate divisor
  mov  dx,UART_BASE+3
  mov  al,UART_PARAMS
  out  dx,al  ; write parameters
  
  ; is it a 16550A?
  mov  dx,UART_BASE+2
  in   al,dx
  and  al,11000000b
  cmp  al,11000000b
  jne  iu_nofifos
  mov  bitxfifo,16
  mov  dx,UART_BASE+2
  mov  al,11000111b
  out  dx,al  ; clear and enable the fifos if they exist
iu_nofifos:
  mov  dx,UART_BASE+1
  mov  al,00000001b  ; allow RX interrupts
  out  dx,al
  mov  dx,UART_BASE
  in  al,dx  ; clear receiver
  mov  dx,UART_BASE+5
  in  al,dx  ; clear line status
  inc  dx
  in  al,dx  ; clear modem status
  ; free interrupt in the ICU
  in  al,21h
  and  al,ONMASK
  out  21h,al
  ; and enable ints from the UART
  mov  dx,UART_BASE+4
  mov  al,00001000b
  out  dx,al
  ret
init_UART  endp

clear_screen  proc near
  mov  ah,0fh  ; allow all kinds of video adapters to be used
  int  10h
  cmp  al,7
  je  cs_1
  mov  al,3
cs_1:
  xor  ah,ah
  int  10h
  ret
clear_screen  endp

clean_up  proc near
  ds_dgroup
  ; lock int in the ICU
  in  al,21h
  or  al,OFFMASK
  out  21h,al
  xor  ax,ax
  mov  dx,UART_BASE+4  ; disconnect the UART from the int line
  out  dx,al
  mov  dx,UART_BASE+1  ; disable UART ints
  out  dx,al
  mov  dx,UART_BASE+2  ; disable the fifos (old software relies on it)
  out  dx,al
  ; restore int vector
  lds  dx,old_intptr
  mov  ax,2500h+INTNUM
  int  21h
  ret
clean_up  endp

output_char  proc near
  push  si
  push  ax
oc_cr:
  push  ax
  mov  ah,0eh  ; output character using BIOS TTY
  int  10h     ; it's your task to improve this
  pop  ax
  cmp  al,0dh  ; add LF after CR; change it if you don't like it
  mov  al,0ah
  je  oc_cr
  pop  ax
  pop  si
  ret
output_char  endp

send_char proc near
  push  si
  push  ax
  ds_dgroup
  pop  ax
  mov  si,txhead
  mov  byte ptr [si],al
  inc  si
  ; check for wrap-around
  tx_checkwrap
  mov  txhead,si
  ; test if the interrupt is running at the moment
  mov  dx,UART_BASE+5
  in  al,dx
  test  al,40h
  je  sc_dontcrank
  ; crank it up
  ; note that this might not work with some very old 8250s
  mov  dx,UART_BASE+1
  mov  al,00000011b
  out  dx,al
sc_dontcrank:
  pop  si
  ret
send_char  endp

send_string  proc near
  ; sends a null-terminated string pointed at by DS:SI
  ds_dgroup
  cld
ss_more:
  lodsb
  or  al,al
  je  ss_end
  call send_char
  jmp  ss_more
ss_end:
  ret
send_string  endp

end start
