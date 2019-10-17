/*
 * LEVEL1.C
 *
 * (c) Copyright 2005, vbmacher
 *
 * Tu je zahrnuty PRVY komunikacny protokol (level 1)
 *
 */

#include <serial.h>
#include <rtl.h>
#include <colors.h>


/*

  Level 1 komunikacneho protokolu vykonava riadiacu cast
  fyzickeho portu. Stara sa o to, ktory port je aktualny,
  stara sa o inicializaciu portov a tiez ich detekciu

*/

  #define portCOM1 0x3F8
  #define portCOM2 0x2F8
  #define portCOM3 0x3E8
  #define portCOM4 0x2E8

  CPORT COM[4]; // databaza seriovych portov
  int PORT;
  char serBUF[512];

  // zoznam portov
  int ports[4] = { portCOM1, portCOM2, portCOM3, portCOM4 };

  // delic frekvencie
  int divisor[] = { 0x900, 0x600, 0x417, 0x359, 0x300, 0x180, 0xC0,
		  0x60, 0x40, 0x3A, 0x30, 0x20, 0x18, 0x10, 0xC,
		  0x6, 0x3, 0x2, 0x1 };

  // START signal a STOP signal
  #define charSTART 0x15
  #define charEND 0x16

  #define COM1_ON 0x0EF   // 11101111b (vypne COM 2,4 - 4. bit)
  #define COM2_ON 0x0F7   // 11110111b (vypne COM 1,3 - 3. bit)

  // procedura zisti rychlo informacie o danom porte
  void portINFO(int portNUM) {

    // podla typu pouziteho portu
    // nainstalujem prerusenie podla:
    // COM 1,3: 0x0C
    // COM 2,4: 0x0B

    switch (portNUM) {
      case 0:
      case 2:
        COM[portNUM].uartINT = 0x0C;
        COM[portNUM].uartONMASK = COM1_ON;
        COM[portNUM].uartOFFMASK = ~COM1_ON;
        break;
      case 1:
      case 3:
        COM[portNUM].uartINT = 0x0B;
        COM[portNUM].uartONMASK = COM2_ON;
        COM[portNUM].uartOFFMASK = ~COM2_ON;
        break;
      default:
        // chybny port nastaveny...
        COM[portNUM].uartINT = 0;
        COM[portNUM].uartONMASK = 0;
        COM[portNUM].uartOFFMASK = ~0;
        break;
    }
  }

  // instalacia prerusenia

  void serSETINT(int portNUM)
  {
    deinitUART(COM[portNUM].uartINT, COM[portNUM].uartOLDINT,
               COM[portNUM].uartOFFMASK);
    setINT(COM[portNUM].uartINT, FP_SEG(serINT),
           FP_OFF(serINT), &COM[portNUM].uartOLDINT);
    initUART(COM[portNUM].uartONMASK);

    asm {
      // enable interrupts from the UART
      mov dx, word ptr [PORT]
      add dx, 4               // MCR
      mov al, 00001000b       // OUT2 bit set
      out dx, al
    }
  }


/****************************** EXPORT *************************/

// fcia posle zadane udaje po linke.
// Pouziva 1. level komunikacneho protokolu.
// PREFIX | DATA | POSTFIX
// Data sa potvrdzuju po kazdom prijati co i len bytu
// o konkretne udaje sa stara kom. protokol 2. urovne
void serSEND_DATA(char *buf, int nsize)
{
  int n;

  n = strlen(buf);
  if (!n) return;

  memset(serBUF, 0, 512);
  serBUF[0] = charSTART;
  strcpy(serBUF+1,buf);
  serBUF[n+1] = charEND;

  serTX();
}

// nastavi vlastnosti portu, ktory je v premennej PORT
void serUART_SET(int portNUM)
{
  int divi;
  unsigned char BPS, line_params;

  BPS = COM[portNUM].BPS;
  line_params = COM[portNUM].LINE_PARAMS;
  divi = divisor[BPS];
  asm {
    cli
    mov dx, word ptr [PORT]
    add dx, 3
    mov al, 0x80   // nastav DLAB (menic bps)
    out dx, al
    sub dx, 3
    mov ax, word ptr [divi]
    out dx, ax
    add dx, 3
    mov al, byte ptr [line_params]
    out dx, al
    inc dx
    and al, 00001000b
    out dx, al
  }
}

int serINIT()
{
  int i;

  textattr(atrINIT);
  cprintf("\nInitializing serial communication...");

  textattr(atrBORDER);
  for (i = 3; i >= 0; i--) {
    PORT = ports[i];
    COM[i].UART = detectUART();
    COM[i].IRQ = detectIRQ();
    cprintf("\n\tCOM%d> IRQ: 0x%x, UART: ", i+1, COM[i].IRQ);
    switch (COM[i].UART) {
      case 1:
	cputs("8250");
	break;
      case 2:
	cputs("16450/82450");
	break;
      case 3:
	cputs("16550");
	break;
      case 4:
	cputs("16550A");
	break;
      default:
	cputs("unknown");
        break;
    }
    if (!COM[i].UART) continue;
    COM[i].BPS = bps9600;
    COM[i].LINE_PARAMS = line8n1;
    COM[i].TEST = 0;
    portINFO(i);
  }

  PORT = ports[0];
  serUART_SET(0);
  serSETINT(0);
  return 0;
}
