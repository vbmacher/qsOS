/*
 * KFS.H
 *
 * (c) Copyright, Peter Jakubco ml.
 *
 * Hlavickovy subor pre Kristoph File System (KFS)
 *
 * RELASE NOTES
 * ~~~~~~~~~~~~
 * 9.12.2001 - Initial header file
 *
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
typedef struct {
   unsigned char nIPL[490];
   unsigned char nVolumeLabel[15];
   unsigned int nNumOfFiles;
   unsigned int nNumOfClusters;
   unsigned int nFAT_Size;
   unsigned char nSignature;
} nFAT_Header;

//Hlavicka suboru
typedef struct {
   unsigned char nFileName[13];
   unsigned char nFileExt[4];
   long nFileDate;
   long nFileAttributes;
   unsigned int nFileIndex;
   unsigned int nBeforeDirIndex;
   unsigned int nNumOfClusters;
   unsigned char nIsDir;
} nFAT_File;

//Hlavicka sektoru pre subory
typedef struct {
   nFAT_File nFile[16]; //16 informacii o suboroch v 1 sektore
} nFAT_FSector;

//Hlavicka clusteru
typedef struct {
   unsigned int nFileIndex;
   unsigned int nNumOfCluster;
   unsigned int nFree; //00 - Free, 01 - Saved
   unsigned int nNumOfChars; //max. 512
   //pozicia na fyzic. disku je:(nSector-1)*64+nPosition (pouziva sa aj 0. sektor)
} nFAT_Cluster;

//Hlavicka sektoru pre clustery
typedef struct {
   nFAT_Cluster nCluster[64]; //64 informacii o clusteroch v 1 sektore
} nFAT_CSector;

//Fyzicke data
typedef struct {
  char nClusterData[SECTORSIZE];
} nFAT_Data;

int biosdisk(int,int,int,int,int,void*);
int ReadSector(int,int,int,void*);
int ReadClusterSector(int,void*);
int ReadClusterData(int,int,void*);
int ReadFileSector(int,void*);
int GetSpt();
int ReadFile(char*,void*,int);
int DiskInitialize(int);
int GetTracks();
int ResetDisk();