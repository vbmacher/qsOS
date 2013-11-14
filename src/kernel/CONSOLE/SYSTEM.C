/*
 * SYSTEM.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <console.h>
#include <rtl.h>
#include <colors.h>
#include <serial.h>

int bpointer;
char ARGUMENT[51];  // max. 50 znakov na prikaz/argument
char cfname[16];    // meno suboru pomocna premenna

int haveto_draw = 0;  // 0 = draw console, 1 = not draw


struct sys_commands sys_table[] = {"HALT", tokHALT,
                                   "LINE", tokLINE,
                                   "STATUS", tokSTATUS,
                                   "NICK", tokNICK,
                                   "REBOOT", tokREBOOT,
                                   "HELP", tokHELP,
                                   "DIR", tokDIR,
                                   "MKDIR", tokMKDIR,
                                   "READ", tokREAD,
                                   "DELFILE", tokDELFILE,
                                   "DELDIR", tokDELDIR,
                                   "CD", tokCD,
                                   "FORMAT", tokFORMAT,
                                   "RUN", tokRUN,
                                   "DUMPKERNEL", tokDUMPKERNEL};


// vypise uvodny text
void conDRAW_SYSTEM()
{
  int i;

  asm {        // cls
    mov ax, 3
    int 0x10
  }
  textattr(atrBANNER);
  cprintf("\n\t\tWelcome to system console\n");
  
  textattr(atrSUBBANNER);
  cprintf("\n\t\tFor help type 'help'. (Press F2 for chatt console)\n");

  textattr(atrBORDER);
}

// nacita nasledujuci (alebo prvy) argument
void getARG(char *buf)
{
  int i,j=0;

  buf += bpointer;
  for (i = 0; i < 11; i++)
    ARGUMENT[i] = '\0';
  i = 0;
  if (!bpointer) j = 1;
  while (iswhite(*buf)) { 
    buf++; bpointer++;
  }
  while (!iswhite(*buf) && (*buf)) {
    if (j)
      (*buf) = toupper(*buf);
    ARGUMENT[i++] = *buf;
    buf++;
    bpointer++;
  }
  ARGUMENT[i] = '\0';
}

// funkcia najde prikaz v tabulke
int lookCOMMAND()
{
  int i;
  for (i = 0; *sys_table[i].sysNAME; i++)
    if (!strcmp(sys_table[i].sysNAME, ARGUMENT))
      return sys_table[i].sysTOKEN;
  return tokUNKNOWN;
}

void conSYS_SCANBUF(char *buf)
{
  int tok;

  bpointer = 0;
  getARG(buf);
  tok = lookCOMMAND();

  if (!(ARGUMENT[0])) return;
  if (tok == tokUNKNOWN) {
    textattr(atrERROR);
    cprintf("\n\t(SYS): Unknown command '%s'", ARGUMENT);
    textattr(atrBORDER);
    return;
  }
  cputs("\n");
  switch (tok) {
    case tokHALT:
      commandHALT();
      break;
    case tokLINE:
      commandLINE();
      break;
    case tokSTATUS:
      commandSTATUS();
      break;
    case tokNICK: 
      commandNICK(0);
      break;
    case tokREBOOT:
      commandREBOOT();
      break;
    case tokHELP:
      commandHELP();
      break;
    case tokDIR:
      commandDIR();
      break;
    case tokMKDIR:
      getARG(buf);
      commandMKDIR(ARGUMENT);
      break;
    case tokREAD:
      getARG(buf);
      commandREAD(ARGUMENT);
      break;
    case tokDELFILE:
      getARG(buf);
      commandDELFILE(ARGUMENT);
      break;
    case tokDELDIR:
      getARG(buf);
      commandDELDIR(ARGUMENT);
      break;
    case tokCD:
      getARG(buf);
      commandCD(ARGUMENT);
      break;
    case tokFORMAT:
      commandFORMAT();
      break;
    case tokRUN:
      getARG(buf);
      commandRUN(ARGUMENT);
      break;
    case tokDUMPKERNEL:
      commandDUMPKERNEL();
      break;
  }
}

/*********************** EXPORT *********************************/

// konzola prikazov
void conSYSTEM()
{
  char sys_buff[80];
  int i;

  if (!haveto_draw)
    conDRAW_SYSTEM();

  while (conACTIVE == 0) {
    textattr(atrCONSOLE);
    cprintf("\nSYS: ");
    textattr(atrBORDER);
    cgets(sys_buff);
    conSYS_SCANBUF(sys_buff);
    for (i = 0; i < 80; i++)
      sys_buff[i] = '\0';
  }
}
