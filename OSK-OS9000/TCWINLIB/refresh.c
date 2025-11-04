/*
 * User callable refresh window functions
 */

#include "tclib.h"

Refresh() /* toggle/set or force screen refresh */
{
	char org;
	
	org = win_update;
	win_update = TRUE;
	ShowChanges(ALL_WIN);
	win_update = org;
}

RefreshOn()
{
	win_update = TRUE;
	ShowChanges(ALL_WIN); /* force a refresh now... */
}

RefreshOff()
{
	win_update = FALSE;
}
