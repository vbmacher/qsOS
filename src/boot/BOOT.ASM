;
; BOOT.ASM
;
; (c) Copyright 2005, vbmacher
;
; QuickSimpleBootLoader (QSBL)
;
; Vypis pamate: 07C0:0000 = start (to je strasne vysoko v pamati)
;               1000:0000 = kernel
;               0000:9000 = zasobnik (a nizzsie)
;
; Do 0920:0000 sa nacita hlavny adresar (viem ze ma cislo 3) 
; Do 0940:0000 sa nacita 1. superblok
; Samotne jadro OS sa nasledne nacita na 1000:0000
;
; !!!POZOR!!!: Tento Bootsektor predpoklada existenciu disketovky
;              SPT = 18 !!! (sector per track) tj. 1,44 MB 3,5"
;
; Revizie
; ^^^^^^^
;  v1.00b (zo dna 21.8.2005) - necita kernel vacsi ako 64kB
;  v1.00 full (den 7.9.2005) - vsetko ok okrem ^
;
;

.model tiny
.code
.startup
  jmp start

  msg_dataerror db 10,13,'QSBL: Data error',0
  msg_kernelnfound db 10,13,'QSBL: Kernel not found',0
  msg_load db 10,13,'QSBL: Loading kernel...',0
  msg_kernelfault db 10,13,'QSBL: Wrong kernel',0
  msg_starting db 10,13,'QSBL: Starting QSS...',10,13,0
  kernel_fn db 'qss', 5 dup(0),'sys',0
  header db 'QSS'

  dir_seg equ 520h
  sup_seg equ 540h
  ker_seg equ 1000h
  stack_seg equ 0h

  spt equ 18

start:
  ;musim presunut bootloader, aby nezavadzal kernelu
  mov ax, 7C0h
  mov ds, ax
  xor si, si
  mov ax, 900h
  mov es, ax
  xor di, di
  mov cx, 100h
  rep movsw

  db 0EAh ;skok koli zmene offsetu (CS = 8FF0h, IP = offset jump)
  dw offset jump
  dw 08F0h
jump:
  mov sp, stack_seg
  mov ss, sp
  mov sp, 8FF0h ;nastavenie zasobnika
  push cs
  pop ds
  push sup_seg
  pop es
  cld ; toto budem potrebovat (srch, print, ...)

;
; CS = 8F0h     , IP = offset+10h
; SS = stack_seg, SP = 8FF0h
; DS = CS       , ES = sup_seg
; -----------------------------
; SI = offset DS veci
; DI = offset ES veci
;

  ;uvodna sprava
  mov si, offset msg_load
  call print

  ;nacitanie kernelu
  xor ah, ah
  xor dl, dl
  int 13h    ; reset disketovky

  xor bx, bx
  xor ch, ch
  mov cl, 2
  xor dx, dx
  call read_phy ; 1. superblok
  push dir_seg
  pop es
  mov cl, 4h    ; sektor: 4 
  call read_phy ; hlavny adresar

  ;ES = dir_seg
  ;BX = file count
  ;DI = offset na filename (premenny)
  ;SI = offset na kernel_fn
  ;DX = offset na filename (stale na 0. poziciu)
srch: ; najde subor s kernelom
  xor di,di
  mov bx, word ptr es:[di] ;pocet suborov

  ;ES:DI = n.ta polozka suboru
  add di, 6 ;prva polozka
  mov dx, di ;dx sluzi na uchovanie 0. pozicie mena v aktualnej polozke
next:
  mov si, offset kernel_fn
  cmp byte ptr es:[di],0
  je ld_next
  dec bx
  mov cx, 11 ;8 meno a 3 pripona
  repe cmpsb
  je found
  ;nenasla sa: dalsia
ld_next:
  cmp bx, 0
  je kerror
  add dx, 12
  mov di, dx
  jmp next

kerror:
  mov si, offset msg_kernelnfound
  call print
  jmp halt
  
found:
  ;nacitam odkaz na ilink, odkial mam odkaz na link...
  ;di odkazuje na polozku ilink, lebo porovnanie
  ;suborov skoncilo inkrementaciou DI az za poslednu poziciu,
  ;kde nasleduje prave ilink
  xor ah, ah
  mov al, byte ptr es:[di] ;v al je ilink
  mov bl, 3
  mul bl     ; i-ta polozka krat velkost polozky = pozicia (v ax)
  mov bx, ax ; bx = pozicia ilinku
  push sup_seg
  pop es

  ;ES = sup_seg

  xor di, di
  add di, bx  ; nastavim index na dany ilink, 0.ta polozka (ftype)
  inc di      ; lebo bl odkazuje na ftype a potrebujem link
  mov ax, word ptr es:[di] ; v ax je link

;***************** NACITANIE KERNELU *******************

  ; AX = link
  ; ES = sup_seg
  ; DS = CS = 08F0h
  ; SI = ?, DI = ?

  ;pracovny moze byt kludne aj sup_seg uz ho nepotrebujem
  ;do neho nacitam dblok po dbloku. Data z nich
  ;postupne presuniem na "bezpecne" miesto, na adresu
  ;kernela, tj.: kernel_seg:0000

  xor di, di
  xor bx, bx

  ; BX = 0
  ; CX = pocet dat
  ; SI = polozka dat, alebo link
  ; DI = offset kernelu

  ; tato rutina dokaze spracovat jedine 64kB dat urcite
  ; viac dat neviem - podla toho, ci je instrukcia movsb
  ; schopna menit aj segment es po preplneni offsetu di

  push sup_seg
  pop ds
  ; DS = sup_seg

read_next:
  call decode_lp ;dekoduje link (v ax) a nacita do ES:BX
  mov cx, word ptr es:[bx] ;pocet dat (malo by byt 506)

  mov si, bx
  add si, 6  ;nastavenie na polozku dat

  push ker_seg ; kernel segment
  pop es
  ; ES = ker_seg

  ;presunie data do ES:DI
  rep movsb

  push sup_seg
  pop es
  ; ES = sup_seg

  mov si, 2
  mov ax, word ptr ds:[si] ;dalsi link
  cmp ax, 0
  jne read_next

; **************** Kontrola kernela ***************************
  ; DS = CS = 8FF0h
  ; ES = ker_seg
  ; DI = 0
  ; CX = 3 (header 3 bytov dlhy = 'QFS')

  push cs
  pop ds
  mov si, offset header
  push ker_seg
  pop es
  xor di, di
  mov cx, 3
  repe cmpsb  ; kontrola ci obsahuje na zaciatku 'QSS'
  jne kfault

;*************************** START **************************

  ; header ok
  ; vypis hlasky samozrejme
  mov si, offset msg_starting
  call print

  ;skok
  db 0EAH
  dw 03h       ; offset za header ('QSS')
  dw ker_seg

;*********** FUNKCIE, atd. ***********************************

;prevedie link (v AX) do fyz. parametrov
decode_lp:
  push bx
  push ax
  xor dh, dh      ; dh = hlava zatial 0
  mov bl, 2 * spt
  div bl
  mov ch, al      ; ch = stopa
  cmp ah, spt
  jl @h0
  mov dh, 1       ; dh = hlava 1
@h0:
  pop ax          ; v ax znova link
  mov bl, spt
  div bl
  mov cl, ah
  inc cl          ; cl = sektor
  xor dl, dl
  pop bx

read_phy:
  mov ax, 0201h ; sektorov: 1, sluzba: 2 (citanie z jednotky)
  int 13h
  jnc @re
  mov ax, 0201h
  int 13h
  jc derror
@re:
  ret

derror:
  mov si, offset msg_dataerror
  call print
  jmp halt

kfault:
  push cs
  pop ds
  mov si, offset msg_kernelfault
  call print
  jmp halt

;DS:SI zdroj udajov
print:
  cld
  xor bh, bh
  mov ah, 0eh
prt:
  lodsb
  cmp al, 0
  je Tret_print
  int 10h
  jmp short prt
Tret_print:
  ret

halt:
  cli  ;zakaz preruseni
  hlt

 nic db 68 dup (0)
 ww dw 55AAh
end
