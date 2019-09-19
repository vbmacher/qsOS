/*
 * K_FORMAT.CPP
 *
 * Kristoph File System formatter
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
 * 19.12.2001 - v0.50b - Bug fixed with creating a FAT
 * 25.01.2002 - v0.51b - Some optimizations added
 * 17.03.2002 - v0.52b - Some optimizations added,
 *			 Bug fixed with getting a size of FAT
 *
 *
 */

/*
  Princip formatovania:
  ~~~~~~~~~~~~~~~~~~~~~
  Najprv sa precita max. 1024+490 (1514) bytov zo subora, ktory
  bude predstavovat bootsector (KFS.IMG). Ten sa zapise na prve dva cele
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
#include "k_format.h"

//Zisti velkost informacii o clusteroch.
int GetFatClusters(int drive,int spt)
{
 int nTracks;         // Pocet stop
 char nBuff[512];     // Buffer na zistenie poctu stop a hlaviciek
 int nFCS;            // Pocet sektorov pre informacie o clusteroch
 long nAllSectors;    // Pocet vsetkych sektorov
 int iCS;             // Velkost FAT-ky
 double dCS;          // To iste, ale pomocne

 //Najprv zistime velkost diskety:
 biosdisk(PARAM, drive, 0, 0, 0, 0, nBuff);
 nTracks = nBuff[1] + 1; //0 - Sectors per track
		     //1 - Max. num. of track
		     //2 - Max. num. of head
		     //3 - Number of mechanics
 printf("\nPocet stop:%d", nTracks);
 nAllSectors = spt * nTracks * (nBuff[2] + 1);

 //FAT Cluster size v bytoch
 nFCS = (nAllSectors - 35) * 8;
 /* 35 preto, lebo 3 sektory->FAT a 32 sektory-FILES
    8 preto, lebo 1 informacia o clusteri
    sa sklada z 3 udajov:  -index subora * 2
			   -cislo clustera * 2
			   -kde sa cluster nachadza * 4
 */
 iCS = nFCS/512;
 dCS = nFCS/512;
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
 // nFATSize je v bytoch !!!
 // nFCS zase v sektoroch
 nFATSize = 35 * SECTORSIZE;
 nFATSize += (nFCS * SECTORSIZE);

 printf("\nPocet bytov v 1.sektore: %d",SECTORSIZE);
 printf("\nPocet bytov urcenych pre BootLoader: %ld", (long)(3*SECTORSIZE));
 printf("\nPocet bytov urcenych pre subory: %ld", (long)(32*SECTORSIZE));

 return nFATSize;
}

//Funkcia zisti Sectors Per Head (Sektory na stopu)
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

//Zapise sektor na disketu
int WriteSector(int drive,int nStartSector,int nHead,int nTrack,void *buff)
{
 return biosdisk(WRITE,drive,nHead,nTrack,nStartSector,1,buff);
}

//Nastavi informacie o FAT-ke.
int SetFATInfo(int drive,nFAT_Header *x)
{
 return WriteSector(drive,3,0,0,x);
}

//Funkcia vytvori novu FAT tabulku.
int CreateFat(int drive,int spt,int nFCS,char *bFN)
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
 // Vynulujeme buffer
 setmem(buff,3 * SECTORSIZE - 22,'\0');
 //Otvorime subor s Boot Sektorom
 if ((handle = open(bFN, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1) {
   printf("Chyba otvarania suboru (subor asi neexistuje)\n");
   exit(1);
 }
 if ((bytes = read(handle, buff, 3 * SECTORSIZE - 22)) == -1) {
   printf("Chyba citania.\n");
   exit(1);
 }
 else
   printf("Precitane: %d bytov.\n", bytes);

 nBL=(char *)malloc(SECTORSIZE);

 //Vkladanie BootSektora do nIPL
 for (int l=1;l<=2;l++)
 {
  for (k=0;k<SECTORSIZE;k++)
    if (l==2)
      nBL[k]=buff[k+SECTORSIZE];
    else
      nBL[k]=buff[k];
  WriteSector(drive,l,0,0,nBL);
 }
 free(nBL);

 for (k = 0;k < (bytes - 2 * SECTORSIZE);k++)
   nFH->nIPL[k] = buff[k + 2 * SECTORSIZE];
 free(buff); //uvolnime buffer s takou dlzkou.

 strcpy(nFH->nVolumeLabel,"Root"); // Oznacenie diskety
 nFH->nNumOfFiles = 0;             // Pocet suborov
 nFH->nNumOfClusters = 0;          // Pocet clusterov
 nFH->nFAT_Size=35 + nFCS;         // Dlzka FAT tabulky
 nFH->nSignature = 0xDD;           // Podpis FAT tabulky

 //Vytvorime FAT tabulku.
 SetFATInfo(drive,nFH);
 //Napiseme FAT_Files a FAT_Clusters
 nHead = nTrack = 0;
 nSector = 4;
 //Tento buffer pouzijeme aj vo FAT_Clusters
 buff = (char *)malloc(SECTORSIZE);

 //reset
 biosdisk(RESET,drive,0,0,0,0,buff);
 setmem(buff,SECTORSIZE,'\0');

 i=0;
 for (i=0;i<(nFCS+32);i++)
 {
   WriteSector(drive,nSector,nHead,nTrack,buff);
   nSector++;
   if (nSector>spt) {
     nHead = (nHead + 1) & 1;
     nSector = 1;
     if (nHead == 0)
       nTrack++;
   }
 }

 //Dealokacia pamate
 free(buff);
 delete nFH; //objekt je uz nepotrebny.

 return 0;
}


int main()
{
 double nFatSize;
 int nFATClusters;
 char *bootFN;
 int spt;

 printf("\n\nKristoph Filesystem Formatter (KFSF) verzia 0.52b");
 printf("\n(c) Copyright 2001-2002, P. Jakubco ml.\n");

 printf("\nVlozte prazdnu naformatovanu disketu na\nlogicke formatovanie a stlacte -ENTER-.\n");
 if (getch() != 13) {
   printf("Stlacena klavesa nebola -ENTER-, koncim formatovanie.\n");
   exit(1);
 }

 printf("\nPripravujem logicke formatovanie...\n");
 spt = GetSpt(0); //0 - A:\ 1 - C:\
 printf("\nSektory na stopu: %02d", spt);
 nFATClusters = GetFatClusters(0, spt);
 nFatSize = GetFatSize(nFATClusters);

 printf("\nCelkova dlzka FAT tabulky: %lg bytov.\n",nFatSize);
 printf("\nVlozte nazov suboru s BootSektorom:");
 gets(bootFN);

 printf("\nVsetko pripravene na formatovanie. Stlacte -ENTER- pre formatovanie.\n");
 if (getch() != 13) {
   printf("Stlacena klavesa nebola -ENTER-, koncim formatovanie.\n");
   exit(1);
 }

 printf("\nFormatujem...\n");
 CreateFat(0, spt, nFATClusters, bootFN);

 printf("\nHotovo.");
 return 0;
}
