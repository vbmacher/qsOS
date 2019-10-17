/*
 * MAU.C
 *
 * (c) Copyright 2005, vbmacher
 *
 */

#include <stdio.h>
#include <dos.h>
#include "mau.h"
#include "colors.h"

// velkost stranky = 2 kB
#define PAGE_SIZE 2
#define MAX_ITEMS 512

typedef struct {
  long phy_addr;
  char used;
  char page_count;
} mem_tab;

// pre max. 1MB pamate, velkost stranky 2kB = 2048 bytov
static mem_tab memory[MAX_ITEMS];
int max_items;
long MemStart;

void clear_mem()
{
 int i;
 for (i = 0; i < MAX_ITEMS; i++) {
  memory[i].phy_addr = 0;
  memory[i].used = 0;
  memory[i].page_count = 0;
 }
}

// vyhlada volnych n stranok za sebou
// navrat: 1 = nasiel, 0 = nie
// POZOR = rekurzivna funkcia
int search_mem(int count, int start, int *where)
{
  int i;
  if (!count) return 1;
  if (start >= max_items) return 0;

  for (i = start; i < max_items; i++)
    if (!memory[i].used)
      if (search_mem(count-1,i+1, where)) {
	*where = i;
        return 1;
      }
  return 0;
}

/************************ EXPORT ********************************/

// alokuje pamat
// size v bytoch
void *Xmalloc(int size)
{
  int pc = 0;  // pocet stranok
  int i, start, end;

  if (!size) return NULL;
  // prepocita size na stranky
  pc = size / (PAGE_SIZE * 1024);
  if (!pc) pc++;

  if (!search_mem(pc,0,&start)) {
    printf("\nMAU: Out of memory");
    return NULL;
  }

  // oznacenie pouzitosti
  end = start+pc;
  for (i = start; (i < max_items) && (i < end); i++) {
    memory[i].used = 1;
    memory[i].phy_addr = MK_FP(FP_SEG((long)(PAGE_SIZE * i * 1024)),
			       FP_OFF((long)(PAGE_SIZE * i * 1024)));
    memory[i].phy_addr += MemStart;
    memory[i].page_count = pc--;
  }

  return (memory[start].phy_addr);
}

// uvolni pamat
void Xfree(void far **block)
{
  int i,j;

  // vyhladanie bloku
  for (i = 0; i < max_items; i++) {
    if ((memory[i].phy_addr == *block)
     && (memory[i].used)) {
      for (j = i; j < (i + memory[i].page_count); j++) {
	memory[i].page_count = 0;
	memory[i].phy_addr = 0;
	memory[i].used = 0;
      }
      *block = 0L;
      return;
    }
  }
  printf("\nMAU: Block not found");
}

// volna pamat
long Xcoreleft()
{
  int i,fpc = 0;

  for (i = 0; i < max_items; i++)
    if (!memory[i].used) fpc++;

  return (fpc * PAGE_SIZE);
}

void main()
{
  unsigned char *DATA, *DATA1;
  int i;

  // max. pocet poloziek
  max_items = MAX_ITEMS; //(mem - 1024) / PAGE_SIZE;

  // cistenie tabulky pamate
  clear_mem();

  // zaciatok alokovania pamate
  MemStart = MK_FP(0x1000,0xC000); //(fyzicka adresa)

  printf("\n\n\nFree memory: %ld kB", Xcoreleft());
  printf("\nAllocating 4kB...");
  DATA = Xmalloc(4096);
  printf("\nAddress is : 0x%x", DATA);
  printf("\nSegment: 0x%x, Offset: 0x%x", FP_SEG(DATA), FP_OFF(DATA));

  printf("\nAllocating next 4kB...");
  DATA1 = Xmalloc(4096);
  printf("\nAddress is : 0x%x", DATA1);
  printf("\nSegment: 0x%x, Offset: 0x%x", FP_SEG(DATA1), FP_OFF(DATA1));

  printf("\nFreeing memory...");
  Xfree(&DATA);
  printf("\nAddress is : 0x%x", DATA);
  printf("\nSegment: 0x%x, Offset: 0x%x", FP_SEG(DATA), FP_OFF(DATA));
  Xfree(&DATA1);
  printf("\nAddress is : 0x%x", DATA1);
  printf("\nSegment: 0x%x, Offset: 0x%x", FP_SEG(DATA1), FP_OFF(DATA1));

  getchar();
}