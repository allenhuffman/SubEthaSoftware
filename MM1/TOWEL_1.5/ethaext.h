/*
 * EthaExt.h - all external variables used by EthaWin library routines.
 * 
 * 1.00 09/18/93 - Initial version
 * 1.01 03/10/95 - convert pal/clr "chars" to "ints", MM/1 updates
 * 1.02 09/26/95 - Updates for TCWIN and NOMOUSE
 */

#ifndef NOMOUSE
#include <CGFX/mouse.h>
#endif
#include "sgstat.h"

extern char *index();

/* global place for tmode status */

extern struct sgbuf sgbufold,sgbufnew;

/* global variables for routines */

#ifndef TCWIN
#ifdef OSK
extern int pal_r[]; /* palette red ... MM/1 3-byte palette assigments */
extern int pal_g[]; /* palette green */
extern int pal_b[]; /* palette blue */
#else
extern int pal[];
#endif
#endif
extern int clr[]; /* was [COLORS] */
extern char hotkey;
#ifndef NOMOUSE
extern char mouseon;
extern char mouseport;
extern char mouseres;
extern int mouse_x,mouse_y,old_x,old_y;
extern MSRET ms; /* mouse packet */
#endif
extern int sigcode;
extern int menu_x[],menu_w[],menu_h[]; /* was [MAXMENUS] */

/* global variables for pull-down menus */

extern char *menu_names[];
extern char *menu_keys;
extern int menu_stat[][MAXOPTS];
extern char *menu_chars[];
extern char *menu_text[][MAXOPTS];
