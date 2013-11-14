#ifndef __LOG__
#define __LOG__
  #include "physic_i.h"

  // superbloky su na sektoroch 1-2
  #define SUP_1 1
  #define SUP_2 2

  #define INODE_COUNT (int)(SECTOR_SIZE / sizeof(inode)) // max. pocet iuzlov v 1 sektore
  #define FILE_COUNT (int)((SECTOR_SIZE - sizeof(dbdir_info)) / sizeof(dirdata)) // max. pocet suborov v 1 sektore pre adresar
  #define DATA_COUNT (long)(SECTOR_SIZE - sizeof(dbfile_info))

  #define DATA_ERROR 2
  #define FAIL 1
  #define SUCCESS 0

// 5 => 3
typedef struct {
  unsigned char ftype;
  unsigned int link;
} inode;

typedef struct {
  inode inodes[INODE_COUNT];
  unsigned char xd[SECTOR_SIZE - sizeof(inode)*INODE_COUNT];
} inodes_sec;

// 7 => 5
typedef struct {
  unsigned int fcount;
  unsigned int link;      // dalsi datablok
  unsigned char ilink;    // i-uzol rodica
  unsigned char me_ilink; // odkaz na ilink sameho seba
} dbdir_info;

// 7 => 5
typedef struct {
  unsigned int dcount;
  unsigned int link;
  unsigned char ilink;
  unsigned char reserved; //
} dbfile_info;

typedef struct {
  char fname[8];
  char fext[3];
  unsigned char ilink;
} dirdata;

typedef struct {
  dbdir_info info;
  dirdata files[FILE_COUNT];
  unsigned char data[SECTOR_SIZE - sizeof(dbdir_info) - sizeof(dirdata)*FILE_COUNT];
} dbdir;

typedef struct {
  dbfile_info info;
  unsigned char data[SECTOR_SIZE - sizeof(dbfile_info)];
} dbfile;

  // globalne buffery
  extern dbdir dir;
  extern inodes_sec IN;

  typedef struct {
    int HEAD;
    int TRACK;
    int SECTOR;
  } xPP;

  extern xPP PP;

  unsigned char get_inode(unsigned char i_node);
  int format_log();
  int create_log(char fname[8], char fext[3], unsigned char idir, unsigned char idir_parent, unsigned char i_node);
  int remove_log(char fname[8], char fext[3], unsigned char idir, unsigned char i_node);
  unsigned int write_log(char fname[8], char fext[3], unsigned char idir, unsigned int dcount, unsigned long dpos, char *buf, int showper);
  unsigned int read_log(char fname[8], char fext[3], unsigned char idir, unsigned int dcount, unsigned long dpos, char *buf, int showper);
  long freespace_log();
  long diskcap_log();
  long filesize_log(char fname[8], char fext[3], unsigned char idir);
  int getfname_log(char fname[8], char fext[3], unsigned char idir, int index);
  int filecount_log(unsigned char idir);

#endif