/*
   MAIN_FNC.H

   (c) Copyright 2005, vbmacher

*/

#ifndef __MAIN_FNC__
#define __MAIN_FNC__

  void about();
  int format_fdd();
  int create_file(char *s);
  int create_dir(char *s);
  int delete_file(char *s);
  int delete_dir(char *s);
  int write_file(char *s, char *buffer, unsigned long pos, unsigned buflen, int user_input, int show_origper);
  int read_file(char *s, char *buffer, unsigned long pos, unsigned buflen, int user_input, int show_origper);
  int set_path(char *s);
  void show_dir();
  void dotest_fdd();
  void disk_info();
  void reboot_system();


#endif