/*
 * SERIAL.H
 *
 * (c) Copyright 2005, vbmacher
 *
 * seriova komunikacia
 *
 */

#ifndef __SERIAL__
#define __SERIAL__

  // indexy delica (divisor) pre bps
  #define bps50 0
  #define bps75 1
  #define bps110 2
  #define bps134_5 3
  #define bps150 4
  #define bps300 5
  #define bps600 6
  #define bps1200 7
  #define bps1800 8
  #define bps2000 9
  #define bps2400 10
  #define bps3600 11
  #define bps4800 12
  #define bps7200 13
  #define bps9600 14
  #define bps19200 15
  #define bps38400 16
  #define bps57600 17
  #define bps115200 18

  // vsetky mozne parametre linky
  #define line5n1 0
  #define line5n2 4  // std.
  #define line5e1 24
  #define line5e2 28
  #define line5o1 8
  #define line5o2 12
  #define line5m1 40
  #define line5m2 44
  #define line5s1 56
  #define line5s2 60
  #define line6n1 1
  #define line6n2 5
  #define line6e1 25
  #define line6e2 29
  #define line6o1 9
  #define line6o2 13
  #define line6m1 41
  #define line6m2 45
  #define line6s1 57
  #define line6s2 61
  #define line7n1 2
  #define line7n2 6
  #define line7e1 26
  #define line7e2 30
  #define line7o1 10
  #define line7o2 14
  #define line7m1 42
  #define line7m2 46
  #define line7s1 58
  #define line7s2 62
  #define line8n1 3   // std.
  #define line8n2 7
  #define line8e1 27  // std.
  #define line8e2 31
  #define line8o1 11
  #define line8o2 15
  #define line8m1 43
  #define line8m2 47
  #define line8s1 59
  #define line8s2 63

  // detect.asm

  #ifdef __cplusplus
    extern "C" {
  #endif

    int detectUART();
    int detectIRQ();

  #ifdef __cplusplus
    };
  #endif

  // serial.asm
  void serINT();
  void serTX();


  // level1.c

  typedef struct {
    int UART;
    int IRQ;
    int BPS;
    unsigned char LINE_PARAMS;
    unsigned char TEST;
    unsigned char uartONMASK;
    unsigned char uartOFFMASK;
    unsigned char uartINT;
    unsigned long uartOLDINT;
  } CPORT;

  extern CPORT COM[];
  extern int PORT;
  extern int ports[];
  extern char serBUF[512];

  int serINIT();
  void serUART_SET(int portNUM);
  void serSETINT(int portNUM);

  int serGET_CHAR();
  void serSEND_DATA(char *buf, int nsize);

  // level2.c
  #define typeID 01
  #define typeNICK 02
  #define typeROW 03
  #define sentSENDING 01
  #define sentGET 02
 
  typedef struct {
    unsigned char type;
    unsigned char sent;
  } comDATAHDR;

  extern int comSTATUS;  // 0 - error, 1 - ok
  extern char myNICK[11];
  extern char strangerNICK[11];

  void comGETNICK();
  void comGETID();
  void comSENDNICK();
  void comSENDID();
  void comSENDROW(char *buf);
  void comSCANBUF();
  void comINIT();

#endif
