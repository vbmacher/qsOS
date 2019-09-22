/*
 * DISK.H
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#ifndef __DISK__
#define __DISK__

  int biosdisk(int cmd, int drive, int head, int track,
             int sector, int nsects, void *buffer);


  
#endif
