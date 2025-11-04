/*
 * User callable CGFX routine:  Underline On/Off
 */
 
#include <stdio.h>
#include "tclib.h"

UndlnOn(wpath)
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);
	
	tmp->attr = tmp->attr | UNDERLINE;
}

UndlnOff(wpath)
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);
	
	tmp->attr = tmp->attr & ~UNDERLINE;
}
