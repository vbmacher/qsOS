/*
 * NVIDEO.CPP
 *
 * (c) Copyright 2001, P. Jakubco ml.
 *
 * Adapter & Video setup
 *
 */

#include <video.h>
#include <console.h>

int ModeParams(int nAdapter,SystemParams *V_Params)
{
 int dSegment;
 int dLines;
 int dPage;
 int dMode;
 int dPoints;

 asm {
  xor bx, bx
  mov ah, 0x0F
  int 0x10
 }

 dPage = _BX;  // Aktualna stranka
 dMode = _AL;  // Aktualny mod
 dSegment = 0xB800;

 if (dMode == 0x07)
  dSegment = 0xB000; // CGA/MDA/HGA

asm {
  push ds
  push si

  xor ax, ax
  mov ds, ax
  mov si, 0x485
  lodsw

  pop si
  pop ds
}
  dPoints = _AX;
  dLines = 25;

  if (nAdapter != 0) {
   asm {
	push ds
	push si

	xor ax, ax
	mov ds, ax
	mov si, 0x484 // Cislo max. riadka
	lodsb
	inc al        // my chceme pocet riadkov

	pop si
	pop ds
   }
   dLines = _AL;
  }

 // VESA info !!!!
 


// if (memcmp(V_Params->vActual.vVESAInfo.vID,"VESA",4) == 0)
   V_Params->vActual.vVESA = TRUE;
// else
//   V_Params->vActual.vVESA = FALSE;
  V_Params->vActual.vAdapter = nAdapter;
  V_Params->vActual.vPage    = dPage;
  V_Params->vActual.vMode    = dMode;
  V_Params->vActual.vSegment = dSegment;
  V_Params->vActual.fPoints  = dPoints;
  V_Params->vActual.vLines   = dLines;

 return 0;
}

void VideoInitialize(SystemParams *V_Params)
{
 int docasAdapter; // Docasny adapter, ktory sa priradi do nAdapter

 //Checks adapter
 docasAdapter=BasicDetect();

 //Zisti parametry aktualneho modu
 ModeParams(docasAdapter,V_Params);
}



