/*
 * User callable CGFX routine:  Select specified window, move it to top
 */

#include <stdio.h>
#include "tclib.h"
 
int Select(wpath)
int wpath;
{
	struct WINDOW *tmp,*prev,*next;
	
	tmp = PathToWin(wpath); /* attempt to locate window first */
	if (tmp == NULL) return(ERROR); /* nonexistant?  exit with error */	

	/* if here, must have matched window so now we just move it */
	
	tmp->prev->next = tmp->next; /* remove from list */
	tmp->next->prev = tmp->prev;

	InsertWin(tmp);

	cur_win = tmp;

	ShowChanges(ALL_WIN); /* redraw, showing this window up top */

	return(OK); /* successfully done! */
}
