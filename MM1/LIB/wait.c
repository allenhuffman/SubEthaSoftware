#ifdef TCWIN
#include "ethawin.h"
#else
#include <ETHAWIN/ethawin.h>
#endif

extern int sigcode;

Wait(wpath)
int wpath;
{
   char ch;
   while(1) { /* do all of this... */
#ifdef TCWIN
      if (_gs_rdy(0)>0) { /* if char waiting, */
         read(0,&ch,1); /* read it */
#else
      if (_gs_rdy(wpath)>0) { /* if char waiting, */
         read(wpath,&ch,1); /* read it */
#endif
         break; /* and get out */
      }
#ifndef NOMOUSE
      if (sigcode==MOUSESIG) { /* then if mouse click detected... */
         sigcode=0;
         _ss_msig(wpath,MOUSESIG); /* reset mouse signal */
         break; /* then get outta here... */
      }
#endif
      tsleep(10); /* sleep for a bit */
   }
}
