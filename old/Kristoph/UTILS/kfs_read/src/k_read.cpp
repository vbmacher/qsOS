/*
 * K_READ.CPP
 *
 * Program cita sektory z diskety
 *
 * (c) Copyright 2001-2002, Peter Jakubco ml.
 *
 * PROGRAM JE SUCASTOU OPERACNEHO SYSTEMU KRISTOPH
 *
 * datum vytvorenia: 2.11.2001 (v0.00000001a)
 *
 * RELASE NOTES
 * ~~~~~~~~~~~~
 * 29.11.2001   v0.10b   Initial relase, not functing
 * 9.12.2001    v0.20b  Functing relase
 * 3.12.2001    v0.30b  Bug fixed - Number of readed chars in cluster
 *                                  wasn't max.512
 *                      Bug fixed - While writing, Number of cluster didn't increment
 *                      Bug fixed - Datasector was always at 1 lesser.
 *                      Bug fixed - Number of chars in cluster was always 512
 * 4.12.2001   v0.31b   Bug fixed - New file was always at 1 greater.
 * 5.12.2001   v0.32b   Bug fixed - Variable has too little size
 *                      Bug fixed - Variable has illegal size
 *                                  (in function read the return value was long type)
 *                      Bug fixed - the double return value wasn't double
 *                                  and it maked the cluster was at 1 lesser.
 * 7.12.2001   v0.40b   Bug fixed - in saving file on FAT when reading file
 *                                  from KFS, the file on FAT wasn't in binary format.
 *                      Function added - CountFreeSpace - counts a free space
 *                      Bug fixed - in checking the capacity the buffer wasn't allocated.
 *                      Optimization with showing info of FAT table.
 *                      Optimization with copying a tempBuffer to FAT data (used memcpy)
 *                      BIG !!! Bug fixed - While reading, the sectors didn't increment.
 *                      Optimization with copying a FAT data to tempBuffer (used memcpy)
 * 8.12.2001   v0.41b   Bug fixed - While trying to read drive parameters in
 *                      GetDiskCapacity(), registers ES:DI weren't saved.
 * 9.12.2001   v0.42b   FAT signature added
 *                      KFS checking added (by signature)
 *                      Bug fixed - while comparing filenames, the file extension wasn't compared.
 * 14.12.2001  v0.50b   Added 2 new sectors for BootLoader and some new optimizations.
 * 16.12.2001  v0.51b   Bug fixed with getting info about FATSize
 * 21.12.2001  v0.52b   Bug fixed with deleting file - there was ignored file existion
 * 25.1.2002   v0.53b   Some optimizations added for function Dir
 * 			Added file size to function Dir
 * 30.1.2002   v0.54b   Bug fixed with reading a DOS file (65 532 bytes, not 65 534)
 *			Some optimizations added
 *                      Bug fixed with deleting file
 * 01.2.2002   v0.55b   Bugs fixed with deleting file (extensions control added;
 *                      Disabled every cluster checking, changed only for real
 *			cluster count checking)
 *			Bugs fixed with saving file (capacity size checking;
 *			deallocation on error exits)
 * 18.03.2002  v0.56b   Bugs fixed with creating, saving and opening files.
 *			A '\0' char was replaced by a ' '
 *			Some optimizations added
 *			Bugs fixed with Dir function
 * 19.03.2002 v0.57b    Some bugs fixed with saving files...
 *			CountFreeSpace() function was removed, free space
 *			on diskette was counted by new simply method.
 * 22.03.2002 v0.58b    Some optimizations added
 *			Bug fixed with changing actual path (jumping to dirs)
 * 23.03.2002 v0.59b    Some optimizations added
 *		        Bug fixed with deleting files
 */

#include <bios.h>     // biosdisk
#include <stdio.h>    // printf,...
#include <stdlib.h>   // exit
#include <ctype.h>    // tolower,toupper
#include <conio.h>    // getch,clrscr,...
#include <string.h>   // strlen, strcpy, strcat, ...
#include <mem.h>      // setmem
#include <io.h>       // open
#include <fcntl.h>    // konstanty pre open
#include <sys\stat.h> // to iste
#include "k_read.h"   // KFS Header

// Aktualna cesta
int currentPath;
// ------------------------------------------------------------

// Precita sektor z disku
int ReadSector(int drive,int nStartSector,int nHead,int nTrack,void *buff)
{
 return biosdisk(READ,drive,nHead,nTrack,nStartSector,1,buff);
}

// Napise sektor na disk
int WriteSector(int drive,int nStartSector,int nHead,int nTrack,void *buff)
{
 return biosdisk(WRITE,drive,nHead,nTrack,nStartSector,1,buff);
}

// Precita 1 sektor pre clustery
int ReadClusterSector(int drive,int spt,int nSector,void *buff)
{
 // Inicializuje pociatocne hodnoty
 int nTrack=0;
 int nHead=1;
 int nRealSector=17; //35. sektor, 1 sa pripocita za chvilu.
 int i;

 // Vypocita skutocny sektor - nSector !< 1 !!!
 for (i=1; i <= nSector; i++)
 {
  nRealSector++;
  if (nRealSector>spt) {
    nHead=(nHead+1)&1;
    nRealSector=1;
    if (nHead==0)
      ++nTrack;
  }
 }
 return ReadSector(drive,nRealSector,nHead,nTrack,buff);
}

// Napise 1 sektor pre clustery
int WriteClusterSector(int drive,int spt,int nSector,void *buff)
{
 // Inicializuje pociatocne hodnoty
 int nTrack=0;
 int nHead=1;
 int nRealSector=17;

 // Vypocita skutocny sektor, nSector !<1 !!!
 for (int i=1;i<=nSector;i++)
 {
  nRealSector++;
  if (nRealSector>spt) {
    nHead = (nHead + 1) & 1;
    nRealSector = 1;
    if (nHead == 0)
      ++nTrack;
  }
 }
 return WriteSector(drive,nRealSector,nHead,nTrack,buff);
}

// Precita data z daneho clustera (fyzicke data)
int ReadClusterData(int drive,int spt,int nSector,int nFatSize,void *buff)
{
 int nTrack=0;      // Stopa
 int nHead=0;       // Hlava
 int nRealSector;   // Skutocny sektor
 int i;
 nRealSector=nFatSize+1;

 // Skutocny sektor je urcite > ako spt, preto treba dopocitat
 // nTrack a nHead - a samozrejme podla toho zmenit aj nRealSector.
 for (i=1;i<=nFatSize;i++)
 {
  if (nRealSector>spt) {
    nHead=(nHead+1)&1;
    if (nRealSector>19)
      nRealSector-=spt;
    else
      break;
    if (nHead==0)
      ++nTrack;
  }
 }
 nRealSector--; //Za chvilu sa to vrati...

 // A teraz dopocitame potrebnu hodnotu nRealSector.
 // nSector nesmie byt < ako 1
 for (i=1;i<=nSector;i++)
 {
  nRealSector++; //Teraz sa to vratilo.
  if (nRealSector>spt) {
    nHead=(nHead+1)&1;
    nRealSector=1;
    if (nHead==0)
      ++nTrack;
  }
 }
 return ReadSector(drive,nRealSector,nHead,nTrack,buff);
}

// Napise data do daneho clustera (fyzicke data)
int WriteClusterData(int drive,int spt,int nSector,int nFatSize,void *buff)
{
 int nTrack=0;      // Stopa
 int nHead=0;       // Hlava
 int nRealSector;   // Skutocny sektor
 int i;
 nRealSector=nFatSize+1;

 // Skutocny sektor je urcite > ako spt, preto treba dopocitat
 // nTrack a nHead - a samozrejme podla toho zmenit aj nRealSector.
 for (i=1;i<=nFatSize;i++)
 {
  if (nRealSector>spt) {
    nHead = (nHead + 1) & 1;
    if (nRealSector > 19)
      nRealSector -= spt;
    else
      break;
    if (nHead==0)
      ++nTrack;
  }
 }
 nRealSector--; //Za chvilu sa to vrati...

 // A teraz dopocitame potrebnu hodnotu nRealSector.
 // nSector nesmie byt < ako 1
 for (i=1;i<=nSector;i++)
 {
  nRealSector++; // Teraz sa to vratilo.
  if (nRealSector > spt) {
    nHead=(nHead + 1) & 1;
    nRealSector = 1;
    if (nHead == 0)
      ++nTrack;
    }
  }
 return WriteSector(drive,nRealSector,nHead,nTrack,buff);
}

// Precita sektor so subormi
int ReadFileSector(int drive,int spt,int nSector,void *buff)
{
 int nTrack=0;      // Stopa
 int nHead=0;       // Hlava
 int nRealSector=3; // Skutocny sektor-1

 // nSector nesmie byt < 1
 // Vypocitame potrebnu hodnotu nRealSector, nHead a nTrack.
 for (int i=1;i<=nSector;i++)
 {
  nRealSector++;       // Skutocny sektor pre subory je 2, hlava 0, stopa 0.
  if (nRealSector>spt) {
    nHead=(nHead + 1) & 1;
    nRealSector=1;
    if (nHead==0)
      ++nTrack;
  }
 }
 return ReadSector(drive,nRealSector,nHead,nTrack,buff);
}

// Napise sektor so subormi
int WriteFileSector(int drive,int spt,int nSector,void *buff)
{
 int nTrack=0;
 int nHead=0;
 int nRealSector=3;

  // Vypocitame potrebnu hodnotu pre nRealSector, nHead a nTrack.
  for (int i=1;i<=nSector;i++)
  {
   nRealSector++;
   if (nRealSector>spt) {
     nHead=(nHead + 1) & 1;
     nRealSector=1;
     if (nHead == 0)
       ++nTrack;
   }
  }
 return WriteSector(drive,nRealSector,nHead,nTrack,buff);
}

// Funkcia na zistenie Sectors Per Track (SPT) (Sektory Na Stopu - SNS)
int GetSpt(int drive)
{
 static int nsect[] = {18, 15, 9};
 char *buffer;
 int i, status;

 buffer=(char *)malloc(SECTORSIZE);
 biosdisk(RESET,drive,0,0,0,0,buffer);
 status=biosdisk(READ,drive,0,10,1,1,buffer);
 if (status==0x06)	// Disketa bola vymenena ?
   status=biosdisk(READ,drive,0,0,1,1,buffer);
 for (i=0;i<sizeof(nsect)/sizeof(int);++i)
 {
  biosdisk(RESET,drive,0,0,0,0,buffer);
  status=biosdisk(READ,drive,0,0,nsect[i],1,buffer);
  if (status==0x06)
    status=biosdisk(READ,drive,0,0,nsect[i],1,buffer);
  if (status==0x00)
    break;
 }
 free(buffer);
 if (i == sizeof(nsect)/sizeof(int)) {
   printf("\nNemozem zistit pocet sektorov na stopu pre tuto disketu.\n");
   exit(1);
 }
  return(nsect[i]);
}

// Funkcia zisti informacie of FAT tabulke
int GetFATInfo(int drive,nFAT_Header *x)
{
 return ReadSector(drive,3,0,0,x); // Precita sektor 3, hlavu 0, stopu 0.
}

// Funkcia nastavi informacie o FAT tabulke
int SetFATInfo(int drive,nFAT_Header *x)
{
 return WriteSector(drive,3,0,0,x);
}

// Funkcia CreateFile vytvori subor - bez jeho udajov
int CreateFile(int drive,int spt,char nFileName[17],unsigned char nIsDir,int nBefDInd)
{
 nFAT_FSector *nFF = new nFAT_FSector; // Hlavicka sektoru suborov
 nFAT_Header *nFH = new nFAT_Header;   // Hlavicka FAT tabulky
 int nPosition;                        // Pozicia v sektore pre subory
 int nSector;                          // Relativny sektor pre subory
 int oldNumOfFiles;                    // Stary pocet suborov
 int i, u;

 u = 0;
 for (i = 0;i < 17; i++)
   if ((memcmp((const void *)nFileName[i]," ",1) != 0) && (memcmp((const void *)nFileName[i],"\0",1) != 0)) {
     u = 1;
     break;
   }
 if (u == 0) {
   printf("\nNeplatny nazov suboru");
   delete nFH;
   delete nFF;
   return -1;
 }

 // Zisti informacie o FAT tabulke
 GetFATInfo(drive,nFH);

 // Maximalne mnozstvo suborov je 512.Skontrolujeme to.
 if (nFH->nNumOfFiles == 512) {
   printf("\nNa tuto disketu sa uz nezmesti ziadny subor.");
   delete nFH;
   delete nFF;
   return -1;
 }

 // Aktualny pocet suborov sa stava starym, lebo pridavame dalsi subor.
 oldNumOfFiles = nFH->nNumOfFiles;

 // Updatujeme tabulku FAT...
 nFH->nNumOfFiles++;
 SetFATInfo(drive, nFH);

 delete nFH; //Dealokujeme pamat.
 // Vypocitame, ze na ktorom sektore a v ktorej pozicii v tomto sektore
 // sa bude nachadzat novy subor.
 // sektor
 nSector = (oldNumOfFiles + 1) / 16; // V 1 sektore je 16 suborov.
 // POZOR: nSector moze byt aj nulovy !!!
 // pozicia
 nPosition = (oldNumOfFiles + 1) - (nSector * 16);
 nSector++; // Zatial bol nSector iba pocet plnych sektorov.
	    // Teraz je to pozicia.
 // Vytvorime novy subor.
 ReadFileSector(drive, spt, nSector, nFF);

 // Vlozime do struktury meno suboru.
 for (i = 0; i < 17; i++)
   nFF->nFile[nPosition-1].nFileName[i] = nFileName[i];

 nFF->nFile[nPosition-1].nFileDate=0;                // Zatial datum je 0.
 nFF->nFile[nPosition-1].nFileAttributes=0;          // Atributy tiez.
 nFF->nFile[nPosition-1].nFileIndex=oldNumOfFiles+1; // Pocet suborov
 nFF->nFile[nPosition-1].nBeforeDirIndex=nBefDInd;   // 0 - Root
 nFF->nFile[nPosition-1].nNumOfClusters=0;           // Subor je bez udajov !
 nFF->nFile[nPosition-1].nIsDir=nIsDir;              // TRUE - Adresar

 if (WriteFileSector(drive,spt,nSector,nFF) != 0) {
   printf("\nStala sa chyba pri zapisovani informacii o subore.");
   delete nFF;
   return -1;
 }
 delete nFF;
 return 0;
}

// Funkcia Dir - zobrazi vsetky adresare a subory v aktualnom adresari.
// nCurrPath sa dava koli objektivnosti.
void Dir(int drive,int spt,int nCurrPath)
{
 nFAT_FSector *nFF = new nFAT_FSector; // Potrebujeme iba sektor so subormi.
 int i,j;                              // Pomocne premenne.
 long iSize;			       // Dlzka suboru
 int nSector;

 // Ak niesme v korenovom adresari, vypiseme ".."
 if (nCurrPath > 0)
   printf("..\n");

 // Najprv vypiseme vsetky adresare
 for (nSector=1; nSector < 33; nSector++)
 {
  ReadFileSector(drive,spt,nSector,nFF);
  for (i = 0;i < 16; i++)
   if ((memcmp(nFF->nFile[i].nFileName,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",17) != 0) && (memcmp(nFF->nFile[i].nFileName,"                 ",17) != 0))
     if (nFF->nFile[i].nBeforeDirIndex == nCurrPath)
       if (nFF->nFile[i].nIsDir == TRUE)
         if (memcmp((const void *)nFF->nFile[i].nFileName[12]," ",1) == 0)
	   printf("\rDIR %13s%4s  : %2d\n",nFF->nFile[i].nFileName,nFF->nFile[i].nFileExt,nFF->nFile[i].nFileIndex);
	 else
	   printf("\rDIR %17s     : %2d\n",nFF->nFile[i].nFileName,nFF->nFile[i].nFileIndex);
 }
 // Teraz subory...
 for (nSector=1; nSector < 33; nSector++)
 {
  ReadFileSector(drive,spt,nSector,nFF);
  for (i = 0; i < 16; i++)
   if ((memcmp(nFF->nFile[i].nFileName,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",17) != 0) && (memcmp(nFF->nFile[i].nFileName,"                 ",17) != 0))
     if (nFF->nFile[i].nBeforeDirIndex == nCurrPath)
       if (nFF->nFile[i].nIsDir == FALSE) {
	 if (nFF->nFile[i].nNumOfClusters > 1)
	   iSize = (long)(nFF->nFile[i].nNumOfClusters - 1) * SECTORSIZE + SECTORSIZE; // Chyba - nie + SECTORSIZE, ale + Pocet bytov v poslednom clusteri
	 else
	   iSize = SECTORSIZE; // Chyba - nie sectorsize, ale pocet bytov v clusteri
	 if (memcmp((const void *)nFF->nFile[i].nFileName[12]," ",1) == 0)
	   printf("\rFIL %13s.%4s :%2d  [%ld b]\n",nFF->nFile[i].nFileName,nFF->nFile[i].nFileExt,nFF->nFile[i].nFileIndex, iSize);
	 else {
	   printf("\rFIL ");
	   for (j = 0;j < 13; j++)
	     printf("%c",nFF->nFile[i].nFileName[j]);
	   printf(".");
	   for (j = 0;j < 4; j++)
	     printf("%c",nFF->nFile[i].nFileExt[j]);
	 }
      }
  }
 delete nFF; //Dealokujeme pamat.
}

// Funkcia ChPath - zmeni cestu (Current Path)
int ChPath(int drive,int spt,int nCurrPath,char nDirName[17])
{
 nFAT_FSector *nFF = new nFAT_FSector; // Sektor pre subory
 char nUp;                             // Ci ideme o 1 level vyssie
 int newPath;                          // Nova cesta
 int i, k;                             // Pomocne premenne
 char *nFN;                            // Meno adresara
 int nSector;                          // Pri hladani adresara

 // Alokujeme pamat a prevadzame vsetky pismena na velke.
 nFN = (char *)malloc(17);
 for (k = 0; k < 17; k++)
   nFN[k] = toupper(nDirName[k]);

 newPath = nCurrPath; // Ak by nahodou neexistovalo meno adresara.
 // Ak je meno adresara "..", ideme o 1 level vyssie.

 if (strcmp(nDirName,"..") == 0)
   nUp=TRUE;
 else
   nUp=FALSE;

 // Ak ideme o 1 level vyssie,
 if (nUp == TRUE) {
   // Najde predchadzajuci adresar
   for (nSector = 1; nSector <= 32; nSector++)
   {
    ReadFileSector(drive,spt,nSector,nFF);
    for (i = 0; i < 16; i++)
      if (strcmp(nFF->nFile[i].nFileName,"") != 0)
	if (nFF->nFile[i].nFileIndex == nCurrPath)
	  if (nFF->nFile[i].nIsDir == TRUE) { //Toto je iba kontrola
	    newPath=nFF->nFile[i].nBeforeDirIndex;
	    break;
	  }
   }
 }
 else {
   // Najde meno adresara
   for (nSector=1;nSector<=32;nSector++)
   {
    ReadFileSector(drive,spt,nSector,nFF);
    for (i=0;i<16;i++)
      if (nFF->nFile[i].nBeforeDirIndex == currentPath)
	if (strcmp(nFF->nFile[i].nFileName,nFN) == 0)
	  if (nFF->nFile[i].nIsDir == TRUE) {
	    newPath=nFF->nFile[i].nFileIndex;
	    break;
	  }
    }
  }
 free(nFN);
 delete nFF; // Dealokujeme pamat.
 return newPath;
}

// Funkcia zisti celkovu kapacitu disku (v sektoroch)
int GetDiskCapacity(int drive,int spt)
{
 char *buff;  // Buffer na citanie parametrov, musi mat dlzku 512 bytov.
 int nTracks; // Celkovy pocet stop
 int nCap;

 buff = (char *)malloc(SECTORSIZE);
 asm push es
 asm push di

 if (biosdisk(PARAM, drive, 0, 0, 0, 0, buff) != 0) {
   printf("\nStala sa chyba pri zistovani informacii o diskete");
   free(buff);
   asm pop di
   asm pop es
   return -1;
 }

 nTracks = buff[1] + 1; //1 - Number of tracks, 2 - Number of heads
 nCap = nTracks * spt * (buff[2] + 1);

 asm pop di
 asm pop es

 free(buff);
 return nCap;
}

// Funkcia na ulozenie subora.
int SaveFile(int drive,int spt,char nFileName[17],char *nFName)
{
 // Budeme potrebovat:
 nFAT_FSector *nFF = new nFAT_FSector; // Sektor so suborami
 nFAT_CSector *nFC = new nFAT_CSector; // Sektor s clusterami
 nFAT_Header *nFH = new nFAT_Header;   // Hlavicku FAT tabulky
 nFAT_Data *nFD = new nFAT_Data;       // Sektor s fyzickymi datami
 int i,k,l;                            // Pomocne premenne
 long j,nPos;
 int nSector;                          // Cislo sektora pre clustery a
				       // pozicia v sektore
 int nNumOfCl;                         // Pocet informacii o clusteroch
 int nFileIndex;                       // Index otvoreneho subora
 int dataSector;                       // Cislo sektora pre fyz. data
 char *nFN;                            // Meno suboru
 int nPocetClusters;                   // Pocet clusterov pre data
 double nPocCl;                        // To iste, ale iba pomocne
 char *docasBuffer;                    // Buffer pre fyzicke data
 char *nBuff;			       // A tak dalej, .....
 int oldNumOfClusters,handle;
 long lenBuff;
 unsigned int bytes;
 unsigned int naZapis;

 // Precitame do nBuff cely subor.
 if ((handle = open(nFName, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1) {
   printf("\nChyba otvarania suboru (subor asi neexistuje)");
   return 1;
 }
 lenBuff = filelength(handle);
 nBuff = (char *)malloc(lenBuff);
 if (lenBuff <= 65532) {
  if ((bytes = read(handle, nBuff, lenBuff)) != lenBuff) {
    printf("\nChyba citania.");
    return 1;
  }
 }
 else {
   // Tu: Treba dorobit citanie suborov vacsich ako
   // 65532 bytov - rozdelit na viacnasobne citanie read.
   // Da sa eliminovat pouzitim programu fsplit.exe, ktory
   // rozdeli subor po 32767 bytoch. Tie potom nacitame postupne
   // s tymto programom.
   printf("\nSimulator zatial nepodporuje subory vacsie ako 65 532 bytov.");
   return 1;
 }
 printf("\nPrecitane: %u bytov zo suboru.\n", bytes);
 close(handle);

 // Alokujeme meno suboru
 nFN = (char *)malloc(17);
 // Prevedieme subor na male pismena
 for (i = 0; i < 17; i++)
   nFN[i] = tolower(nFileName[i]);

 // Zistime informacie o FAT-ke.
 GetFATInfo(drive,nFH);

 // Rozdelime data na clustery
 nPocCl = (double)lenBuff / (double)SECTORSIZE; //SECTORSIZE=CLUSTERSIZE !!!
 nPocetClusters = lenBuff / SECTORSIZE;
 if (nPocetClusters < nPocCl)
   nPocetClusters++;
 else if (nPocetClusters == 0)
   nPocetClusters++;

 // Celkovy pocet informacii o clusteroch
 nNumOfCl = nFH->nFAT_Size - 35;

 // Teraz si musime skontrolovat, ci mame dost volneho miesta.
 if ((nNumOfCl * 64) == nFH->nNumOfClusters) {
   printf("\nMalo miesta na disku.");
   delete nFF;
   delete nFC;
   delete nFH;
   delete nFD;
   free(nBuff);
   free(nFN);
   return 1;
 }
 else if ((nNumOfCl*64-nFH->nNumOfClusters) < nPocetClusters) {
   printf("\nMalo miesta na disku.");
   delete nFF;
   delete nFC;
   delete nFH;
   delete nFD;
   free(nBuff);
   free(nFN);
   return 1;
 }

 // Hladame subor (potrebujeme nFileIndex)
 dataSector = 5; // Indikuje, ci subor existuje.
 nPocCl = 0;
 for (i = 1; i <= 32; i++)
 {
  ReadFileSector(drive,spt,i,nFF);
  for (k=0;k<16;k++)
  {
   if (memcmp(nFF->nFile[k].nFileName,nFN,17) == 0)
     if (nFF->nFile[k].nIsDir==FALSE) {
       nFileIndex=nFF->nFile[k].nFileIndex;
       oldNumOfClusters=nFF->nFile[k].nNumOfClusters;
       dataSector=0;
       nPocCl=-5;
       break;
     }
  }
  if (nPocCl == -5)
    break;
 }
 if (dataSector == 5) {
   printf("\nSubor sa nenasiel !");
   delete nFF;
   delete nFC;
   delete nFH;
   delete nFD;
   free(nBuff);
   free(nFN);
   return 1;
 }

 // Ukladame clustery... (velmi zlozity kod.)
 nPos = 0;
 // Alokujeme docasny buffer
 docasBuffer = (char *)malloc(SECTORSIZE); //SECTORSIZE = CLUSTERSIZE !
 // Vynulujeme data
 setmem(docasBuffer, 512, '\0');

 for (k = 0; k < nPocetClusters; k++)
 {
  // Presunieme 512 bytov z nBuff do docasBuffer
  if (lenBuff >= SECTORSIZE) {
    for (j = nPos; j < (SECTORSIZE + nPos); j++)
      docasBuffer[j-nPos] = nBuff[j];
    lenBuff -= SECTORSIZE;
    nPos += SECTORSIZE;
    naZapis = SECTORSIZE;
  }
  else {
    for (j = nPos; j < (lenBuff + nPos); j++)
      docasBuffer[j-nPos] = nBuff[j];
    // Tu uz to musi byt posledny krat. A vzdy.
    nPos += lenBuff;
    naZapis = lenBuff;
    lenBuff = 0;
  }
  // Zistujeme, kde mozme ulozit nase clustery.
  for (i = 1; i <= nNumOfCl; i++)
  {
   ReadClusterSector(drive, spt, i, nFC);
   for (l = 0; l < 64; l++)
   {
    dataSector = 0;
    // Najdeme kluster v subore, v ktorom je este volne miesto.
    if (nFC->nCluster[l].nFree == 0) {
      // Nasli sme ho.Teraz ho ulozime do informacii
      nFC->nCluster[l].nFileIndex = nFileIndex;            // Index subora
      nFC->nCluster[l].nNumOfCluster = oldNumOfClusters+1; //Cislo clustera
      oldNumOfClusters++;
      nFC->nCluster[l].nNumOfChars = naZapis;              // Pocet znakov
      nFC->nCluster[l].nFree = 1;                          // Obsadeny cluster
      WriteClusterSector(drive,spt,i,nFC);
      // a teraz do PHYSIC disku.
      memcpy(nFD->nClusterData, docasBuffer, naZapis);
      // poz.dat.sektora:(nSector-1)*64+nPosition (pouziva sa aj 0. sektor)
      dataSector = (i - 1) * 64 + l;
      dataSector++;
      WriteClusterData(drive, spt, dataSector, nFH->nFAT_Size, nFD);
      printf("Zapisane %d bytov, %4d. cluster\r", naZapis, k);
      dataSector = -5; // Indikuje, aby sme opustili nasledujuce for.
      break;
    }
   }
   if (dataSector == -5)
     break;
  }
 }

 // Uff! ;)
 // Teraz najdeme subor
 for (nSector = 1; nSector <= 32; nSector++)
 {
  ReadFileSector(drive, spt, nSector, nFF);
  for (i = 0; i < 16; i++)
  {
   dataSector = 0;
   if (memcmp(nFF->nFile[i].nFileName, nFN, 17) == 0)
     if (nFF->nFile[i].nIsDir == FALSE)
       if (nFF->nFile[i].nFileIndex == nFileIndex) {
	 // Updatujeme subor o novy pocet clusterov
	 nFF->nFile[i].nNumOfClusters += nPocetClusters;
	 WriteFileSector(drive, spt, nSector, nFF);
	 dataSector =- 4; // Opustame vsetky fory...
	 break;
       }
  }
  if (dataSector == -4)
    break;
 }
 // Updatujeme tabulku FAT o novy pocet clusterov.
 nFH->nNumOfClusters += nPocetClusters;
 SetFATInfo(drive, nFH);

 free(nFN);         // Dealokujeme vsetku pamat.
 free(docasBuffer);
 free(nBuff);
 delete nFF;
 delete nFC;
 delete nFH;
 delete nFD;
 return 0;
}

// Funkcia na otvorenie subora.
int OpenFile(int drive,int spt,char nFileName[17],char *nFName, int cPath)
{
 // Budeme potrebovat:
 nFAT_FSector *nFF = new nFAT_FSector; // Sektor so suborami
 nFAT_CSector *nFC = new nFAT_CSector; // Sektor s clusterami
 nFAT_Header *nFH = new nFAT_Header;   // Hlavicku FAT tabulky
 nFAT_Data *nFD = new nFAT_Data;       // Sektor s fyzickymi datami
 int i,k,j,l;                          // Pomocne premenne
 int nNumOfCl;                         // Pocet informacii o clusteroch
 int nFileIndex;                       // Index otvoreneho subora
 int dataSector;                       // Cislo sektora pre fyz. data
 char *nFN;                            // Meno suboru
 int nPocetClusters;                   // Pocet clusterov pre data
 int isFile,handle;
 char *nBuff;
 long bytes;

 // Alokujeme meno suboru
 nFN = (char *)malloc(17);
 nBuff = (char *)malloc(SECTORSIZE);
 // Prevedieme subor na male pismena
 for (i = 0; i < 17; i++)
   nFN[i] = tolower(nFileName[i]);
 // Zistime informacie o FAT-ke.
 GetFATInfo(drive, nFH);
 // Celkovy pocet informacii o clusteroch
 nNumOfCl = nFH->nFAT_Size - 35;
 // Hladame subor (potrebujeme nFileIndex)
 isFile = 0;
 for (i = 1; i <= 32; i++)
 {
  dataSector = 0;
  ReadFileSector(drive, spt, i, nFF);
  for (k = 0; k < 16; k++)
  {
   if (memcmp(nFF->nFile[k].nFileName, nFN, 16) == 0)
     if (nFF->nFile[k].nIsDir == FALSE)
       if (nFF->nFile[k].nBeforeDirIndex == cPath)  {
	 nFileIndex = nFF->nFile[k].nFileIndex;
	 nPocetClusters = nFF->nFile[k].nNumOfClusters;
	 printf("\nPocet clusterov: %d\n", nPocetClusters);
	 isFile = 5;
	 dataSector = -9;
	 break;
       }
  }
  if (dataSector == -9)
    break;
 }
 if (isFile != 5) {
   printf("\nSubor sa nenasiel !");
   return 1;
 }

 delete nFF; // Cim skor, tym lepsie ;)

 // Otvarame clustery...
 if ((handle = open(nFName, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY,S_IREAD | S_IWRITE)) == -1) {
  printf("\nChyba otvarania suboru.");
  return 1;
 }

 dataSector = 0;
 for (k = 0; k < nPocetClusters; k++)
 {
  // Citame clustery zaradom
  for (i = 1; i <= nNumOfCl; i++)
  {
   ReadClusterSector(drive, spt, i, nFC);
   for (l = 0; l < 64; l++)
   {
    dataSector = 0;
    if (nFC->nCluster[l].nFileIndex == nFileIndex)
      if (nFC->nCluster[l].nNumOfCluster == (k + 1)) {
	// poz.dat.sektora:(nSector-1)+nPosition (pouziva sa aj 0. sektor)
	dataSector = (i - 1) * 64 + l;
	dataSector++;
	ReadClusterData(drive, spt, dataSector, nFH->nFAT_Size, nFD);
	// ulozime data do bufferu
	memcpy(nBuff, nFD->nClusterData, nFC->nCluster[l].nNumOfChars);
	if ((bytes = write(handle, nBuff, nFC->nCluster[l].nNumOfChars)) == -1) {
	  printf("\nChyba zapisovania.");
	  return 1;
	}
	else
	  printf("Zapisane: %d. cluster, %ld bytov zo suboru.\r",k, bytes);
	dataSector = -5;
	break;
      }
   }
     if (dataSector == -5)
      break;
  }
 }

 close(handle);
 free(nFN);         // Dealokujeme vsetku pamat.
 free(nBuff);
 delete nFC;
 delete nFH;
 delete nFD;
 return 0;
}

// Funkcia sa pouziva na vymazanie informacii o clusteroch
// VYHODA: Daju sa obnovit stratene subory.
int DeleteFileClusters(int drive, int spt, int nFileIndex, int nNumOfCl)
{
 nFAT_CSector *nFC = new nFAT_CSector;
 nFAT_Header *nFH = new nFAT_Header;
 int i = 0,l;
 int g = 0; // Nezavisle pocitadlo, ktore kontroluje pretecenie vestkych clusterov

 GetFATInfo(drive, nFH);
 while ((g <= nFH->nNumOfClusters) && (i <= nNumOfCl)) {
   ReadClusterSector(drive, spt, g + 1, nFC);
   for (l = 0; l < 64; l++)
     if (i <= nNumOfCl) {
       if (nFC->nCluster[l].nFileIndex == nFileIndex)
	 if (nFC->nCluster[l].nFree != 0) {
	   nFC->nCluster[l].nFree = 0;
	   WriteClusterSector(drive, spt, i, nFC);
	   i++;
	 }
     }
     else
       break;
   g++;
 }
 nFH->nNumOfClusters -= nNumOfCl;
 SetFATInfo(drive, nFH);

 delete nFC;
 delete nFH;
 return 0;
}

// Funkcia je na vymazavanie suborov a adresarov
int DeleteFile(int drive,int spt,char *nFN,int isDir)
{
 // Funkcia vymazava subor vtedy a len vtedy, ak existuje.
 // Neporovnava, ci ma subor heslo, alebo ake ma atributy...
 // Alebo akemu pouzivatelovi patri :)
 nFAT_FSector *nFF = new nFAT_FSector;
 nFAT_Header *nFH = new nFAT_Header;
 int i,l;
 int g;
 int nFIndex;

 for (i = 1; i <= 32; i++)
 {
  g = 0;
  ReadFileSector(drive, spt, i, nFF);
  for (l = 0; l < 16; l++)
    if (memcmp(nFF->nFile[l].nFileName, nFN, 17) == 0)
      if (nFF->nFile[l].nIsDir == isDir) {
	nFIndex = nFF->nFile[l].nFileIndex;
	nFF->nFile[l].nFileName[0] = '\0';
	WriteFileSector(drive, spt, i, nFF);
	g = -5;
	break;
      }
  if (g == -5)
    break;
 }
 if (g == 0) {
   printf("\nSubor neexistuje !!!");
   delete nFF;
   delete nFH;
   return 1;
 }

 if (memcmp(nFN, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 17) == 0) {
   printf("\nChybny nazov suboru !!!");
   delete nFF;
   delete nFH;
   return 1;
 }

 DeleteFileClusters(drive, spt, nFIndex, nFF->nFile[l].nNumOfClusters);
 // Updatujeme tabulku FAT o novy pocet suborov.
 GetFATInfo(drive, nFH);
 nFH->nNumOfFiles--;
 SetFATInfo(drive, nFH);

 delete nFF;
 delete nFH;
 return 0;
}

void ShowFATInfo(int spt)
{
 nFAT_Header *nFH = new nFAT_Header;
 int nFree;
 int nCapacity;

 if (GetFATInfo(0, nFH) != 0) {
   printf("\nNemozem zistit informacie o FAT tabulke.");
   return;
 }
 nCapacity = GetDiskCapacity(0, spt) - nFH->nFAT_Size;
 nFree = nCapacity - nFH->nNumOfClusters;

 printf("\nDISKETA:");
 printf("\nSektory na stopu: %d\n", spt);
 printf("\nSUBOROVY SYSTEM:");
 printf("\nOznacenie diskety:          %s", nFH->nVolumeLabel);
 printf("\nPocet suborov:              %d", nFH->nNumOfFiles);
 printf("\nDlzka FAT tabulky:          %ldb", (long)(nFH->nFAT_Size * SECTORSIZE));
 printf("\nKapacita disku:             %ldb", (long)nCapacity * (long)SECTORSIZE);
 printf("\nMax. pocet clusterov:       %ld", (long)(64 * (nFH->nFAT_Size - 35)));
 printf("\nVolne miesto:               %ldb", (long)nFree * (long)SECTORSIZE);

 printf("\n\nStlacte lubovolnu klavesu.");
 getch();

 delete nFH;
}

//-----------------------------------------------------

int main()
{
 int spt;                          // Sectors Per Track - Sektory na stopu
 int nFree;                        // Kapacita disku v sektoroch
 int i;                            // Pomocna premenna
 char isEND;                       // Indikuje, ci je koniec
 char newFileName[13];             // Meno suboru
 char newFileExt[4];               // Pripona suboru
 char newDirName[17];              // Meno adresara
 char nFileName[64];               // Meno subora na disku
 nFAT_Header *aa=new nFAT_Header;  // Hlavicka FAT tabulky

 currentPath = 0;

 printf("\nKristoph Filesystem Simulator (KFSS) verzia 0.59b");
 printf("\n(c) Copyright 2001-2002, P. Jakubco ml.\n\n");
 printf("Vlozte disketu a stlacte -ENTER-.\n");

 if (getch() != 13)
  {
   printf("\nStlacena klavesa nebola -ENTER-, koncim.");
   return 1;
  }
 // Zisti Sectors per track
 spt=GetSpt(0); //0 - A:\ 1 - C:\ ....

 // Skontroluje, ci na diskete naozaj mame KFS.
 GetFATInfo(0,aa);
 if (aa->nSignature != 0xDD) {
  printf("\nVlozena disketa ma iny suborovy system.");
  delete aa;
  return -1; // Koniec programu
 }

// Tu zacina hlavny cyklus:
Start:

 clrscr();
 printf("\nZakladna ponuka simulatora KFS:\n");
 printf("\n0 - Vytvorit novy subor");
 printf("\n1 - Vytvorit novy adresar");
 printf("\n2 - Pozriet si zoznam suborov");
 printf("\n3 - Vymazat subor");
 printf("\n4 - Vymazat adresar");
 printf("\n5 - Informacie of FAT tabulke");
 printf("\n6 - Vojst do adresara");
 printf("\n7 - Ulozit subor");
 printf("\n8 - Otvorit subor");
 printf("\n9 - Koniec\n");

 // Zatial este nekoncime
 isEND=FALSE;
 switch (getch()) {
  case '0':
   printf("\nVlozte nazov suboru (max.13 znakov):");
   gets(newFileName);
   printf("\nVlozte priponu suboru (max. 4 znaky):");
   gets(newFileExt);
   setmem(newDirName,17,' ');
   strcpy(newDirName,newFileName);

   // Prevedieme nazov a priponu do 1 suvisleho celku
   // a prazdne znaky zamenime na nulove (0).
   for (i=13;i<=17;i++)
     newDirName[i]=newFileExt[i-13];
   for (i=0; i<strlen(newDirName); i++)
     newDirName[i] = tolower(newDirName[i]);
   CreateFile(0,spt,newDirName,FALSE,currentPath);
   break;
 case '1':
   printf("\nVlozte nazov adresara (max.17 znakov):");
   setmem(newDirName,17,' ');
   gets(newDirName);
   for (i=0; i<strlen(newDirName); i++)
     newDirName[i] = toupper(newDirName[i]);
   CreateFile(0,spt,newDirName,TRUE,currentPath);
   break;
 case '2':
   Dir(0,spt,currentPath);
   printf("\nStlacte lubovolnu klavesu...");
   getch();
   break;
 case '3':
   printf("\nVlozte nazov suboru (max.13 znakov):");
   gets(newFileName);
   printf("\nVlozte priponu suboru (max. 4 znaky):");
   gets(newFileExt);
   setmem(newDirName,17,' ');
   strcpy(newDirName,newFileName);
   for (i=13;i<=17;i++)
     newDirName[i]=newFileExt[i-13];
   for (i=0; i<strlen(newDirName); i++)
     newDirName[i] = tolower(newDirName[i]);
   DeleteFile(0,spt,newDirName, FALSE);
   printf("\nStlacte lubovolnu klavesu...");
   getch();
   break;
 case '4':
   printf("\nVlozte nazov adresara (max.17 znakov):");
   setmem(newDirName,17,' ');
   gets(newDirName);
   for (i=0; i<strlen(newDirName); i++)
     newDirName[i] = toupper(newDirName[i]);
   DeleteFile(0,spt,newDirName,TRUE);
   printf("\nStlacte lubovolnu klavesu...");
   getch();
   break;
 case '5':
   ShowFATInfo(spt);
   break;
 case '6':
   printf("\nVlozte meno adresara (max. 17 znakov):");
   gets(newDirName);
   currentPath=ChPath(0,spt,currentPath,newDirName);
   break;
 case '7':
   printf("\nVlozte nazov suboru (max.13 znakov):");
   gets(newFileName);
   printf("\nVlozte priponu suboru (max. 4 znaky):");
   gets(newFileExt);
   setmem(newDirName,17,' ');
   strcpy(newDirName,newFileName);
   for (i=13;i<=17;i++)
     newDirName[i]=newFileExt[i-13];
   for (i=0; i<strlen(newDirName); i++)
     newDirName[i] = tolower(newDirName[i]);
   printf("\nVlozte nazov suboru, ktory sa ulozi na disketu:\n");
   gets(nFileName);
   SaveFile(0,spt,newDirName,nFileName);
   printf("\nStlacte lubovolnu klavesu...");
   getch();
  break;
 case '8':
   printf("\nVlozte nazov suboru (max.13 znakov):");
   gets(newFileName);
   printf("\nVlozte priponu suboru (max. 4 znaky):");
   gets(newFileExt);
   printf("\n\nVlozte meno suboru, do ktoreho sa obsah ulozi:");
   gets(nFileName);
   setmem(newDirName,17,' ');
   strcpy(newDirName,newFileName);
   for (i=13;i<=17;i++)
     newDirName[i]=newFileExt[i-13];
   for (i=0; i<strlen(newDirName); i++)
     newDirName[i] = tolower(newDirName[i]);
   OpenFile(0,spt,newDirName,nFileName,currentPath);
   printf("\nSubor bol ulozeny do: \"%s\".",nFileName);
   printf("\n\nStlacte lubovolnu klavesu...");
   getch();
   break;
 case '9':
   isEND = TRUE;
   break;
 default:
   break;
};
if (isEND==FALSE)
 goto Start;

End:

printf("\nHotovo.");

return 0;
}
