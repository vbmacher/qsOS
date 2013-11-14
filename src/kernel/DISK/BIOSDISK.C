/*
 *  BIOSDISK.C
 *
 * (c) Copyright 2005, vbmacher
 * Copyright (c) 1987, 1992 by Borland International
 * All Rights Reserved.
 *
 * Modified from BorlandC++ v3.1 CRTL library
 *
 */

#pragma inline
#include <asmrules.h>
#include <disk.h>

int biosdisk(int cmd, int drive, int head, int track,
             int sector, int nsects, void *buffer)
{
  #if !(LDATA)
    asm     push    ds
    asm     pop     es
  #endif
  asm     mov     ah, cmd
  asm     mov     al, nsects
  asm     LES_    bx, buffer
  asm     mov     cx, track
  asm     shr     cx, 1
  asm     shr     cx, 1
  asm     and     cl, 0C0h
  asm     add     cl, sector
  asm     mov     ch, track
  asm     mov     dh, head
  asm     mov     dl, drive
  asm     int     013h
  asm     cmp     BY0(cmd), 8
  asm     jne     BiosDiskEnd
  asm     LES_    bx, buffer
  asm     mov     W0(ES_ [bx]), cx
  asm     mov     W1(ES_ [bx]), dx
BiosDiskEnd:
  return _AH;
}
