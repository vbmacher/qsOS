/*
 * CHATT.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <console.h>
#include <rtl.h>
#include <colors.h>
#include <serial.h>

int have_to_send = 0;

void conDRAW_CHATT()
{
  asm {        // cls
    mov ax, 3
    int 0x10
  }
  comSENDNICK();
  textattr(atrBANNER);
  cprintf("\n\t\tWelcome to CHATT console\n");
  
  textattr(atrSUBBANNER);
  cprintf("\n\t\tLet's chatt... (Press F1 for system console)\n");

  textattr(atrBORDER);
}

/*********************** EXPORT *********************************/

// funkcia je konzolou chattu
void conCHATT()
{
  char chatt_buff[80];
  int i;

  // init chatt console - draw
  conDRAW_CHATT();

  while (conACTIVE == 1) {
    textattr(atrCONSOLE);
    cprintf("\n(%s)->(%s): ", myNICK, strangerNICK);
    textattr(atrBORDER);
    cgets(chatt_buff);
    comSENDROW(chatt_buff);
    for (i = 0; i < 80; i++)
      chatt_buff[i] = '\0';
  }
}
