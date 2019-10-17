/*
   PHYSIC_IO.C

   Modul na fyzicku spravu QFS

   (c) Copyright 2005, P. Jakubco

   modifikovane z emulatora QFS

*/

#include <rtl.h>
#include <physic_i.h>
#include <disk.h>
#include <colors.h>

#define RESET 0
#define READ 2
#define WRITE 3
#define VERIFY 4

/*

0  ³_DISK_RESET  ³ Resets disk system, forcing the drive controller to
   ³             ³ do a hard reset; ignore all other parameters
1  ³_DISK_STATUS ³ Returns the status of the last disk operation;
   ³             ³ ignores all other parameters
2  ³_DISK_READ   ³ Reads one or more disk sectors into memory
3  ³_DISK_WRITE  ³ Writes one or more disk sectors from memory
4  ³_DISK_VERIFY ³ Verifies one or more sectors
5  ³_DISK_FORMAT ³ Formats a track


*/

// globalne premenne v ramci tohoto modulu
int SPT_FDD;
int SECT_COUNT;

// funkcia cita fyzicky z disku
int read_phy(int head, int track, int sector, int nsectors, void *buffer)
{
  int status;

  status = biosdisk(READ,FDD,head,track,sector,nsectors,buffer);
  if (status) {
    biosdisk(RESET,FDD,0,0,0,0,buffer);
    status = biosdisk(READ,FDD,head,track,sector,nsectors,buffer);
  }
  return status;
}

// funkcia cita fyzicky z disku
int write_phy(int head, int track, int sector, int nsectors, void *buffer)
{
  int status;
  status = biosdisk(WRITE,FDD,head,track,sector,nsectors,buffer);
  if (status) {
    biosdisk(RESET,FDD,0,0,0,0,buffer);
    status = biosdisk(WRITE,FDD,head,track,sector,nsectors,buffer);
  }
  return status;
}

// funkcia overi sektor(y) na diskete
int verify_phy(int head, int track, int sector, int nsectors, void *buffer)
{
  int status;
  status = biosdisk(VERIFY,FDD,head,track,sector,nsectors,buffer);
  if (status) {
    biosdisk(RESET,FDD,0,0,0,0,buffer);
    status = biosdisk(VERIFY,FDD,head,track,sector,nsectors,buffer);
  }
  return status;
}

// funkcia zisti pocet sektorov/stopa
int getspt()
{
  char buffer[SECTOR_SIZE];
  int sect[] = {18,15,9};
  int s_count[] = {2880, 2400, 1440};
  int i,status;

  biosdisk(RESET,FDD,0,0,0,0,buffer);
  status = biosdisk(READ,FDD,0,10,1,1,buffer);
  if (status == 0x06)
    biosdisk(READ,FDD,0,0,1,1,buffer);

  for (i = 0; i < (sizeof(sect)/sizeof(int)); ++i) {
    biosdisk(RESET,FDD,0,0,0,0,buffer);
    status = biosdisk(READ,FDD,0,0,sect[i],1,buffer);
    if (status == 0x06)
      status = biosdisk(READ,FDD,0,0,sect[i],1,buffer);
    if (!status) break;
  }
  if (status) {
    textattr(atrERROR);
    cprintf("\n\tDISK(L0): Can't find out SPT for this disk !");
    textattr(atrBORDER);
  }

  SECT_COUNT = s_count[i];
  return sect[i];
}

/*  MODULove FUNkCIE  */
int diskINITPHY()
{
  textattr(atrINIT);
  cprintf("\nInitializing disk control (level 0)...");
  SPT_FDD = getspt();
  textattr(atrBORDER);
  cprintf("\n\tSectors per track: ");
  textattr(atrIMPTEXT);
  cprintf("%d", SPT_FDD);
  if (!SPT_FDD) return -1;
  return 0;
}

void diskDESTROYPHY()
{
  textattr(atrINIT);
  cprintf("\nDestroying disk control (level 0)...");
  textattr(atrBORDER);
  SPT_FDD = 0;
  SECT_COUNT = 0;
}