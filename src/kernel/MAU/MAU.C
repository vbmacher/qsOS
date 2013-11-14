/*
 * MAU.C
 *
 * (c) Copyright 2005, vbmacher
 *
 * Tue - 24.8.2005 fuuuuuu. Jednoducha FirstLevel MAU hotova....
 *
 */

#include <mau.h>
#include <colors.h>
#include <rtl.h>

// velkost stranky = 2 kB
#define PAGE_SIZE 2
#define MAX_ITEMS 476  // velkost pamate: 0EE000h (100000h-12000h)

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
void *malloc(int size)
{
  int pc = 0;  // pocet stranok
  int i, start, end;

  if (!size) return NULL;
  // prepocita size na stranky
  pc = size / (PAGE_SIZE * 1024);
  if (!pc) pc++;

  if (!search_mem(pc,0,&start)) {
    textattr(atrERROR);
    cprintf("\n\tMAU: Out of memory");
    textattr(atrBORDER);
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

  cprintf("\naddr: %x", memory[start].phy_addr);
  return (memory[start].phy_addr);
}

// uvolni pamat
void free(void far **block)
{
  int i,j;

cprintf("\nfree: addr: %x", *block);
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
  textattr(atrERROR);
  cprintf("\n\tMAU: Block not found");
  textattr(atrBORDER);
}

// volna pamat
long coreleft()
{
  int i,fpc = 0;

  for (i = 0; i < max_items; i++)
    if (!memory[i].used) fpc++;

  return (fpc * PAGE_SIZE);
}

void mauINIT()
{
  textattr(atrINIT);
  cprintf("\nStarting Memory Allocation Unit...");
  textattr(atrBORDER);
  cprintf("\n\tMemory: ");
  textattr(atrIMPTEXT);
  cprintf("%ld", mem);
  textattr(atrBORDER);
  cprintf(" kB");

  // povolenie linky A20
  EnableA20();
  cprintf("\n\tLink A20 enabled...");
  
  // max. pocet poloziek
  if (mem < MAX_ITEMS)
    max_items = mem - 36;
  else
    max_items = MAX_ITEMS - 36;

  // cistenie tabulky pamate
  clear_mem();

  // zaciatok alokovania pamate
  MemStart = MK_FP(0x3400,0); //(fyzicka adresa)

  textattr(atrBORDER);
  cprintf("\n\tFree memory: ");
  textattr(atrIMPTEXT);
  cprintf("%ld", coreleft());
  textattr(atrBORDER);
  cputs(" kB");
}

void mauDESTROY()
{
  textattr(atrINIT);
  cprintf("\nFreeing memory...");
  clear_mem();
  cprintf("\nStopping Memory Allocation Unit...");
  textattr(atrBORDER);
  DisableA20();
  cprintf("\n\tLink A20 disabled...");
}
