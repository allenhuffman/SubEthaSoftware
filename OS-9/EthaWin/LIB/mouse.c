/*
 * mouse.c - stuff dealing with the mouse
 *
 */

#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>

extern int sigcode;

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

/*
 * Returns the number of clicks (if any) that are waiting)
 */

int MouseClick(wpath)
int wpath;
{
#ifdef OSK
   extern int pt_ccta; /* click counter for MM/1 */
#endif

   if (sigcode==MOUSESIG) { /* if mouse click signal detected */
      sigcode=0;
      _ss_msig(wpath,MOUSESIG); /* reset mouse signal */
#ifndef OSK
      return(ms.pt_ccta+1); /* return number of clicks waiting */
#else
      return(pt_ccta);
#endif
   }
   return(0); /* else no clicks waiting */
}
