/*
    MAIN_FNC.C

    (c) Copyright 2005, vbmacher

    programove funkcie hlavneho modulu

*/
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include "logic_io.h"
#include "app_io.h"
#include "main.h"

char cfname[16];

// funkcia cita z klavesnice text do fname
// pred tym, ako caka na vstup, vypise na obrazovku text (prem. text)
char *user_input(char *fname, char *text, int len)
{
  cprintf(text);
  // max. dlzka vstupu
  cfname[0] = len;
  fflush(stdin);
  cgets(cfname);
  fname = cfname + 2;
  return (cfname+2);
}

/* EXPORT */

// vypis informacii o programe
void about()
{
  textattr(0x0A);
  cprintf("\n\rAbout QFSemu\n\r");
  textattr(0x0F);
  cprintf("\n\rQFSemu v1.00\n\rQuick File System emulator\n\rWithout dynamic memory allocation\n\r");
  cprintf("\n\r(c) Copyright 2005, vbmacher");
}

// funkcia naformatuje disketu
int format_fdd()
{
  int status;
  status = format_log();
  if (status == DATA_ERROR) {
    textattr(ERROR);
    cprintf("\n\rFormat incomplete. Data error. STATUS: %d", status);
    textattr(BORDER);
  }
  else if (status == FAIL) {
    textattr(ERROR);
    cprintf("\n\rFormat incomplete. Failed. STATUS: %d", status);
    textattr(BORDER);
  }
  else {
    textattr(OK);
    cprintf("\n\rFormat complete. STATUS: %d", status);
    textattr(BORDER);
  }
  return status;
}

// funkcia vytvori subor (nie adresar!) na diskete
// vstupom je parameter fil. Ten moze byt aj NULL.
// ak je null, zavola sa uzivatelsky vstup z klavsenice
int create_file(char *fil)
{
  char *fname;
  int status;

  if (fil == NULL)
    fname = user_input(fname,"\n\rEnter (new) file name: ",13);
  else {
    strcpy(cfname,fil);
    fname = cfname;
  }
  status = create_app(fname,1);
  if (status == DATA_ERROR) {
    textattr(ERROR);
    cprintf("\n\rError: file NOT created. Data error. STATUS: %d", status);
    textattr(BORDER);
  }
  else if (status == FAIL) {
    textattr(ERROR);
    cprintf("\n\rError: file NOT created. Failed. STATUS: %d", status);
    textattr(BORDER);
  }
  else {
    textattr(OK);
    cprintf("\n\rFile created successfully. STATUS: %d", status);
    textattr(BORDER);
  }
  return status;
}

// funkcia vytvori adresar (nie subor!)
// vstupom je parameter fil. Ten moze byt aj NULL.
// ak je null, zavola sa uzivatelsky vstup z klavsenice
int create_dir(char *fil)
{
  char *fname;
  int status;

  if (fil == NULL)
    fname = user_input(fname,"\n\rEnter (new) dir name: ",13);
  else {
    strcpy(cfname,fil);
    fname = cfname;
  }
  status = create_app(fname,2);
  if (status == DATA_ERROR) {
    textattr(ERROR);
    cprintf("\n\rError: dir NOT created. Data error. STATUS: %d", status);
    textattr(BORDER);
  }
  else if (status == FAIL) {
    textattr(ERROR);
    cprintf("\n\rError: dir NOT created. Failed. STATUS: %d", status);
    textattr(BORDER);
  }
  else {
    textattr(OK);
    cprintf("\n\rDir created successfully. STATUS: %d", status);
    textattr(BORDER);
  }
  return status;
}

// funkcia zmaze subor (nie adresar!)
// vstupom je parameter fil. Ten moze byt aj NULL.
// ak je null, zavola sa uzivatelsky vstup z klavsenice
int delete_file(char *fil)
{
  char *fname;
  int status;

  if (fil == NULL)
    fname = user_input(fname,"\n\rEnter (existing) file name: ",13);
  else {
    strcpy(cfname,fil);
    fname = cfname;
  }
  status = remove_app(fname,1);
  if (status == DATA_ERROR) {
    textattr(ERROR);
    cprintf("\n\rError: file NOT removed. Data error. STATUS: %d", status);
    textattr(BORDER);
  }
  else if (status == FAIL) {
    textattr(ERROR);
    cprintf("\n\rError: file NOT removed. Failed. STATUS: %d", status);
    textattr(BORDER);
  }
  else {
    textattr(OK);
    cprintf("\n\rFile removed successfully. STATUS: %d", status);
    textattr(BORDER);
  }
  return status;
}

// funkcia zmaze adresar (nie subor!)
// vstupom je parameter fil. Ten moze byt aj NULL.
// ak je null, zavola sa uzivatelsky vstup z klavsenice
int delete_dir(char *fil)
{
  char *fname;
  int status;

  if (fil == NULL)
    fname = user_input(fname,"\n\rEnter (existing) dir name: ",13);
  else {
    strcpy(cfname,fil);
    fname = cfname;
  }
  status = remove_app(fname,2);
  if (status == DATA_ERROR) {
    textattr(ERROR);
    cprintf("\n\rError: dir NOT removed. Data error. STATUS: %d", status);
    textattr(BORDER);
  }
  else if (status == FAIL) {
    textattr(ERROR);
    cprintf("\n\rError: dir NOT removed. Failed. STATUS: %d", status);
    textattr(BORDER);
  }
  else {
    textattr(OK);
    cprintf("\n\rDir removed successfully. STATUS: %d", status);
    textattr(BORDER);
  }
  return status;
}

// funkcia zapise do suboru data
// parameter fil moze byt aj NULL. ak je null,
// zavola sa uzivatelsky vstup z klavsenice
// parameter pos: pozicia v subore na diskete
// buffer: odkial sa ma zapisovat
// buflen: velkost zapisovanych dat
// u_input: ci uzivatel nastavuje aj poziciu a dlzku zapisovanych
// dat (v tom pripade sa parametre pos a buflen nepouziju)
int write_file(char *fil, char *buffer, unsigned long pos,
  unsigned buflen, int u_input, int show_origper)
{
  char *fname;
  char cfdosname[20];
  char *fdosname;
  FileHandle fh;
  FILE *fr;
  int status;
  int bufptr = 0;
  int bytes = 0;
  int DOSF;
  char cbuf[505];
  char *buf;
  int usebuf = 1;
  clock_t t1,t2;

  t1 = clock();
  DOSF = 0;
  if (fil == NULL) {
    fname = user_input(fname,"\n\rEnter (existing) file name: ",13);
    if (!strcmp(fname, "")) {
      textattr(ERROR);
      cprintf("\n\rError: file name can not be empty");
      textattr(BORDER);
      status = 0;
      goto ex;
    }
    strcpy(cfname, fname);
    fname = cfname;
  }
  else {
    strcpy(cfname,fil);
    fname = cfname;
  }
  status = fopen_app(&fh, fname);
  if (status) {
    textattr(ERROR);
    cprintf("\n\rError: File %s does NOT exist",fname);
    textattr(BORDER);
    cprintf("\n\rDo you want to create the file [Y/N] ? ");
    status = getchar();
    if (status == 'Y' || status == 'y') {
      status = create_file(fname);
      if (status) goto ex;
      status = fopen_app(&fh, fname);
      if (status) goto ex;
    }
    else {
      status = 0;
      goto ex;
    }
  }
  // ak je buffer prazdny, citanie zo subora z disku (FAT)
  if (buffer == NULL) {
    cprintf("\n\rEnter (existing) DOS file name: ");
    cfdosname[0] = 17;
    cgets(cfdosname);
    fdosname = cfdosname + 2;
    if (!strcmp(fdosname,"")) {
      cprintf("\n\rUsing file: '%s'", fname);
      strcpy(fdosname,fname);
    }
    fr = fopen(fdosname,"rb");
    if (fr == NULL) {
      textattr(ERROR);
      cprintf("\n\rError: File %s does NOT exist",fdosname);
      textattr(BORDER);
      status = 0;
      goto ex;
    }
    DOSF = 1;
    buf = cbuf;
  } else buf = buffer;
  if (u_input) {
    cprintf("\n\rEnter a position in file for writing: ");
    cscanf("%ld", &pos);
    fflush(stdin);
    cprintf("\n\rDo you want read whole DOS file ? [Y/N] ");
    status = getchar();
    if (status == 'Y' || status == 'y')
      usebuf = 0;
    else {
      usebuf = 1;
      cprintf("\n\rEnter read size (bytes): ");
      cscanf("%d", &buflen);
      fflush(stdin);
    }
  }
  fsetpos_app(&fh,pos);
  cprintf("\n\rFile position: %ld", fgetpos_app(&fh));
  cprintf("\n\rWriting...\n\r");
DOS_read:
  if (DOSF) {
    // predpokl. velkost buffera: 512 bytov
    if (!usebuf) {
      bytes = 0;
      while ((status = fgetc(fr)), !feof(fr)) {
	bytes++;
	*(buf+bufptr) = status;
	bufptr++;
	if (bytes == 505) {
	  bufptr = 0;
	  goto wr_next;
	}
      }
    } else {
      if (buflen > 505) {
	bytes = fread(buf+bufptr, 505, 1, fr);
	bytes *= 505;
	buflen -= 505;
	bufptr += 505;
      }
      else {
	bytes = fread(buf+bufptr, buflen, 1, fr);
	bytes *= buflen;
	bufptr += buflen;
	buflen = 0;
      }
    }
    if (feof(fr)) {
     fclose(fr);
     DOSF = 0;
    }
    if (!bytes) goto ex;
  } else bytes = buflen;
wr_next:
  status = fwrite_app(&fh,bytes,buf,show_origper);
  if (!status) goto ex;
  if (DOSF) goto DOS_read;
ex:
  t2 = clock();
  cprintf(" | Time: %lf", (t2 - t1)/(double)CLK_TCK);

  if (!status) {
    textattr(ERROR);
    cprintf("\n\rError: File NOT saved. Bytes written: %d", status);
    textattr(BORDER);
  }
  else {
    textattr(OK);
    cprintf("\n\rFile saved. Bytes written: %d", status);
    textattr(BORDER);
  }
  fclose_app(&fh);
  fflush(stdout);
  fflush(stdin);
  cprintf("\n\rFile size: %ld B", filesize_app(fname));

  return status;
}

// funkcia cita zo suboru data
// popis parametrov analogicky ako v predch. fcii
int read_file(char *fil, char *buffer, unsigned long pos,
  unsigned buflen, int u_input, int show_origper)
{
  char *fname;
  char cfdosname[20];
  char *fdosname;
  FileHandle fh;
  FILE *fw;
  int status;
  int bufptr = 0;
  int bytes = 0;
  int DOSF;
  char cbuf[505];
  char *buf;
  int usebuf = 1;
  long size,sizeDOS;
  clock_t t1,t2;

  t1 = clock();
  DOSF = 0;
  bytes = 0;
  if (fil == NULL)
    fname = user_input(fname,"\n\rEnter (existing) file name: ",13);
  else {
    strcpy(cfname,fil);
    fname = cfname;
  }
  status = fopen_app(&fh, fname);
  if (status) {
    textattr(ERROR);
    cprintf("\n\rError: File %s does NOT exist",fname);
    textattr(BORDER);
    status = 0;
    goto ex;
  }
  // ak je buffer prazdny, citanie zo subora na disk (FAT)
  if (buffer == NULL) {
    cprintf("\n\rEnter DOS file name: ");
    cfdosname[0] = 17;
    cgets(cfdosname);
    fdosname = cfdosname + 2;
    if (!strcmp(fdosname,"")) {
      cprintf("\n\rUsing file: '%s'", fname);
      strcpy(fdosname,fname);
    }
    fw = fopen(fdosname,"wb");
    if (fw == NULL) {
      textattr(ERROR);
      cprintf("\n\rError: Can't open file %s",fdosname);
      textattr(BORDER);
      status = 0;
      goto ex;
    }
    DOSF = 1;
    buf = cbuf;
  } else buf = buffer;
  if (u_input) {
    cprintf("\n\rEnter a position in file for reading: ");
    cscanf("%ld", &pos);
    fflush(stdin);
    cprintf("\n\rDo you want read whole file ? [Y/N] ");
    status = getchar();
    if (status == 'Y' || status == 'y')
      usebuf = 0;
    else {
      usebuf = 1;
      cprintf("\n\rEnter read size (bytes): ");
      cscanf("%d", &buflen);
      fflush(stdin);
    }
  }
  fsetpos_app(&fh,pos);
  cprintf("\n\rFile position: %ld", fgetpos_app(&fh));
  cprintf("\n\rReading...\n\r");
DOS_read:
  if (DOSF) {
    // predpokl. velkost buffera: 512 bytov
    if (!usebuf) {
      while (bytes = fread_app(&fh,505,buf,show_origper))
	fwrite(buf,bytes,1,fw);
    } else {
      if (buflen > 505) {
	while (bytes = fread_app(&fh,505,buf,show_origper)) {
	  fwrite(buf,bytes,1,fw);
	  if (buflen > 505) buflen -= 505;
	  else break;
	}
	bytes = fread_app(&fh,buflen,buf,show_origper);
	fwrite(buf,bytes,1,fw);
      }
      else {
	bytes = fread_app(&fh,buflen,buf,show_origper);
	fwrite(buf,bytes,1,fw);
      }
    }
    fclose(fw);
  } else {
    bufptr = 0;
    if (!usebuf) {
      while (bytes = fread_app(&fh,505,buf+bufptr,show_origper))
	bufptr += bytes;
    } else {
      if (buflen > 505) {
	while (bytes = fread_app(&fh,505,buf+bufptr,show_origper)) {
	  bufptr += bytes;
	  if (buflen > 505) buflen -= 505;
	  else break;
	}
	bytes = fread_app(&fh,buflen,buf+bufptr,show_origper);
	bufptr += bytes;
      }
      else {
	bytes = fread_app(&fh,buflen,buf,show_origper);
	bufptr = 1;
      }
    }
  }
ex:
  t2 = clock();
  cprintf(" | Time: %lf", (t2 - t1)/(double)CLK_TCK);

  size = filesize_app(fname);
  if (DOSF) {
    status = open(fdosname, O_RDONLY);
    sizeDOS = filelength(status);
    cprintf("\n\rDOS file size: %ld B", sizeDOS);
    close(status);
    if (size == sizeDOS) bufptr = 1;
    else bufptr = 0;
  }

  cprintf("\n\rFile size: %ld B", size);
  if (!bufptr) {
    textattr(ERROR);
    cprintf("\n\rError: File NOT read. Bytes read: %ld", size);
    textattr(BORDER);
  }
  else {
    textattr(OK);
    cprintf("\n\rFile read. Bytes read: %d", buflen);
    textattr(BORDER);
  }
  fclose_app(&fh);
  fflush(stdout);
  fflush(stdin);

  return bytes;
}

// funkcia nastavi novu cestu
// ak je param. fil == NULL, potom sa cesta nacita z klavesnice
int set_path(char *fil)
{
  char *fname;
  int status;

  if (fil == NULL)
    fname = user_input(fname,"\n\rEnter (new) path (.. up-level): ",13);
  else {
    strcpy(cfname,fil);
    fname = cfname;
  }
  status = setpath_app(fname);
  if (status) {
    textattr(ERROR);
    cprintf("\n\rDir '%s' does NOT exist. STATUS: %d", fname,status);
    textattr(BORDER);
  }
  cprintf("\n\rPath: %s", getpath_app());
  return status;
}

// fcia zobrazi strukturu aktualneho adresara
void show_dir()
{
  char fn[12];
  int n,fcount,ftype;

  cprintf("\n\rPath: %s\n\r", getpath_app());

  textattr(sDIR);
  fcount = filecount_app();
  if (PATH > 0)
    cprintf("\n\r    ..");
  for (n = 0; n < fcount; n++) {
    ftype = getfname_app(fn,n);
    if (ftype == 2)
      cprintf("\n\r    %s", fn);
  }
  textattr(sFILE);
  for (n = 0; n < fcount; n++) {
    ftype = getfname_app(fn,n);
    if (ftype == 1)
      cprintf("\n\r    %s | %ld B", fn, filesize_app(fn));
  }
  textattr(BORDER);
  cprintf("\n\r\n\rFile count: %d", fcount);
}

// funkcia vykona test diskety
void dotest_fdd()
{
  int status;
  char data[18];
  char fn[12];
  FileHandle fh;

  textattr(sFILE);
  cprintf("\n\rRUNNING TEST");
  textattr(ERROR);
  cprintf("\n\rALL DATA ON DISK WILL BE LOST !");
  textattr(BORDER);
  status = format_fdd();
  if (status) goto ex;
  cprintf("\n\rFree space: %ld B", freespace_log());
  cprintf("\n\rDisk capacity: %ld B", diskcap_log());
  status = create_dir("MYDIR.001");
  if (status) goto ex;
  status = create_file("peter.txt");
  if (status) goto ex;
  write_file("peter.txt","THIS IS A TEST", 2005, 14,0,1);
  cprintf("\n\rFile size (peter.txt): %ld B", filesize_app("peter.txt"));
  show_dir();
  status = set_path("MYDIR.001");
  if (status) goto ex;
  status = create_dir("MYDIR.002");
  if (status) goto ex;
  status = create_file("tomas.txt");
  if (status) goto ex;
  write_file("tomas.txt","WHAT'S UP NEGeR ?", 190005, 17,0,1);
  cprintf("\n\rFile size (tomas.txt): %ld B", filesize_app("tomas.txt"));
  set_path("..");

  cprintf("\n\rReading file: peter.txt\n\r");
  read_file("peter.txt", data,2005,14,0,1);
  data[14] =0;
  cprintf("\n\r%s",data);

  cprintf("\n\rReading file: tomas.txt\n\r");
  if (set_path("MYDIR.001")) goto ex;
  read_file("tomas.txt", data,190005,17,0,1);
  data[17]=0;
  cprintf("\n\r%s",data);

  cprintf("\n\rFree space: %ld B", freespace_log());
  set_path("..");
  status = delete_dir("MYDIR.001");
  if (status) goto ex;
  delete_file("peter.txt");
  cprintf("\n\rFree space: %ld B", freespace_log());
ex:
}

// fcia zisti informacie o diskete a jej parametroch
void disk_info()
{
  cprintf("\n\rSectors per track: %d", SPT_FDD);
  cprintf("\n\rSector count: %d", SECT_COUNT);
  cprintf("\n\rDisk capacity: %ld B", diskcap_log());
  cprintf("\n\rFree space: %ld B", freespace_log());
  cprintf("\n\rPath: %s", getpath_app());
}

void reboot_system()
{
  asm {
    cli
    xor ax, ax
    mov es, ax
    mov bx, 0x472
    mov word ptr es:[bx], 0x1234
    db 0x0EA
    dw 0
    dw 0x0FFFF
  }
}
