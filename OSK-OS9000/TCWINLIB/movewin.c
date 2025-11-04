/*
 * User callable TCWin routine:  Move/relocate window
 */
 
#include <stdio.h>
#include "tclib.h"

int MoveWin(wpath,x,y)
int wpath,x,y;
{
	struct WINDOW *tmp;
	
	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to find this window */	

	if ( (x + tmp->w) < main_win->w) tmp->x = x;
	if ( (y + tmp->h) < main_win->h) tmp->y = y;

	ShowChanges(ALL_WIN);
	return(OK);
}
