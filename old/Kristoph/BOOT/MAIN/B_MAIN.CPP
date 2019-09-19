/*
 * B_MAIN.CPP
 *
 * Pokracovanie boot sektora.
 *
 * (c) Copyright 2001, P. Jakubco ml.
 *
 * RELASE NOTES
 * ~~~~~~~~~~~~
 * 25.11.2001 - v0.10 - Initial relase, not functing
 * 15.12.2001 - v0.50 - Updated with getting SPT and BIOS reading functions
 * 17.12.2001 - v0.60 - Updated with reading a Kernel
 *                      Some bugs fixed
 * 18.12.2001 - v0.70 - Some BIG bugs fixed
 * 27.12.2001 - v0.71 - Updated with 36 Sectors Per Track
 * 31.12.2001 - v0.72 - Kernel address changed
 * 31.01.2002 - v0.73 - BIG Bug fixed with ReadFile function, now you can
 *			read any data greater than 65 535 bytes !!!
 */

#include <text.h>
#include <kfs.h>
#include <kos.h>

int biosdisk(int,int,int,int,int,int,void*);
int ReadSector(int,int,int,int,void*);
int ReadClusterSector(int,int,int,void*);
int ReadClusterData(int,int,int,int,void*);
int ReadFileSector(int,int,int,void*);
int GetSpt(int);
int ReadFile(int,int,char*,void*,int);
int memcmp(const void far *,const void far *,int);
void memcpy (void far *,void far *,int);

int main()
{
  // Toto je este vzdy BOOT sector, teraz nacitame subor
  // KRISTOPH.SYS ako obraz praveho operacneho systemu.
  //
  //Dame ho na adresu: KERNEL_ADDR
  int spt,i;

  puts("\n\rLoading Kernel...");

  //Zistime Sectors per head

  spt=GetSpt(0); //0 - A:
  if (spt==1)
    return 1;
nLoad:
 char nFN[17]="kristoph     sys ";

 for (i=0;i<17;i++)  // Zameni znaky 32 (" ") za znaky 0.
  if (nFN[i] == ' ')
    nFN[i]='\0';

 ReadFile(0,spt,nFN,(char *)KERNEL_ADDR,0);


 // Kontrola headeru
 if ((memcmp((char *)KERNEL_ADDR,"KOS",3)) == 1) {
   puts("\n\rBad Kernel!");
   return 1;
 }

 // Skok na KERNEL_ADDR uz vykona BootSector
 return 0; // Ziadny skok
}

void memcpy (void far *dest, void far *str, int size)
{
 asm {
		push    si
		push    di
		push    ds
		les     di,[dest]
		lds     si,[str]
		mov     cx,[size]
		shr     cx,1
		cld
		rep     movsw
		jnc     MemCpyDone
		movsb
     }
MemCpyDone:
 asm {
		pop     ds
		pop     di
		pop     si
 }
}


int memcmp(const void *s1, const void *s2, int count)
{
 asm {
	push    si
	push    di
	push    ds
	push    es

	xor     ax,ax
	les     di, [s1]
	lds     si, [s2]
	mov     cx, [count]
	cld
	rep     cmpsb
	je x
	mov al, 1
  }
  x:
  asm {
	pop     es
	pop     ds
	pop     di
	pop     si
   }
 return _AX;
}


//Funkcia zisti Sectors Per Head (Sektory na stopu)
int GetSpt(int drive)
{
 static int nsect[] = {36, 18, 15, 9};
 char *buffer=(char *)BUFFER_ADDR;
 int i, status;
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
 if (i == sizeof(nsect)/sizeof(int))
  {
   puts("\n\rError: Unknown diskette");
   return 1;
  }

 return(nsect[i]);
}


//BIOSDISK -- Precita sektor z disku
int biosdisk(int cmd,int drive, int head, int track, int sector, int nsects, void *buffer)
{
 asm {
  push es
  mov dh, byte ptr [head] //10
  mov dl, byte ptr [drive] //8
  mov ch, byte ptr [track] //12
  mov cl, byte ptr [sector] //14
  mov al, byte ptr [nsects] //16
  mov ah, byte ptr [cmd]
  les bx, [buffer]
  int 0x13
  pop es
 }
 return _AH;
}

//Precita sektor z disku
int ReadSector(int drive,int nStartSector,int nHead,int nTrack,void *buff)
{
 return biosdisk(READ,drive,nHead,nTrack,nStartSector,1,buff);
}

//Precita 1 sektor pre clustery
int ReadClusterSector(int drive,int spt,int nSector,void *buff)
{
 //Inicializuje pociatocne hodnoty
 int nTrack=0;
 int nHead=1;
 int nRealSector=17; //35. sektor, 1 sa pripocita za chvilu.

 //Vypocita skutocny sektor - nSector !< 1 !!!
 for (int i=1;i<=nSector;i++)
  {
   nRealSector++;
   if (nRealSector>spt)
    { nHead=(nHead+1)&1;
      nRealSector=1;
      if (nHead==0)
       ++nTrack;
    }
  }

 return ReadSector(drive,nRealSector,nHead,nTrack,buff);
}

//Precita data z daneho clustera (fyzicke data)
int ReadClusterData(int drive,int spt,int nSector,int nFatSize,void *buff)
{
 int nTrack=0;      // Stopa
 int nHead=0;       // Hlava
 int nRealSector;   // Skutocny sektor
 int i;
 nRealSector=nFatSize+1;

 //Skutocny sektor je urcite > ako spt, preto treba dopocitat
 //nTrack a nHead - a samozrejme podla toho zmenit aj nRealSector.
 for (i=1;i<=nFatSize;i++)
  {
   if (nRealSector>spt)
    {
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

 //A teraz dopocitame potrebnu hodnotu nRealSector.
 //nSector nesmie byt < ako 1
 for (i=1;i<=nSector;i++)
  {
    nRealSector++; //Teraz sa to vratilo.
    if (nRealSector>spt)
     {
      nHead=(nHead+1)&1;
      nRealSector=1;
      if (nHead==0)
	++nTrack;
     }
  }

 return ReadSector(drive,nRealSector,nHead,nTrack,buff);
}

//Precita sektor so subormi
int ReadFileSector(int drive,int spt,int nSector,void *buff)
 {
  int nTrack=0;      // Stopa
  int nHead=0;       // Hlava
  int nRealSector=3; // Skutocny sektor

  //nSector nesmie byt < 1
  //Vypocitame potrebnu hodnotu nRealSector, nHead a nTrack.
  for (int i=1;i<=nSector;i++)
   {
    nRealSector++; //Skutocny sektor pre subory je 2, hlava 0, stopa 0.
    if (nRealSector>spt)
     {
      nHead=(nHead+1)&1;
      nRealSector=1;
      if (nHead==0)
	++nTrack;
     }
   }

 return ReadSector(drive,nRealSector,nHead,nTrack,buff);
}


//Precita subor
int ReadFile(int drive,int spt,char *nFN,void *buff, int cPath)
{
 // Budeme potrebovat:
 nFAT_FSector *nFF =(nFAT_FSector *)0x40000000; // Sektor so suborami
 nFAT_CSector *nFC =(nFAT_CSector *)0x40000200; //(&nFF+SECTORSIZE+1); // Sektor s clusterami
 nFAT_Header *nFH =(nFAT_Header *)0x40000400; //(&nFC+SECTORSIZE+1);   // Hlavicku FAT tabulky
 nFAT_Data *nFD =(nFAT_Data *)0x40000600; //(&nFH+SECTORSIZE+1);       // Sektor s fyzickymi datami
 int i,k,l;                          // Pomocne premenne
 long nOfsCount;		       // Pocitanie offsetu (ci neprekrocil 0xFFFF)
 int nNumOfCl;                         // Pocet informacii o clusteroch
 int nFileIndex;                       // Index otvoreneho subora
 int dataSector;                       // Cislo sektora pre fyz. data
 int nPocetClusters;                   // Pocet clusterov pre data
 int isFile;

 //Zistime informacie o FAT-ke.
 ReadSector(drive,3,0,0,nFH); // 3, lebo mame 3 boot sektory :)
 //Celkovy pocet informacii o clusteroch
 nNumOfCl=nFH->nFAT_Size-35;

 //Hladame subor (potrebujeme nFileIndex)
 isFile=0;
 for (i=1;i<=32;i++)
 {
   dataSector=0;
   ReadFileSector(drive,spt,i,nFF);
   for (k=0;k<16;k++)
   {
    if (memcmp(nFF->nFile[k].nFileName,nFN,16) == 0)
     if (nFF->nFile[k].nIsDir == FALSE)
      if (nFF->nFile[k].nBeforeDirIndex == cPath)
      {
       nFileIndex=nFF->nFile[k].nFileIndex;
       nPocetClusters=nFF->nFile[k].nNumOfClusters;
       isFile=5;
       dataSector=-9;
       break;
      }
   }

  if (dataSector == -9)
   break;
 }
 if (isFile != 5) {
   puts("\n\rError:Kernel not found");
   return 1;
 }

 //Otvarame clustery...
 dataSector=0;
 nOfsCount = 0;
 for (k=0;k<nPocetClusters;k++)
  {
   //Citame clustery zaradom
   for (i=1;i<=nNumOfCl;i++)
    {
     ReadClusterSector(drive,spt,i,nFC);
     for (l=0;l<64;l++)
     {
      dataSector=0;
      if (nFC->nCluster[l].nFileIndex == nFileIndex)
       if (nFC->nCluster[l].nNumOfCluster == k+1)
	{
	 //poz.dat.sektora:(nSector-1)+nPosition (pouziva sa aj 0. sektor)
	 dataSector=(i-1)*64+l;
	 dataSector++;
	 ReadClusterData(drive,spt,dataSector,nFH->nFAT_Size,nFD);
	 //ulozime data do bufferu

	 memcpy(buff, nFD->nClusterData, nFC->nCluster[l].nNumOfChars);
         buff = MK_FP(FP_SEG(buff),FP_OFS(buff)+nFC->nCluster[l].nNumOfChars);
	 nOfsCount += nFC->nCluster[l].nNumOfChars;

	// Ak citame data vacsie ako 65535, musime inkrementovat aj
	// segment.
	 if (nOfsCount >= 0xFFFF) { // 0xFFFF = 65535
	   buff = MK_FP(FP_SEG(buff)+0x1000,0); // 0000:FFFF = 0FFF:F000
	   nOfsCount = 0;		      // a ked chceme zvacsit segment
	 }				      // o 1, nemozeme spravit +1, lebo
					      // by sme dostali:
	 dataSector=-5;			      // 0001:0000 = 0000:0010
	 break;				      // cim by sme neznicili header,
	}				      // ale poskodili by sme zvysok
     }					      // kernelu. Preto musime segment
     if (dataSector==-5)		      // inkrementovat o 0x1000, cim
      break;				      // dostaneme: 1000:0000.
    }
  }

// Vypneme motorcek na disketovke, co ak je kernel
// bad a my sa zasekneme ? A to cely cas bude na tej
// disketovke svietit ? Ale kdeze, mame predsa toto:
//asm {
// push dx
// mov dx, 0x3F2
// xor al, al
// out dx, al
// pop dx
//}

// Vypol som to koli miestu na diskete.

 return 0;
}



