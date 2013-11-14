/*
 * RUN.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <process.h>
#include <colors.h>

void commandRUN(char *fileNAME)
{
  if (!strcmp(fileNAME,"")) {
    textattr(atrBORDER);
    cprintf("\n\tUsage: RUN filename\n");
    return;
  }
  cputs("\n\r");
  loadPROCESS(fileNAME);
}
