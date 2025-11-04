/*
 * User callable CGF routines:  Foreground/Background color
 */

#include <stdio.h>
#include "tclib.h"

int FColor(wpath, c)
int wpath,c;
{
	struct WINDOW *tmp;

	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);
	
	tmp->color = (tmp->color & 15) | (c<<4);
}

int BColor(wpath, c)
int wpath,c;
{
	struct WINDOW *tmp;
	
	tmp = (struct WINDOW *)PathToWin(wpath);
	if (tmp == NULL) return(ERROR);
	
	tmp->color = (tmp->color & (15<<4) ) | c;
}
