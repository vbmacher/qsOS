/*
 * MISC.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>
#include <serial.h>

// funkcia cita z klavesnice text do fname
// pred tym, ako caka na vstup, vypise na obrazovku text (prem. text)
char *user_input(char *fname, char *text)
{
  cprintf(text);
  cgets(cfname);
  fname = cfname;
  return cfname;
}

void printBPS(int ind)
{
  textattr(atrBORDER);
  cprintf("\n\tCurrent BPS: ");
  textattr(atrIMPTEXT);
  switch (COM[ind].BPS) {
    case bps50:
      cputs("50");
      break;
    case bps75:
      cputs("75");
      break;
    case bps110:
      cputs("110");
      break;
    case bps134_5:
      cputs("134.5");
      break;
    case bps150:
      cputs("150");
      break;
    case bps300:
      cputs("300");
      break;
    case bps600:
      cputs("600");
      break;
    case bps1200:
      cputs("1200");
      break;
    case bps1800:
      cputs("1800");
      break;
    case bps2000:
      cputs("2000");
      break;
    case bps2400:
      cputs("2400");
      break;
    case bps3600:
      cputs("3600");
      break;
    case bps4800:
      cputs("4800");
      break;
    case bps7200:
      cputs("7200");
      break;
    case bps9600:
      cputs("9600");
      break;
    case bps19200:
      cputs("19200");
      break;
    case bps38400:
      cputs("38400");
      break;
    case bps57600:
      cputs("57600");
      break;
    case bps115200:
      cputs("115200");
      break;
  }
  textattr(atrBORDER);
}

void printLINE_PARAMS(int ind)
{
  int dbits, sbits;
  char parity;

  textattr(atrBORDER);
  cprintf("\n\tCurrent line parameters: ");
  textattr(atrIMPTEXT);
  switch (COM[ind].LINE_PARAMS) {
    case line5e1:
      dbits = 5;
      sbits = 1;
      parity = 'e';
      break;
    case line5e2:
      dbits = 5;
      sbits = 2;
      parity = 'e';
      break;
    case line5o1:
      dbits = 5;
      sbits = 1;
      parity = 'o';
      break;
    case line5o2:
      dbits = 5;
      sbits = 2;
      parity = 'o';
      break;
    case line5m1:
      dbits = 5;
      sbits = 1;
      parity = 'm';
      break;
    case line5m2:
      dbits = 5;
      sbits = 2;
      parity = 'm';
      break;
    case line5s1:
      dbits = 5;
      sbits = 1;
      parity = 's';
      break;
    case line5s2:
      dbits = 5;
      sbits = 2;
      parity = 's';
      break;
    case line5n1:
      dbits = 5;
      sbits = 1;
      parity = 'n';
      break;
    case line5n2:
      dbits = 5;
      sbits = 2;
      parity = 'n';
      break;
    case line6e1:
      dbits = 6;
      sbits = 1;
      parity = 'e';
      break;
    case line6e2:
      dbits = 6;
      sbits = 2;
      parity = 'e';
      break;
    case line6o1:
      dbits = 6;
      sbits = 1;
      parity = 'o';
      break;
    case line6o2:
      dbits = 6;
      sbits = 2;
      parity = 'o';
      break;
    case line6m1:
      dbits = 6;
      sbits = 1;
      parity = 'm';
      break;
    case line6m2:
      dbits = 6;
      sbits = 2;
      parity = 'm';
      break;
    case line6s1:
      dbits = 6;
      sbits = 1;
      parity = 's';
      break;
    case line6s2:
      dbits = 6;
      sbits = 2;
      parity = 's';
      break;
    case line6n1:
      dbits = 6;
      sbits = 1;
      parity = 'n';
      break;
    case line6n2:
      dbits = 6;
      sbits = 2;
      parity = 'n';
      break;
    case line7e1:
      dbits = 7;
      sbits = 1;
      parity = 'e';
      break;
    case line7e2:
      dbits = 7;
      sbits = 2;
      parity = 'e';
      break;
    case line7o1:
      dbits = 7;
      sbits = 1;
      parity = 'o';
      break;
    case line7o2:
      dbits = 7;
      sbits = 2;
      parity = 'o';
      break;
    case line7m1:
      dbits = 7;
      sbits = 1;
      parity = 'm';
      break;
    case line7m2:
      dbits = 7;
      sbits = 2;
      parity = 'm';
      break;
    case line7s1:
      dbits = 7;
      sbits = 1;
      parity = 's';
      break;
    case line7s2:
      dbits = 7;
      sbits = 2;
      parity = 's';
      break;
    case line7n1:
      dbits = 7;
      sbits = 1;
      parity = 'n';
      break;
    case line7n2:
      dbits = 7;
      sbits = 2;
      parity = 'n';
      break;
    case line8e1:
      dbits = 8;
      sbits = 1;
      parity = 'e';
      break;
    case line8e2:
      dbits = 8;
      sbits = 2;
      parity = 'e';
      break;
    case line8o1:
      dbits = 8;
      sbits = 1;
      parity = 'o';
      break;
    case line8o2:
      dbits = 8;
      sbits = 2;
      parity = 'o';
      break;
    case line8m1:
      dbits = 8;
      sbits = 1;
      parity = 'm';
      break;
    case line8m2:
      dbits = 8;
      sbits = 2;
      parity = 'm';
      break;
    case line8s1:
      dbits = 8;
      sbits = 1;
      parity = 's';
      break;
    case line8s2:
      dbits = 8;
      sbits = 2;
      parity = 's';
      break;
    case line8n1:
      dbits = 8;
      sbits = 1;
      parity = 'n';
      break;
    case line8n2:
      dbits = 8;
      sbits = 2;
      parity = 'n';
      break;
  }
  cprintf("%d data bits, %d stop bits, ",dbits,sbits);
  switch (parity) {
    case 'n':
      cputs("none");
      break;
    case 'e':
      cputs("even");
      break;
    case 'o':
      cputs("odd");
      break;
    case 'm':
      cputs("mark");
      break;
    case 's':
      cputs("space");
      break;
  }
  cputs(" parity");
  textattr(atrBORDER);
}
