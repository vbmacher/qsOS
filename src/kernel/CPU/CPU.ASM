;
; CPU.ASM
;
; (c) Copyright 2005, vbmacher
;
;

.model compact
.486
.data
.code
  extrn _vendor_str
  extrn _brand_str

; POZOR: tato funkcia sa moze volat AZ ked je na 100% iste,
; ze procesor podporuje CPUID
public _checkBRAND
_checkBRAND proc
	mov eax, 80000000h
	db 0Fh  ;cpuid
	db 0A2h
	;chcem brand string (cize komplet nazov procesora)
	test eax, 80000000h
	jnz test_brand_string

	xor ax, ax
	ret
test_brand_string:
	cmp eax, 80000004h
	jl @bret
	mov ax, 1
@bret:
	ret
_checkBRAND endp

;POZOR: smie sa volat iba pri existencii CPUID
;void cpuPVENDOR();
public _cpuPVENDOR
_cpuPVENDOR proc
        mov byte ptr [_vendor_str+12],0
	xor eax, eax
	db 0Fh
	db 0A2h

        ; vendor
	push es
	push ds
	pop es
        mov di, offset _vendor_str
	mov eax, ebx
	cld
	stosd
	mov eax, edx
	stosd
	mov eax, ecx
	stosd
	pop es
        xor ax, ax
	ret
_cpuPVENDOR endp

;POZOR: moze sa volat iba pri existencii CPUID !!
;void cpuPBRAND();
public _cpuPBRAND
_cpuPBRAND proc
        mov byte ptr [_brand_str+47],0
	mov eax, 80000002h
	db 0Fh
	db 0A2h

	;brand 1/3
	push es
	push ds
	pop es
	mov di, offset _brand_str
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
	pop es
	ret
_cpuPBRAND endp

end
