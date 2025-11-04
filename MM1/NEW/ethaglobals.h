 /*
 * EthaGlobals.h - global variable definitions to be included with any
 * EthaWin program.  Also contains the default values for the .cfg file.
 *
 * 1.00 9/18/93 - Initial version
 * 1.01 5/30/94 - Add "makewin" variable
 * 1.02 9/13/94 - Fix default palettes for RGB monochrome
 * 1.03 3/10/95 - converting "char" to "ints", MM/1 updates
 * *.** 3/95    - changed menu_x, menu_w, menu_h from arrays to pointer
 *                  to ints (arrays will be dynamically allocated).
 */

#include <mouse.h>
#include <sgstat.h>

extern char *index();

/* global place for tmode status */

struct sgbuf sgbufold,sgbufnew;

/* global variables routines */

#ifdef OSK
int pal_r[16]= { /* 16 colors available */
	000,255,000,000,000,000,000,000,000,000,000,000,063,127,191,255
};
int pal_g[16]= {
	000,255,000,000,000,000,000,000,000,000,000,000,063,127,191,255
};
int pal_b[16]= {
	000,255,000,000,000,000,000,000,000,000,000,000,063,127,191,255
};
int clr[COLORS]= { /* use which slot for which item */
	 15,0,13,15,14,0,14,0,15,15,0,14,13,0,15,13 /* monochrome clrs */
};
#else
int pal[8]= { /* only 8 useable colors on CoCo 3 text screen */
   0,7,56,63,0,7,56,63
};
int clr[COLORS]={
   3 , 0, 1, 3, 2, 0, 2, 0, 3, 3, 0, 2, 1, 0, 3, 1
};
#endif

char hotkey=FALSE; /* explained later... */
char makewin=TRUE; /* 5/30/94 make window on startup? */
char mouseon=TRUE; /* default mouse ON */
char mouseport=1; /* default RIGHT port */
char mouseres=1; /* default HI-RES */
int sigcode; /* for trapping signals */
int *menu_x,*menu_w,*menu_h;
int mouse_x,mouse_y,old_x,old_y;
MSRET ms; /* mouse packet */

/* global variables for pull-down menus */

extern char *menu_names[];
extern char *menu_keys;
extern int menu_stat[][MAXOPTS];
extern char *menu_chars[];
extern char *menu_text[][MAXOPTS];
