/*
 * User callable CGFX routine:  Move Cursor to X,Y
 */

#include <stdio.h>
#include "tclib.h"

int CurXY(wpath, x, y)
int wpath,x,y;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* no window, exit with error */

	if (x > tmp->warea_w) x = tmp->warea_w; /* bounds checking */
	if (y > tmp->warea_h) y = tmp->warea_h;
	if (x<0) x=0;
	if (y<0) y=0;
	tmp->cur_x = x;
	tmp->cur_y = y;

	setcursor( x + tmp->x + tmp->warea_x , y + tmp->y + tmp->warea_y );
}
