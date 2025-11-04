/*
 * Insert supplied window structure after current window in list
 */

#include "tclib.h"
 
InsertWin(tmp)
struct WINDOW *tmp;
{
	tmp->next = cur_win->next;
	tmp->prev = cur_win;

	cur_win->next->prev = tmp;
	cur_win->next = tmp;
}
