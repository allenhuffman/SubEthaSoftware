/*
 * LineInput.c - input routine for EthaWin interface
 *
 * 0.01  9/23/93 - NULL terminate string
 * 0.02 10/20/93 - increment "length" to make it work right.
 * 0.03 06/20/94 - don't turn off cursor if mouse is in use
 * 1.01 09/26/95 - Updates for TCWIN and NOMOUSE
 */

#ifdef TCWIN
#include "ethawin.h"
#include "ethaext.h"
#include <stdio.h>
#else
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>
#endif

LineInput(wpath,x,y,prompt,line,length)
int wpath,x,y;
char *prompt,*line;
int length;
{
   CurXY(wpath,x,y);
   cwrite(wpath,prompt,80);
   if (length==1) {
#ifdef TCWIN
      read(0,line,1);
#else
      read(wpath,line,1); /* get one character and return */
#endif
      line[0]=toupper(line[0]);
   } else {
      sgbufnew.sg_echo=1; /* turn echo on */
#ifdef TCWIN
      _ss_opt(0,&sgbufnew);
#else
      _ss_opt(wpath,&sgbufnew);
      CurOn(wpath);
#endif
#ifdef TCWIN
      line[readln(0,line,length+1)-1]=0; /* read line & NULL terminate */
#else
      line[readln(wpath,line,length+1)-1]=0; /* read line & NULL terminate */
#endif
#ifndef NOMOUSE
      if (!mouseon) CurOff(wpath); /* turn cursor off if not using mouse */
#endif
      sgbufnew.sg_echo=0;
#ifdef TCWIN
      _ss_opt(0,&sgbufnew);
#else
      _ss_opt(wpath,&sgbufnew);
#endif
   }
   if (line[0]==13) line[0]=0;
}
