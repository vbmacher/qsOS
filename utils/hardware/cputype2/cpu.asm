;
; CPU.ASM
;
; (c) Copyright 2005, vbmacher
;
;
.model tiny
.486P
.data
  msg_is db 10,13,'CPU_ID is available$'
  msg_not db 10,13,'CPU_ID is not available$'
  msg_notbrand db 10,13,'Brand string is not supported$'

  msg_vendor db 10,13, 'Vendor: '
  vendor db 12 dup (0), '$'
  msg_brand db 10,13, 'Brand string: '
  brand db 48 dup(0),'$'

  msg_type db 10,13, 'Type: '
  ptype dw ?,'$'

.code
.startup

start:

  ;check for cpuid instruction
  pushf
  pushf
  pop eax
  mov ebx, eax
  xor eax, 200000h
  push eax
  popf
  pushf
  pop eax
  popf
  and eax, 200000h
  and ebx, 200000h
  cmp eax, ebx
  jnz cpu_id
;  setnz al

  mov dx, offset msg_not
  mov ah, 09h
  int 21h

  ; we have 486 and earlier processor


 
  jmp @@exit
cpu_id:
  mov dx, offset msg_is
  mov ah, 09h
  int 21h
  
  ;cpuid
  xor eax, eax
  db 0Fh
  db 0A2h

  ;vendor
  push ds
  pop es
  mov di, offset vendor
  cld
  mov eax, ebx
  stosd
  mov eax, edx
  stosd
  mov eax, ecx
  stosd

  mov dx, offset msg_vendor
  mov ah, 09h
  int 21h

  ;cpuid
  mov eax, 80000000h
  db 0Fh
  db 0A2h

  ;chcem brand string (cize komplet nazov procesora)
  test eax, 80000000h
  jnz test_brand_string

  mov ah, 09h
  mov dx, offset msg_notbrand
  int 21h
  jmp @@exit

test_brand_string:
  cmp eax, 80000004h
  jge print_brand

  mov ah, 09h
  mov dx, offset msg_notbrand
  int 21h
  jmp @@exit

print_brand:
  mov eax, 80000002h
  db 0Fh
  db 0A2h

  ;brand 1/3
  push ds
  pop es
  mov di, offset brand
  cld
  stosd
  mov eax, ebx
  stosd
  mov eax, ecx
  stosd
  mov eax, edx
  stosd

  ;brand 2/3
  mov eax, 80000003h
  db 0Fh
  db 0A2h
  stosd
  mov eax, ebx
  stosd
  mov eax, ecx
  stosd
  mov eax, edx
  stosd

  ;brand 3/3
  mov eax, 80000004h
  db 0Fh
  db 0A2h
  stosd
  mov eax, ebx
  stosd
  mov eax, ecx
  stosd
  mov eax, edx
  stosd

  mov dx, offset msg_brand
  mov ah, 09h
  int 21h
   
  ;processor type
  mov eax, 1
  db 0Fh
  db 0A2h
  ror eax, 12
  and eax, 11b
  mov di, offset ptype
  stosw
  mov di, offset ptype
  add word ptr es:[di],'0'

  mov ah,09h
  mov dx, offset msg_type
  int 21h


@@exit:
.exit
end