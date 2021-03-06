/*
	MAIN.C
	(c) Copyright 2005, vbmacher
	verzia: 1.00

	Hlavny modul - GUI pre QFSEMU

   � - 218, � - 196, � - 194, � - 195, � - 191, � - 179, � - 193
   � - 180, � - 192, � - 217


*/

#include <stdio.h>
#include <conio.h>
#include "physic_i.h"
#include "logic_io.h"
#include "app_io.h"
#include "main.h"
#include "graph.h"
#include "main_fnc.h"

int main()
{
  int key;
  int sel, old_sel;

  printf("\nLoading...");

  Init_PHY();
  Init_LOG();
  Init_APP();

  sel = 0;
  old_sel = 0;
start:
  mainmenu(sel);

  // kbhit
  fflush(stdin);
  key = 0;
  asm {
    mov ax, 0x0305
    xor bx, bx
    int 0x16
  }
key_loop:
  while (!kbhit()) ;
  asm {
    xor ah, ah
    int 0x16
    mov byte ptr [key], ah // scan kod
  }

  switch (key) {
    case 0x50: // dole
      old_sel = sel;
      sel++;
      if (sel == menu_count) sel = 0;
      sel_item(sel,old_sel);
      goto key_loop;
    case 0x48: // hore
      old_sel = sel;
      if (!sel) sel = menu_count;
      sel--;
      sel_item(sel, old_sel);
      goto key_loop;
    case 0xE0:
    case 0x1C: // enter
      break;
    default:
      goto key_loop;
  }

  draw_window();
  switch (sel) {
    case CREATE_FILE:
      create_file(NULL);
      break;
    case DELETE_FILE:
      delete_file(NULL);
      break;
    case WRITE_FILE:
      write_file(NULL,NULL,0,0,1,0);
      cgets(NULL);
      break;
    case READ_FILE:
      read_file(NULL,NULL,0,0,1,0);
      cgets(NULL);
      break;
    case CREATE_DIR:
      create_dir(NULL);
      break;
    case DELETE_DIR:
      delete_dir(NULL);
      break;
    case SHOW_DIR:
      show_dir();
      break;
    case SET_PATH:
      set_path(NULL);
      break;
    case DISK_INFO:
      disk_info();
      break;
    case FORMAT_FDD:
      format_fdd();
      break;
    case FDD_TEST:
      dotest_fdd();
      break;
    case ABOUT:
      about();
      break;
    case EXIT:
      goto ext;
    case REBOOT:
      cprintf("\nRebooting (hot)...");
      reboot_system();
      break;
  }

  textattr(BORDER);
  cprintf("\n\r\n\rPress any key to continue...");
  getchar();
  goto start;

ext:
  window(1,1,80,25);
  textattr(0x07);
  clrscr();

  Destroy_APP();
  Destroy_LOG();
  Destroy_PHY();

  return 0;
}