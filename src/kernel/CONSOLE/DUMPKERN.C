/*
 * DUMPKERNEL.C
 *
 * (c) Copyright 2005, vbmacher
 *
 * Prikaz sluzi na to, aby som zistil, ci sa nacital cely kernel
 * do pamate tak, ako by mal.
 *
 */

#include <rtl.h>
#include <colors.h>
#include <app_io.h>

void commandDUMPKERNEL()
{
  FileHandle fh;
  int status;
  char far *s;

  status = create_app("kernel.dmp",1);
  status = fopen_app(&fh, "kernel.dmp");
  status = fwrite_app(&fh, 25000, MK_FP(0x1000,0));
  status = fclose_app(&fh);
}