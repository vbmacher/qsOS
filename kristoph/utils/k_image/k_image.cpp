/*
 * K_IMAGE.CPP
 *
 * Program urobi obraz diskety do suboru.
 *
 * (c) Copyright 2001-2002, P. Jakubco ml.
 *
 */

 /*
  * Relase notes
  * ~~~~~~~~~~~~
  * December 2001 - v0.1 - Initial relase
  * 18.03.2002 - v0.2 - Some bugs fixed and some optimization added
  *
  *
  *
  *
  *
  *
  *
  */

#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#include <mem.h>
#include <alloc.h>

#define SECTORSIZE 512
#define RESET 0  // Resets disk system, forcing the drive controller to
		 // do a hard reset; ignore all other parameters
#define STATUS 1 // Returns the status of the last disk operation;
		 // ignores all other parameters
#define READ 2   // Reads one or more disk sectors into memory
#define WRITE 3  // Writes one or more disk sectors from memory
#define VERIFY 4 // Verifies one or more sectors
#define FORMAT 5 // Formats a track
#define PARAM 8  // Returns the current drive parameters, in buffer's
		 // first 4 bytes

int getspt(int drive)
{
  static int nsect[] = {18, 15, 9};
  char *buffer;
  int i, status;

  buffer = (char *)malloc(SECTORSIZE);
  biosdisk(RESET, drive, 0, 0, 0, 0, buffer);
  status = biosdisk(READ, drive, 0, 10, 1, 1, buffer);
  if (status == 0x06)			/* Door signal change?	*/
    status = biosdisk(READ, drive, 0, 0, 1, 1, buffer);

  for (i = 0; i < sizeof(nsect) / sizeof(int); ++i)
    {
      biosdisk(RESET, drive, 0, 0, 0, 0, buffer);
      status = biosdisk(READ, drive, 0, 0, nsect[i], 1, buffer);
      if (status == 0x06)
	status = biosdisk(READ, drive, 0, 0, nsect[i], 1, buffer);
      if (status == 0x00)
	break;
    }
  free(buffer);

  if (i == sizeof(nsect)/sizeof(int))
    {
      printf("\nNemozem zistit, kolko je sektorov na stopu pre tuto disketu.\n");
      exit(1);
    }
  return(nsect[i]);
}

int ReadData(long nSize, int drive,int nSpt,char *nFN)
 {
   int handle;
   int nRealSector;
   int nHead;
   int nTrack;
   int status;
   char *buffer;
   long i;
   long aSize;

   nTrack=0;
   nRealSector=1;
   nHead=0;
   buffer=(char *)malloc(SECTORSIZE);

   if ((handle = open(nFN, O_WRONLY | O_CREAT | O_TRUNC,S_IREAD | S_IWRITE)) == -1) {
      printf("Chyba otvarania suboru.\n");
      return -1;
   }
   aSize=0;
   printf("\n");
   for (i=1; i <= nSize; i++)
    {
      status = biosdisk(READ,drive,nHead,nTrack,nRealSector,1,buffer);
      if (status != 0) {
	if (status == 0x10)
	  printf("\nChyba citania dat.");
	else if (status == 0x04)
	  printf("\nPozadovany sektor nebol najdeny.");
	else if (status == 0x0A) {
	  printf("\nNajdeny chybny sektor na stope %d, hlava %d, sektor %d.",nTrack,nHead,nRealSector);
	  setmem(buffer,SECTORSIZE,'\0');
	  goto f;
	}
	return -1;
      }
      else
	printf("\rCitam z diskety: Stopa: %02d, Hlava %02d, Sektor %02d",nTrack,nHead,nRealSector);
    f:
      if (write(handle, buffer, SECTORSIZE) != SECTORSIZE) {
	printf("\nChyba zapisovania do suboru (asi je plny disk).");
	return -1;
      }
      aSize += SECTORSIZE;
      nRealSector++;
      if (nRealSector > nSpt) {
	nHead= (nHead + 1) & 1;
	nRealSector = 1;
	if (nHead == 0)
	  nTrack++;
      }
    }
   status = biosdisk(RESET,drive,0,0,0,0,buffer);
   close(handle);
   free(buffer);
   printf("\nUlozene %ld bytov do suboru.", aSize);

   return 0;
}

int main(void)
{
 int spt;
 char *fn;

 printf("\nKristoph Disk Reader (KDR) v0.2b");
 printf("\n(c) Copyright 2001-2002, P. Jakubco ml.\n");
 printf("\nVlozte disketu a stlacte -ENTER-.");

 if (getch() != 13) {
   printf("\nStlacena klavesa nebola -ENTER-, koncim.");
   return -1;
 }

 spt = getspt(0); //0 - A:\
		  //1 - C:\ ...
 printf("\nSektory na stopu: %02d", spt);
 printf("\n\nVlozte nazov suboru:");
 gets(fn);

 printf("\nCitam data z diskety...");
 //2880 - sector count
 ReadData(2880,0,spt,fn);

 printf("\nHotovo.");

 return 0;
}
