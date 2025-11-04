/*
 * User callable CGFX routine:  Change Working Area
 */

#include <stdio.h>
#include "tclib.h"

int CWArea(wpath,x,y,w,h) /* change working area for specified window */
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	if (x >= tmp->w) x = tmp->w;
	if (y >= tmp->h) y = tmp->y;
	if (w >= tmp->w - x) w=tmp->w - x;
	if (h >= tmp->h - y) h=tmp->h - y;
	tmp->warea_x = x; /* define new working area */
	tmp->warea_y = y;
	tmp->warea_w = w;
	tmp->warea_h = h;
	tmp->cur_x = 0; /* home cursor */
	tmp->cur_y = 0;
}
