;
; SERINT.ASM
;
; (c) Copyright 2005, vbmacher
;
; Interrupt handler + communication protocol level 0
;
; Protokol urovne 0 vykonava fyzicke posielanie dat cez
; linku, a kazdy znak odosle/prijme dva krat. Pokial znak
; nebol prijaty 2 krat za sebou, niekde nastal zmatok a prijaty
; znak sa ignoruje.
; Protokol by mal vediet pracovat so vsetkymi COM portami (od 1 po 4).


; Problemy a Riesenia
; ^^^^^^^^^^^^^^^^^^^
;
; problem1
; ^^^^^^^^
; Predavanie premennych - z assembleru do C ide ale z C do assembleru NIE
; A rovnako tak volanie funkcii - zaujimave je vsak, ze pri volani funkcie
; main z start.asm v pohode ide...
; RIESENIE: volanie funkcii musi ist cez lomitka: napr. 'call [_comSCANBUF]'
; a co sa tyka premennych: ked su v cecku premenne globalne, tak v assembleri
; su extrn deklarovane v segmente code a nie data!!
;
; problem2
; ^^^^^^^^
; Cez virtualny port vo VMWare VirtualMachine komunikacia prebieha v pohode,
; no v reale nie. Skusanie na 67% potvrdilo, ze skutocny port pri vysielani
; retazca posiela z neho iba jeden znak, teda akoby ostatne vynechaval...
; Druhou moznostou (33%) je to, ze sa znaky vysielaju vsetky, ale nefunguje
; spravne prijimacia jednotka...
; ODOVODNENIE: situacia suvisi s rychlostou skutocneho UARTu a CPU. Kedze
; Vo VMWare je UART simulovany CPU, cize jeho rychlost je rovnaka ako rychlost
; CPU. Kedze je aj posielanie znakov realizovane cez prerusenie: pri odosielani
; znaku je umelo vyvolane prerusenie, ktore otvori komunikacny kanal portu, posle
; znak a zase ho uzavrie. Otvorenie a uzavretie komunikacneho kanalu vo VMWare
; trva nepatrny cas, ale v skutocnom porte to trva ovela dlhsie. Preto posielanie
; niekolkych znakov za sebou sposobuje pri pouziti skutocneho portu ich stratu z
; dovodu cakacej doby pre otvorenie resp. uzavretie komunikacneho kanalu.
; RIESENIE: Najprv som po overeni vyvolal po kazdom poslani znaku prerusenie,
; int 10h na zistenie polohy kurzora na obrazovke. Cas na vykonanie tohto
; prerusenia bohate stacil na vnutornu operaciu UARTu pri otvarani/uzatvarani
; komunikacneho kanalu. No kedze to nie je prilis efektne riesenie, rozhodol
; som sa, rovnako ako pri procedure serRX, poslat cely buffer naraz pri jednorazovom
; otvoreni kom. kanalu. To ma odlahci od pouzivani zvlastnych rieseni a tiez 
; odbremeni UART od ustavicneho otvarania/zatvarania kom. kanalu


masm
.model compact
.386
.data
        ; konstanty fyzickeho prenosu, prve hlavicky dat
        charSTART equ 15h
        charEND equ 16h

	; RX jednotka
	rx_count db 0    ; index znaku (max. 3)
	rx_chr db 0      ; aktualne nacitany znak (resp. prvy znak z trojice)
	buf_ind dw 0     ; index v bufferi

        ; INT unit
	int_servicetab dw serMODEM, _serTX, serRX, serSTATUS

.code
        extrn _serBUF:near ; prijimaci buffer, premenna velkosti 512 bytov
        extrn _comSCANBUF:near
	extrn _PORT: word

; interrupt - seriove prerusenie daneho portu -
public _serINT
_serINT proc near
	pusha
	push es
	push ds

	in al, 21h
	or al, 0FFh	; zakazanie preruseni UARTu
	out 21h, al

	mov al, 20h     ; vyslanie signalu EOI do 8259
	out 20h, al

int_loop:
	mov dx, word ptr [_PORT]
	add dx, 2       ; Interrupt Identification Register
	in al, dx
	test al, 1      ; je prerusenie ?
	jnz @@INT_ret   ; nie
	and ax, 6       ; spristupni len identifikaciu prerusenia, ktore
			; bude tvorit vlastne index v tabulke adries
			; procedur na obsluhu toho-ktoreho prerusenia
	mov si, ax
	call word ptr cs:int_servicetab[si] ;zavolanie prislusnej procedury
	jmp int_loop
@@INT_ret:

	in al, 21h
	xor al, al      ; povolenie vsetkych preruseni na ICU
	out 21h, al

	pop ds
	pop es
	popa
	iret
_serINT endp

; funkcia zmaze buffer
clearBUFFER proc
	push cx
	push di
	push es

        xor ax, ax
        mov word ptr [buf_ind], ax 		; vynuluje index v bufferi

        mov cx, 256				; hmm velkost buffera
        mov di, offset _serBUF
        push cs					; premenna _serBUF je
						; globalna, preto jej
						; segmentova cast adresy
						; sa nachadza v CS
        pop es
        rep stosw 				; zmaze buffer

	pop es
	pop di
	pop cx
	ret
clearBUFFER endp

;***************************** jednotlive funkcne jednotky ************************

; jednotka na prijimanie znakov
; - protocol level0: 2x prijimanie rovnakeho znaku
serRX proc near
	mov dx, word ptr [_PORT]		; aktualny COM port
	in al, dx				; citanie z portu

	mov bl, byte ptr [rx_count]		; BL <- index znaku
	or bl, bl				; BL = 0 ?
	jnz cmp_chars	 			; ak nie, skok
	mov byte ptr [rx_chr], al		; inak do znaku sa da nacitany znak
	mov byte ptr [rx_count], 1		; index znaku <- 1

	jmp @@RX_ret				; vrat sa, toto je len prvy znak

cmp_chars:
	cmp al, byte ptr [rx_chr]		; v AL je nacitany znak z portu,
						; AL = predch. ?
	je inc_ind 				; ak ano, skok

	mov byte ptr [rx_chr], al		; inak je to neocakavany znak, ktory
						; z dovodu synchronizacie sa
						; pouzije ako novy znak
	mov byte ptr [rx_count], 1		; index znaku <- 1
        jmp @@RX_ret				; a skok na navrat

inc_ind:
	inc byte ptr [rx_count]
	cmp byte ptr [rx_count], 1
	jg scan_char 				; ak rx_count > 2, skok
	jmp @@RX_ret				; inak skok na navrat

 ;tu uz je znak pripraveny, rozlisuje sa druh znaku
scan_char:
	xor al, al
	mov byte ptr [rx_count], al		; indexacia znaku odznova

        ;zisti, ake data prisli
        mov al, byte ptr [rx_chr]

	cmp al, charSTART
	jne cEND
cSTART:
	call clearBUFFER
	jmp @@RX_ret
cEND:
	cmp al, charEND
	jne cDATA

	mov bx, word ptr [buf_ind]
	inc bx
	mov si, offset _serBUF
	mov byte ptr [bx+si], 0			; ASCIIz na koniec stringu
        jmp @@RX_dready				; koniec prijimania dat, navrat

cDATA:
        mov al, byte ptr [rx_chr]
	mov di, offset _serBUF
	add di, word ptr [buf_ind]
        push cs
	pop es
	stosb
	sub di, offset _serBUF
        mov word ptr [buf_ind], di
	cmp di, 511				; velkost buffera - 0ovy znak pre ASCIIz
	jle @@RX_ret
	mov al, charEND
	jmp short cEND

@@RX_dready:
	push ds
	push es
	mov ax, cs
	mov ds, ax
	mov es, ax

        call [_comSCANBUF]			; protokol urovne 1

	pop es
	pop ds

@@RX_ret:
        mov dx, word ptr [_PORT]
	add dx, 5				; Line Status Register (LSR)
	in al, dx
	test al, 1				; su dalsie data ?
	jne serRX 				; ak ano, skok
	test al, 40h
	je @real_ret
	call _serTX
@real_ret:
	ret
serRX endp


; jednotka na vysielanie znakov
; 2x posle znak
public _serTX
_serTX proc near
	mov si, offset _serBUF
	cld
dalsi_posli:
	lodsb
	mov bl, al
	or al, al
	jz tx_end

	mov cx, 2
tx_posli:
	dec cx
	mov al, bl

	mov dx, word ptr [_PORT]
	out dx, al                 ; vysielanie znaku

	add dx, 5
test_again:
	in al, dx                  ; citanie registra LSR (aby sa vynuloval transmitter)
	test al, 20h
	je test_again

	or cx, cx
	jne tx_posli

	jmp short dalsi_posli
tx_end:
	mov dx, word ptr [_PORT]
	inc dx
	in al, dx
	and al, 00001101b
	out dx, al                 ; zakaze THRE

	ret
_serTX endp

; iba precita register
serMODEM proc near
	mov dx, word ptr [_PORT]
	add dx, 6
	in al, dx
	ret
serMODEM endp

; iba precita register
serSTATUS proc near
	mov dx, word ptr [_PORT]
	add dx, 5
	in al, dx
	ret
serSTATUS endp


;**************************** EXPORT *********************************

;inicializacia UARTu
public _initUART
_initUART proc c
        arg uartONMASK:byte
	push dx

	mov dx, word ptr [_PORT]
	add dx, 2
	xor al, al
	out dx, al 			; zmaze a zakaze FIFO obvody
	dec dx
	mov al, 00000001b
	out dx, al			; povoli RX prerusenia
	dec dx
	in al, dx			; zmaze prijaty byte
	add dx, 5
	in al, dx			; zmaze status linky
	inc dx
	in al, dx			; zmaze status modemu

	; podla typu COM portu uvolni dane prerusenie na ICU
        ; (Interrupt Control Unit) - jednotka spravy preruseni
	in al, 21h
	and al, byte ptr [uartONMASK]
        ;xor al, al ; povoli vsetky prerusenia
	out 21h, al

	pop dx
	ret
_initUART endp

;deinicializacia UARTu
;POZOR!! procedura moze volat az po volani procedury portINFO,
;pretoze meni vektory prerusenia
public _deinitUART
_deinitUART proc c
        arg uartINT:byte, uartOLDINT:dword, uartOFFMASK:byte 
	push bx
	push dx

	push es
	cli
	xor ax, ax
	xor bx, bx
	mov es, ax
	mov bl, byte ptr [uartINT]
	shl bx, 2
	mov ax, word ptr ds:[uartOLDINT]
	mov word ptr es:[bx], ax
        mov ax, word ptr ds:[uartOLDINT+2]
	mov word ptr es:[bx+2], ax
	sti
	pop es

	in al, 21h			; maskovanie prerusenia UARTu
	or al, byte ptr [uartOFFMASK]
	out 21h, al

	mov dx, word ptr [_PORT]
	inc dx
	xor al, al			; zakaze prerusenia v UARTe
	out dx, al
	add dx, 3
	out dx, al			; a odpoji UART z ICU

	pop dx
	pop bx

	ret
_deinitUART endp

               
end
