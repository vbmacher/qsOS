/*
 * KOS.H
 *
 * (c) Copyright 2001-2002, Peter Jakubco ml.
 *
 */

#ifndef __KOS__
#define __KOS__

 #define STACK_SEG  0x5000      
 #define BOOT_SEG   0x07B0      // Segment bootsektora
 #define SETUP_SEG  0x9000      // Segment premiestneneho bootsectora
 #define SETUP_ADDR 0x90000000  // Adresa docasneho kernela
 #define KERNEL_SEG 0x1000      // Segment kernela
 #define KERNEL_ADDR 0x10000000 // Adresa kernela
 #define BUFFER_ADDR 0x40000000 // Adresa buffera bude bootsector

#define MK_FP(s,o) ((void _seg *)(s) + (void near *)(o))
#define peekb(s,o) (*(unsigned char *)MK_FP(s,o))

#define FP_SEG(ptr) ((unsigned short)(void _seg *)(void far *)(ptr))
#define FP_OFS(ptr) ((unsigned short)(ptr))

#endif