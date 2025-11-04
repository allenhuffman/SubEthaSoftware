#ifdef TCWIN
#include "ethawin.h"
#include "ethaext.h"
#else
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>
#endif

int System(wpath,cmd)
int wpath;
char *cmd;
{
   int i,status;
   char ch;
   char errmsg[17];

   sgbufnew.sg_echo=1; /* echo on */
   sgbufnew.sg_kbich=5; /* v1.03 */
   sgbufnew.sg_kbach=3;
   _ss_opt(wpath,&sgbufnew);
   CurOn(wpath);
   status=system(cmd);
   CurOff(wpath);
   sgbufnew.sg_echo=0; /* echo off */
   sgbufnew.sg_kbich=0;
   sgbufnew.sg_kbach=0;
   _ss_opt(wpath,&sgbufnew);

   /* v1.01 */
#ifdef TCWIN
   while (_gs_rdy(0)>0) { /* read characters while available */
      read(0,&ch,1);
#else
   while (_gs_rdy(wpath)>0) { /* read characters while available */
      read(wpath,&ch,1);
#endif
      if (ch==QUIT) {
         if (YesNo(wpath,"Keyboard Abort!")) return(TRUE);
      }
   }
   if (status==0) return(FALSE); /* no error */
   sprintf(errmsg, "Err #%3.3d Abort!",status);
   if (YesNo(wpath,errmsg)) return(TRUE);
}

/* NOTE: Can we use itoa() instead of sprintf? */
