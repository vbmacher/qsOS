/*
 * DIR.C
 *
 * (c) Copyright 2005, vbmacher
 *
 * Funkcia je modifikovana z emulatora QFS
 *
 */

#include <rtl.h>
#include <colors.h>
#include <app_io.h>

// fcia zobrazi strukturu aktualneho adresara
void commandDIR()
{
  char fn[12];
  int n,fcount,ftype;

  textattr(atrBORDER);
  cprintf("\n\tPath: ");
  textattr(atrIMPTEXT);
  cprintf("%s\n", getpath_app());

  textattr(atrDIR);
  fcount = filecount_app();
  if (PATH > 0)
    cprintf("\n\t..");
  for (n = 0; n < fcount; n++) {
    ftype = getfname_app(fn,n);
    if (ftype == 2)
      cprintf("\n\t%s", fn);
  }
  textattr(atrFILE);
  for (n = 0; n < fcount; n++) {
    ftype = getfname_app(fn,n);
    if (ftype == 1)
      cprintf("\n\t%s | %ld B", fn, filesize_app(fn));
  }              
  textattr(atrBORDER);
  cprintf("\n\n\tFile count: ");
  textattr(atrIMPTEXT);
  cprintf("%d\n", fcount);
}

