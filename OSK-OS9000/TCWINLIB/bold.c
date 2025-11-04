/*
 * User callable CGFX routine:  Bold On/Off
 */
 
#include <stdio.h>
#include "tclib.h"

BoldSw(wpath,toggle)
int wpath,toggle;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);

	if (toggle == 0) { /* turn off? */
		tmp->attr = tmp->attr & ~BOLD;
	} else { /* else turn on */
		tmp->attr = tmp->attr | BOLD;
	}
}
