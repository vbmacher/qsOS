.model small
.stack 100h
.data

	readmsg db      'The buffers match.  Hard disk read using ports.$'
	failmsg db      'The buffers do not match.$'

	buffer  db      512 dup ('V')
	buffer2 db      512 dup ('L')

.code
.386
start:
	mov ax, @data
	mov es, ax

	mov     dx,1f6h         ;Drive and head port
	mov     al,0a0h         ;Drive 0, head 0
	out     dx,al

	mov     dx,1f2h         ;Sector count port
	mov     al,1            ;Read one sector
	out     dx,al

	mov     dx,1f3h         ;Sector number port
	mov     al,1            ;Read sector one
	out     dx,al

	mov     dx,1f4h         ;Cylinder low port
	mov     al,0            ;Cylinder 0
	out     dx,al

	mov     dx,1f5h         ;Cylinder high port
	mov     al,0            ;The rest of the cylinder 0
	out     dx,al

	mov     dx,1f7h         ;Command port
	mov     al,20h          ;Read with retry.
	out     dx,al
still_going:
	in      al,dx
	test    al,8            ;This means the sector buffer requires
				;servicing.
	jz      still_going     ;Don't continue until the sector buffer
				;is ready.

	mov     cx,512/2        ;One sector /2
	mov     di,offset buffer
	mov     dx,1f0h         ;Data port - data comes in and out of here.
	rep     insw

;   ------

	mov     ax,201h         ;Read using int13h then compare buffers.
	mov     dx,80h
	mov     cx,1
	mov     bx,offset buffer2
	int     13h

	mov     cx,512
	mov     si,offset buffer
	mov     di,offset buffer2
	repe    cmpsb
	jne     failure
	mov     ah,9
	mov     dx,offset readmsg
	int     21h
	jmp     good_exit
failure:
	mov     ah,9
	mov     dx,offset failmsg
	int     21h
good_exit:
	mov     ax,4c00h        ;Exit the program
	int     21h
end