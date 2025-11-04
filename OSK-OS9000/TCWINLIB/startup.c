/*
 * TCWin Startup Routines by Allen C. Huffman
 *
 * Termcap based L2/K-Win windowing environment
 *
 * 0.00 09/15/95 - initial work began in Princeton, NJ
 *
 * User callable TCWin routines:  Startup/Shutdown system
 */

#include <stdio.h>
#include <strings.h>
#include "tclib.h"

/* Global variables used by TCWin routines */

struct WINDOW *cur_win; 		/* currently used Window or NULL if none */
struct WINDOW *main_win; 		/* main (background) Window pointer */
char win_update = TRUE;			/* auto-matic screen update flag */
char last_attr,last_color;		/* global screen attributes (for reference) */
extern int termsizeX,termsizeY;	/* terminal LINES and WIDTH value */

/*
 * User callable TCWin routine:  Startup and return path number
 */

int WinStartup(x, y, w, h, f, b)
int x,y,w,h,f,b;
{
	int i;

	setupTermcap(); /* initialize termcap stuff */

	if (w<MINX) w = termsizeX;
	if (h<MINY) h = termsizeY;

	if ( (w > termsizeX ) || (h > termsizeY ) ) {
		fputs("Terminal screen too small for this application.\n",stderr);
		exit(0);
	}

	i = MakeWin( x , y , w , h , f , b );
	if (i == ERROR) {
		fputs("Unable to open primary buffer.\n",stderr);
		exit(0);
	}
	main_win = cur_win;

	i = MakeWin( x , y , w , h , f , b );
	if (i == ERROR) {
		/* NOTE:  need to free the first set! */
		KillWin(main_win->number);
		fputs("Unable to open secondary buffer.\n",stderr);
		exit(0);
	}
	main_win->next = cur_win;
	main_win->prev = cur_win;
	cur_win->next = main_win;
	cur_win->prev = main_win;

/* used to InitWindow(main_win) here for some reason... */

	cur_win = NULL; /* flag so it doesn't link new window to list */

	i = MakeWin( x , y , w , h , f , b );
	if (i == ERROR) {
		fputs("Unable to open main window.\n",stderr);
		exit(0);
	}

	ShowAll(ALL_WIN); /* display cleared initial working screen */

	return(i); /* return path associated with this main screen */
}

/*
 * User callable TCWin routine:  Shutdown
 */

WinShutdown(wpath)
int wpath;
{
	KillWin(wpath);

	/* Brute force */
	if (main_win->next != NULL) {
		free(main_win->next->textbuf); /* free 2nd main buffer */
		free(main_win->next->attrbuf);
		free(main_win->next->colorbuf);
		free(main_win->next);
	}
	if (main_win != NULL) {
		free(main_win->textbuf); /* free main buffer */
		free(main_win->attrbuf);
		free(main_win->colorbuf);
		free(main_win);
	}
	destructTermcap(); /* do this... */
}
