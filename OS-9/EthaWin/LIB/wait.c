#include <ETHAWIN/ethawin.h>

extern int sigcode;

Wait(wpath)
int wpath;
{
   char ch;
   while(1) { /* do all of this... */
      if (_gs_rdy(wpath)>0) { /* if char waiting, */
         read(wpath,&ch,1); /* read it */
         break; /* and get out */
      }
      if (sigcode==MOUSESIG) { /* then if mouse click detected... */
         sigcode=0;
         _ss_msig(wpath,MOUSESIG); /* reset mouse signal */
         break; /* then get outta here... */
      }
      tsleep(10); /* sleep for a bit */
   }
}
