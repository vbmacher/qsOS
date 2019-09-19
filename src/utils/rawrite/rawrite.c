#if defined(__TURBOC__)
#include <alloc.h>
#include <dir.h>
#else
#include <malloc.h>
#endif
#include <bios.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE	0
#define TRUE	(!FALSE)

#define SECTORSIZE	512
#if !defined(__TURBOC__)
#define MAXPATH _MAX_PATH
#endif

#if defined(__TURBOC__)
/*
  BIOS Disk Function Commands
  */
#define	RESET	0
#define	LAST	1
#define	READ	2
#define	WRITE	3
#define	VERIFY	4
#define	FORMAT	5
#else
/*
  Microsoft C
  */
#define RESET  _DISK_RESET
#define LAST   _DISK_STATUS
#define READ   _DISK_READ
#define WRITE  _DISK_WRITE
#define VERIFY _DISK_VERIFY
#define FORMAT _DISK_FORMAT

static unsigned biosdisk(unsigned, unsigned, unsigned, unsigned, unsigned,
	unsigned, void far *);

static unsigned
biosdisk(service, drive, head, track, sector, nsectors, buffer)
     unsigned service;
     unsigned drive;
     unsigned head;
     unsigned track;
     unsigned sector;
     unsigned nsectors;
     void far *buffer;
{
  struct diskinfo_t diskinfo;

  diskinfo.drive = drive;
  diskinfo.head = head;
  diskinfo.track = track;
  diskinfo.sector = sector;
  diskinfo.nsectors = nsectors;
  diskinfo.buffer = buffer;

  return (_bios_disk(service, &diskinfo) >> 8);
}
#endif

int done;

#if defined(__TURBOC__)
/*
 Catch ^C and ^Break.
*/
int
handler(void)
{
  done = TRUE;
  return(0);
}
#else
int
handler(void)
{
  signal(SIGINT, SIG_IGN);
  done = TRUE;
  signal(SIGINT, handler);
  return(0);
}
#endif

static void
msg(char (*s))
{
  fprintf(stderr, "%s\n", s);
  _exit(1);
}

/*
  Identify the error code with a real error message.
  */
void
Error(int (status))
{
  switch (status)
    {
    case 0x00:	msg("Operacia Dokoncena");				break;
    case 0x01:	msg("Chybny prikaz");					break;
    case 0x02:	msg("Address mark not found");				break;
    case 0x03:	msg("Attempt to write on write-protected disk");	break;
    case 0x04:	msg("Sektor neexistuje");				break;
    case 0x05:	msg("Reset failed (hard disk)");			break;
    case 0x06:	msg("Disk changed since last operation");		break;
    case 0x07:	msg("Drive parameter activity failed");			break;
    case 0x08:	msg("DMA overrun");					break;
    case 0x09:	msg("Attempt to DMA across 64K boundary");		break;
    case 0x0A:	msg("Najdeny chybny sektor");				break;
    case 0x0B:	msg("Najdena chybna stopa");				break;
    case 0x0C:	msg("Unsupported track");				break;
    case 0x10:	msg("Bad CRC/ECC on disk read");			break;
    case 0x11:	msg("CRC/ECC corrected data error");			break;
    case 0x20:	msg("Controller has failed");				break;
    case 0x40:	msg("Seek operation failed");				break;
    case 0x80:	msg("Attachment failed to respond");			break;
    case 0xAA:	msg("Drive not ready (hard disk only");			break;
    case 0xBB:	msg("Undefined error occurred (hard disk only)");	break;
    case 0xCC:	msg("Write fault occurred");				break;
    case 0xE0:	msg("Status error");					break;
    case 0xFF:	msg("Sense operation failed");				break;
    }
  _exit(1);
}

/*
 Identify what kind of diskette is installed in the specified drive.
 Return the number of sectors per track assumed as follows:
 9	-	360 K and 720 K 5.25".
15	-	1.2 M HD	5.25".
18	-	1.44 M		3.5".
*/
int
nsects(int (drive))
{
  static int nsect[] = {18, 15, 9};

#if defined(__TURBOC__)
  char *buffer;
#else
  char far *buffer;
#endif
  int i, status;

/*
 Read sector 1, head 0, track 0 to get the BIOS running.
*/
#if defined(__TURBOC__)
  buffer = (char *)malloc(SECTORSIZE);
#else
  buffer = (char far *)_fmalloc(SECTORSIZE);
#endif
  biosdisk(RESET, drive, 0, 0, 0, 0, buffer);
  status = biosdisk(READ, drive, 0, 10, 1, 1, buffer);
  if (status == 0x06)			/* Door signal change?	*/
    status = biosdisk(READ, drive, 0, 0, 1, 1, buffer);

  for (i = 0; i < sizeof(nsect) / sizeof(int); ++i)
    {
      biosdisk(RESET, drive, 0, 0, 0, 0, buffer);
      status = biosdisk(READ, drive, 0, 0, nsect[i], 1, buffer);
      if (status == 0x06)
	status = biosdisk(READ, drive, 0, 0, nsect[i], 1, buffer);
      if (status == 0x00)
	break;
    }
#if defined(__TURBOC__)
  free(buffer);
#else
  _ffree(buffer);
#endif

  if (i == sizeof(nsect)/sizeof(int))
    {
      msg("Nemozem zistit, kolko je sektorov na stopu pre tuto disketu.");
    }
  return(nsect[i]);
}

void
main(argc, argv)
     int argc;
     char *argv[];
{
  char fname[MAXPATH], drvtmp[4];
  char far *buffer, far *bufbase;
  int count, fdin, drive, head, track, status, spt, buflength, ns;
  unsigned long addrtmp;
  int fname_spec, drive_spec, no_wait, i;

  done = 0;
  fname_spec = 0;
  drive_spec = 0;
  no_wait = 0;

  puts("MakeImage 2.0 - Napise obraz operacneho systemu na disketu\n");
#if defined(__TURBOC__)
  ctrlbrk(handler);
/* #else
  Install Microsoft SIGINT handler later in the routine,
  ie. after any user input has completed.
  */
#endif
  for (i = 1; i < argc; i++)
    {
      /* Check each argument for valid options. */
      if (*argv[i] == '/')
	{
	  switch (argv[i][1])
		{
	    case 'f','F':
	      if (i + 1 < argc)
		{
		  strncpy(fname, argv[i + 1], sizeof(fname) - 1);
		  fname[sizeof(fname) - 1] = '\0';
		  fname_spec = TRUE;
		  i++;
		}
	      else
		{
		  fprintf(stderr, "filename must follow /f option\n");
		  exit(1);
		}
	      break;
	    case 'd','D':
	      if (i + 1 < argc)
		{
		  drvtmp[0] = *argv[i + 1];
		  drvtmp[1] = '\0';
		  drive_spec = TRUE;
		  i++;
		}
	      else
		{
		  fprintf(stderr, "drive letter must follow /d option\n");
		  exit(1);
		}
	      break;
	    case 'n','N':
	      no_wait = TRUE;
	      break;
	    case 'h':
	      puts("\nMakeImage option information:\n");
	      puts("\t/f <file>: specify disk image file");
	      puts("\t/d <drive>: specify diskette drive to use;");
		  puts("\t\tmust be either A or B");
	      puts("\t/n: don't wait for user to insert diskette --");
	      puts("\t\tassumes diskette is waiting in selected drive");
	      puts("\t/h: print this help message and exit\n");
	      exit(1);
	      break;
	    default:
	      fprintf(stderr, "mki: '%s' - neznama volba.\n", argv[i]);
	      exit(1);
	    }
	}
      else
	{
	  fprintf(stderr, "mki: '%s' - neznama volba.\n", argv[i]);
	  fprintf(stderr, "Napiste 'mki /h' pre instrukcie.\n");
	  exit(1);
	}
    }
  if (!fname_spec)
    {
      printf("Vlozte nazov suboru s OS: ");
      fgets(fname, sizeof(fname), stdin);
      if (strchr(fname, '\n') != (char *) NULL)
	*(strchr(fname, '\n')) = '\0';
	}
  _fmode = O_BINARY;
  if ((fdin = open(fname, O_RDONLY)) < 0)
    {
      perror(fname);
      exit(1);
    }
  if (done)
    exit(1);

  if (!drive_spec)
    {
	  printf("Vlozte cielove oznacenie disketovej mechaniky: ");
      fgets(drvtmp, sizeof(drvtmp), stdin);
      if (strchr(drvtmp, '\n') != (char *) NULL)
	*(strchr(drvtmp, '\n')) = '\0';
    }
  strupr(drvtmp);
  if (strlen(drvtmp) == 0 || drvtmp[0] < 'A' || drvtmp[0] > 'B')
    {
      fprintf(stderr, "Oznacenie bolo '%s'; musi byt A alebo B.\n", drvtmp);
      exit(1);
    }
  if (done)
	exit(1);
  drive = drvtmp[0] - 'A';
  if (!no_wait)
    {
      printf("Prosim vlozte prazdnu naformatovanu disketu do");
      printf("mechaniky %c: a stlacte -ENTER- :", drive + 'A');
#if defined(__TURBOC__)
      while (bioskey(1) == 0) ;				/* Wait...	*/
      if ((bioskey(0) & 0x7F) == 3) exit(1);		/* Check for ^C	*/
#else
      if ((_bios_keybrd(_KEYBRD_READ) & 0xff) == 0x03)
	/* Exit if Ctrl-C was pressed. */
	exit(1);
#endif
      putchar('\n');
    }
  if (done)
    exit(1);

#if !defined(__TURBOC__)
/*
  Install Microsoft C SIGINT (Ctrl-C) handler.
  */
  signal(SIGINT, handler);
#endif

/*
 * Determine number of sectors per track and allocate buffers.
 */
  spt = nsects(drive);
  buflength = spt * SECTORSIZE;
  /*
    Allocate double the necessary space to make the 64K DMA boundary
	adjustment easy.
	*/
#if defined(__TURBOC__)
  buffer = (char *)malloc(buflength * 2);
  if (buffer == (char *) NULL)
#else
  buffer = (char far *)_fmalloc(buflength * 2);
  if (buffer == (char far *) NULL)
#endif
    {
      fprintf(stderr, "Couldn't allocate track buffer\n");
      exit(1);
	}
  bufbase = buffer;
/*
  Now mangle the buffer base address to avoid physical 64Kb boundary
  problems with DMA.  If the end of the track buffer is not in the same
  physical 64Kb block as the start of the buffer, then start the track
  buffer in the second half of our allocated memory.
  */
  addrtmp = (((unsigned long)FP_SEG(bufbase)) << 4) + FP_OFF(bufbase);
#if defined(DEBUG)
  printf("Physical address of buffer base = %8.8lX\n", addrtmp);
#endif
  if ((addrtmp + (unsigned long)buflength) & 0xffff0000L !=
       addrtmp & 0xffff0000L)
    {
	  /*
	Use the second half of the buffer.  This will work until
	diskette tracks exceed 64 sectors, which shouldn't be in the
	near future.
	*/
      bufbase += buflength;
    }
  printf("Number of sectors per track for this disk is %d\n", spt);
  printf("Writing image to drive %c:.  Press ^C to abort.\n", drive + 'A');

/*
 * Start writing data to diskette until there is no more data to write.
 */

  head = track = 0;
  while ((count = read(fdin, bufbase, buflength)) > 0 && !done)
    {
      printf("Track: %02d  Head: %2d\r", track, head);
      status = biosdisk(WRITE, drive, head, track, 1, spt, bufbase);

// int biosdisk(int cmd, int drive, int head, int track, int sector,
//   int nsects, void *buffer);


      if (status != 0)
	Error(status);

      if ((head = (head + 1) & 1) == 0)
	++track;
    }
  if (eof(fdin))
    {
      printf("\nDone.\n");
      biosdisk(READ, drive, 0, 0, 1, 1, buffer);		/* Retract head	*/
    }
  exit(0);
}	/* end main */
