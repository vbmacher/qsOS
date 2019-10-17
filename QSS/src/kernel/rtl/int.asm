;
; INT.ASM
;
; (c) Copyright 2005, vbmacher

.model compact
.data
.code

  public _setINT
  _setINT proc c
    arg vector:byte, segm:word, offs:word, oldint:dword

	push bx
	push es

	cli
	xor ax, ax
	xor bx, bx
	mov es, ax
	mov bl, byte ptr [vector]
	shl bx, 2
	;zaloha
	mov ax, word ptr es:[bx] ;offset
	mov word ptr [oldint], ax
	mov ax, word ptr es:[bx+2] ;segment
	mov word ptr [oldint+2], ax
	;zmena samotneho vektora
	mov ax, word ptr [offs]
	mov word ptr es:[bx], ax
	mov ax, word ptr [segm]
	mov word ptr es:[bx+2], ax
	sti

	pop es
	pop bx
	ret
  _setINT endp

end
