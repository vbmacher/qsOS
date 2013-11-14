#ifndef __PHY__
  #define __PHY__

  #define SECTOR_SIZE 512

  #define FDD 0
  #define HDD 0x80

  extern int SPT_FDD;
  extern int SECT_COUNT;

  int getspt();
  int read_phy(int, int, int, int, void *);
  int write_phy(int, int, int, int, void *);

  int diskINITPHY();
  void diskDESTROYPHY();

#endif
