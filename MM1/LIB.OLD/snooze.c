#ifdef TCWIN
#include "ethawin.h"
#else
#include <ETHAWIN/ethawin.h>
#endif

#ifndef NOMOUSE
#include <CGFX/mouse.h>
extern MSRET ms;
#endif

Snooze(wpath)
int wpath;
{
#ifndef NOMOUSE
   while(!ms.pt_valid)  /* User has gone to another window... */
   {
      sleep(1);  /* Sleep for 1 second... no need to slow down system! */
      _gs_mous(wpath,&ms);  /* reread mouse info to see if User is back */
   }
#endif
}
