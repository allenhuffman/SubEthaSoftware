#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>

int System(wpath,cmd)
int wpath;
char *cmd;
{
   int i,status;
   char ch;
   char errmsg[17];

   sgbufnew.sg_echo=1; /* echo on */
   _ss_opt(wpath,&sgbufnew);
   status=system(cmd);
   sgbufnew.sg_echo=0; /* echo off */
   _ss_opt(wpath,&sgbufnew);

   /* v1.01 */
   while (_gs_rdy(wpath)>0) { /* read characters while available */
      read(wpath,&ch,1);
      if (ch==QUIT) {
         if (YesNo(wpath,"Keyboard Abort!")) return(TRUE);
      }
   }
   if (status==0) return(FALSE); /* no error */
   sprintf(errmsg, "Err #%3.3d Abort!",status);
   if (YesNo(wpath,errmsg)) return(TRUE);
}

/* NOTE: Can we use itoa() instead of sprintf? */
