/*
 * NICK.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>
#include <serial.h>

void commandNICK(int arg)
{
  if (!arg) {
    textattr(atrBORDER);
    cprintf("\n\tCurrent nick is: ");
    textattr(atrIMPTEXT);
    cprintf("%s", myNICK);
    textattr(atrBORDER);
    cprintf("\n\tEnter new nick : ");
    cgets(&myNICK);
  }
  comSENDNICK();
}
