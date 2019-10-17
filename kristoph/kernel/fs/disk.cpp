/*
 * DISK.CPP
 *
 * Funkcie na ovladanie disku.
 *
 * (c) Copyright 2001, P. Jakubco ml.
 *
 * RELASE NOTES
 * ~~~~~~~~~~~~
 * 20.12.2001 - v0.10 - Initial relase
 * 26.12.2001 - v0.11 - Updated with GetTracks function
 * 27.12.2001 - v0.12 - Updated with 36 Sectors per track
 * 31.01.2002 - v0.2  - Big bug fixed with reading file
 */
#include <console.h>
#include <kfs.h>
#include <kos.h>

int spt;
int drive;

//BIOSDISK -- Precita sektor z disku
int biosdisk(int cmd,int head, int track, int sector, int nsects, void *buffer)
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


//Funkcia zisti Sectors Per Head (Sektory na stopu)
int GetSpt()
{
 static int nsect[] = {36, 18, 15, 9};
 char *buffer=(char *)SYS_BUFFER;
 int i, status;
 biosdisk(RESET, 0, 0, 0, 0, buffer);
 status = biosdisk(READ, 0, 10, 1, 1, buffer);
 if (status == 0x06)			/* Door signal change?	*/
  status = biosdisk(READ, 0, 0, 1, 1, buffer);
 for (i = 0; i < sizeof(nsect) / sizeof(int); ++i)
  {
   biosdisk(RESET, 0, 0, 0, 0, buffer);
   status = biosdisk(READ, 0, 0, nsect[i], 1, buffer);
   if (status == 0x06)
    status = biosdisk(READ, 0, 0, nsect[i], 1, buffer);
    if (status == 0x00)
     break;
  }
 if (i == sizeof(nsect)/sizeof(int))
  {
   puts("\n\rError: Can't figure sectors/track");
   return 1;
  }

 return(nsect[i]);
}

//Precita sektor z disku
int ReadSector(int nStartSector,int nHead,int nTrack,void *buff)
{
 return biosdisk(READ,nHead,nTrack,nStartSector,1,buff);
}

//Precita 1 sektor pre clustery
int ReadClusterSector(int nSector,void *buff)
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

 return ReadSector(nRealSector,nHead,nTrack,buff);
}

//Precita data z daneho clustera (fyzicke data)
int ReadClusterData(int nSector,int nFatSize,void *buff)
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

 return ReadSector(nRealSector,nHead,nTrack,buff);
}

//Precita sektor so subormi
int ReadFileSector(int nSector,void *buff)
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

 return ReadSector(nRealSector,nHead,nTrack,buff);
}


//Precita subor
int ReadFile(char nFN[17],void *buff, int cPath)
{
 // Budeme potrebovat:
 nFAT_FSector *nFF = (nFAT_FSector *)0x40000000; // Sektor so suborami
 nFAT_CSector *nFC = (nFAT_CSector *)0x40000200; // Sektor s clusterami
 nFAT_Header *nFH = (nFAT_Header *)0x40000400;   // Hlavicku FAT tabulky
 nFAT_Data *nFD = (nFAT_Data *)0x40000600;       // Sektor s fyzickymi datami
 int i,k,l;                            // Pomocne premenne
 unsigned int nNumOfCl;                // Pocet informacii o clusteroch
 long nOfsCount;
 int nFileIndex;                       // Index otvoreneho subora
 int dataSector;                       // Cislo sektora pre fyz. data
 int nPocetClusters;                   // Pocet clusterov pre data
 int isFile;

// Prekonvertujeme nFN na spravny format.
 for (i = 0; i < 17; i++) {
  if (nFN[i] == ' ') 
    nFN[i] = '\0';
 }

 ResetDisk();
 //Zistime informacie o FAT-ke.
 ReadSector(3,0,0,nFH);
 //Celkovy pocet informacii o clusteroch
 nNumOfCl=nFH->nFAT_Size-35;

 //Hladame subor (potrebujeme nFileIndex)
 isFile=0;
 for (i=1;i<=32;i++)
 {
   dataSector=0;
   ReadFileSector(i,nFF);
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
   printf("\n\rError: File not found");
   return 1;
 }

 // Otvarame clustery...
 dataSector=0;
 for (k=0;k<nPocetClusters;k++)
  {
   // Citame clustery zaradom
   for (i=1;i<=nNumOfCl;i++)
    {
     ReadClusterSector(i,nFC);
     for (l=0;l<64;l++)
     {
      dataSector=0;
      if (nFC->nCluster[l].nFileIndex == nFileIndex)
       if (nFC->nCluster[l].nNumOfCluster == k+1)
	{
         // poz.dat.sektora:(nSector-1)+nPosition (pouziva sa aj 0. sektor)
	 dataSector=(i-1)*64+l;
	 dataSector++;
	 ReadClusterData(dataSector,nNumOfCl+35,nFD);

	 //ulozime data do bufferu
	 memcpy(buff, nFD->nClusterData, nFC->nCluster[l].nNumOfChars);
         buff = MK_FP(FP_SEG(buff),FP_OFS(buff)+nFC->nCluster[l].nNumOfChars);
	 nOfsCount += nFC->nCluster[l].nNumOfChars;

	// Ak citame data vacsie ako 65535, musime inkrementovat aj
	// segment.
	 if (nOfsCount >= 0xFFFF) { // 0xFFFF = 65535
           buff = MK_FP(FP_SEG(buff)+0x1000,0);
           nOfsCount = 0;                    
         }                                   

	 dataSector=-5;
	 break;
	}
     }
     if (dataSector==-5)
      break;
    }
  }

 return ResetDisk();
}

//Funkcia inicializuje docasny ovladac disku
int DiskInitialize(int Xdrive)
{
  drive=Xdrive;
  ResetDisk();
  spt=GetSpt(); //0 - A:\, ...
  if (spt==1) {
    return 0;
  }
  return spt;
}

int GetTracks()
{
 int nT;
 char *nBuff=(char *)SYS_BUFFER;
 nT= nBuff[1]+1;
 return nT; //0 - Sectors per track
	    //1 - Max. num. of track
	    //2 - Max. num. of head
	    //3 - Number of mechanics
}

int ResetDisk()
{
 int stat;
 char *buff= (char *)SYS_BUFFER;
 stat=biosdisk(0,0,0,0,0,buff);
 asm {
	push dx
        mov dx,0x3F2
	xor al, al
	out dx, al
	pop dx
 }
 return stat;
}
