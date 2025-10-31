/*
 * LineInput.c - input routine for EthaWin interface
 *
 * 0.01  9/23/93 - NULL terminate string
 * 0.02 10/20/93 - increment "length" to make it work right.
 * 0.03 06/20/94 - don't turn off cursor if mouse is in use
 */

#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>

LineInput(wpath,x,y,prompt,line,length)
int wpath,x,y;
char *prompt,*line;
int length;
{
   CurXY(wpath,x,y);
   cwrite(wpath,prompt,80);
   if (length==1) {
      read(wpath,line,1); /* get one character and return */
      line[0]=toupper(line[0]);
   } 
   else {
      sgbufnew.sg_echo=1; /* turn echo on */
      _ss_opt(wpath,&sgbufnew);
      CurOn(wpath);
      line[readln(wpath,line,length+1)-1]=0; /* read line & NULL terminate */
      if (!mouseon) CurOff(wpath); /* turn cursor off if not using mouse */
      sgbufnew.sg_echo=0;
      _ss_opt(wpath,&sgbufnew);
   }
   if (line[0]==13) line[0]=0;
}
