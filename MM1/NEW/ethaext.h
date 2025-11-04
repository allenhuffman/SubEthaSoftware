/*
 * EthaExt.h - all external variables used by EthaWin library routines.
 * 
 * 1.00 9/18/93 - Initial version
 * 1.01 3/10/95 - convert pal/clr "chars" to "ints", MM/1 updates
 */

#include <CGFX/mouse.h>
#include <sgstat.h>

extern char *index();

/* global place for tmode status */

extern struct sgbuf sgbufold,sgbufnew;

/* global variables for routines */

#ifdef OSK
extern int pal_r[]; /* palette red ... MM/1 3-byte palette assigments */
extern int pal_g[]; /* palette green */
extern int pal_b[]; /* palette blue */
#else
extern int pal[];
#endif
extern int clr[]; /* was [COLORS] */
extern char hotkey;
extern char mouseon;
extern char mouseport;
extern char mouseres;
extern int sigcode;
extern int menu_x[],menu_w[],menu_h[]; /* was [MAXMENUS] */
extern int mouse_x,mouse_y,old_x,old_y;
extern MSRET ms; /* mouse packet */

/* global variables for pull-down menus */

extern char *menu_names[];
extern char *menu_keys;
extern int menu_stat[][MAXOPTS];
extern char *menu_chars[];
extern char *menu_text[][MAXOPTS];
