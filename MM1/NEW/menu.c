/*
 * menu.c - menuing routines for the EthaWin library
 * Copyright (C) 1993,1994 by Allen C. Huffman of Sub-Etha Software
 *
 * Note: expects users' program to have About(wpath); and Help(wpath);
 *
 * 0.00 9/18/93 - Library version.
 * 0.01 9/19/93 - addition to passing pointer to "ch" variable...
 * 0.02 9/20/93 - addition to passing "click" status variable...
 * 0.03 6/02/94 - change pull-down menu scaling (be nice to mice) and
 *                correctly support 8 menu options
 * *.** 3/21/95 - add extra parameter to determine if holding the mouse
 *                  button down acts like repeated clicks (yes if TRUE)
 *                take mouse updating and such out of CheckMenu() and
 *                  place it in checkMouse() (located in mouse.c)
 *                move MouseClick() to mouse.c - C.Hyde
 * 1.00 3/10/95 - MM/1 updates
 */

#include <stdinc.h>
#include <ethawin.h>

#include "ethaext.h"

char CheckMenu(wpath,menu,option,ch,click,repeat)
int wpath,*menu,*option;
char *ch; /* pointer to character read, if any... */
char *click; /* returned status of mouse click */
char repeat; /* flag for using click repeat */
{
   int i;
   char *pos;

   if ((*click=checkMouse(wpath, repeat))) {
      if (mouse_y==1) { /* if clicked on control line, */
         if ((mouse_x>=2) && (mouse_x<=6)) { /* check for ABOUT click */
            About(wpath);
            return(FALSE);
         } /* end About */
         if ((mouse_x>=74) && (mouse_x<=77)) { /* check for HELP click */
            Help(wpath);
            return(FALSE);
         } /* end Help */
         for (i=0;i<strlen(menu_keys);i++) {
            if ((mouse_x>=menu_x[i]) && (mouse_x<=(menu_x[i]+strlen(menu_names[i]))-5)) {
               *menu=i;
               if (DoMenu(wpath,menu,option)) return(TRUE);
               /*     goto test; */
               return(FALSE);
            } /* end x >= menu_x[i] */
         } /* end for */
      } /* end y == 1 */
   } /* end if checkMouse() */

   if (_gs_rdy(wpath)>0) { /* character waiting... */
      read(wpath,ch,1); /* ch is already a pointer */
      if (*ch<0) { /* if ALT character */
         *ch=*ch-128; /* make ASCII */
         *ch=toupper(*ch);
         if (*ch=='A') { /* check for Alt-A */
            About(wpath);
            return(FALSE);
         }
         if (*ch=='H') { /* check for Alt-H */
            Help(wpath);
            return(FALSE);
         }
         pos=index(menu_keys,*ch);
         if (pos!=0) { /* if match found */
            *menu=pos-menu_keys;
            if (DoMenu(wpath,menu,option)) return(TRUE);
         }
      } /* end if Alt char */
   } /* end char waiting */
   return(FALSE);
} /* end CheckMenu() */

int DoMenu(wpath,menu,option)
int wpath,*menu,*option;
{
   int i,temp,tempopt;
   int max_opts; /* how many menu options */
   char select[1]; /* pointer to selected/deselected character */
   char ch; /* keypress */

   char *pos; /* pointer to a character */
   int comnum;
   char done; /* used for option select */
   char completelydone=FALSE; /* used for final selection */
   char aborted=FALSE;

   do {
      *option=42; /* temp value... */
      done=FALSE;
      temp=*menu;
      max_opts=strlen(menu_chars[*menu]);

      CurXY(wpath,menu_x[*menu],1); /* move to menu name */
      FColor(wpath,clr[MBFORE]);
      BColor(wpath,clr[MBBACK]);
      ReVOn(wpath);
      cwrite(wpath,menu_names[*menu],80); /* highlight menu name */
      ReVOff(wpath);

      PullDown(wpath,menu_x[*menu],2,menu_w[*menu],menu_h[*menu]);

      for (i=0;i<max_opts;i++) {
         if (menu_stat[*menu][i] & 2) { /* if flagged item */
            select[0]=CHECKFLAG;
         } 
         else {
            select[0]=' ';
         }
         CurXY(wpath,0,i);
         if (menu_stat[*menu][i] & 1) { /* if non-selectable item */
            FColor(wpath,clr[DESELECTED]); /* so make it dark */
            write(wpath,select,1);
            cwrite(wpath,menu_text[*menu][i],80);
            FColor(wpath,clr[PDFORE]);
         } 
         else {
            write(wpath,select,1);
            cwrite(wpath,menu_text[*menu][i],80);
            if (*option==42) *option=i;
         }       
      } /* at this point, pull-down menu has been displayed */
      tempopt=*option;

      /* Highlight initial option... */

      CurXY(wpath,1,*option); /* move to initial option */
      ReVOn(wpath);
      cwrite(wpath,menu_text[*menu][*option],80); /* print highlighted opt */

      do {
         if (*option!=tempopt) { /* if choice has changed, */
            /* was <= below */
            if (tempopt<max_opts) { /* and is not bottom line */
               CurXY(wpath,1,tempopt); /* move to old option */
               ReVOff(wpath);
               cwrite(wpath,menu_text[*menu][tempopt],80); /* unhighlight old */
            }
            CurXY(wpath,1,*option); /* move to current option */
            ReVOn(wpath);
            cwrite(wpath,menu_text[*menu][*option],80); /* highlighted opt */
            tempopt=*option;
         }
         Snooze(wpath); 
         if (_gs_rdy(wpath)>0) { /* character waiting */
            read(wpath,&ch,1);
            ch=toupper(ch);

            pos=index(menu_chars[*menu],ch);
            if (pos!=0) { /* if match found... */
               comnum=pos-menu_chars[*menu];
               if ((menu_stat[*menu][comnum] & 1)==0) { /* if selected okay */
                  *option=comnum; /* then it's okay */
                  if (hotkey) {
                     done=TRUE;
                     completelydone=TRUE;
                     break; /* if hotkey do cmnd else... */
                  }
                  continue; /* just highlight the option */
               }
            }

            switch(ch) {
            case ENTER:
               if (*option>=max_opts) aborted=TRUE;
               done=TRUE;
               completelydone=TRUE;
               break;
            case UP:
               do {
                  (*option)--;
                  if (*option<=-1) *option=max_opts-1;
               } 
               while (menu_stat[*menu][*option] & 1);
               break;
            case DOWN:
               do {
                  (*option)++;
                  if (*option>=max_opts) *option=0;
               } 
               while (menu_stat[*menu][*option] & 1);
               break;
            case LEFT:
               (*menu)--;
               if (*menu<0) *menu=strlen(menu_keys)-1;
               done=TRUE;
               break;
            case RIGHT:
               (*menu)++;
               if (*menu>strlen(menu_keys)-1) *menu=0;
               done=TRUE;
               break;
            case QUIT:
               aborted=TRUE;
               done=TRUE;
               completelydone=TRUE;
               break;
            }  
         }
         if (mouseon) { /* if mouse is on, check it */
            _gs_mous(wpath,&ms); /* read mouse information */
            /* v0.03 */
#ifdef OSK
            if (ms.pt_area==2) { /* do this if IN menu */

               mouse_y=(ms.pt_wry)/8;  /* For OSK, use Window Relative */
               /*     if (ms.pt_area!=2) mouse_y=max_opts;*/ /* Mouse wandered from menu */
#else
               mouse_y=ms.pt_acy/8-2;
               if (mouse_y<0) mouse_y=0;
#endif
               if (mouse_y>max_opts) mouse_y=max_opts;

               /* This routine here could probably be a bit more graceful... */

               if (old_y!=mouse_y) { /* if mouse has moved, */
                  if (mouse_y<max_opts) {
                     if (!(menu_stat[*menu][mouse_y] & 1)) {
                        old_y=mouse_y;
                     }
                  }
                  *option=mouse_y;
                  continue;
               }
#ifdef OSK
            }
#endif
            if (MouseClick(wpath)) {
#ifndef OSK
               if (*option>=max_opts) aborted=TRUE;
#else
               if (ms.pt_area!=2) aborted=TRUE; /* if click outside menu */
#endif
               done=TRUE; /* this or . . . */
               completelydone=TRUE; /* maybe that */
               break;
            }
         }
      } 
      while (!done); /* loop until done... */

      EndWin(wpath);
      CurXY(wpath,menu_x[temp],1); /* move to menu name */
      FColor(wpath,clr[MBFORE]);
      ReVOff(wpath);
      cwrite(wpath,menu_names[temp],80); /* un-highlight it */
      FColor(wpath,clr[FORE]); /* set back to main screen colors */
      BColor(wpath,clr[BACK]);
   } 
   while(!completelydone);

   if ((done) && (!aborted)) return(TRUE); /* if we were done above, option selected */
   return(FALSE); /* else nothing selected, return false */
} /* end DoMenu() */

/* end menu.c */
