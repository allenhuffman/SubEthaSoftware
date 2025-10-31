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
 * 1.00 3/10/95 - MM/1 updates, MBPrint routines
 *      3/28/95 - Add "maclike" menu operation for MM/1 version, modify
 *                MouseClick() to return # of clicks waiting, if any,
 *                moved MouseClick to mouse.c file.
 * 1.01 4/05/95 - made return ch=NULL if no keypress detected
 */

#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>

char CheckMenu(wpath,menu,option,ch,click)
int wpath,*menu,*option;
char *ch; /* pointer to character read, if any... */
char *click; /* returned status of mouse click */
{
   int i;
   char *pos;

   if (mouseon) { /* if mouse is on */
brute_force:
      _gs_mous(wpath,&ms);
      Snooze(wpath);
      mouse_x=ms.pt_wrx/8; /* scale to text screen */
      mouse_y=ms.pt_wry/8;
      if ((old_x!=mouse_x) || (old_y!=mouse_y)) {
#ifndef OSK
         CurXY(wpath,mouse_x,mouse_y); /* update cursor position (not OSK) */
#endif
         old_x=mouse_x; /* and save position */
         old_y=mouse_y;
      }
      if ((*click=MouseClick(wpath))>0) {
#ifdef OSK
         if (mouse_y==0) /* Mouse click on top_line... */
         {
            if (mouse_x==0) MoveWin(wpath);
            if (mouse_x==79) BackWin(wpath);
         }
#endif
         if (mouse_y==1) { /* if clicked on control line, */
#ifdef OSK
            if (mouse_x==0) { /* click QUIT box? */
               Bell(wpath);
               return(FALSE);                  
            }
#endif
            if ((mouse_x>=2) && (mouse_x<=6)) { /* check for ABOUT click */
               DoAbout(wpath);
#ifdef OSK
               if (ms.pt_cbsa) {
                  sigcode=MOUSESIG; /* fake mouse click */
                  goto brute_force; /* and return/rescan */
               }
#endif
               return(FALSE);
            }
            if ((mouse_x>=74) && (mouse_x<=77)) { /* check for HELP click */
               DoHelp(wpath);
#ifdef OSK
               if (ms.pt_cbsa) {
                  sigcode=MOUSESIG; /* fake mouse click */
                  goto brute_force; /* and return/rescan */
               }
#endif
               return(FALSE);
            }
            for (i=0;i<strlen(menu_keys);i++) {
               if ((mouse_x>=menu_x[i]) && (mouse_x<=(menu_x[i]+strlen(menu_names[i]))-5)) {
                  *menu=i;
                  if (DoMenu(wpath,menu,option)) return(TRUE);
#ifdef OSK
                  if (ms.pt_cbsa) {
                     sigcode=MOUSESIG; /* fake mouse click */
                     goto brute_force; /* and return/rescan */
                  }
#endif
                  return(FALSE);
               }
            }
         }
      } /* end of "if (MouseClick)" */
   } /* if here, wasn't mouse click or valid so check ALT keys */
   if (_gs_rdy(wpath)>0) { /* character waiting... */
      read(wpath,ch,1); /* ch is already a pointer */
      if (*ch<0) { /* if ALT character */
         *ch=*ch-128; /* make ASCII */
         *ch=toupper(*ch);
         if (*ch=='A') { /* check for Alt-A */
            DoAbout(wpath);
			return(FALSE);
         }
         if (*ch=='H') { /* check for Alt-H */
            DoHelp(wpath);
            return(FALSE);
         }
         pos=index(menu_keys,*ch);
         if (pos!=0) { /* if match found */
            *menu=pos-menu_keys;
            if (DoMenu(wpath,menu,option)) return(TRUE);
         }
      }
   } else {
      *ch=NULL; /* nothing waiting, return NULL */
   } /* end of "if (_gs_rdy>0)" */
   return(FALSE);
}

DoAbout(wpath) /* highlight menu bar option and display About() info */
int wpath;
{
   MBPrint(wpath,2,TRUE,"\x1f\x22A\x1f\x23bout");
   About(wpath);
   MBPrint(wpath,2,FALSE,"\x1f\x22A\x1f\x23bout");
   return(FALSE);
}

DoHelp(wpath) /* highlight menu bar option and display About() info */
int wpath;
{
   MBPrint(wpath,74,TRUE,"\x1f\x22H\x1f\x23elp");
   Help(wpath);
   MBPrint(wpath,74,FALSE,"\x1f\x22H\x1f\x23elp");
   return(FALSE);
}

/*
 * Since I needed this more than one time, I just made a routine for it.
 * This is used to highlight a menubar option (if revon==TRUE) or turn it
 * back to normal (if revon==FALSE)...
 *
 */

MBPrint(wpath,x,revon,text)
int wpath,x,revon;
char *text;
{
   CurXY(wpath,x,1); /* move to menu name */
   FColor(wpath,clr[MBFORE]);
   BColor(wpath,clr[MBBACK]);
   if (revon) ReVOn(wpath);
   cwrite(wpath,text,80); /* highlight menu name */
   if (revon) ReVOff(wpath);
   FColor(wpath,clr[FORE]);
   BColor(wpath,clr[BACK]);
   return(FALSE);
}

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
   char maclike,checkit;

   maclike=FALSE;
   checkit=TRUE;

   do {
      *option=42; /* temp value... */
      done=FALSE;
      temp=*menu;
      max_opts=strlen(menu_chars[*menu]);

      MBPrint(wpath,menu_x[*menu],TRUE,menu_names[*menu]);

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
#ifdef OSK
            if (ms.pt_area==2) { /* do this if IN menu */
#endif
               if (checkit) { /* need to check for toggle status? */
                  if (maclike==FALSE) { /* no maclike menus? */
                     if (ms.pt_cbsa>0) { /* if button still down */
                        maclike=TRUE; /* yes, want maclike */
                     }
                  }
                  checkit=FALSE;
               }
#ifdef OSK
               mouse_y=(ms.pt_wry)/8;  /* For OSK, use Window Relative */
#else
               mouse_y=(ms.pt_acy/8)-2;
               if (mouse_y<0) mouse_y=0;
#endif
               if (mouse_y>max_opts) mouse_y=max_opts;

               if (old_y!=mouse_y) { /* if mouse has moved, */
                  old_y=mouse_y;
                  if (mouse_y<max_opts) {
                     if ((menu_stat[*menu][mouse_y] & 1)) continue;
                  }
                  *option=mouse_y;
                  continue;
               }
#ifdef OSK
            } /* end of "if (ms.pt_area==2)" */
#endif
            if ( ((!maclike) && (MouseClick(wpath))) ||
                 ((maclike) && (ms.pt_cbsa==0)) ) {
#ifdef OSK
               if (ms.pt_area!=2) aborted=TRUE;
#else
               if (*option==max_opts) aborted=TRUE;
#endif
               done=TRUE;
               completelydone=TRUE;
               break;
            }
         } /* end of "if (mouseon)" */
      } 
      while (!done); /* loop until done... */

      EndWin(wpath);

      MBPrint(wpath,menu_x[temp],FALSE,menu_names[temp]);
   } 
   while(!completelydone);

   if ((done) && (!aborted)) return(TRUE); /* if we were done above, option selected */
   return(FALSE); /* else nothing selected, return false */
}

/* end of "menu.c" */
