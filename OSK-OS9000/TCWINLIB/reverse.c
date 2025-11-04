/*
 * User callable CGF routine:  Reverse video On/Off
 */

#include <stdio.h>
#include "tclib.h"

int ReVOn(wpath)
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);

	tmp->attr = tmp->attr | REVERSE;
}


int ReVOff(wpath)
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);
	
	tmp->attr = tmp->attr & ~REVERSE;
}
