/*
 * K_FORMAT.H
 *
 * Header file
 *
 * (c) Copyright 2001-2002, P. Jakubco ml.
 *
 */

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

//Global variables
struct nFAT_Header {
   unsigned char nIPL[490];
   unsigned char nVolumeLabel[15];
   unsigned int nNumOfFiles;
   unsigned int nNumOfClusters;
   unsigned int nFAT_Size;
   unsigned char nSignature;
};

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

struct nFAT_Cluster {
   unsigned int nFileIndex;
   unsigned int nNumOfCluster;
   unsigned int nSector;
   unsigned int nPosition;
};

