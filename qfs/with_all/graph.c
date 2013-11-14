/*
   GRAPH.C

   (c) Copyright 2005, vbmacher

   Graficke prostredie, GUI

*/
#include <conio.h>
#include "main.h"

char *menu_items[] = { "Create file", "Delete file", "Write file",
		       "Read file", "Create dir", "Delete dir",
		       "Show dir", "Change path", "Disk info",
		       "Format FDD", "Test FDD", "About QFSemu", "Exit" };
const menu_count = 13;

/* GRAFICKE FUNKCIE - GUI */

// funkcia vypise zoznam menu
void draw_items(char **it, int count, int X, int Y, int sel)
{
  int i;
  gotoxy(X, Y);
  textattr(NOR_ITEM);
  for (i = 0; i < count; i++) {
    gotoxy(X,Y+i);
    if (sel == i) {
      textattr(SEL_ITEM);
      cputs(it[i]);
      textattr(NOR_ITEM);
    }
    else
      cputs(it[i]);
  }
}

// funkcia oznaci danu funkciu
void sel_item(int sel, int old_sel)
{
  gotoxy(M_LX+1, M_UY+3+old_sel);
  textattr(NOR_ITEM);
  cputs(menu_items[old_sel]);
  gotoxy(M_LX+1, M_UY+3+sel);
  textattr(SEL_ITEM);
  cputs(menu_items[sel]);
}

void mainmenu(int sel)
{
  int i;

  textattr(0x90);
  window(1,1,80,25);
  clrscr();
  textattr(BORDER);
  gotoxy(M_LX,M_UY);
  cputs("Ú");
  for (i = 1; i < (M_WIDTH-1); i++)
    cputs("Ä");
  cputs("¿");
  gotoxy(M_LX,M_UY+1);
  cputs("³                  ³");
  textattr(BANNER);
  gotoxy(M_LX+2,M_UY+1);
  cputs("   Main menu");
  textattr(BORDER);
  gotoxy(M_LX,M_UY+2);
  cputs("ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´");
  for (i = 3; i < (M_HEIGHT-1); i++) {
    gotoxy(M_LX,M_UY+i);
    cputs("³                  ³");
  }
  gotoxy(M_LX,M_UY+i);
  cputs("À");
  for (i = 1; i < (M_WIDTH-1); i++)
    cputs("Ä");
  cputs("Ù");
  draw_items(menu_items,menu_count,M_LX+1,M_UY+3,sel);
}

// pracovne okno
// po ukonceni prace sa zatvori
void draw_window()
{
  int i,n;

  textattr(BORDER);
  gotoxy(W_LX,W_UY);
  cputs("Ú");
  for (i = 1; i < (W_WIDTH-1); i++)
    cputs("Ä");
  cputs("¿");
  gotoxy(W_LX,W_UY+1);
  cputs("³");
  for (i = 1; i < (W_WIDTH-1); i++)
    cputs(" ");
  cputs("³");
  textattr(BANNER);
  gotoxy(W_LX+2,W_UY+1);
  cputs("   Work window");
  textattr(BORDER);
  gotoxy(W_LX,W_UY+2);
  cputs("Ã");
  for (i = 1; i < (W_WIDTH-1); i++)
    cputs("Ä");
  cputs("´");
  for (i = 3; i < (W_HEIGHT-1); i++) {
    gotoxy(W_LX,W_UY+i);
    cputs("³");
    for (n = 1; n < (W_WIDTH-1); n++)
      cputs(" ");
    cputs("³");
  }
  gotoxy(W_LX,W_UY+i);
  cputs("À");
  for (i = 1; i < (W_WIDTH-1); i++)
    cputs("Ä");
  cputs("Ù");

  // hl. funkcia
  textattr(BORDER);
  window(W_LX+1,W_UY+3,W_LX+W_WIDTH-2,W_UY+W_HEIGHT-2);
}
