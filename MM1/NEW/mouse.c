#include <stdinc.h>
#include <ethawin.h>

#include "ethaext.h"

MouseOn(wpath) /* turn mouse stuff on */
int wpath;
{
#ifndef OSK
   _ss_mous(wpath,3,10,0); /* turn on auto-scan, timeout, etc. */
   CurOn(wpath); /* make sure text mouse cursor is on */
#else
   CurOff(wpath);
#endif
   sigcode=0;
   _ss_msig(wpath,MOUSESIG); /* setup mouse signal */
}

MouseOff(wpath) /* turn mouse stuff off... */
int wpath;
{
#ifndef OSK
   _ss_mous(wpath,0,0,0); /* turn off auto-scan, timeout, etc. */
   CurOff(wpath); /* turn off text mouse cursor */
#endif
   sigcode=0;
   _ss_rel(wpath); /* clear mouse signal */
}

/* checkMouse - version 1.0
 *         WB - Chris Hyde, 3/95
 *
 *  This function checks to see if the mouse has moved since the last call, if
 *  it has the cursor location is updated.  It also checks to see if the mouse
 *  button has been pressed, if it has TRUE is returned, otherwise FALSE is
 *  returned.  (this code was taken from CheckMenu() and modified so the
 *  actual mouse coordinates are used, rather than window relative).
 */

char checkMouse(wpath, repeat)
int wpath;
char repeat;
{
 if (mouseon)
 { /* if mouse is on */
    _gs_mous(wpath, &ms); /* read mouse information */
    Snooze(wpath); /* sleep if user is not on this window... */
    mouse_x=ms.pt_acx/8; /* scale to text screen */
    mouse_y=ms.pt_acy/8;
    if ((old_x!=mouse_x) || (old_y!=mouse_y))
    {
#ifndef OSK
    CurXY(wpath,mouse_x,mouse_y); /* update cursor position (not OSK) */
#endif
    old_x=mouse_x; /* and save position */
    old_y=mouse_y;
    } /* end if mouse moved */
 } /* end if mouseon */

    return(MouseClick(wpath, repeat));
} /* end checkMouse() */

/* MouseClick
 *           additions by C. Hyde, 3/95
 *
 * Checks to see if the mouse button has been pressed, returns TRUE if it
 * has and FALSE if not.  (code taken from CheckMenu.c)
 *
 * If repeat is true then the function checks to see how long the current
 * state of button A has existed (either button up or button down).  If the
 * current state has been longer than 20 ticks the current state is returned
 * (1 for down, 0 for up).
 *
 */
int MouseClick(wpath, repeat)
int wpath;
char repeat;
{
 if (sigcode==MOUSESIG)
 { /* if mouse click detected... */
    sigcode=0;
    _ss_msig(wpath, MOUSESIG); /* reset mouse signal */
    return(TRUE);
 } /* end if sigcode == MOUSESIG */
 if (repeat && ((ms.pt_ttsa>20) || (ms.pt_ttsa<0)))
    return(ms.pt_cbsa);
return(FALSE);
} /* end MouseClick() */

/* end mouse.c */

 
