/*
 *
 *  CONSOLE.H
 *
 * (c) Copyright 2001, P. Jakubco
 *
 *
 * RELASE NOTES
 * ~~~~~~~~~~~~
 * 19.12.2001 - Initial file
 * 29.1.2002  - String functions added (strlen,strcmp,strcat,stricmp,strchr,atol,strcpy)
 *              macro #define __STRING__ added
 *              class CString added and all her functions
 */

#ifndef __STRING__
#define __STRING__

#include <defs.h>

 typedef unsigned int size_t;
 typedef unsigned char size_c;

 #define BIN 2
 #define OCT 8
 #define DEC 10
 #define HEX 16

#define MK_FP(s,o) ((void _seg *)(s) + (void near *)(o))
#define peekb(s,o) (*(unsigned char *)MK_FP(s,o))

#define FP_SEG(ptr) ((unsigned short)(void _seg *)(void far *)(ptr))
#define FP_OFS(ptr) ((unsigned short)(ptr))

#define PhysAddr(lAddr) ( ((long)FP_SEG(lAddr) << 4) + (long)FP_OFS(lAddr) )

#define toupper(c) ((c) + 'A' - 'a')
#define tolower(c) ((c) + 'a' - 'A')

 extern void gotoxy(int x, int y);
 extern void wherexy(int *x, int *y);
 extern void puts(const char *str);
 extern void putch(int ch);

#ifdef __cplusplus
 extern "C" {
#endif
 void far memcpy (void far *dest, void far *str, size_t size);
 void far memset(void far *dest, size_c value, size_t count);

#ifdef __cplusplus
 };
#endif

 int memcmp(const void *s1, const void *s2, size_t count); // far
 extern char GetDigit(int Digit);
 void printf(const char *format,...);

#ifdef __cplusplus
extern "C" {
#endif
 
 int strlen(const char far *str);
 char *far strcpy(char far *dest, const char far *src);
 char *far strcat(char far *dest, const char far *src);
 char *far strchr(const char far *str, int c);
 int strcmp(const char far *s1, const char far *s2);
 int stricmp(const char far *s1, const char far *s2);
 long atol(const char far *str);

#ifdef __cplusplus
};
#endif

#endif

