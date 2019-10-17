/*
 * CONSOLE.H
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#ifndef __CONSOLE__
#define __CONSOLE__

  extern char conACTIVE;
  extern unsigned char conSCANCODE;
  extern int haveto_draw;
  extern char cfname[16];

  void conSYSTEM();
  void conCHATT();
  void conGET_DATA(char *buf);

  void commandHALT();
  void commandLINE();
  void commandSTATUS();
  void commandNICK();
  void commandREBOOT();
  void commandDIR();
  int commandMKDIR(char *newDIR);
  void commandREAD(char *fileNAME);
  int commandDELFILE(char *fileNAME);
  int commandDELDIR(char *dirNAME);
  int commandCD(char *dirNAME);
  void commandFORMAT();
  void commandRUN(char *fileNAME);
  void commandDUMPKERNEL();

  // system.c
  #define tokHALT 0
  #define tokLINE 1
  #define tokSTATUS 2
  #define tokNICK 3
  #define tokREBOOT 4
  #define tokHELP 5
  #define tokDIR 6
  #define tokMKDIR 7
  #define tokREAD 8
  #define tokDELFILE 9
  #define tokDELDIR 10
  #define tokCD 11
  #define tokFORMAT 12
  #define tokRUN 13
  #define tokDUMPKERNEL 14
  #define tokUNKNOWN 255

  struct sys_commands {
    char sysNAME[11];
    int sysTOKEN;
  };

  // misc.c
  char *user_input(char *fname, char *text);
  void printBPS(int ind);
  void printLINE_PARAMS(int ind);

#endif
