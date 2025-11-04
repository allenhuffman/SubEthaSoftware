/*
 * Make a new window and return path associated with it
 */

#include <stdio.h>
#include "tclib.h"
 
int MakeWin(x,y,w,h,f,b)
int x,y,w,h,f,b;
{
	struct WINDOW *win;
	int i;

	if (cur_win != NULL) {
		if ( w > main_win->w ) w = main_win->w;
		if ( h > main_win->h ) h = main_win->h;
		if ( x  >= (main_win->w - w) ) x = main_win->w - w;
		if ( y  >= (main_win->h - h) ) y = main_win->h - h;
	}

	win = (struct WINDOW *)malloc(sizeof(struct WINDOW));
	if (win==NULL) return(ERROR);
	
	win->textbuf = (char *) malloc(w*h);
	if (win->textbuf == NULL) {
		free(win);
		return(ERROR);
	}
	win->attrbuf = (char *) malloc(w*h);
	if (win->attrbuf == NULL) {
		free(win->textbuf);
		free(win);
		return(ERROR);
	}
	win->colorbuf = (char *) malloc(w*h);
	if (win->colorbuf == NULL) {
		free(win->textbuf);
		free(win->attrbuf);
		free(win);
		return(ERROR);
	}
	/* if here, assume everything was created... */

	win->x = x;
	win->y = y;
	win->w = w;
	win->h = h;
	win->warea_x = 0;
	win->warea_y = 0;
	win->warea_w = w;
	win->warea_h = h;
	win->cur_x = 0;
	win->cur_y = 0;

	win->attr = 0;
	win->color = (f<<4 | b);

	InitWindow(win); /* initialize da buffers */
	
	if (cur_win == NULL) { /* if there is no window, make the first one */
		win->next = win;
		win->prev = win;
	} else { /* otherwise just add it in the middle */
		InsertWin(win);
	}
	win->number = GetAvail();

	cur_win=win;

	return(win->number);
}

/*
 * Returns next available window path number
 */
 
int GetAvail()
{
	int wpath,used;
	struct WINDOW *tmp;

	wpath=FIRSTWIN; /* start with window #3 (skip 0, 1, 2 for stdio stuff) */
	if (cur_win == NULL) return(wpath); /* first window? */
	
	do {
		used=FALSE;
		tmp = cur_win;
		do {
			if (tmp->number == wpath) { /* it's already in use! */
				wpath++; /* move on to next */
				used=TRUE;
				break;
			}
			tmp = tmp->next;
		} while(tmp != cur_win); /* while still more to check */
	} while(used==TRUE); /* while still hasn't found one */

	return(wpath);
}


/* clear/zero window data buffers */

InitWindow(tmp)
struct WINDOW *tmp;
{
	memset(tmp->textbuf , FILLCHAR , tmp->w * tmp->h );
	memset(tmp->attrbuf , tmp->attr , tmp->w * tmp->h );
	memset(tmp->colorbuf , tmp->color , tmp->w * tmp->h );
}
