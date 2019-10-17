/*
 * K_BOOT.CPP
 *
 * Kristoph File System formatter - BootSector replacer
 *
 * (c) Copyright 2001-2002, P. Jakubco ml.
 *
 *
 * Relase notes
 * ~~~~~~~~~~~~
 * 02.12.2001 - v0.10b - Initial relase
 * 09.12.2001 - v0.20b - Added FAT signature
 * 14.12.2001 - v0.30b - Added 2 another sectors for BootLoader
 *                       and added some optimizations
 * 15.12.2001 - v0.31b - Added some informations on screen
 * 16.12.2001 - v0.40b - Some optimizations added
 *                       Bug fixed with getting info about FATSize
 *                       Bug fixer with setting info about FAT
 * 25.01.2002 - v0.41b - Some optimizations added
 * 18.03.2002 - v0.42b - Some optimizations added
 *
 *
 */

/*
  Princip formatovania:
  ~~~~~~~~~~~~~~~~~~~~~
  Najprv sa precita max. 1024+490 (1514) bytov zo subora, ktory
  bude predstavovat bootsector (KFS.IMG).Ten sa zapise na prve dva cele
  a na treti ciastocny sektor a do zvysku tretieho sektora sa zapise FAT
  header. Po nom nasleduje 32 sektorov urcenych pre subory a dalsich x
  sektorov, ktore popisuju data jednotlinym suborom. Ako posledne nasleduju
  samotne data.

*/

#include <alloc.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#include <mem.h>
#include <string.h>
#include "k_boot.h"


//Zisti velkost informacii o clusteroch.
int GetFatClusters(int drive,int spt)
{
 int nTracks;         // Pocet stop
 char nBuff[512];     // Buffer na zistenie poctu stop a hlaviciek
 int nFCS;            // Pocet sektorov pre informacie o clusteroch
 long nAllSectors;    // Pocet vsetkych sektorov
 int iCS;             // Velkost FAT-ky
 double dCS;          // To iste, ale pomocne

 // Najprv zistime velkost diskety:
 biosdisk(PARAM, drive, 0, 0, 0, 0, nBuff);
 nTracks = nBuff[1] + 1; //0 - Sectors per track
		     //1 - Max. num. of track
		     //2 - Max. num. of head
		     //3 - Number of mechanics
 printf("\nPocet stop: %d", nTracks);
 nAllSectors = spt * nTracks * (nBuff[2] + 1);

 // FAT Cluster size v bytoch
 nFCS = (nAllSectors - 35) * 8;
 /* 35 preto, lebo 3 sektory->FAT a 32 sektory-FILES
    8 preto, lebo 1 informacia o clusteri
    sa sklada z 3 udajov:  -index subora * 2
                           -cislo clustera * 2
			   -kde sa cluster nachadza * 4
 */
 iCS = nFCS / 512;
 dCS = nFCS / 512;
 if (iCS < dCS)
   iCS++;

 printf("\nPocet sektorov urcenych pre informacie o clusteroch: %d", iCS);
 printf("\nCelkovy pocet sektorov na diskete: %ld\n", nAllSectors);

 return iCS;
}

// Zisti celkovu dlzku novej FAT-ky.
long GetFatSize(int nFCS)
{
 long nFATSize;

 // Vieme, ze FAT ma 1 sektor na Header, 32 sektorov na FileNames a
 // Zvysok na Fat_Clusters.
 nFATSize = 35 * SECTORSIZE;
 nFATSize += (nFCS * SECTORSIZE);

 printf("\nPocet bytov v 1.sektore: %d",SECTORSIZE);
 printf("\nPocet bytov urcenych pre subory: %ld", (long)(32*SECTORSIZE));
 printf("\nPocet bytov urcenych pre BootLoader: %ld", (long)(3*SECTORSIZE));

 return nFATSize;
}

// Funkcia zisti Sectors Per Head (Sektory na stopu)
int GetSpt(int drive)
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
 if (i == (sizeof(nsect) / sizeof(int))) {
   printf("\nNemozem zistit, kolko je sektorov na stopu pre tuto disketu.\n");
   exit(1);
 }

 return(nsect[i]);
}

// Zapise sektor na disketu
int WriteSector(int drive,int nStartSector,int nHead,int nTrack,void *buff)
{
 return biosdisk(WRITE,drive,nHead,nTrack,nStartSector,1,buff);
}

// Precita sektor na disketu
int ReadSector(int drive,int nStartSector,int nHead,int nTrack,void *buff)
{
 return biosdisk(READ,drive,nHead,nTrack,nStartSector,1,buff);
}

// Nastavi informacie o FAT-ke.
int SetFATInfo(int drive,nFAT_Header *x)
{
 return WriteSector(drive,3,0,0,x);
}

// Zisti informacie o FAT-ke.
int GetFATInfo(int drive,nFAT_Header *x)
{
 return ReadSector(drive,3,0,0,x);
}

// Funkcia vytvori novu FAT tabulku.
int CreateFat(int drive,int nFCS,char *bFN)
{
 char *buff;                       // Buffer
 char *nBL;			   // Docasny buffer pre BootLoader
 nFAT_Header *nFH=new nFAT_Header; // Vytvorime novy objekt - Hlavicku FAT-ky
 int handle, bytes;                // handle - pre subor s bootsektorom,
				   // bytes - pocet nacitanych suborov.
 int nHead, nTrack, nSector;       // Hlava, stopa a sektor
 int i,k;

 //Alokujeme pamat
 buff = (char *)malloc(3 * SECTORSIZE - 22);
 setmem(buff, 3 * SECTORSIZE - 22, '\0');

 // Otvorime subor s Boot Sektorom
 if ((handle =open(bFN, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1) {
   printf("Chyba otvarania suboru (subor asi neexistuje)\n");
   exit(1);
 }
 if ((bytes = read(handle, buff, 3*SECTORSIZE-22)) == -1) {
   printf("Chyba citania.\n");
   exit(1);
 }
 else 
   printf("Precitane: %d bytov.\n", bytes);

 nBL = (char *)malloc(SECTORSIZE);

 // Vkladanie BootSektora do nIPL
 for (int l = 1; l <= 2; l++)
 {
  for (k = 0; k < SECTORSIZE; k++)
   if (l == 2)
     nBL[k] = buff[k + SECTORSIZE];
   else
     nBL[k] = buff[k];
  WriteSector(drive, l, 0, 0, nBL);
 }
 free(nBL);

 GetFATInfo(0, nFH);

 for (k = 0; k < (bytes -2 * SECTORSIZE); k++)
   nFH->nIPL[k] = buff[k + 2 * SECTORSIZE];

 strcpy(nFH->nVolumeLabel, "Root"); // Oznacenie diskety
 nFH->nFAT_Size = 35 + nFCS;           // Dlzka FAT tabulky
 nFH->nSignature = 0xDD;             // Podpis FAT tabulky

 // Vytvorime FAT tabulku.
 SetFATInfo(drive, nFH);

 // Dealokacia pamate
 free(buff);
 delete nFH; //objekt je uz nepotrebny.
 return 0;
}


int main()
{
 long nFatSize;
 int nFATClusters;
 char *bootFN;
 int spt;

 printf("\n\nKristoph BootSector Replacer (KBR) verzia 0.42b");
 printf("\n(c) Copyright 2001, P. Jakubco ml.\n");
 printf("\nVlozte prazdnu naformatovanu disketu na\nlogicke formatovanie a stlacte -ENTER-.\n");
 if (getch() != 13) {
   printf("Stlacena klavesa nebola -ENTER-, koncim formatovanie.\n");
   exit(1);
 }

 printf("\nPripravujem logicke formatovanie...");

 spt = GetSpt(0); //0 - A:\ 1 - C:\
 printf("\nSektory na stopu: %02d",spt);
 nFATClusters = GetFatClusters(0, spt);
 nFatSize = GetFatSize(nFATClusters);

 printf("\nCelkova dlzka FAT tabulky: %ld bytov.\n",nFatSize);

 printf("\nVlozte nazov suboru s BootSektorom:");
 gets(bootFN);

 printf("\nVsetko pripravene na formatovanie. Stlacte -ENTER- pre formatovanie.");
 if (getch() != 13) {
   printf("\nStlacena klavesa nebola -ENTER-, koncim formatovanie.");
   exit(1);
 }
 printf("\nFormatujem...");

 CreateFat(0, nFATClusters, bootFN);

 printf("\nHotovo.");
 return 0;
}
