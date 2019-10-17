/*
 * LEVEL2.C
 *
 * (c) Copyright 2005, vbmacher
 *
 * Protokol level 2
 *
 */

#include <rtl.h>
#include <serial.h>
#include <console.h>
#include <colors.h>


/*
   Level2 sa stara uz o vyhodnocovanie prijatych dat a
   tiez odosielanim potrebnych dat komunikuje s druhym PC.
   Tento level sa stara uz o udaje samotne, nie o prenos dat.
*/

char myNICK[11];       // moj nick
char strangerNICK[11]; // cudzi nick

char BUF[512];

int comSTATUS = 0;         // 0 - error, 1 - ok

// funkcia zisti ake data prisli a vykona prislusne operacie s nimi
// hmm fest problemova funkcia... preco???
void comSCANBUF()
{
  comDATAHDR header;

  memcpy(&header, serBUF, sizeof(comDATAHDR));

  switch (header.type) {
    case typeID:
      if (header.sent == sentSENDING) {
        if (memcmp(serBUF+sizeof(comDATAHDR),"QSS",3)) comSTATUS = 0;
        else comSTATUS = 1;
      }
      else if (header.sent == sentGET) {
        comSENDID();
      }
      break;
    case typeNICK:
      if (header.sent == sentSENDING) {
        memcpy(strangerNICK,serBUF+sizeof(comDATAHDR),strlen(serBUF)-sizeof(comDATAHDR));
        if (conACTIVE) {
          textattr(atrCONSOLE);
          cprintf("\n(%s)->(%s): ", myNICK, strangerNICK);
          textattr(atrBORDER);
        } else if (!conACTIVE) {
          textattr(atrCONSOLE);
          cprintf("\nSYS: ");
          textattr(atrBORDER);
        }
      } 
      break;
    case typeROW:
      if (header.sent == sentSENDING) {
        conGET_DATA(serBUF+sizeof(comDATAHDR));
      }
      else if (header.sent == sentGET) 
        ; // ???
      break;
    default:
      break;
  }
  memset(serBUF,0,512);
}

/*************************** EXPORT ********************************/

void conGET_DATA(char *buf)
{
  if (conACTIVE) {
    textattr(atrMESSAGE);
    cprintf("\nMessage(%s)->(%s):\n", strangerNICK, myNICK);
    textattr(atrBORDER);
    cprintf("%s\n", buf);
    textattr(atrBORDER);
  } else if (!conACTIVE) {
    textattr(atrMESFAILED);
    cprintf("\nYou had message: (%s)->(%s)\n", strangerNICK, myNICK);
    textattr(atrBORDER);
    textattr(atrBORDER);
  }
  if (conACTIVE) {
    textattr(atrCONSOLE);
    cprintf("\n(%s)->(%s): ", myNICK, strangerNICK);
    textattr(atrBORDER);
  } else if (!conACTIVE) {
    textattr(atrCONSOLE);
    cprintf("\nSYS: ");
    textattr(atrBORDER);
  }
}

// funkcia posle prikaz na zistenie nicku
void comGETNICK()
{
  comDATAHDR header;

  header.type = typeNICK;
  header.sent = sentGET;
  serSEND_DATA((char *)&header, sizeof(comDATAHDR));
}

void comGETID()
{
  comDATAHDR header;

  header.type = typeID;
  header.sent = sentGET;
  serSEND_DATA((char *)&header, sizeof(comDATAHDR));
}

void comSENDNICK()
{
  comDATAHDR header;

  memset(BUF, 0, 512);
  header.type = typeNICK;
  header.sent = sentSENDING;
  memcpy(BUF, &header, sizeof(comDATAHDR));
  memcpy(BUF+sizeof(comDATAHDR),myNICK, strlen(myNICK)+1);
  serSEND_DATA(BUF,strlen(myNICK)+sizeof(comDATAHDR)+1);
}

void comSENDID()
{
  comDATAHDR header;

  memset(BUF, 0, 512);
  header.type = typeID;
  header.sent = sentSENDING;
  memcpy(BUF, &header, sizeof(comDATAHDR));
  memcpy(BUF+sizeof(comDATAHDR),"QSS", 4);
  serSEND_DATA(BUF,sizeof(comDATAHDR)+4);
}

void comSENDROW(char *buf)
{
  comDATAHDR header;

  memset(BUF, 0, 512);
  header.type = typeROW;
  header.sent = sentSENDING;
  memcpy(BUF, &header,sizeof(comDATAHDR));
  memcpy(BUF+sizeof(comDATAHDR), buf, strlen(buf));
  serSEND_DATA(BUF, sizeof(comDATAHDR)+strlen(buf)+1);
}


// inicializacia komunikacie
void comINIT()
{
  comGETID();
  textattr(atrBORDER);
  cprintf("\n\nEnter your nick (max.10 chars): ");

  memset(strangerNICK,0,11);
  memset(myNICK,0,11);

  cgets(myNICK);
  comSENDNICK();
  comGETNICK();
}
