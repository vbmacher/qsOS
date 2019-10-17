/*
 * FORMAT.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <logic_io.h>

// funkcia naformatuje disketu
int commandFORMAT()
{
  int status;

  textattr(colRED);
  cprintf("\n\tALL DATA ON DISK WILL BE LOST !");
  cprintf("\n\tDo you want to continue [Y/N] ?");
  textattr(atrBORDER);
  status = getchar();
  if ((status == 'Y') || (status == 'y')) ;
  else return FAIL;

  status = format_log();
  if (status == DATA_ERROR) {
    textattr(atrERROR);
    cprintf("\n\tFormat incomplete. Data error. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else if (status == FAIL) {
    textattr(atrERROR);
    cprintf("\n\tFormat incomplete. Failed. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else {
    textattr(atrOK);
    cprintf("\n\tFormat complete. STATUS: %d", status);
    textattr(atrBORDER);
  }
  return status;
}

