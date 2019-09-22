/*
 * RTL.H
 *
 * (c) Copyright 2002-2005, vbmacher
 *
 */

#ifndef __RTL__
  #define __RTL__

    #ifndef NULL
    #  if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
    #    define NULL    0
    #  else
    #    define NULL    0L
    #  endif
    #endif

    typedef unsigned int size_t;
    typedef unsigned char size_c;

    #define CLK_TCK 18.21

    #define BIN 2
    #define OCT 8
    #define DEC 10
    #define HEX 16

    #define _toupper(c) ((c) + 'A' - 'a')
    #define _tolower(c) ((c) + 'a' - 'A')

    #define isalpha(c)  (( ((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z')) ? 1 : 0)
    #define isdigit(c)  ( (((c) >= '0') && (c) <= '9') ? 1 : 0 )

    #define MK_FP(s,o) ((void _seg *)(s) + (void near *)(o))
    #define peekb(s,o) (*(unsigned char *)MK_FP(s,o))

    #define FP_SEG(ptr) ((unsigned short)(void _seg *)(void far *)(ptr))
    #define FP_OFF(ptr) ((unsigned short)(ptr))

    #define PhysAddr(lAddr) ( ((long)FP_SEG(lAddr) << 4) + (long)FP_OFS(lAddr) )

    extern unsigned char attribute;

  #ifdef __cplusplus
    extern "C" {
  #endif

    int memcmp(const void *s1,const void *s2, unsigned short count);
    void memset(void *dest, int value, unsigned short count);
    void memmove(void *dest, void *src, unsigned short size);
    void memcpy(void *dest, void *src, unsigned short size);

    int strlen(const char *str);
    char *strcpy(char *dest, const char *src);
    char *strcat(char *dest, const char *src);
    char *strchr(const char *str, int c);
    char *itoa(long value, char *str, int base);
    int strcmp(const char *s1, const char *s2);
    int stricmp(const char *s1, const char *s2);
    long atol(const char *str);

    void puts(const char far *s);
    void cputs(const char far *s);
    void putch(int ch);
    void gotoxy(int x, int y);
    void wherexy(int *x, int *y);
    int getchar();
    void textattr(unsigned char att);

  #ifdef __cplusplus
    };
  #endif

  void printf(const char *str, ...);
  void cprintf(const char *str, ...);
  void gets(char *s);
  void cgets(char *s);
  int iswhite(char c);
  int isdelim(char c);
  int toupper(int ch);
  int tolower(int ch);

  void setINT(int vector, int seg, int off, unsigned long *oldint);
  void halt();

  // time.c
  unsigned long time();

#endif
