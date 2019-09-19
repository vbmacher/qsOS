;
; FONT2.ASM
;
; (c) Copyright 2002, Tibor Marchyn (XeN)
;
; This file is a part of Kristoph Operating System (KOS)
;
;Relase notes
;~~~~~~~~~~~~
; 24.01.2002   v0.1   Initial relase


.model small
.stack 100h
.data

def0  db    00000000b ;"0" - 48  ; makcen
      db    00000000b            ; makcen
      db    01111110b            ; jadro
      db    10000011b            ; jadro
      db    10000101b            ; jadro
      db    10001001b     
      db    10010001b
      db    10010001b
      db    10010001b
      db    10100001b
      db    11000001b
      db    01111110b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def1  db    00000000b ;"1" - 49  ; makcen
      db    00000000b            ; makcen
      db    00001100b            ; jadro
      db    00010100b            ; jadro
      db    00100100b            ; jadro
      db    00000100b     
      db    00000100b
      db    00000100b
      db    00000100b
      db    00000100b
      db    00000100b
      db    00000100b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def2  db    00000000b ;"2" - 50  ; makcen
      db    00000000b            ; makcen
      db    01111110b            ; jadro
      db    10000001b            ; jadro
      db    00000001b            ; jadro
      db    00000010b     
      db    00000100b
      db    00001000b
      db    00010000b
      db    00100000b
      db    01000000b
      db    11111111b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def3  db    00000000b ;"3" - 51  ; makcen
      db    00000000b            ; makcen
      db    00111110b            ; jadro
      db    01000001b            ; jadro
      db    00000001b            ; jadro
      db    00000010b     
      db    00011100b
      db    00000010b
      db    00000001b
      db    00000001b
      db    01000001b
      db    00111110b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def4  db    00000000b ;"4" - 52  ; makcen
      db    00000000b            ; makcen
      db    00001000b            ; jadro
      db    00010000b            ; jadro
      db    00100000b            ; jadro
      db    01000000b     
      db    01000000b
      db    10001000b
      db    11111111b
      db    00001000b
      db    00001000b
      db    00001000b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def5  db    00000000b ;"5" - 53  ; makcen
      db    00000000b            ; makcen
      db    11111111b            ; jadro
      db    10000000b            ; jadro
      db    10000000b            ; jadro
      db    10000000b     
      db    01111110b
      db    00000001b
      db    00000001b
      db    00000001b
      db    10000001b
      db    01111110b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def6  db    00000000b ;"6" - 54  ; makcen
      db    00000000b            ; makcen
      db    01111110b            ; jadro
      db    10000001b            ; jadro
      db    10000000b            ; jadro
      db    10000000b
      db    10111110b
      db    11000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    01111110b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def7  db    00000000b ;"7" - 55  ; makcen
      db    00000000b            ; makcen
      db    11111111b            ; jadro
      db    00000010b            ; jadro
      db    00000100b            ; jadro
      db    00001000b     
      db    00010000b
      db    00100000b
      db    00100000b
      db    00100000b
      db    00100000b
      db    00100000b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def8  db    00000000b ;"8" - 56  ; makcen
      db    00000000b            ; makcen
      db    00111100b            ; jadro
      db    01000010b            ; jadro
      db    01000010b            ; jadro
      db    01000010b     
      db    01111110b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    01111110b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

def9  db    00000000b ;":" - 57  ; makcen
      db    00000000b            ; makcen
      db    00111110b            ; jadro
      db    01000001b            ; jadro
      db    01000001b            ; jadro
      db    01000001b     
      db    00111111b
      db    00000001b
      db    00000001b
      db    00000001b
      db    01000001b
      db    00111110b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

defDVOJBOD  db    00000000b ;":" - 58  ; makcen
            db    00000000b            ; makcen
            db    00000000b            ; jadro
            db    00000000b            ; jadro
            db    00000000b            ; jadro
            db    00000000b     
            db    00011000b
            db    00000000b
            db    00000000b
            db    00000000b
            db    00011000b
            db    00000000b        ;
            db    00000000b        ;spodok
            db    00000000b        ;spodok
            db    00000000b
            db    00000000b

defBODKOCI  db    00000000b ;";" - 59  ; makcen
            db    00000000b            ; makcen
            db    00000000b            ; jadro
            db    00000000b            ; jadro
            db    00000000b            ; jadro
            db    00000000b     
            db    00000000b
            db    00010000b
            db    00000000b
            db    00010000b
            db    00010000b
            db    00100000b        ;
            db    00000000b        ;spodok
            db    00000000b        ;spodok
            db    00000000b
            db    00000000b

defSIKZATL  db    00000000b ;"<" - 60  ; makcen
            db    00000000b            ; makcen
            db    00000000b            ; jadro
            db    00000010b            ; jadro
            db    00000100b            ; jadro
            db    00001000b     
            db    00010000b
            db    00100000b
            db    00010000b
            db    00001000b
            db    00000100b
            db    00000010b        ;
            db    00000000b        ;spodok
            db    00000000b        ;spodok
            db    00000000b
            db    00000000b

defROVNASA  db    00000000b ;"=" - 61  ; makcen
            db    00000000b            ; makcen
            db    00000000b            ; jadro
            db    00000000b            ; jadro
            db    00000000b            ; jadro
            db    00000000b
            db    01111110b
            db    00000000b
            db    01111110b
            db    00000000b
            db    00000000b
            db    00000000b        ;
            db    00000000b        ;spodok
            db    00000000b        ;spodok
            db    00000000b
            db    00000000b

defSIKZATP  db    00000000b ;"<" - 62  ; makcen
            db    00000000b            ; makcen
            db    00000000b            ; jadro
            db    01000000b            ; jadro
            db    00100000b            ; jadro
            db    00010000b
            db    00001000b
            db    00000100b
            db    00001000b
            db    00010000b
            db    00100000b
            db    01000000b        ;
            db    00000000b        ;spodok
            db    00000000b        ;spodok
            db    00000000b
            db    00000000b

defOTAZNIK  db    00000000b ;"?" - 63  ; makcen
            db    00000000b            ; makcen
            db    00111100b            ; jadro
            db    01000010b            ; jadro
            db    00000010b            ; jadro
            db    00000100b
            db    00001000b
            db    00010000b
            db    00010000b
            db    00010000b
            db    00000000b
            db    00010000b        ;
            db    00000000b        ;spodok
            db    00000000b        ;spodok
            db    00000000b
            db    00000000b

defZAVINAC  db    00000000b ;"@" - 64  ; makcen
            db    00000000b            ; makcen
            db    00000000b            ; jadro
            db    00000000b            ; jadro
            db    00111100b            ; jadro
            db    01000010b
            db    10000001b
            db    10011001b
            db    10101001b
            db    10011111b
            db    10000000b
            db    01111100b        ;
            db    00000000b        ;spodok
            db    00000000b        ;spodok
            db    00000000b
            db    00000000b

defA  db    00000000b ;"A" - 65  ; makcen
      db    00000000b            ; makcen
      db    00011000b            ; jadro
      db    00100100b            ; jadro
      db    00100100b            ; jadro
      db    01000010b
      db    01000010b
      db    10000001b
      db    10000001b
      db    11111111b
      db    10000001b
      db    10000001b        ;
      db    00000000b        ;spodok
      db    00000000b        ;spodok
      db    00000000b
      db    00000000b

defB  db    00000000b     ;"B" - 66
      db    00000000b
      db    11111100b
      db    10000010b
      db    10000010b
      db    10000010b
      db    11111110b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    11111110b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defC  db    00000000b ;"C" - 67
      db    00000000b
      db    00111110b
      db    01000001b
      db    10000001b
      db    10000001b
      db    10000000b
      db    10000000b
      db    10000001b
      db    10000001b
      db    01000001b
      db    00111110b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defD  db    00000000b ;"D" - 68
      db    00000000b
      db    11111100b
      db    10000010b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000010b
      db    11111100b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defE  db    00000000b ;"E" - 69
      db    00000000b
      db    11111110b
      db    10000001b
      db    10000000b
      db    10000000b
      db    11111100b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000001b
      db    11111110b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defF  db    00000000b ;"F" - 70
      db    00000000b
      db    11111111b
      db    10000000b
      db    10000000b
      db    10000000b
      db    11111100b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defG  db    00000000b ;"G" - 71
      db    00000000b
      db    00111100b
      db    01000010b
      db    10000001b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10011110b
      db    10000001b
      db    01000010b
      db    00111100b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defH  db    00000000b ;"H" - 72
      db    00000000b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    11111111b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defI  db    00000000b ;"I" - 73
      db    00000000b
      db    11111110b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    11111110b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defJ  db    00000000b ;"J" - 74
      db    00000000b
      db    00111111b
      db    00000001b
      db    00000001b
      db    00000001b
      db    00000001b
      db    00000001b
      db    00000001b
      db    10000001b
      db    01000010b
      db    00111100b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defK  db    00000000b ;"K" - 75
      db    00000000b
      db    10000001b
      db    10000010b
      db    10000100b
      db    10001000b
      db    10010000b
      db    11110000b
      db    10001000b
      db    10000100b
      db    10000010b
      db    10000001b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defL  db    00000000b ;"L" - 76
      db    00000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    11111111b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defM  db    00000000b ;"M" - 77
      db    00000000b
      db    10000001b
      db    11000011b
      db    10100101b
      db    10011001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defN  db    00000000b ;"N" - 78
      db    00000000b
      db    10000001b
      db    10000001b
      db    11000001b
      db    10100001b
      db    10010001b
      db    10001001b
      db    10000101b
      db    10000011b
      db    10000001b
      db    10000001b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defO  db    00000000b ;"O" - 79
      db    00000000b
      db    01111110b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    01111110b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defP  db    00000000b ;"P" - 80
      db    00000000b
      db    11111110b
      db    10000001b
      db    10000001b
      db    10000001b
      db    11111110b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defQ  db    00000000b ;"Q"-81
      db    00000000b
      db    01111110b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10001001b
      db    10000101b
      db    01111110b
      db    00000010b
      db    00000001b
      db    00000000b
      db    00000000b

defR  db    00000000b ;"R" - 82
      db    00000000b
      db    11111110b
      db    10000001b
      db    10000001b
      db    10000001b
      db    11111110b
      db    10010000b
      db    10001000b
      db    10000100b
      db    10000010b
      db    10000001b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defS  db    00000000b ;"S" - 83
      db    00000000b
      db    00111110b
      db    01000001b
      db    10000000b
      db    01000000b
      db    00100000b
      db    00011100b
      db    00000010b
      db    00000001b
      db    10000010b
      db    01111100b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defT  db    00000000b ;"T" - 84
      db    00000000b
      db    11111110b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defU  db    00000000b ;"U" - 85
      db    00000000b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    01000010b
      db    00111100b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defV  db    00000000b ;"V" - 86
      db    00000000b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    01000010b
      db    00100100b
      db    00011000b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defW  db    00000000b ;"W" - 87
      db    00000000b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    10000001b
      db    01011010b
      db    00100100b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defX  db    00000000b ;"X" - 88
      db    00000000b
      db    10000001b
      db    01000010b
      db    00100100b
      db    00100100b
      db    00011000b
      db    00011000b
      db    00100100b
      db    00100100b
      db    01000010b
      db    10000001b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defY  db    00000000b ;"Y" - 89
      db    00000000b
      db    10000010b
      db    10000010b
      db    01000100b
      db    00101000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00010000b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defZ  db    00000000b ;"Z" - 90
      db    00000000b
      db    11111111b
      db    00000001b
      db    00000010b
      db    00000100b
      db    00001000b
      db    00010000b
      db    00100000b
      db    01000000b
      db    10000000b
      db    11111111b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

defHZ  db    00000000b ;"[" - 91
       db    00000000b
       db    11111100b
       db    10000000b
       db    10000000b
       db    10000000b
       db    10000000b
       db    10000000b
       db    10000000b
       db    10000000b
       db    10000000b
       db    11111100b
       db    00000000b
       db    00000000b
       db    00000000b
       db    00000000b

defLOMITKO  db    00000000b ;"\" - 92
       db    00000000b
       db    00000000b
       db    10000000b
       db    01000000b
       db    00100000b
       db    00010000b
       db    00001000b
       db    00000100b
       db    00000010b
       db    00000001b
       db    00000000b
       db    00000000b
       db    00000000b
       db    00000000b
       db    00000000b

defHZ2  db    00000000b ;"]" - 93
        db    00000000b
        db    00111111b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00111111b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defVOKAN  db    00011000b ;"^" - 94
          db    00100100b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b
          db    00000000b

defPODTRZITKO  db    00000000b ;"_" - 95
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    11111111b

defAPOSOPAC  db    00001000b ;"`" - 96
             db    00000100b
             db    00000010b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b
             db    00000000b

defAA   db    00000000b ;"a" - 97
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00111110b
        db    00000001b
        db    00111111b
        db    01000001b
        db    01000011b
        db    00111101b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defBB   db    00000000b ;"b" - 98
        db    00000000b
        db    10000000b
        db    10000000b
        db    10000000b
        db    10000000b
        db    10111100b
        db    11000010b
        db    10000001b
        db    10000001b
        db    11000010b
        db    10111100b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defCC   db    00000000b ;"c" - 99
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00111110b
        db    01000001b
        db    10000000b
        db    10000000b
        db    01000001b
        db    00111110b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defDD   db    00000000b ;"d" - 100
        db    00000000b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00111101b
        db    01000011b
        db    10000001b
        db    10000001b
        db    01000011b
        db    00111101b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defEE   db    00000000b ;"e" - 101
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00111110b
        db    01000001b
        db    10111111b
        db    10000000b
        db    01000001b
        db    00111110b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defFF   db    00000000b ;"f" - 102
        db    00000000b
        db    00011100b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00111100b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defGG   db    00000000b ;"g" - 103
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00111110b
        db    01000001b
        db    10000001b
        db    10000001b
        db    01000011b
        db    00111101b
        db    00000001b
        db    10000001b
        db    01000010b
        db    00111100b

defHH   db    00000000b ;"h" - 104
        db    00000000b
        db    01000000b
        db    01000000b
        db    01000000b
        db    01000000b
        db    01011110b
        db    01100001b
        db    01000001b
        db    01000001b
        db    01000001b
        db    01000001b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defII   db    00000000b ;"i" - 105
        db    00000000b
        db    00000000b
        db    00000000b
        db    00001000b
        db    00000000b
        db    00001000b
        db    00001000b
        db    00001000b
        db    00001000b
        db    00001000b
        db    00001000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defJJ   db    00000000b ;"j" - 106
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000001b
        db    00000000b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b
        db    01000001b
        db    00100010b
        db    00011100b

defKK   db    00000000b ;"k" - 107
        db    00000000b
        db    01000000b
        db    01000000b
        db    01000001b
        db    01000010b
        db    01000100b
        db    01111000b
        db    01000100b
        db    01000010b
        db    01000001b
        db    01000001b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defLL   db    00000000b ;"l" - 108
        db    00000000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00001100b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defMM   db    00000000b ;"m" - 109
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    11101110b
        db    10010001b
        db    10010001b
        db    10010001b
        db    10010001b
        db    10010001b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defNN   db    00000000b ;"n" - 110
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    10111110b
        db    11000001b
        db    10000001b
        db    10000001b
        db    10000001b
        db    10000001b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defOO   db    00000000b ;"o" - 111
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    01111110b
        db    10000001b
        db    10000001b
        db    10000001b
        db    10000001b
        db    01111110b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defPP   db    00000000b ;"p" - 112
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    01111110b
        db    10000001b
        db    10000001b
        db    10000001b
        db    11000001b
        db    10111110b
        db    10000000b
        db    10000000b
        db    10000000b
        db    10000000b

defQQ   db    00000000b ;"q" - 113
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    01111110b
        db    10000001b
        db    10000001b
        db    10000001b
        db    10000001b
        db    01111111b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00000001b

defRR   db    00000000b ;"r" - 114
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00101110b
        db    00110000b
        db    00100000b
        db    00100000b
        db    00100000b
        db    00100000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defSS   db    00000000b ;"s" - 115
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    01111110b
        db    10000001b
        db    01110000b
        db    00001110b
        db    10000001b
        db    01111110b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defTT   db    00000000b ;"t" - 116
        db    00000000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00111100b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00010000b
        db    00011100b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defUU   db    00000000b ;"u" - 117
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    10000001b
        db    10000001b
        db    10000001b
        db    10000011b
        db    10000101b
        db    01111001b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defVV   db    00000000b ;"v" - 118
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    10000001b
        db    10000001b
        db    10000001b
        db    01000010b
        db    00100100b
        db    00011000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defWW   db    00000000b ;"w" - 119
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    10000001b
        db    10000001b
        db    10000001b
        db    10000001b
        db    10011001b
        db    01100110b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defXX   db    00000000b ;"x" - 120
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    10000001b
        db    01000010b
        db    00100100b
        db    00111100b
        db    01000010b
        db    10000001b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defYY   db    00000000b ;"y" - 121
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    10000001b
        db    10000001b
        db    10000001b
        db    10000001b
        db    01000001b
        db    00111111b
        db    00000001b
        db    00000001b
        db    00000001b
        db    00111110b

defZZ   db    00000000b ;"z" - 122
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b
        db    11111111b
        db    00000010b
        db    00001100b
        db    00110000b
        db    01000000b
        db    11111111b
        db    00000000b
        db    00000000b
        db    00000000b
        db    00000000b

defZLOZZATL   db    00000000b ;"{" - 123
              db    00000000b
              db    00000110b
              db    00001000b
              db    00001000b
              db    00110000b
              db    01000000b
              db    00110000b
              db    00001000b
              db    00001000b
              db    00001000b
              db    00000110b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
;tota tu volovina je 60 pismeno
defDVOJCIAR   db    00000000b ;"|" - 124
              db    00000000b
              db    00010000b
              db    00010000b
              db    00010000b
              db    00010000b
              db    00000000b
              db    00000000b
              db    00010000b
              db    00010000b
              db    00010000b
              db    00010000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
;toto je 61 pismeno
defZLOZZATVP  db    00000000b ;"}" - 125
              db    00000000b
              db    01100000b
              db    00010000b
              db    00010000b
              db    00001100b
              db    00000010b
              db    00001100b
              db    00010000b
              db    00010000b
              db    00010000b
              db    01100000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b

defHOVLNOVKA  db    00110010b ;"~" - 126
              db    01001100b ;62 pismeno
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b
              db    00000000b

defCM db    00010100b ;"C" s makcenom - 128
      db    00001000b
      db    00111100b
      db    01000010b
      db    10000001b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000000b
      db    10000001b
      db    01000010b
      db    00111100b
      db    00000000b
      db    00000000b
      db    00000000b
      db    00000000b

.code
start:

;Pomocou sluzby 1100h zmenime celu sadu fontu

mov ax, @data
mov es, ax
mov bp, offset def0 ; ES:BP - Ukazatel na tabulku predloh


mov bh, 16          ; vyska znaku (VGA matica 16x8)
mov bl, 00h         ; blok fontu (VGA)
mov cx, 79          ; pocet pismen - 58 je cela abeceda bez cisel + [\]
mov dx, 48          ; ascii kod prveho definovaneho znaku
mov ax, 1100h       ; sluzba
int 10h

mov bh, 16
mov bl, 00h
mov cx, 1
mov dx, 128 ;ascii kod v HEX = 80h
mov ax, @data
mov es, ax
mov ax, 1100h
mov bp, offset defCM
int 10h

mov ax, 4C00h
int 21h
end start
