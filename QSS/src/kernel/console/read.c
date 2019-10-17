/*
 * READ.C
 *
 * (c) Copyright 2005, vbmacher
 *
 * Funkcia, ktora dokaze precitat subor - zobrazi ho na obrazovku
 * chcem, aby podporovala moznost strankovania
 * Je to jednoduche - po kazdom napr. 20.tom riadku sa pocka na
 * stlacenie klavesy...
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>
#include <app_io.h>

void commandREAD(char *fileNAME)
{
  FileHandle fh;
  char *fname;
  int status;
  char buf[506];
  int riadok,i;
  int bytes;

  if (!strcmp(fileNAME,"")) {
    textattr(atrBORDER);
    cprintf("\n\tUsage: READ [filename]\n");
    fname = user_input(fname,"\n\tEnter (existing) file name: ");
    strcpy(cfname, fname);
  }
  else 
    strcpy(cfname,fileNAME);
  fname = cfname;
  status = fopen_app(&fh, fname);
  if (status) {
    textattr(atrERROR);
    cprintf("\n\tError: File %s does NOT exist",fname);
    textattr(atrBORDER);
    status = 0;
    goto ex;
  }

  // vypis textoveho suboru
  riadok = 0;
  cputs("\n\r");
  while (bytes = fread_app(&fh,505,buf)) {
    buf[505] = '\0';
    for (i = 0; i < bytes; i++) {
      cprintf("%c",buf[i]);
      if (buf[i] == '\n') {
        riadok++;
        if (riadok == 20) {
          cprintf("\n\t\t\t--- Press any key to continue ---\n\n");
          getchar();
          riadok = 0;
        }
      }
    }
  }

ex:
  fclose_app(&fh);
}
