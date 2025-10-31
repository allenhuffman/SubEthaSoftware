/*
 * Chooser.c - displays overlay window and lets user select something from
 *             it...maybe...  Auto-sized, auto-centered...tada!
 *
 * 1.00 3/30/95 - Initial version based on Towel code
 */

#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>

#define Y_POS 2
#define X_POS 1
#define NULL 0

int Chooser(wpath,label,lines,item,maxitem)
int wpath;
char *label;
int lines;
char *item[];
int maxitem;
{
   int i,start,pointer,oldpointer,width,spc;
   int update,loop,abort;
   char ch,click;

   extern int pt_ccta;

   width=17; /* minimum width for window to be */
   for (i=0; i<maxitem; i++) {
      start=strlen(item[i]);
      if (start>width) width=start;
   }

   start=0;
   pointer=0;
   oldpointer=1;
   update=TRUE;
   loop=TRUE;
   abort=FALSE;

   PopUp(wpath,label,38-width/2,SCREENLEN/2-lines/2-3,width+4,lines+4);
   ReVOn(wpath);
   spc=(width-16)/2;
   PrintAt(wpath,X_POS+spc,0," UP ");
   PrintAt(wpath,X_POS+6+spc,0,"DOWN");
   PrintAt(wpath,X_POS+12+spc,0,"QUIT");
   ReVOff(wpath);
   do {
      if (update==TRUE) {
         for (i=0; i<lines; i++) { /* display initial list of maxitem */
            CurXY(wpath,X_POS,i+Y_POS);
            ErLine(wpath); /* wipe line clear */
            PrintAt(wpath,X_POS,i+Y_POS,item[i+start]);
         }
         update=FALSE;
      }
      if (pointer!=oldpointer) { /* update pointer if necessary */
         PrintAt(wpath,X_POS,oldpointer+Y_POS,item[oldpointer+start]);
         ReVOn(wpath);
         PrintAt(wpath,X_POS,pointer+Y_POS,item[pointer+start]);
         ReVOff(wpath);
         oldpointer=pointer;
      }
      if (mouseon) { /* v1.01 bug fix, only do this if mouse is on */
         _gs_mous(wpath,&ms);
         mouse_x=ms.pt_wrx/8;
         mouse_y=ms.pt_wry/8;
         click=MouseClick(wpath);
#ifdef OSK
         if ((click) && (ms.pt_area!=2)) ch=QUIT; /* brutally forced */
#endif
         if (ms.pt_cbsa==TRUE) { /* is button down? */
            if (mouse_y==Y_POS-2) { /* on UP/DOWN line? */
               if ((mouse_x>=X_POS+spc) && (mouse_x<=X_POS+3+spc)) { /* UP? */
                  if (click>1) { /* double click? */
                     ch=SHIFTUP;
                  } 
                  else {
                     ch=UP;
                  }
               } 
               else { /* if not on UP, check if on DOWN */
                  if ((mouse_x>=X_POS+6+spc) && (mouse_x<=X_POS+9+spc)) {
                     if (click>1) { /* double click? */
                        ch=SHIFTDOWN;
                     } 
                     else {
                        ch=DOWN;
                     }
                  }
                  else { /* if not UP or DOWN, check QUIT */
                     if ((mouse_x>=X_POS+12+spc) && (mouse_x<=X_POS+15+spc)) {
                        ch=QUIT;
                     }
                  }
               }
            }
         }
         if ((mouse_y>=Y_POS) && (mouse_y<Y_POS+lines)) { /* if on file */
            i=strlen(item[start+mouse_y-Y_POS])-1;
            if ((mouse_x>=X_POS) && (mouse_x<=X_POS+i)) {
               pointer=mouse_y-Y_POS;
               if (click) {
                  ch=ENTER;
               }
            }
         }
      } /* end of mouse checking code */

      if (_gs_rdy(wpath)>0) {
         read(wpath,&ch,1);
      }
      if (ch!=NULL) {
         switch(ch) {
         case UP:
            if (pointer>0) { /* if pointer not already at top, */
               pointer--; /* decrement it */
               break; /* and exit */
            } /* else assume marker is already at top, so */
            if (start>0) { /* if filelist is not already at top, */
               PrintAt(wpath,X_POS,Y_POS,item[start]); /* unhighlight */
               start--; /* decrement it */
               CurXY(wpath,X_POS,Y_POS+lines-1);
               ErLine(wpath);
               CurXY(wpath,X_POS,Y_POS); /* move to first filelist line */
               InsLin(wpath); /* insert a line there */
               ReVOn(wpath);
               PrintAt(wpath,X_POS,Y_POS,item[start]);
               ReVOff(wpath);
            }
            break; /* done with this stuff */
         case DOWN:
            if ((pointer<lines-1) && (pointer<maxitem-1)) {
               pointer++;
               break;
            }
            if (start<maxitem-lines) {
               PrintAt(wpath,X_POS,Y_POS+lines-1,item[start+pointer]);
               start++;
               CurXY(wpath,X_POS,Y_POS);
               DelLine(wpath);
               ReVOn(wpath);
               PrintAt(wpath,X_POS,Y_POS+lines-1,item[start+pointer]);
               ReVOff(wpath);
            }
            break;
         case SHIFTUP:
            if (pointer>0) {
               pointer=0;
               break;
            }
            if (start>0) {
               start=start-lines;
               if (start<0) start=0;
               /* v1.01, make work like ved */
               pointer=lines-1;
               update=TRUE;
            } /* may have bug with oldpointer update...? */
            break;
         case SHIFTDOWN:
            if (pointer<lines-1) {
               pointer=lines-1;
               if (pointer>=maxitem) pointer=maxitem-1;
               break;
            }
            if (start<maxitem-lines) {
               start=start+lines;
               if (start>maxitem-lines) start=maxitem-lines;
               /* v1.01, make work like ved */
               pointer=0;
               update=TRUE;
            }
            break;
         case CTRLUP: /* v1.01 */
            pointer=0;
            if (start==0) break; /* if less than screenfull */
            start=0;
            oldpointer=1;
            update=TRUE;
            break;
         case CTRLDOWN: /* v1.01 this is kinda brute forced... */
            pointer=lines-1;
            oldpointer=0;
            if (pointer>=maxitem) {
               pointer=maxitem-1;
               break;
            }
            if (start<maxitem-lines) { /* if not displaying last page, */
               start=maxitem-lines;
               update=TRUE;
            }
            break;
         case ENTER: /* select item and return */
            loop=FALSE; /* break out of loop */
            break;
         case QUIT: /* quit! */
            loop=FALSE;
            abort=TRUE;
            break;
         }
         ch=NULL;
      }
   } 
   while(loop); /* while still looping . . . */
   
   EndWin(wpath);
   if (abort) return(-1);
   return(start+pointer);
}
