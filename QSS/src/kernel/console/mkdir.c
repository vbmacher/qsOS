/*
 * MKDIR.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>
#include <app_io.h>
#include <logic_io.h>

int commandMKDIR(char *newDIR)
{
  char *fname;
  int status;

  if (!strcmp(newDIR,"")) {
    textattr(atrBORDER);
    cprintf("\n\tUsage: MKDIR [new_dirname]\n");
    fname = user_input(fname,"\n\tEnter (new) dir name: ");
  }
  else {
    strcpy(cfname,newDIR);
    fname = cfname;
  }
  status = create_app(fname,2);
  if (status == DATA_ERROR) {
    textattr(atrERROR);
    cprintf("\n\tError: dir NOT created. Data error. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else if (status == FAIL) {
    textattr(atrERROR);
    cprintf("\n\tError: dir NOT created. Failed. STATUS: %d", status);
    textattr(atrBORDER);
  }
  else {
    textattr(atrOK);
    cprintf("\n\tDir created successfully. STATUS: %d", status);
    textattr(atrBORDER);
  }
  return status;
}
