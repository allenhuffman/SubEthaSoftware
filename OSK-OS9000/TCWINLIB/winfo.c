/*
 * User callable USELESS routine:  duh...
 */
 
#include <stdio.h>
#include "tclib.h"

WInfo(wpath) /* display info on current window list */
int wpath;
{
	struct WINDOW *tmp;
	char text[80];
	sprintf(text,"System Window Status:\n\n");
	Text(wpath,text);
	
	if (cur_win == NULL) {
		Text(wpath,"No windows defined.\n");
		return(OK);
	}
	sprintf(text,"(Currently selected window is #%d)\n",cur_win->number);
	Text(wpath,text);
	tmp = cur_win;
	do {
		sprintf(text,"Win %d - x:%d y:%d w:%d h:%d (next-%d, prev-%d)\n",
			tmp->number, tmp->x, tmp->y, tmp->w, tmp->h,
			tmp->next->number, tmp->prev->number );
		Text(wpath,text);
		tmp = tmp->prev;
	} while (tmp != cur_win);

	return(OK);
}
