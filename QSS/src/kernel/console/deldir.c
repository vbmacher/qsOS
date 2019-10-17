/*
 * DELDIR.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <console.h>
#include <rtl.h>
#include <colors.h>
#include <app_io.h>
#include <logic_io.h>

int commandDELDIR(char *fileNAME)
{
  char *fname;
  int status;

  if (!strcmp(fileNAME,"")) {
    cprintf("\n\tUsage: DELDIR [dirname]\n");
    fname = user_input(fname,"\n\tEnter (existing) dir name: ");
    strcpy(cfname, fname);
  }
  else
    strcpy(cfname,fileNAME);
  fname = cfname;
  status = remove_app(fname,2);
  if (status == DATA_ERROR) {
    textattr(atrERROR);
    cprintf("\n\tError: dir NOT deleted. Data error. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else if (status == FAIL) {
    textattr(atrERROR);
    cprintf("\n\tError: dir NOT deleted. Failed. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else {
    textattr(atrOK);
    cprintf("\n\tDir deleted successfully. STATUS: %d", status);
    textattr(atrBORDER);
  }
  return status;
}
