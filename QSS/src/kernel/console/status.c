/*
 * STATUS.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>
#include <serial.h>

void commandSTATUS()
{
  int i,c;
  unsigned long t1=0, t2=0, a=0;

  switch (PORT) {
    case 0x3F8: i = 0; break;
    case 0x2F8: i = 1; break;
    case 0x3E8: i = 2; break;
    case 0x2E8: i = 3; break;
  }
  textattr(atrBORDER);
  cprintf("\n\tCurrent adapter: ");
  textattr(atrIMPTEXT);
  cprintf("COM%d", i + 1);
  textattr(atrBORDER);
  cprintf("\n\tIRQ            : ");
  textattr(atrIMPTEXT);
  cprintf("0x%x", COM[i].IRQ);
  textattr(atrBORDER);
  cprintf("\n\tUART           : ");
  textattr(atrIMPTEXT);
  switch (COM[i].UART) {
    case 1:
      cputs("8250");
      break;
    case 2:
      cputs("16450/82450");
      break;
    case 3:
      cputs("16550");
      break;
    case 4:
      cputs("16550A");
      break;
    default:
      cputs("unknown");
      break;
  }
  cprintf("\n\tTesting params : ");
  printBPS(i);
  printLINE_PARAMS(i);

  comSTATUS = 0;
  comGETID();

  t1 = time();

  cprintf("\n>>> time: 0 s <<< ");
  do {
    t2 = time();
    if ((t2-t1) != a) {
      a = t2 - t1;
      cprintf("\r>>> time: %lu s <<< ",(t2-t1));
    }
  } while ( ((t2 - t1) <= 3) && !comSTATUS );
  
  if (comSTATUS == 1) {
    textattr(atrOK);
    cputs("ok.");
    textattr(atrBORDER);
  } else {
    textattr(atrERROR);
    cputs("failed.");
    textattr(atrBORDER);
  }
  comGETNICK();
  cprintf("\n\tYour nick      : %s", myNICK);
  cprintf("\n\tStranger's nick: %s", strangerNICK);
}
