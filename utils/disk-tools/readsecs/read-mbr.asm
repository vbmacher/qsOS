; Program nacita a vypise 0.sektor z diskety (MBR)

.model small
.stack 100h
.data
        err db 'Nastala chyba pri citani z disku$'
	buf db 512 dup (0) ; buffer pre nacitany sektor
.code
	xor ah, ah         ; reset disku
	xor dl, dl         ; 1. disketa
	int 13h            ; volanie BIOS sluzby

	mov ax, @data      ; ziskanie segmentu dat
	mov es, ax         ; vlozenie segmentu do ES
	mov ds, ax         ; aj do DS (pre neskorsi vypis)
	mov bx, offset buf ; BX <- offset buffera (cela adresa je ES:BX)
	xor dx, dx         ; 1. disketa (DL), 0. hlava (DH)
	xor ch, ch         ; cylinder (pre disketu by mal byt 0)
	mov cl, 0          ; cislo sektora
	mov ah, 02h        ; sluzba: citanie sektorov z disku
	mov al, 1          ; pocet sektorov
	int 13h            ; volanie BIOS sluzby

        mov ah, 01         ; status poslednej op.
        xor dl, dl         ; 1. disketa
        int 13h            ; volanie BIOS sluzby
        jnc vypis          ; ak je chyba, je nastavene carry

        mov ah, 09h        ; vypis retazca
        mov dx, offset err ; DX <- offset chybovej hlasky
        int 21h            ; volanie MSDOS sluzby

        mov al, ah         ; do AL chybovy kod
        mov ah, 4Ch        ; ukoncenie programu
        int 21h            ; volanie MSDOS sluzby

vypis:
        mov ax, 0301h      ; zapis na disk, 1 sektor
        xor cx, cx         ; 0. cylinder, 0. sektor
        xor dh, dh         ; 0. hlava
        mov dl, 1          ; do 2. diskety
        mov bx, offset buf ; ES:BX adresa
        int 13h            ; volanie sluzby

	mov cx, 512        ; pocet znakov
	mov si, offset buf ; SI <- offset buffera
	cld                ; lodsb sposobi inkrementaciu SI
	mov ah, 02h        ; vypis na standardny vystup
l:	lodsb              ; nacitaj znak z DS:SI do AL
	mov dl, al         ; sluzba vyzaduje znak v DL
	int 21h            ; volanie MSDOS sluzby
	loop l             ; CX--; if (CX > 0) goto l

        mov ax, 4c00h      ; ukoncenie programu bez chyby
        int 21h            ; volanie MSDOS sluzby

end