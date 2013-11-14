/*
   LOGIC_IO.C

   (c) Copyright 2005, Peter Jakubco

   Modul spravuje logicku vrstvu QFS

*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "physic_i.h"
#include "logic_io.h"
#include "main.h"
#include "boot.h"

xPP PP;
dbfile file;   // globalne buffery
dbdir dir;
inodes_sec IN;
char *file_type[] = { "nothing", "file", "dir" };

// funkcia dekoduje logicke parametre na fyzicke
void decode_lp(int LS)
{
  PP.TRACK = LS / (2 * SPT_FDD);
  PP.SECTOR = LS % (SPT_FDD) + 1;
  PP.HEAD = ((LS % (2 * SPT_FDD)) < SPT_FDD) ? 0 : 1;
}

// funkcia enkoduje fyzicke na logicke parametre
int encode_pp()
{
  return (2 * SPT_FDD * PP.TRACK + PP.SECTOR + PP.HEAD * SPT_FDD);
}

// funkcia najde odkaz na 1. datablok suboru / adresara
// vracia v tvare LS
int find_db(unsigned char pin)
{
  pin = get_inode(pin);
  return (IN.inodes[pin].link);
}

// funkcia najde volny i-uzol. Ak nie je najdeny
// pozor!!! : vtedy vrati (2*INODE_COUNT+1)
// inak normalne inode
int find_fin()
{
  int i, LS;

  LS = SUP_1;
  while (LS <= SUP_2) {
    decode_lp(LS);
    read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN);

    for (i = 0; i < INODE_COUNT; i++)
      if (!IN.inodes[i].ftype)
	return (i + (LS-1)*INODE_COUNT);
    LS++;
  }

  return (2*INODE_COUNT+1);  // chybovy stav
}

// funkcia zisti, ci je volna polozka v adresari
// vrati poziciu v databloku (norm)
// vrati (FILE_COUNT+1) (fail)
// db najprv obsahuje cislo databloku
// premennej dir, potom v db vrati cislo databloku, kde je volne miesto
int find_ffil(dbdir *Rdir, int *db)
{
  int i, pdb;

  pdb = *db;
srch:
  for (i = 0; i < FILE_COUNT; i++)
    if (!Rdir->files[i].fname[0]) {
      *db = pdb;
      return i;
    }
  if (Rdir->info.link) {
    pdb = Rdir->info.link;
    decode_lp(pdb);
    read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)Rdir);
    goto srch;
  }
  *db = pdb;
  return (FILE_COUNT+1);
}

// funkcia najde n volnych dblokov a vrati odkaz na posledny
// v pripade, ze parameter old_db je 0, zacina sa hladat od zaciatku
// inak od neho
int find_fdb(int n, int old_db)
{
  int i;         // pom. premenna
  int count = 0; // dosiahnuty pocet najdenych dblokov

  if (!old_db) old_db = SUP_2+2;
  for (i = old_db; i < SECT_COUNT; i++) {
    decode_lp(i);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) continue; // chybne nebereme
    if (!dir.info.fcount) {
      count++;
      if (n == count) return i;
    }
  }
  return 0;
}

// funkcia vyhlada subor/adresar. Do i_node: 1-subor, 2-adresar
// vrati: do ilink odkaz na iuzol, do db odkaz na 1. datablok
// navrat.hodnota: 1 - nasiel sa, 0 - nenasiel sa
int search_file(char fname[8], char fext[3], unsigned char i_node, unsigned char idir, unsigned char *ilink, int *db)
{
  int LS;
  unsigned char pin;
  int i;

  // 1. datablok adresara
  LS = find_db(idir);
  if (!LS) goto FDB_done;
FDB:
  decode_lp(LS);
  if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return DATA_ERROR;

  if (dir.info.fcount > 0)
    for (i = 0; i < FILE_COUNT; i++)
      if (!memcmp(dir.files[i].fname,fname,8) && !memcmp(dir.files[i].fext,fext,3)) {
	pin = get_inode(dir.files[i].ilink);
	if (IN.inodes[pin].ftype == i_node) {
	  *ilink = dir.files[i].ilink;
	  *db = IN.inodes[pin].link;
	  return 1;
	}
      }
  if (dir.info.link) LS = dir.info.link;
  else goto FDB_done;
  goto FDB; //ak su viac datablokov adresara
FDB_done:
  return 0;
}

// fcia vymaze buffer
void clear_buffer(unsigned char *buffer, int nsize)
{
  int i;
  for (i = 0; i < nsize; i++) buffer[i] = 0;
}



/************************** EXPORT *****************************/

// funkcia najde dany i-uzol
unsigned char get_inode(unsigned char pin)
{
  int LS;

  // inody su na LS od 1-2
  if (pin <= INODE_COUNT) LS = SUP_1;
  else LS = SUP_2;
  decode_lp(LS);
  read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN);
  pin -= (LS-1)*INODE_COUNT;
  return pin;
}


// funkcia naformatuje disketu systemom QFS
int format_log()
{
  int i;

  cprintf("\n\rVerifying disk...");
  for (i = 1; i < 4; i++) {
    decode_lp(i);
    if (read_phy(PP.HEAD, PP.TRACK, PP.SECTOR, 1, (void *)&IN)) return DATA_ERROR;
  }

  cprintf("\n\rFormatting...\n\r");

  clear_buffer((unsigned char *)&IN, sizeof(inodes_sec));
  // do 1. sektora => bootloader
  memcpy(&IN,data,494);
  decode_lp(0);
  write_phy(PP.HEAD, PP.TRACK, PP.SECTOR, 1, (void *)&IN);
  clear_buffer((unsigned char *)&IN, sizeof(inodes_sec));
  IN.inodes[0].ftype = 2;
  IN.inodes[0].link = 3; // hlavny adresar musi byt tiez prideleny
  decode_lp(1);
  write_phy(PP.HEAD, PP.TRACK, PP.SECTOR, 1, (void *)&IN);
  clear_buffer((unsigned char *)&IN, sizeof(inodes_sec));
  textattr(PERCENT);
  for (i = 2; i < SECT_COUNT; i++) {
    decode_lp(i);
    write_phy(PP.HEAD, PP.TRACK, PP.SECTOR, 1, (void *)&IN);
    cprintf("\r  %3.2lf %%", (double)((double)i*100.0 / ((double)SECT_COUNT-1.0)));
  }
  textattr(BORDER);
  return SUCCESS;
}

// funkcia vytvori novy subor/podadresar na disku
// parameter idir:        odkaz na rodica (dblok adresara kde sa ma vyt)
//           idir_parent: odkaz na rodica idiru
int create_log(char fname[8], char fext[3],
    unsigned char idir, unsigned char idir_parent,
    unsigned char ftype)
{
  int LS;           // pomocny - logicky sektor
  int new_in, fpos; // pomocne odkazy
  int new_db;
  int i;            // pomocne premenne
  unsigned char pin;
  unsigned long p_l;

  // hladanie suboru/podadresara
  if (search_file(fname,fext,ftype,idir, NULL, NULL)) return FAIL;

  fpos = 0;
  // hladanie volneho i-uzla
  new_in = find_fin();
  if (new_in == (2*INODE_COUNT+1)) return FAIL;
  // 1. datablok adresara
  LS = find_db(idir);
try_next_pos:
  if (LS) {
    decode_lp(LS);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void*)&dir)) return DATA_ERROR;
    // hladanie volneho miesta v databloku pre adresar
    fpos = find_ffil(&dir, &LS);
  }
  if ((fpos == (FILE_COUNT+1)) || !LS) {
    // treba vytvorit novy datablok pre adresar
    new_db = find_fdb(1,0);
    if (!new_db) return FAIL;
    fpos = 0;
    if (LS) {  // ak sa subor nezmestil do prveho dbloku
      if (dir.info.link) {
	LS = dir.info.link;
	goto try_next_pos;
      }
      clear_buffer((void *)&dir,sizeof(dbdir));
      dir.info.link = new_db;
      dir.info.me_ilink = idir;
      dir.info.ilink = idir_parent;
      decode_lp(LS); // ls ukazuje na posledny db adresara
      write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void*)&dir);
    } else {
      // ak nie je 1. dblok adresara
      // zapis do i-uzla 1. datablok adresara
      pin = get_inode(idir);
      IN.inodes[pin].link = new_db;
      write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN);
    }
    LS = new_db;
    // informacie o adresari ma iba jeho 1. datablok.
  }
  // zapis suboru/podadresara do adresara
  dir.info.ilink = idir_parent;
  dir.info.me_ilink = idir;
  memcpy(dir.files[fpos].fname,fname,8);
  memcpy(dir.files[fpos].fext,fext,3);
  dir.files[fpos].ilink = new_in;
  dir.info.fcount++;
  decode_lp(LS);
  write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir);
  // zapis iuzla
  new_in = get_inode(new_in);
  IN.inodes[new_in].ftype = ftype;
  IN.inodes[new_in].link = 0;
  write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN);
  return SUCCESS;
}

// fcia vymaze subor/podadresar
int remove_log(char fname[8], char fext[3], unsigned char idir, unsigned char ftype)
{
  int LS;              // pomocne logicke sektory
  int db, old_db;      // --||--||-- ...
  int i, pin, fcount;
  unsigned char ilink;
  int status;

  // hladanie suboru/podadresara
  if (!(status = search_file(fname,fext,ftype,idir,&ilink,&db))) return FAIL;
  else if (status == DATA_ERROR) return DATA_ERROR;

  cprintf("\n\rRemoving %s...", file_type[ftype]);
  LS = db;
rem_db:
  if (!db) goto rem_rem;  // ak subor nema datablok, treba ho z adr. vymazat
  // mazanie dblokov
  decode_lp(db);
  if (ftype == 1) {
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file)) return DATA_ERROR;
    LS = file.info.link;  // odkaz na dalsi datablok
    // mazanie buffera
    clear_buffer((unsigned char *)&file, sizeof(dbfile));
    write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file);  // vymaz databloku
    db = LS;
    goto rem_db;
  } else if (ftype == 2) {
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return DATA_ERROR;
    // nasl. cyklus vymaze rekurzivne vsetky subory a podadresare
    // mazaneho adresara
    LS = dir.info.link;
    for (i = 0; i < FILE_COUNT; i++)
      if (dir.files[i].fname[0]) {
	pin = get_inode(dir.files[i].ilink);
	status = remove_log(dir.files[i].fname,dir.files[i].fext,ilink,IN.inodes[pin].ftype);
	if (status) return status;
	decode_lp(db);
	if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return DATA_ERROR;
      }
    // a teraz samotny adresar a jeho databloky
    clear_buffer((unsigned char *)&dir, sizeof(dbdir));
    decode_lp(db);
    write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir);
    db = LS;
    goto rem_db;
  }

rem_rem:
  // zmazanie polozky v adresari => POZOR ak je cely dblok prazdny,
  // musim ho uvolnit !!!
  db = find_db(idir);
  old_db = 0;
rem_next:
  decode_lp(db);
  if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return DATA_ERROR;
  old_db = db;
  db = dir.info.link;
  for (i = 0; i < FILE_COUNT; i++)
    if (!memcmp(dir.files[i].fname,fname,8) && !memcmp(dir.files[i].fext,fext,3)) {
      clear_buffer((unsigned char *)&dir.files[i], sizeof(dirdata));
      dir.info.fcount--; // na toto pozor !!!
      write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir);
      if (!dir.info.fcount) {  // ak uz je dblok prazdny, musim ho uvolnit
	decode_lp(old_db);
	if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return FAIL;
	dir.info.link = 0;
	write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir);
      }
      // vymazanie i-uzla
      pin = get_inode(ilink);
      IN.inodes[pin].ftype = 0;
      IN.inodes[pin].link = 0;
      write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN);
      return SUCCESS;
    }

  if (db) goto rem_next;
  return FAIL;
}

// zapis do suboru
// vrati skutocne zapisane byty
// showper sluzi na to, ci sa maju zobrazovat percenta
unsigned int write_log(char fname[8], char fext[3],
  unsigned char idir, unsigned int dcount,
  unsigned long dpos, char *buf, int showper)
{
  int oLS, LS;                  // pomocne logicke sektory
  char *buffer;            // pomocny buffer
  unsigned int pos, ret_val;    // navratova hodnota = dcount na konci
  unsigned char fil_ilink;
  int fil_db, db,i;
  double per_max, per_spos;

  if (!dcount) return 0;

  buffer = buf;
  ret_val = 0;
  // hladanie suboru
  if (!search_file(fname,fext,1,idir,&fil_ilink,&fil_db)) return 0;

  oLS = -1;
  LS = fil_db;
  db = (int)(dpos / (long)DATA_COUNT) + 1;
  pos = (int)(dpos - (long)(((long)db-(long)1)*(long)DATA_COUNT));
  per_spos = 0.0;
  per_max = dpos+dcount;

  // nastavenie pozicie a zapisovanie
  i = 1;
write_more:
  while (i <= db) {
    per_spos = ((double)i-1.0)*(double)DATA_COUNT;
    i++;
    if (showper) {
      textattr(PERCENT);
      cprintf("\r  %3.2lf \%  ",(100.0*per_spos) / per_max);
      textattr(BORDER);
    }
    if (!LS) {
      if (!(LS = find_fdb(1,oLS+1))) return ret_val;
      else if (!fil_db) {
	fil_db = LS;
	fil_ilink = get_inode(fil_ilink);
	IN.inodes[fil_ilink].ftype = 1;
	IN.inodes[fil_ilink].link = fil_db;
	write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN);
      }
    }
    if (oLS != -1) {
      decode_lp(oLS);
      if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file)) return ret_val;
      file.info.dcount = (unsigned int)DATA_COUNT;
      file.info.link = LS;
      write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file);
      oLS = -1;
    }
    decode_lp(LS);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file)) return ret_val;
    if (!file.info.link) oLS = LS;
    LS = file.info.link;
  }
  per_spos = ((double)db-1.0)*(double)DATA_COUNT;
  per_spos += pos;
  per_spos += ret_val;
  if (showper) {
    textattr(PERCENT);
    cprintf("\r  %3.2lf \% ",(100.0*per_spos) / per_max);
    textattr(BORDER);
  }
  if ((DATA_COUNT - pos) > dcount) {
    memcpy((void *)(file.data+pos),(void *)buffer,dcount);
    if (file.info.dcount < (dcount+pos))
      file.info.dcount = dcount+pos;
    write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file);
    ret_val += dcount;
    per_spos += (double)dcount;
  } else {
    file.info.dcount = (unsigned int)DATA_COUNT;
    memcpy((void *)(file.data+pos),(void *)buffer,((unsigned int)DATA_COUNT - pos));
    write_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file);
    ret_val += ((unsigned int)DATA_COUNT-pos);
    buffer += ((unsigned int)DATA_COUNT-pos);
    dcount -= ((unsigned int)DATA_COUNT-pos);
    db++;
    pos = 0;
    goto write_more;
  }
  if (showper) {
    textattr(PERCENT);
    cprintf("\r  %3.2lf \% ",(100.0*per_spos) / per_max);
    textattr(BORDER);
  }
  return ret_val;
}

// citanie zo suboru
// vrati skutocne precitane byty
unsigned int read_log(char fname[8], char fext[3], unsigned char idir, unsigned int dcount, unsigned long dpos, char *buf, int showper)
{
  int LS;                  // pomocne logicke sektory
  int fil_db;              // 1.datablok suboru
  unsigned char fil_ilink; // inode suboru
  int db, pos_db, i;       // poradie dbloku, pozicia dat v dbloku, pomocna premenna
  unsigned int ret_val;    // navratova hodnota = dcount na konci
  char *buffer;		   // pomocny buffer
  double per_max, per_spos;  // pomoc pri percentach

  if (dcount <= 0) return 0;

  buffer = buf;
  ret_val = 0;
  // hladanie suboru
  if (!search_file(fname,fext,1,idir,&fil_ilink,&fil_db)) return 0;

  db = (int)(dpos / (long)DATA_COUNT) + 1;
  pos_db = (int)(dpos - (long)(((long)db-(long)1)*(long)DATA_COUNT));

  if (!fil_db) return 0;

  i = 1;
  per_spos = 0.0;
  per_max = dpos+dcount;
  LS = fil_db;
read_more:
  while (i != db) {
    per_spos = ((double)i-1.0)*(double)DATA_COUNT;
    if (showper) {
      textattr(PERCENT);
      cprintf("\r  %3.2lf \% ",(100.0*per_spos) / per_max);
      textattr(BORDER);
    }
    decode_lp(LS);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file)) return 0;
    if (!file.info.link) return ret_val;
    else LS = file.info.link;
    i++;
  }
  per_spos = ((double)db-1.0)*(double)DATA_COUNT;
  per_spos += pos_db;
  per_spos += ret_val;
  if (showper) {
    textattr(PERCENT);
    cprintf("\r  %3.2lf \% ",(100.0*per_spos) / per_max);
    textattr(BORDER);
  }
  decode_lp(LS);
  if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file)) return 0;
  if (pos_db >= file.info.dcount)
    ;  // eof
  else if ((DATA_COUNT - pos_db) >= dcount) {
    if (file.info.dcount <= dcount) {
      memcpy((void *)buffer,(void *)(file.data+pos_db),file.info.dcount);
      ret_val += file.info.dcount;
    }
    else {
      memcpy((void *)buffer,(void *)(file.data+pos_db),dcount);
      ret_val += dcount;
    }
    per_spos += (double)dcount;
  } else {
    if (file.info.dcount < DATA_COUNT) {
      // koniec suboru ??
      memcpy((void *)buffer,(void *)(file.data+pos_db),(file.info.dcount-pos_db));
      ret_val += ((unsigned int)(file.info.dcount-pos_db));
      goto ex;
    }
    memcpy((void *)buffer,(void *)(file.data+pos_db),((unsigned int)DATA_COUNT - pos_db));
    ret_val += ((unsigned int)DATA_COUNT-pos_db);
    buffer += ((unsigned int)DATA_COUNT-pos_db);
    dcount -= ((unsigned int)DATA_COUNT-pos_db);
    db++;
    pos_db = 0;
    goto read_more;
  }
ex:
  if (showper) {
    textattr(PERCENT);
    cprintf("\r  %3.2lf \% ",(100.0*per_spos) / per_max);
    textattr(BORDER);
  }
  return ret_val;
}

// funkcia zisti volne miesto na disku
long freespace_log()
{
  long dbcount;         // pocet datablokov
  int LS,pLS;           // log. sektory
  int i;                // pomocna premenna

  // postup: nacitam vsetky i-uzly, z ktorych zistim dalsie d-bloky
  // je jedno, ci pouzijem dir alebo file => tak zistim dcount
  // potom vypocet volneho miesta je uz jednoduchy

  LS = SUP_1;
  dbcount = 0;
  while (LS <= SUP_2) {
    decode_lp(LS);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN)) return 0;
    for (i = 0; i < INODE_COUNT; i++)
      if (IN.inodes[i].link) {
	pLS = IN.inodes[i].link;
	while (pLS) {
	  dbcount++;
	  decode_lp(pLS);
	  read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir);
	  pLS = dir.info.link;
	}
      }
    LS++;
  }
  return ((long)SECT_COUNT - dbcount - 3L) * (long)DATA_COUNT;
}

// funkcia zisti kapacitu disku
long diskcap_log()
{
  long dcap;
  int dcount;         // pocet datablokov
  int LS, pLS;        // log. sektory
  int i;              // pomocna premenna

  // postup: nacitam vsetky i-uzly, z ktorych zistim dalsie d-bloky
  // ale len adresarov, pouzijem preto dir => tak zistim dcount
  // potom vypocet kapacity disku je uz jednoduchy

  LS = SUP_1;
  dcount = 0;
  while (LS <= SUP_2) {
    decode_lp(LS);
    read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&IN);
    for (i = 0; i < INODE_COUNT; i++)
      if (IN.inodes[i].ftype == 2)
	if (IN.inodes[i].link) {
	  pLS = IN.inodes[i].link;
	  while (pLS) {
	    dcount++;
	    decode_lp(pLS);
	    read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir);
	    pLS = dir.info.link;
	  }
	}
    LS++;
  }

  dcap = ((long)SECT_COUNT - (long)dcount - (long)3) * (long)DATA_COUNT;
  return dcap;
}

// funkcia zisti velkost suboru
long filesize_log(char fname[8], char fext[3], unsigned char idir)
{
  int LS, fil_db;
  unsigned char fil_ilink; // inode suboru
  long ret_val;   // navratova hodnota = velkost suboru
  int i;

  // hladanie suboru
  if (!search_file(fname,fext,1,idir,&fil_ilink,&fil_db)) return 0;

  ret_val = 0;
  LS = fil_db;
  while (LS) {
    decode_lp(LS);
    if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&file)) return 0;
    ret_val += (long)file.info.dcount;
    LS = file.info.link;
  }

  return ret_val;
}

// funkcia vrati pocet suborov v adresari
int filecount_log(unsigned char idir)
{
  int LS; // log. sektor

  // 1. datablok adresara
  LS = find_db(idir);
  if (!LS) return 0;
  decode_lp(LS);
  if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return 0;
  return dir.info.fcount;
}

// funkcia zisti meno a priponu suboru s indexom v danom podadresari
int getfname_log(char fname[8], char fext[3], unsigned char idir, int index)
{
  int LS;
  unsigned char pin;
  int i, ind = 0;

  // 1. datablok adresara
  LS = find_db(idir);
  if (!LS) goto FDB_done;
FDB:
  decode_lp(LS);
  if (read_phy(PP.HEAD,PP.TRACK,PP.SECTOR,1,(void *)&dir)) return DATA_ERROR;
  if (dir.info.fcount <= 0) return 0;

  for (i = 0; i < FILE_COUNT; i++)
    if (dir.files[i].fname[0]) {
      if (ind == index) {
	memcpy(fname,dir.files[i].fname,8);
	memcpy(fext,dir.files[i].fext,3);
	pin = get_inode(dir.files[i].ilink);
	return IN.inodes[pin].ftype;
      }
      ind++;
    }
  if (dir.info.link) LS = dir.info.link;
  else goto FDB_done;
  goto FDB; //ak su viac datablokov adresara
FDB_done:
  return 0;
}

/* MODLULOVE FunkCIE */

int Init_LOG()
{
  return 0;
}

void Destroy_LOG()
{
}