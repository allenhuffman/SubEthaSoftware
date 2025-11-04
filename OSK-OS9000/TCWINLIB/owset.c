/*
 * OWSet/OWEnd emulating wrapper code.  Link only if you need this.
 */

#include <stdio.h>
#include "tclib.h"

/* array to hold "stacked" overlays, and counter to how many are in use */

#define MAX_OWINS 20

static int owins[MAX_OWINS],owin;

/*
 * OWSet and OWEnd emulate CoCo OS-9 Level 2 overlay windows in a "stack"
 * method.  Each call to OWSet creates another overlay ON TOP, and each
 * call to OWEnd kills the most recent overlay.  To directly access an overlay
 * not currently on top you'd need to use the MakeWin/KillWin routines
 * directly.
 */

int OWSet(wpath, save, x, y, w, h, f, b)
int wpath,save,x,y,w,h,f,b;
{
	if (owin >= MAX_OWINS) return(ERROR); /* no more allowed! */

	owins[owin] = MakeWin(x,y,w,h,f,b);
	if (owins[owin] == ERROR) return(ERROR);
	owin++;
}

int OWEnd(wpath)
int wpath;
{
	if (owin>0) { /* if there is at least one overlay, */
		owin--;
		KillWin(owins[owin]);
	}
}
