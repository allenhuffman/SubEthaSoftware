/*
 * EthaGlobals.h - global variable definitions to be included with any
 * EthaWin program.  Also contains the default values for the .cfg file.
 *
 * 1.00 9/18/93 - Initial version
 * 1.01 5/30/94 - Add "makewin" variable
 * 1.02 9/13/94 - Fix default palettes for RGB monochrome
 */

#include <mouse.h>
#include <sgstat.h>

extern char *index();

/* global place for tmode status */

struct sgbuf sgbufold,sgbufnew;

/* global variables routines */

char *pal[8]= {
   0,7,56,63,0,7,56,63
};
char *clr[COLORS]={
   3 , 0, 1, 3, 2, 0, 2, 0, 3, 3, 0, 2, 1, 0, 3, 1
};

char hotkey=FALSE; /* explained later... */
char makewin=TRUE; /* 5/30/94 make window on startup? */
char mouseon=TRUE; /* default mouse ON */
char mouseport=1; /* default RIGHT port */
char mouseres=1; /* default HI-RES */
int sigcode; /* for trapping signals */
int menu_x[MAXMENUS],menu_w[MAXMENUS],menu_h[MAXMENUS];
int mouse_x,mouse_y,old_x,old_y;
MSRET ms; /* mouse packet */

/* global variables for pull-down menus */

extern char *menu_names[];
extern char *menu_keys;
extern int menu_stat[][MAXOPTS];
extern char *menu_chars[];
extern char *menu_text[][MAXOPTS];
