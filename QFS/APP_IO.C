/*
   APP_IO.C

   (c) Copyright 2005, P. Jakubco

   Tento modul je aplikacny modul
*/

#include "app_io.h"
#include "logic_io.h"
#include "physic_i.h"
#include <stdio.h>

char a_fname[8];
char a_fext[3];
char a_filename[12];

unsigned char PATH;    // aktualna cesta
unsigned char oldPATH; // stara cesta

// funkcia prevedie a_filename do a_fname a a_fext
void convert_fn()
{
  int i, n; // pomocne premenne

  for (i = 0; i < 8; i++)
    a_fname[i] = 0;
  for (i = 0; i < 3; i++)
    a_fext[i] = 0;

  for (i = 0; i < 8; i++)
    if (a_filename[i]) {
      if (a_filename[i] != '.')
	a_fname[i] = a_filename[i];
      else break;
    } else break;

con:
  n = i;
  if (a_filename[n] == '.')
    for (i = n+1; (i <= (n+3)) && (i < 11) ; i++) {
      if (a_filename[i])
	a_fext[i-n-1] = a_filename[i];
      else break;
    }
}

// funkcia prevedie a_fname a a_fext do a_filename
void deconvert_fn()
{
  int i,n;

  for (i = 0; i < 8; i++)
    if (a_fname[i])
      a_filename[i] = a_fname[i];
    else {
      a_filename[i] = '\0';
      break;
    }
  if (!a_fext[0]) return;
  a_filename[i] = '.';
  n = i;
  for (i = n+1; (i < 11) && ((i-n-1) < 3); i++)
    if (a_fext[i-n-1])
      a_filename[i] = a_fext[i-n-1];
    else break;
  a_filename[i] = 0;
}

/******************************** EXPORT ****************************/

// Funkcia precita otvoreny subor z handle
int fread_app(FileHandle *handle, int nsize, char *buffer, int showper)
{
  int fbytes;
  if (!handle->ilink) return 0;
  if (!handle->fsize) return 0;

  fbytes = read_log(handle->fname,handle->fext,handle->ilink_d,nsize,handle->fpos, buffer, showper);
  handle->fpos += fbytes;
  return fbytes;
}

// Funkcia precita otvoreny subor z handle
int fwrite_app(FileHandle *handle, int nsize, char *buffer, int showper)
{
  unsigned int fbytes;
  if (!handle->ilink) return 0;

  fbytes = write_log(handle->fname,handle->fext,handle->ilink_d,nsize,handle->fpos, buffer, showper);
  if (fbytes) {
    handle->fsize = filesize_log(handle->fname,handle->fext,handle->ilink_d);
    handle->fpos += fbytes;
  }
  return fbytes;
}

// funkcia otvori subor a vrati odkaz na FileHandle
int fopen_app(FileHandle *handle, const char *filename)
{
  int status;
  unsigned char p1;           // pomocne premenne
  unsigned long p2;

  if (filename == NULL) return FAIL;
  if (!filename[0]) return FAIL;
  strcpy(a_filename, filename);
  convert_fn();

  if (!(status = search_file(a_fname,a_fext,1,PATH,&p1,&p2))) return FAIL;
  else if (status == DATA_ERROR) return DATA_ERROR;

  memcpy(handle->fname,a_fname,8);
  memcpy(handle->fext,a_fext,3);
  handle->ilink = p1;
  handle->ilink_d = PATH;
  handle->link = p2;
  handle->fpos = 0;
  handle->fsize = filesize_log(a_fname, a_fext, PATH);

  return SUCCESS;
}

// funkcia uzavrie subor (vlastne iba zmaze handle)
int fclose_app(FileHandle *handle)
{
  int i;
  for (i = 0; i < 8; i++)
    handle->fname[i] = 0;
  for (i = 0; i < 3; i++)
    handle->fext[i] = 0;
  handle->ilink = 0;
  handle->ilink_d = 0;
  handle->link = 0;
  handle->fpos = 0;
  handle->fsize = 0;
  return SUCCESS;
}

// funkcia zisti, ci sa dosiahol koniec suboru
int feof_app(FileHandle *handle)
{
  if (handle->fpos >= handle->fsize) return 1;
  return 0;
}

// funkcia zisti poziciu v subore
unsigned long fgetpos_app(FileHandle *handle)
{
  return handle->fpos;
}

// funkcia zisti poziciu v subore
void fsetpos_app(FileHandle *handle, unsigned long pos)
{
  handle->fpos = pos;
}

// funkcia vytvori subor (ftype = 1) / adresar (ftype = 2)
int create_app(const char *filename, unsigned char ftype)
{
  if (filename == NULL) return FAIL;
  if (!filename[0]) return FAIL;
  strcpy(a_filename,filename);
  convert_fn();
  return create_log(a_fname, a_fext, PATH, oldPATH, ftype);
}

// funkcia odstrani subor/adresar
int remove_app(const char *filename, unsigned char ftype)
{
  if (filename == NULL) return FAIL;
  if (!filename[0]) return FAIL;
  strcpy(a_filename,filename);
  convert_fn();
  return remove_log(a_fname, a_fext, PATH, ftype);
}

// funkcia nastavi cestu
// tato funkcia pracuje priamo s fyzickym modulom
// toto by tak nemalo byt...
// a funkcia pracuje jednourovnovo
int setpath_app(const char *path)
{
  int LS, status;
  unsigned char pin;

  if (path == NULL) return FAIL;
  if (!path[0]) return FAIL;
  if (!strcmp(path,"..")) {
    // o adr. vyssie - najprv treba najst prvy datablok
    PATH = oldPATH;
    // a oldPATH zistim z jeho databloku
    pin = get_inode(oldPATH);
    LS = IN.inodes[pin].link;
    decode_lp(LS);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return DATA_ERROR;
    oldPATH = dir.info.ilink;
    return SUCCESS;
  }
  strcpy(a_filename,path);
  convert_fn();
  status = search_file(a_fname,a_fext,2,PATH,&pin,&LS);
  if (!status) return FAIL;
  oldPATH = PATH;
  PATH = pin;
  return SUCCESS;
}

// funkcia zisti cestu ku suboru
// tu je mozne zacyklenie. Pridal som tu preto
// ochranu proti deadloopu tak, ze ked sa opakuje cyklus
// 300 krat bez toho, aby sa zmenili jeho parametre, tak
// sa identifikuje deadloop a cesta sa zmeni na '/', cize
// PATH = 0.
char *getpath_app()
{
  int ppos,i;
  static char path_fn[MAX_PATH]; // docasna cesta a vsetko s nou
  int cycle_loop = 0;            // dead_loop ochrana

  int LS, n;                     // log. sektor
  unsigned char pin;             // pom. ukazatel na iuzol
  int me,parent;
  int found;

  me = PATH;
  parent = oldPATH;

  path_fn[0] = '/';
  path_fn[1] = '\0';
  ppos = 1;
  while (parent || me) {
    cycle_loop++;
    // citanie rodica
    pin = get_inode(parent);
    LS = IN.inodes[pin].link;
    if (!LS) return path_fn;
    decode_lp(LS);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return NULL;

    found = 0;
    for (i = 0; i < dir.info.fcount; i++)
      if (dir.files[i].ilink == me) {
	n = get_inode(dir.files[i].ilink);
	if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN)) return NULL;
	if (IN.inodes[n].ftype == 2) { // musi to byt dir
	  cycle_loop = 0;
	  memcpy(a_fname,dir.files[i].fname,8);
	  memcpy(a_fext,dir.files[i].fext,3);
	  deconvert_fn();
	  if (ppos >= MAX_PATH) break;
	  n = strlen(path_fn);
	  memmove(path_fn+strlen(a_filename)+1,path_fn, n);
	  ppos = n;
	  memcpy(path_fn+1,a_filename,strlen(a_filename));
	  ppos += strlen(a_filename);
	  path_fn[ppos] = '/';
	  path_fn[++ppos] = '\0';
	  me = parent;
	  parent = dir.info.ilink;
	  found = 1;
	  break;
	}
      }
    if (ppos >= MAX_PATH) return NULL;
    if (!found)
      if (dir.info.link)
	LS = dir.info.link;
    if (cycle_loop == 300) {
      // nastal dead_loop
      PATH = 0;
      return NULL;
    }
  }

  return path_fn;
}

// funkcia zisti velkost suboru v bytoch
unsigned long filesize_app(const char *filename)
{
  if (filename == NULL) return FAIL;
  if (!filename[0]) return FAIL;

  strcpy(a_filename,filename);
  convert_fn();

  return filesize_log(a_fname,a_fext,PATH);
}

// funkcia vrati meno suboru do filename v poradi index
// malo by byt alokovane 12 bytov vo filename
int getfname_app(const char *filename, int index)
{
  int status;

  if (filename == NULL) return FAIL;

  strcpy(a_filename,filename);
  convert_fn();
  status = getfname_log(a_fname,a_fext,PATH,index);
  deconvert_fn();
  strcpy(filename,a_filename);

  return status;
}

// funkcia vrati pocet suborov v adresari
int filecount_app()
{
  return filecount_log(PATH);
}


/* MODULOVE FUNKCIE  */

int Init_APP()
{
  PATH = 0;
  return 0;
}

void Destroy_APP()
{
  PATH = 0;
}