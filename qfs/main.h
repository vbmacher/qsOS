/*
    MAIN.H

    (c) Copyright 2005, vbmacher


*/

/* COLORS

 0 - black
 1 - dark blue
 2 - green
 3 - light blue
 4 - red
 5 - purple
 6 - brown
 7 - gray
 8 - dark gray
 9 - violin
 0A - light green
 0B - light blue
 0C - light red
 0D - purple
 0E - yellow
 0F - white
 ------
 10 - dark blue bck / black fore
 11 - dark blue bck / dark blue fore
 ...
 ------
 F0 - selected item
 0F - normal

*/

#ifndef __MAIN__
#define __MAIN__

// main menu const
#define M_WIDTH 20
#define M_HEIGHT 4
#define M_LX 1
#define M_UY 1

// working wiondow const
#define W_WIDTH 50
#define W_HEIGHT 24
#define W_LX (M_LX+M_WIDTH)+1
#define W_UY 1

#define SEL_ITEM 0xF0
#define NOR_ITEM 0x0F
#define BANNER 0x0E
#define BORDER 0x07
#define ERROR 0x0C
#define OK 0x03
#define PERCENT 0x0D
#define sDIR 0x02
#define sFILE 0x0A

#define CREATE_FILE 0
#define DELETE_FILE 1
#define WRITE_FILE 2
#define READ_FILE 3
#define CREATE_DIR 4
#define DELETE_DIR 5
#define SHOW_DIR 6
#define SET_PATH 7
#define DISK_INFO 8
#define FORMAT_FDD 9
#define FDD_TEST 10
#define ABOUT 11
#define EXIT 12
#define REBOOT 13



#endif