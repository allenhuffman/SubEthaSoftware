/*
 * Internal TCWin routine:  Returns window pointer associated with path
 */

#include <stdio.h>
#include "tclib.h"

struct WINDOW* PathToWin(wpath)
int wpath;
{
	struct WINDOW *tmp;
	char found;

	if (cur_win == NULL) return(NULL); /* no windows to find! */
	
	tmp = cur_win; /* first, point to current window */
	if ( ! (wpath==STDOUT) || (wpath==cur_win->number) ) { /* not want cur? */
		found=FALSE;
		do { /* then loop through windows, looking for matching wpath */
			if (tmp->number==wpath) { /* is it this one? */
				found=TRUE; /* yes, so flag that we found it */
				break; /* then exit this loop */
			}
			tmp = tmp->next; /* else move to next window */
		} while (tmp != cur_win); /* while not back at start win, do again */

		if (found==FALSE) tmp = NULL; /* not found, so flag as NULL */
	} /* else we assume we are wanting the current window, so just return */
	return(tmp); /* return either found window pointer or NULL */
}
