/*
  GRAPH.H

  (c) Copyright 2005, vbmacher

*/
#ifndef __GRAPH__
#define __GRAPH__

  extern const menu_count;

  void draw_items(char **it, int count, int X, int Y, int sel);
  void sel_item(int sel, int old_sel);
  void mainmenu(int sel);
  void draw_window();

#endif