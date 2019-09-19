#include <io.h>
#include <stdio.h>
#include <dos.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <string.h>

main()
{
int handle;
long nFS=0;

printf("\nFile Rewriter v1.00\n(c) Copyright 2002, P. Jakubco ml.");

_fmode = O_BINARY;
handle = creat("KOS1.PIC", S_IREAD |S_IWRITE);

printf("\nEnter filesize: ");
scanf("%ld", &nFS);
// 45900
for (long n=1; n <= nFS;n++)
  write(handle," ", 1);

/* close the file */
close(handle);
return 0;
}