/*
 * PROCESS.H
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#ifndef __PROCESS__
#define __PROCESS__

  int loadPROCESS(char *fileNAME);
  void runPROCESS(char *buf);
  void freePROCESS();

  void procSET_INT();

#endif
