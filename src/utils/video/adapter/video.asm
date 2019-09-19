;
; VIDEO.ASM
;
; (c) Copyright 2001, P. Jakubco ml.
;
; Display adapter & video mode setup
;
;
;

.model compact
.386p
.data
 nA db ?
.code

start:
  cld
  mov byte ptr [nA], 0 ;Na zaciatku nemame nic
  mov ah, 12h
  mov bh, 10h ;bl?
  int 10h

  test bl, 0FCh
  jnz @basic_ret
  
  mov byte ptr [nA],1 ;Zatial mame EGA
  
  mov ax, 1A00h
  int 10h
  cmp al, 1Ah
  jne @CGA

  mov byte ptr [nA],2 ;A sme na konci - mame VGA adapter
  jmp short @basic_ret

@CGA:
  segds
  mov byte ptr [nA],0
@basic_ret:
  mov al, byte ptr [nA]
  xor ah, ah
xor bh, bh
mov ah, 0eh
cmp al, 0
jne @eg
mov al, 'C'
int 10h
mov al, 'G'
int 10h
mov al,'A'
int 10h
jmp short @exit
@eg:
cmp al, 1
jne @vg
mov al, 'E'
int 10h
mov al, 'G'
int 10h
mov al, 'A'
int 10h
jmp short @exit
@vg:
cmp al, 2
jne @exit
mov al, 'V'
int 10h
mov al,'G'
int 10h
mov al, 'A'
int 10h

@exit:

mov ax, 4c00h
int 21h
end start