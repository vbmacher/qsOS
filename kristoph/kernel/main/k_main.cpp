/*
 * K_MAIN.CPP
 *
 * Kernel
 *
 * (c) Copyright 2001, P. Jakubco ml.
 *
 * RELASE NOTES
 * ~~~~~~~~~~~~
 * 19.12.2001  v0.1   Initial file, not supported user's changes
 * 20.12.2001  v0.2   Functions rewrited to lib and it used
 * 30.12.2001  v0.3   Video modes detection and VESA detection added
 * 23.01.2002  v0.4   Bus type detection added
 * 30.01.2002  v0.7   Graphics User Interface (GUI) added
 */
#include <console.h>
#include <kfs.h>
#include <video.h>
#include <kos.h>

// Funkcie
void HaltComputer(SystemParams*);

int main()
{
 /****************/
 /* Kernel KOS   */
 /*  v1.00       */
 /****************/
 SystemParams *sActual=(SystemParams *)SYS_PARAMS;

 printf("\n\nWelcome to Kristoph Operating System\nThis is a kernel version 1.00\n(c) Copyright 2001-2002, Peter Jakubco ml.");
 printf("\n\nStarting system installation...");

 // ***Disk parameters
 sActual->sFloppy.dSPT=DiskInitialize(0); // Get drive info
 memcpy(&sActual->sFloppy.dType,&sActual->sFloppy.dSPT,2);
 sActual->sFloppy.dLabel='A';
 sActual->sFloppy.dTracks=GetTracks();

 // Info
 if (sActual->sFloppy.dType == 0) {
  printf("\nError: Unknown disk type in drive A\nSystem halted");
  return 0;
 }
 else
  printf("\nFloppy disk type: ");

 if (sActual->sFloppy.dType == n1_2)
  printf("1.2 MB, 5.25\"");
 else if (sActual->sFloppy.dType == n1_44)
  printf("1.44 MB, 3.5\"");
 else if (sActual->sFloppy.dType == n2_88)
  printf("2.88 MB, 3.5\"");


 // ***Video parameters
 VideoInitialize(sActual);

 // Info
 printf("\nVideo adapter: ");
 switch (sActual->vActual.vAdapter) {
  case 0:
   printf("CGA/MDA/HGA\nError: Video adapter not supported\nSystem halted");
   return 0;
  case 1:
   printf("EGA\nError: Video adapter not supported\nSystem halted");
   return 0;
  case 2:
   printf("VGA");
   break;
  default:
   printf("Unknown\nError: Unknown Video adapter\nSystem halted");
   return 0;
 }
 printf("\nVideo mode: ");
 switch (sActual->vActual.vMode) {
  case 0x0:
   printf("Text, 40x25, 16 colors of gray");
   break;
  case 0x1:
   printf("Text, 40x25, 16 colors");
   break;
  case 0x2:
   printf("Text, 80x25, 16 colors of gray");
   break;
  case 0x3:
   printf("Text, 80x25, 16 colors");
   break;
  case 0x4:
   printf("Graphics, 320x200, 2x4 colors");
   break;
  case 0x5:
   printf("Graphics, 320x200, 4 colors of gray");
   break;
  case 0x6:
   printf("Graphics, 640x200, 1 of 8 colors");
   break;
  case 0x7:
   printf("Text, 80x25, black/white");
   break;
  case 0x8:
   printf("Graphics, 160x200, 16 colors");
   break;
  case 0x9:
   printf("Graphics, 320x200, 16 colors");
   break;
  case 0xA:
   printf("Graphics, 640x200, 16 colors");
   break;
  case 0xD:
   printf("Graphics, 320x200, 16 colors");
   memcpy(&sActual->vActual.vSegment," 0",2);  //0xA000
   break;
  case 0xE:
   printf("Graphics, 640x200, 16 colors");
   memcpy(&sActual->vActual.vSegment," 0",2);
   break;
  case 0xF:
   printf("Graphics, 640x350, black/white");
   memcpy(&sActual->vActual.vSegment," 0",2);
   break;
  case 0x10:
   printf("Graphics, 340x350, 16 colors");
   memcpy(&sActual->vActual.vSegment," 0",2);
   break;
  case 0x11:
   printf("Graphics, 640x480, 2 colors");
   memcpy(&sActual->vActual.vSegment," 0",2);
   break;
  case 0x12:
   printf("Graphics, 640x480, 16 colors");
   memcpy(&sActual->vActual.vSegment," 0",2);
   break;
  case 0x13:
   printf("Graphics, 320x200, 256 colors");
   memcpy(&sActual->vActual.vSegment," 0",2);
   break;
  default:
   printf("Unknown");
   break;
 }

 if (sActual->vActual.vVESA == 0) {
  printf("\nError: VESA not supported\nSystem halted.");
  return 0;
 }

 // ***Memory paramters
// sActual->mActual.mCapacity=GetFreeSpace();

 // Info
 printf("\nExtended memory capacity: %ld kB",sActual->mActual.mCapacity);

 // ***Keyboard
 asm {
  mov ax, 0x0305 // Nastavi najvyssiu frekvenciu klavesnice (rychlo)
  xor bx, bx
  int 0x16
 }

 // ***Additional Hardware
 printf("\nBus Type: ");
 asm {
  push es
  push bx
  push ds
  push si

  mov ax, 0x7C0
  mov es, ax
  mov ds, ax
  xor bx, bx
  xor si, si

  mov ah, 0xC0
  int 0x15

  cld
  lodsw
  lodsw
  lodsw

  pop si
  pop ds
  pop bx
  pop es

  and al, 2
  jnz here
}
 printf("PC Bus\nError: There isn't MicroChannel (MCA) Bus.\nSystem Halted.");
 return 0;

here:
 printf("MicroChannel (MCA)");

HaltComputer(sActual);

return 0;
}

void HaltComputer(SystemParams *VParams)
{
  printf("\nHalting the system...");
  // Vratime sa spat do modu, v ktorom sme boli na zaciatku.
  _AX = VParams->vActual.vMode;
  asm {
   xor ah, ah
  int 0x10
  }
  printf("\nYou can now turn off your computer.\nSystem halted");
}