/*
 * User callable TCWin routine:  Returns width/height of specified window
 */
 
#include <stdio.h>
#include "tclib.h"

int GetWH(wpath,w,h)
int wpath,*w,*h;
{
	struct WINDOW *tmp;
	
	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to find this window */	

	*w = tmp->w; /* copy width/height into user's variables and return them */
	*h = tmp->h;

	return(OK);
}
