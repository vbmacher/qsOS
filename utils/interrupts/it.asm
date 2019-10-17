	.386p
INT_TEXT	segment byte public use16 'CODE'
INT_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:INT_TEXT,ds:DGROUP
_DATA	segment word public use16 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public use16 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
INT_TEXT	segment byte public use16 'CODE'
	assume	cs:INT_TEXT
_GetVect	proc	far
	push	bp
	mov	bp,sp
	xor	     ax,ax
	mov	     es,ax
	mov	     bx, word ptr [[bp+6]]
	shl	     bx,2
	mov	     ax,es:[bx]
	mov	     dx,es:[bx + 2]
	pop	bp
	ret	
_GetVect	endp
	assume	cs:INT_TEXT
_SetVect	proc	far
	push	bp
	mov	bp,sp
	xor	     ax,ax
	mov	     es,ax
	mov	     bx, word ptr [[bp+6]]
	shl	     bx,2
	mov	     ax, word ptr [[bp+8]]
	cli	
	mov	     es:[bx],ax
	sti	
	pop	bp
	ret	
_SetVect	endp
	assume	cs:INT_TEXT
_main	proc	far
	ret	
_main	endp
	?debug	C E9
	?debug	C FA00000000
INT_TEXT	ends
_DATA	segment word public use16 'DATA'
s@	label	byte
_DATA	ends
INT_TEXT	segment byte public use16 'CODE'
INT_TEXT	ends
	public	_main
	public	_SetVect
	public	_GetVect
_s@	equ	s@
	end
