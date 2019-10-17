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
.code

 public _BasicDetect

;int BasicDetect();
_BasicDetect proc
  push bp
  mov bp, sp
  push si

  cld
  xor si, si ;Na zaciatku nemame nic
  mov ah, 12h
  mov bh, 10h ;bl?
  mov bl, 10h ;Aj , :>
  int 10h

  test bl, 0FCh
  jnz @basic_ret
  
  inc si ;Zatial mame EGA
  
  mov ax, 1A00h
  int 10h
  cmp al, 1Ah
  jne @CGA

  inc si ;A sme na konci - mame VGA adapter
  jmp short @basic_ret

@CGA:
  xor si, si
@basic_ret:
  mov ax, si
  pop si
  pop bp
  ret
  endp

end
