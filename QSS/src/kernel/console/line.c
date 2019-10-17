/*
 * LINE.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <rtl.h>
#include <colors.h>
#include <console.h>
#include <serial.h>

/************************* EXPORT *******************************/

void commandLINE()
{
  int i, bps, dbits, sbits, parity, t;
  int portNUM;
  switch (PORT) {
    case 0x3F8: i = 0; break;
    case 0x2F8: i = 1; break;
    case 0x3E8: i = 2; break;
    case 0x2E8: i = 3; break;
  }

  portNUM = i;
  textattr(atrBORDER);
  cprintf("\n\tCurrent port is                : ");
  textattr(atrIMPTEXT);
  cprintf("COM%d (0x%x)", i+1, PORT);
  textattr(atrBORDER);
  cprintf("\n\tEnter new port (1-4)           : ");
  i = getchar();
  cprintf("%c",i);
  switch (i) {
    case '1':
     i = 0;
     break;
    case '2':
     i = 1;
     break;
    case '3':
     i = 2;
     break;
    case '4':
     i = 3;
     break;
  }
  PORT = ports[i];
  printBPS(i);

  cprintf("\n\t1 - 50    | 6 - 300  | b - 2400 | g - 19200");
  cprintf("\n\t2 - 75    | 7 - 600  | c - 3600 | h - 38400");
  cprintf("\n\t3 - 110   | 8 - 1200 | d - 4800 | i - 57600");
  cprintf("\n\t4 - 134.5 | 9 - 1800 | e - 7200 | j - 115200");
  cprintf("\n\t5 - 150   | a - 2000 | f - 9600 |");

  cprintf("\n\tEnter new BPS                  : ");
  bps = getchar();
  cprintf("%c",bps);

  switch (bps) {
    case '1':
      COM[i].BPS = 0;
      break;
    case '2':
      COM[i].BPS = 1;
      break;
    case '3':
      COM[i].BPS = 2;
      break;
    case '4':
      COM[i].BPS = 3;
      break;
    case '5':
      COM[i].BPS = 4;
      break;
    case '6':
      COM[i].BPS = 5;
      break;
    case '7':
      COM[i].BPS = 6;
      break;
    case '8':
      COM[i].BPS = 7;
      break;
    case '9':
      COM[i].BPS = 8;
      break;
    case 'a':
      COM[i].BPS = 9;
      break;
    case 'b':
      COM[i].BPS = 10;
      break;
    case 'c':
      COM[i].BPS = 11;
      break;
    case 'd':
      COM[i].BPS = 12;
      break;
    case 'e':
      COM[i].BPS = 13;
      break;
    case 'f':
      COM[i].BPS = 14;
      break;
    case 'g':
      COM[i].BPS = 15;
      break;
    case 'h':
      COM[i].BPS = 16;
      break;
    case 'i':
      COM[i].BPS = 17;
      break;
    case 'j':
      COM[i].BPS = 18;
      break;
  }

  printLINE_PARAMS(i);
  cprintf("\n\tEnter data bits count (5,6,7,8): ");
  dbits = getchar();
  cprintf("%c",dbits);
  cprintf("\n\tEnter stop bits count (1,2)    : ");
  sbits = getchar();
  cprintf("%c",sbits);
  cprintf("\n\tEnter parity (e,o,m,s,n)       : ");
  parity = getchar();
  cprintf("%c",parity);

  switch (dbits) {
    case '5':
      switch (sbits) {
        case '1':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line5e1;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line5o1;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line5m1;
              break;
            case 's':
              COM[i].LINE_PARAMS = line5s1;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line5n1;
              break;
            default:
              break;
          }
          break;
        case '2':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line5e2;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line5o2;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line5m2;
              break;
            case 's':
              COM[i].LINE_PARAMS = line5s2;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line5n2;
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
      break;
    case '6':
      switch (sbits) {
        case '1':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line6e1;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line6o1;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line6m1;
              break;
            case 's':
              COM[i].LINE_PARAMS = line6s1;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line6n1;
              break;
            default:
              break;
          }
          break;
        case '2':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line6e2;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line6o2;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line6m2;
              break;
            case 's':
              COM[i].LINE_PARAMS = line6s2;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line6n2;
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
      break;
    case '7':
      switch (sbits) {
        case '1':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line7e1;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line7o1;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line7m1;
              break;
            case 's':
              COM[i].LINE_PARAMS = line7s1;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line7n1;
              break;
            default:
              break;
          }
          break;
        case '2':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line7e2;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line7o2;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line7m2;
              break;
            case 's':
              COM[i].LINE_PARAMS = line7s2;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line7n2;
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
      break;
    case '8':
      switch (sbits) {
        case '1':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line8e1;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line8o1;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line8m1;
              break;
            case 's':
              COM[i].LINE_PARAMS = line8s1;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line8n1;
              break;
            default:
              break;
          }
          break;
        case '2':
          switch (parity) {
            case 'e':
              COM[i].LINE_PARAMS = line8e2;
              break;
            case 'o':
              COM[i].LINE_PARAMS = line8o2;
              break;
            case 'm':
              COM[i].LINE_PARAMS = line8m2;
              break;
            case 's':
              COM[i].LINE_PARAMS = line8s2;
              break;
            case 'n':
              COM[i].LINE_PARAMS = line8n2;
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  COM[i].TEST = 0;
  deinitUART(COM[portNUM].uartINT, COM[portNUM].uartOLDINT,
             COM[portNUM].uartOFFMASK);
  serUART_SET(i);
  serSETINT(i);
}
