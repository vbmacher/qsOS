/*
 * KFS.H
 *
 * (c) Copyright 2001, P. Jakubco ml.
 *
 */

//Dlzka sektoru
#define SECTORSIZE 512

//Definicie pre BIOSDISK
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

//Hlavicka FAT-ky
struct nFAT_Header {
   unsigned char nIPL[490];
   unsigned char nVolumeLabel[15];
   unsigned int nNumOfFiles;
   unsigned int nNumOfClusters;
   unsigned int nFAT_Size;
   unsigned char nSignature;
};

//Hlavicka suboru
struct nFAT_File {
   unsigned char nFileName[13];
   unsigned char nFileExt[4];
   long nFileDate;
   long nFileAttributes;
   unsigned int nFileIndex;
   unsigned int nBeforeDirIndex;
   unsigned int nNumOfClusters;
   unsigned char nIsDir;
};

//Hlavicka sektoru pre subory
struct nFAT_FSector {
   struct nFAT_File nFile[16]; //16 informacii o suboroch v 1 sektore
};

//Hlavicka clusteru
struct nFAT_Cluster {
   unsigned int nFileIndex;
   unsigned int nNumOfCluster;
   unsigned int nFree; //00 - Free, 01 - Saved
   unsigned int nNumOfChars; //max. 512
   //pozicia na fyzic. disku je:(nSector-1)*64+nPosition (pouziva sa aj 0. sektor)
};

//Hlavicka sektoru pre clustery
struct nFAT_CSector {
   struct nFAT_Cluster nCluster[64]; //64 informacii o clusteroch v 1 sektore
};

//Fyzicke data
struct nFAT_Data {
  char nClusterData[SECTORSIZE];
};

