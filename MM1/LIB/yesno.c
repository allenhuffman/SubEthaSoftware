/*
 * 0.01 06/17/94 - Add Y/N hotkeys and underline
 * 1.01 09/26/95 - Updates for TCWIN and NOMOUSE
 */

#ifdef TCWIN
#include "ethawin.h"
#include "ethaext.h"
#else
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>
#endif

int YesNo(wpath,title) /* are you sure box */
int wpath;
char *title;
{
   int choice,oldchoice;
   char ch,done;

   choice=FALSE; /* YES=TRUE, NO=FALSE */
   oldchoice=TRUE;
   done=FALSE;

   PopUp(wpath,title,30,9,19,7);
   PrintAt(wpath,2,1,"Are You Sure?");
   do {
      if (choice!=oldchoice) {
         if (choice==TRUE) ReVOn(wpath);
#ifdef TCWIN
         PrintAt(wpath,2,3," Yes ");
#else
         PrintAt(wpath,2,3," \x1f\x22Y\x1f\x23ES ");
#endif
         ReVOff(wpath);
         if (choice==FALSE) ReVOn(wpath);
#ifdef TCWIN
         PrintAt(wpath,11,3," No ");
#else
         PrintAt(wpath,11,3," \x1f\x22N\x1f\x23O ");
#endif
         ReVOff(wpath);
         oldchoice=choice;
      }
#ifdef TCWIN
      if (_gs_rdy(0)>0) {
#else
      if (_gs_rdy(wpath)>0) { /* if character waiting */
#endif
         Snooze(wpath);
#ifdef TCWIN
         read(0,&ch,1);
#else
         read(wpath,&ch,1); /* read it */
#endif
         ch=toupper(ch);
         switch (ch) {
         case RIGHT:
            choice=FALSE;
            break;
         case 'N':
            choice=FALSE;
            if (hotkey) done=TRUE;
            break;
         case LEFT:
            choice=TRUE;
            break;
         case 'Y':
            choice=TRUE;
            if (hotkey) done=TRUE;
            break;
         case ENTER:
            done=TRUE;
            break;
         case QUIT:
            choice=FALSE;
            done=TRUE;
            break;
         default:
            break;
         }
      }
#ifndef NOMOUSE /* begin mouse code */
      if (mouseon) {
         _gs_mous(wpath,&ms); /* read mouse information */
         Snooze(wpath);
#ifdef OSK
         if (ms.pt_area==2) { /* if in window, check */
            mouse_x=(ms.pt_wrx)/8; /* scale to inside box */
            mouse_y=(ms.pt_wry)/8;
            if ((old_x!=mouse_x) || (old_y!=mouse_y)) { /* if mouse has moved */
               if (mouse_y==3) { /* if mouse on YES/NO row */
                  if ((mouse_x>1) && (mouse_x<7)) { /* if on YES button */
                     choice=TRUE;
                  } 
                  else {
                     choice=FALSE; /* anywhere else, select NO */
                  }
               }
               old_x=mouse_x;
               old_y=mouse_y;
            }
         }
#else
         if (old_x!=ms.pt_acx) { /* if mouse has moved */
            if (ms.pt_acx>319) { /* if pointing to the right */
               choice=FALSE;
            } 
            else { /* if pointing to the left */
               choice=TRUE;
            }
            old_x=ms.pt_acx;
         }
#endif
         if (sigcode==MOUSESIG) { /* if mouse click detected */
            sigcode=0;
            _ss_msig(wpath,MOUSESIG); /* reset mouse signal */
            done=TRUE;
         }
      }
#endif /* end mouse code */
   } 
   while (!done);
   EndWin(wpath);
   return (choice);
}
