/*
 * User callable CGFX routine:  Blink On/Off
 */

#include <stdio.h>
#include "tclib.h"

BlnkOn(wpath)
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);

	tmp->attr = tmp->attr | BLINK;
}

BlnkOff(wpath)
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);
	
	tmp->attr = tmp->attr & ~BLINK;
}
