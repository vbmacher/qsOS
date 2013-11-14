/*
 * PROCESS.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <mau.h>
#include <colors.h>
#include <app_io.h>
#include <logic_io.h>
#include <console.h>
#include <process.h>

char *proc_addr;
static char cfname[16];

// Nacita nejaky proces do pamate - subor s max. velkostou nejakych  
int loadPROCESS(char *fileNAME)
{
  FileHandle fh;
  char *fname;
  int status;
  long memcap;     // kapacita pamate
  long filesize;   // velkost suboru
  int bytes;
  long buf_pointer;

  if (!strcmp(fileNAME,"")) {
    textattr(atrERROR);
    cprintf("\n\tPAU: Error, empty file name\n");
    textattr(atrBORDER);
    return FAIL;
  }
  else 
    strcpy(cfname,fileNAME);
  fname = cfname;
  status = fopen_app(&fh, fname);
  if (status) {
    textattr(atrERROR);
    cprintf("\n\tPAU: File %s does NOT exist",fname);
    textattr(atrBORDER);
    return FAIL;
  }

  // test velkosti
  memcap = coreleft();
  filesize = fh.fsize;
  
  if (memcap < filesize) {
    textattr(atrERROR);
    cprintf("\n\tPAU: Low memory");
    textattr(atrBORDER);
    status = FAIL;
    goto ex;
  }

  // alokacia pamate
  proc_addr = malloc(filesize);
  buf_pointer = 0;

  // citanie subora do bufferu
  while (bytes = (int)fread_app((FileHandle *)&fh,505,(char *)(proc_addr+buf_pointer)))
    buf_pointer += bytes;

ex:
  fclose_app(&fh);
  if (status) return status;

  // proces je pripraveny na spustenie.
  runPROCESS(proc_addr);
  return status;
}

// spusti proces
// !!! POZOR !!! ZIADNA KONTROLA
// umoznuje spustat jedine COM subory
// zasobnik nechava tak...

void runPROCESS(char *buf)
{
  int xseg, xoff;

  xseg = FP_SEG(buf);
  xoff = FP_OFF(buf);

  xseg -= 0x10;   // com programy maju org100h (v direktive .startup)
  xoff += 0x100;
  haveto_draw = 1;

  asm {
    mov ax, word ptr [xseg]
    mov es, ax
    mov ds, ax
    push ax
    mov ax, word ptr [xoff]
    push ax
    retf
  }
}

void freePROCESS()
{
  asm {
    mov ax, cs
    mov es, ax
    mov ds, ax
  }
  free((void far **)proc_addr);
}