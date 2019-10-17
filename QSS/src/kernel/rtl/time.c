/*
 * TIME.C
 *
 * (c) Copyright 2006, vbmacher
 *
 */


#include <rtl.h>

unsigned long time()
{
  unsigned long c=0;

  asm {
    push cx
    xor ah, ah
    int 1Ah

    mov word ptr [c], dx
    mov word ptr [c+2], cx
  }

  c %= 0x10007;
  c %= 0x444;
  c /= CLK_TCK;

  asm pop cx
  return c;
}