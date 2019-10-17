/*
 * CD.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <console.h>
#include <rtl.h>
#include <colors.h>
#include <app_io.h>

int commandCD(char *dirNAME)
{
  char *fname;
  int status;

  if (!strcmp(dirNAME,"")) {
    cprintf("\n\tUsage: CD [new_path]\n");
    fname = user_input(fname,"\n\rEnter (new) path (.. up-level): ");
    strcpy(cfname, fname);
  }
  else
    strcpy(cfname, dirNAME);
  fname = cfname;
  status = setpath_app(fname);
  if (status) {
    textattr(atrERROR);
    cprintf("\n\tDir '%s' does NOT exist. STATUS: %d", fname,status);
    textattr(atrBORDER);
  }
  cprintf("\n\tNew path: ");
  textattr(atrIMPTEXT);
  cprintf("%s\n", getpath_app());
  textattr(atrBORDER);
  return status;
}
