#include <CGFX/mouse.h>
#include <ETHAWIN/ethawin.h>

extern MSRET ms;

Snooze(wpath)
int wpath;
{
   while(!ms.pt_valid)  /* User has gone to another window... */
   {
      sleep(1);  /* Sleep for 1 second... no need to slow down system! */
      _gs_mous(wpath,&ms);  /* reread mouse info to see if User is back */
   }
}
