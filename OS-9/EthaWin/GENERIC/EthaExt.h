/*
 * Global variables and stuff to be included with any program written
 * using the EthaWin interface.
 *
 */

#include <mouse.h>
#include <sgstat.h>

extern char *index();

/* global place for tmode status */

extern struct sgbuf sgbufold,sgbufnew;

/* global variables routines */

extern char *pal[];
extern char *clr[COLORS];
extern char hotkey;
externchar mouseon;
extern char mouseport;
extern char mouseres;
extern int sigcode;
extern int menu_x[MAXMENUS],menu_w[MAXMENUS],menu_h[MAXMENUS];
extern int mouse_x,mouse_y,old_x,old_y;
extern MSRET ms; /* mouse packet */

/* global variables for pull-down menus */

extern char *menu_names[];
extern char *menu_keys;
extern int menu_stat[][MAXOPTS];
extern char *menu_chars[];
extern char *menu_text[][MAXOPTS];
