/*
 * HELP.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>

void commandHELP()
{
  textattr(atrSUBBANNER);
  cprintf("\n\tAvailable commands:");
  textattr(atrBORDER);
  cprintf("\n\tHALT    - halt processor, disable interrupts");
  cprintf("\n\tHELP    - show this screen");
  cprintf("\n\tLINE    - change line parameters");
  cprintf("\n\tNICK    - change current nick name");
  cprintf("\n\tREBOOT  - reboot computer");
  cprintf("\n\tSTATUS  - show status of current COM port adapter and line parameters");
  cprintf("\n\tDIR     - show current directory content");
  cprintf("\n\tMKDIR   - make a directory");
  cprintf("\n\tREAD    - show file content");
  cprintf("\n\tDELFILE - delete a file");
  cprintf("\n\tDELDIR  - delete a directory");
  cprintf("\n\tCD      - change current path");
  cprintf("\n\tFORMAT  - format a diskette");
  cprintf("\n\tRUN     - run executable file");
  cprintf("\n\tDUMPKERNEL - dump kernel to a file");
}
