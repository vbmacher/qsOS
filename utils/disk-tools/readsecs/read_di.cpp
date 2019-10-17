#include <alloc.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#include <string.h>


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

int getspt(int (drive))
{
  static int nsect[] = {18, 15, 9};
  char *buffer;
  int i, status;

/*
 Read sector 1, head 0, track 0 to get the BIOS running.
*/
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
      getch();
      exit(1);
    }
  return(nsect[i]);
}

int ReadData(long nSize, int drive,int nSpt,char *nFN)
 {
   int handle;
   int res;
   long nRealSector;
   int nHead;
   int nTrack;
   int status;
   char *buffer;
   long i;
   double aSize;

   nTrack=0;
   nRealSector=1;
   nHead=0;
   buffer=(char *)malloc(SECTORSIZE);

   if ((handle = open(nFN, O_WRONLY | O_CREAT | O_TRUNC,
			 S_IREAD | S_IWRITE)) == -1)
   {
      printf("Chyba otvarania suboru.\n");
      exit(1);
   }
   aSize=0;
   for (i=1 ; i<=nSize;i++)
    {
      status=biosdisk(READ,drive,nHead,nTrack,nRealSector,1,buffer);
      if (status != 0)
	     {
		if (status==0x10)
		   printf("Chyba citania dat.\n");
		else if (status==0x04)
		   printf("Pozadovany sektor nebol najdeny.\n");
		else if (status==0x0A)
		  {
		   printf("Najdeny chybny sektor na stope %d, hlava %d, sektor %lg.\n",nTrack,nHead,(double)nRealSector);
		   free(buffer);
		   buffer=(char *)malloc(SECTORSIZE);
		   goto f;
		   }
		exit(1);
	     }

f:
   if ((res=write(handle, buffer, SECTORSIZE)) != SECTORSIZE)
   {
      printf("Chyba zapisovania do suboru (asi je plny disk).\n");
      exit(1);
   }
	aSize+=(double)res;
	nRealSector++;
	 if (nRealSector>nSpt)
	   {
	     nHead=(nHead+1)&1;
	     nRealSector=1;
	     if (nHead==0)
		 ++nTrack;
	   }
 }
   status=biosdisk(RESET,drive,0,0,0,0,buffer);
   close(handle);
   free(buffer);
   printf("Ulozene %lg bytov do suboru.\n", aSize);

return 0;
}

void main()
{
int spt;
char c;
char *fn;

printf("\nKristoph Disk Reader (KDR) verzia 1.00");
printf("\n(c) Copyright 2001, P. Jakubco ml.\n\n");
printf("Vlozte disketu a stlacte -ENTER-.\n");
c=getch();
if (c!=13)
 { printf("\nStlacena klavesa nebola -ENTER-, koncim...");
   exit(1);
 }

spt=getspt(0); //0 - A:\
	       //1 - C:\ ...
printf ("Sektory na stopu: %02d\n",spt);

printf("\n\nVlozte subor:");
gets(fn);

printf("Citam data z diskety...\n");
//2844
ReadData(2844,0,spt,fn);

printf("\nHotovo.");
}
