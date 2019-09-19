/*
 * KOS.H
 *
 * (c) Copyright 2001, Peter Jakubco ml.
 *
 */

#ifndef __KOS__
#define __KOS__

#define SYS_PARAMS 0x07C00000
#define SYS_BUFFER 0x07C00000

typedef struct {
  int vAdapter; // Video Adapter
  int vPage;    // Actual video page
  int vMode;    // Actual video mode
  int vSegment; // Actual video segment
  int fPoints;  // Height of font
  int vLines;   // Number of lines
  bool vVESA;   // Is VESA supported ?
} VideoParam;


//Diskove informacie
typedef enum {
  nUnknown=0,
  n1_2=15,
  n1_44=18,
  n2_88=36
} DiskType;


typedef struct {
 char nBuff[512]; // Buffer, kde sa bude zapisovat
 int dSPT;
 DiskType dType;
 int dTracks;
 char dLabel; // A,B,...
} DiskParams;


//Pamatove informacie
typedef struct {
 unsigned long mCapacity;
// unsigned int mSystemSEG;
} MemoryParams;


//Vseobecne systemove informacie (systemove premenne)
typedef struct {
 DiskParams sFloppy;
 VideoParam vActual;
 MemoryParams mActual;
} SystemParams;

#endif
