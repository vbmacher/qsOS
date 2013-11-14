#ifndef __APP__
  #define __APP__

  #define MAX_PATH 255

  typedef struct {
    char fname[8];
    char fext[3];
    unsigned char ilink_d; // odkaz na i-uzol adresara
    unsigned char ilink;   // i-uzol subora
    unsigned long link;
    unsigned long fpos;
    unsigned long fsize;
  } FileHandle;

  int Init_APP();
  void Destroy_APP();

  int fread_app(FileHandle *handle, int nsize, char *buffer, int showper);
  int fwrite_app(FileHandle *handle, int nsize, char *buffer, int showper);
  int fopen_app(FileHandle *handle, const char *filename);
  int fclose_app(FileHandle *handle);
  int feof_app(FileHandle *handle);
  unsigned long fgetpos_app(FileHandle *handle);
  void fsetpos_app(FileHandle *handle, unsigned long pos);
  int create_app(const char *filename, unsigned char ftype);
  int remove_app(const char *filename, unsigned char ftype);
  unsigned long filesize_app(const char *filename);
  char *getpath_app();
  int setpath_app(const char *path);
  int filecount_app();
  int getfname_app(const char *filename, int index);

  extern unsigned char PATH;    // aktualna cesta

#endif