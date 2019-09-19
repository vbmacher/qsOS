/*
 * SPLIT.C
 *
 * (c) Copyright 2002, P. Jakubco ml.
 *
 * Program rozdeli subor vacsi ako 65535 bytov na rovnake casti.
 */

#include <io.h>
#include <exestruc.h>

void WriteData(int ifh);
void puts(char *str);

static const char *SrcFile = "kristoph.sys";
static char *DestFile = "kristoph.sy0";

static char Buffer[32768];

void main()
{
	int ifh;
	TExeHeader ExeHeader;


	puts("\n\rAtheros Splitter v1.00\n\r(c) Copyright 2002, P. Jakubco ml.");
	puts("\n\r\n\rSearching for file KRISTOPH.SYS...");

	ifh = open(SrcFile,O_RDWR);
	puts("\n\rWriting...");
	WriteData(ifh); 
	close(ifh);
	puts("\n\r\n\rDone.");

}

void WriteData(int ifh)
{
	unsigned short Bytes;
	int ofh;

        ofh = creat(DestFile);
	while ((Bytes = read(ifh,Buffer,32768)) != 0) {
		puts("\n\rCreating new file...");
		ofh = creat(DestFile);
		puts("\n\rWriting data...");
		write(ofh,Buffer,Bytes);
		close(ofh);
		++DestFile[11];
	}
	close(ofh);
}
