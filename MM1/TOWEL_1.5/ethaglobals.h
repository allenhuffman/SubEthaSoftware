 /*
 * EthaGlobals.h - global variable definitions to be included with any
 * EthaWin program.  Also contains the default values for the .cfg file.
 *
 * 1.00 09/18/93 - Initial version
 * 1.01 05/30/94 - Add "makewin" variable
 * 1.02 09/13/94 - Fix default palettes for RGB monochrome
 * 1.03 03/10/95 - converting "char" to "ints", MM/1 updates
 * 1.04 09/26/95 - Updates for TCWIN and NOMOUSE
 */

#ifndef NOMOUSE
#include <CGFX/mouse.h>
#endif
#ifdef _OS9000
#include "sgstat.h"
#else
#include <sgstat.h>
#endif

extern char *index();

/* global place for tmode status */

struct sgbuf sgbufold,sgbufnew;

/* FORE, BACK, BORDER HIGHLIGHTED DESELECTED MBFORE MBBACK TTFORE TTBACK 
 * PUFORE PUBACK PUSHAD PUBORD PDFORE PDBACK PDSHAD */

/* global variables routines */

#ifdef TCWIN
int clr[COLORS]={ /* ANSI color defaults... */
   7 , 0, 0, 3, 5, 0, 5, 0, 7, 7, 4, 0, 7, 0, 7, 0
};
#else
#ifdef OSK
int pal_r[16]= { /* 16 colors available */
	 0 ,255,255,255,255, 0 , 0 ,155, 0 , 0 , 0 , 0 , 63,127,191,255
};
int pal_g[16]= {
	 0 ,255, 0 ,165,255,255, 0 , 48, 0 , 0 , 0 , 0 , 63,127,191,255
};
int pal_b[16]= {
	 0 ,255, 0 , 0 , 0 , 0 ,255,255, 0 , 0 , 0 , 0 , 63,127,191,255
};
int clr[COLORS]= { /* use which slot for which item */
	 15,0,2,15,14,0,14,15,12,15,13,12,14,15,13,12 /* monochrome clrs */
};
#else
int pal[8]= { /* only 8 useable colors on CoCo 3 text screen */
   0,7,56,63,0,7,56,63
};
int clr[COLORS]={
   3 , 0, 1, 3, 2, 0, 2, 0, 3, 3, 0, 2, 1, 0, 3, 1
};
#endif
#endif

#define MAXMENUS 8 /* up to 8 menus supported... */

char hotkey=FALSE; /* explained later... */
char makewin=TRUE; /* 5/30/94 make window on startup? */
#ifndef NOMOUSE
char mouseon=TRUE; /* default mouse ON */
char mouseport=1; /* default RIGHT port */
char mouseres=1; /* default HI-RES */
int mouse_x,mouse_y,old_x,old_y;
MSRET ms; /* mouse packet */
#endif
int sigcode; /* for trapping signals */
int menu_x[MAXMENUS],menu_w[MAXMENUS],menu_h[MAXMENUS];

/* global variables for pull-down menus */

extern char *menu_names[];
extern char *menu_keys;
extern int menu_stat[][MAXOPTS];
extern char *menu_chars[];
extern char *menu_text[][MAXOPTS];
