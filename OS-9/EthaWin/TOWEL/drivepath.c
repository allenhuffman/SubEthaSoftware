#include <stdio.h>

#ifdef TCWIN
#include "ethawin.h"
#include "ethaext.h"
#else
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>
#endif

char *rbfdev[] = { /* drives available */
   "PATH", "/DD", "/D0", "/D1", "/D2", "/H0", "/H1", "/R0", "QUIT"
};

DrivePath(wpath,title,path)
int wpath;
char *title,*path;
{
   int i,maxrbf,rbfsel,oldsel;
   char done,aborted;

#ifdef TCWIN
   RefreshOff();
#endif
   PopUp(wpath,title,5,5,69,7);
   PrintAt(wpath,23,1,"Select Drive or Path:");
   for (i=0; i<9; i++) {
      PrintAt(wpath,11+i*5,3,rbfdev[i]);
   }
#ifdef TCWIN
   RefreshOn();
#endif
   maxrbf=i-1;
   rbfsel=0;
   oldsel=1;

   done=FALSE;
   aborted=FALSE;
   do {
      if (oldsel!=rbfsel) {
         PrintAt(wpath,11+oldsel*5,3,rbfdev[oldsel]);
         ReVOn(wpath);
         PrintAt(wpath,11+rbfsel*5,3,rbfdev[rbfsel]);
         ReVOff(wpath);
         oldsel=rbfsel;
      }
#ifndef NOMOUSE
      if (mouseon) {
         _gs_mous(wpath,&ms);
         Snooze(wpath);
#ifndef OSK
         if (old_x!=ms.pt_acx) {
            rbfsel=ms.pt_acx/(640/maxrbf-1); /* need to fix scaling in here */
            old_x=ms.pt_acx;
         }
#else
         if (ms.pt_area==2) { /* if mouse is in displayed window, */
            mouse_x=(ms.pt_wrx/8); /* scale to inside box */
            mouse_y=(ms.pt_wry/8);
            if ((old_x!=mouse_x) || (old_y!=mouse_y)) { /* if mouse moved */
               if (mouse_y==3) { /* on list row? */
                  if ((mouse_x>10) && (mouse_x<55)) {
                     rbfsel=(mouse_x-11)/5;
                  }
               }
            }           
         }
#endif
         if (MouseClick(wpath)) {
            if (rbfsel==maxrbf) aborted=TRUE; /* if clicked on QUIT */
            done=TRUE;
         }
      }
#endif
#ifdef TCWIN
      if (_gs_rdy(0)>0) {
         read(0,path,1);
#else
      if (_gs_rdy(wpath)>0) {
         Snooze(wpath);
         read(wpath,path,1);
#endif
         if (path[0]==LEFT) {
            rbfsel--;
            if (rbfsel<0) rbfsel=maxrbf;
         }
         if (path[0]==RIGHT) {
            rbfsel++;
            if (rbfsel>maxrbf) rbfsel=0;
         }
         if (path[0]==ENTER) {
            if (rbfsel==maxrbf) aborted=TRUE;
            done=TRUE;
            break;
         }
         if (path[0]==QUIT) {
            aborted=TRUE;
            done=TRUE;
            break;
         }
      }
   } 
   while(!done);
   if (!aborted) {
      if (rbfsel==0) { /* if PATH selected, */
         CurXY(wpath,0,3);
         ErLine(wpath);
         LineInput(wpath,1,3,"> ",path,60); /* get a path */
      } 
      else {
         strcpy(path,rbfdev[rbfsel]);
      }
   } 
   else {
      path[0]='\0'; /* return NULL if aborted */
   }
   EndWin(wpath);
} /* BUG above...null out if QUIT */
