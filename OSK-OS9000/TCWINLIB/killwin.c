/*
 * Kill window associated with given path
 */

#include <stdio.h>
#include "tclib.h"

int KillWin(wpath)
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW*) PathToWin(wpath); /* see if valid window specified */
	if (tmp == NULL) return(ERROR); /* exit if unable to find window */

	if (cur_win->next == cur_win) { /* if points to same, must be only window */
		cur_win = NULL; /* so mark it as gone */
	} else {
		if (tmp == cur_win) { /* if deleting currently displayed window, */
			cur_win = tmp->prev; /* cur_win now is previous window */

	/* kludge to never make main window "on top" unless it's the only window */

			if ( (cur_win->number <= FIRSTWIN) && (cur_win->next->next != cur_win) ) {
				cur_win = cur_win->prev; /* BRUTE FORCE!!! */
			} /* trust me... don't ask! */
		}
		tmp->prev->next = tmp->next; /* remove window from list */
		tmp->next->prev = tmp->prev;
	}

	free(tmp->textbuf); /* deallocate window buffers */
	free(tmp->attrbuf);
	free(tmp->colorbuf);
	free(tmp);

	ShowChanges(ALL_WIN);

	return(OK);
}
