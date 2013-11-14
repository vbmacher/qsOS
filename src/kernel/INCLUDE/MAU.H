/*
 * MAU.H
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#ifndef __MAU__
#define __MAU__

  #ifndef NULL
  #  if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
  #    define NULL    0
  #  else
  #    define NULL    0L
  #  endif
  #endif


  extern long mem;

  void EnableA20();
  void DisableA20();
  void mauINIT();
  void mauDESTROY();

  void *malloc(int size);
  void free(void far **block);
  long coreleft();

#endif
