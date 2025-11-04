/*
 * User callable CGFX routine:  Clear Screen
 */

#include <stdio.h>
#include "tclib.h"

int Clear(wpath)
int wpath;
{
	struct WINDOW *tmp;
	int i,j;

	tmp = (struct WINDOW*) PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* nothing to clear! */

	for ( i=0 ; i < tmp->warea_h ; i++ ) {
		j = tmp->warea_x + ((tmp->warea_y + i) * tmp->w);
		memset( tmp->textbuf + j , FILLCHAR , tmp->warea_w );
		memset( tmp->attrbuf + j , tmp->attr , tmp->warea_w );
		memset( tmp->colorbuf + j , tmp->color , tmp->warea_w );
	}
	tmp->cur_x = 0; /* home cursor */
	tmp->cur_y = 0;

	ShowChanges(ALL_WIN);
	return(OK);
}
