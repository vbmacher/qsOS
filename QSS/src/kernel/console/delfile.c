/*
 * DELFILE.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <console.h>
#include <rtl.h>
#include <colors.h>
#include <app_io.h>
#include <logic_io.h>

int commandDELFILE(char *fileNAME)
{
  char *fname;
  int status;

  if (!strcmp(fileNAME,"")) {
    cprintf("\n\tUsage: DELFILE [filename]\n");
    fname = user_input(fname,"\n\tEnter (existing) file name: ");
    strcpy(cfname, fname);
  }
  else
    strcpy(cfname, fileNAME);
  fname = cfname;
  status = remove_app(fname,1);
  if (status == DATA_ERROR) {
    textattr(atrERROR);
    cprintf("\n\tError: file NOT deleted. Data error. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else if (status == FAIL) {
    textattr(atrERROR);
    cprintf("\n\tError: file NOT deleted. Failed. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else {
    textattr(atrOK);
    cprintf("\n\tFile deleted successfully. STATUS: %d", status);
    textattr(atrBORDER);
  }
  return status;
}
