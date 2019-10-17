/*
 * CPU.H
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#ifndef __CPU__
#define __CPU__

  #ifdef __cplusplus
    extern "C" {
  #endif

    int checkBRAND();
    void cpuPVENDOR();
    void cpuPBRAND();

  #ifdef __cplusplus
    };
  #endif

  void cpuTEST();

#endif
