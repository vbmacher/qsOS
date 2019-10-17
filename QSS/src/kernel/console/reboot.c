/*
 * REBOOT.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>
#include <mau.h>

void commandREBOOT()
{
  mauDESTROY();
  textattr(atrINIT);
  cprintf("\n\nRebooting (hot)...");
  asm {
    cli
    xor ax, ax
    mov es, ax
    mov bx, 0x472
    mov word ptr es:[bx], 0x1234
    db 0xEA
    dw 0
    dw 0xFFFF
  }
}
