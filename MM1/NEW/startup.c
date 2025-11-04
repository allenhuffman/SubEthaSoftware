/*
 * startup.c - EthaWin startup routines.
 *
 * 0.00 9/18/93 - Initial version
 * 0.01 5/30/94 - Pass TRUE/FALSE to startup to control whether or not
 *                ethawin will create it's own screen or use existing one.
 * 0.02 6/13/94 - Make not switch to new screen until palettes are set
 * 0.03 6/17/94 - Move setpal and mouse init routines here from readcfg.c
 * 0.04 9/13/94 - Make stdin/stdout go to wpath, add MenuBar routine, fix
 *                STDOUT/wpath define (close, dup)
 * *.** 3/21/95 - added code to dynamically allocate space for menu_x,
                    menu_w and menu_h - C.Hyde
 * 1.00 3/10/95 - change "char" arrays to "ints", MM/1 updates
 */

#include <modes.h>
#include <ethawin.h>
#include <stdinc.h>

#include "ethaext.h"

extern int errno;

SigHandler(sig)
int sig;
{
   sigcode=sig;
}

/* setup screen, init vars, turn on mouse, etc... */
int Startup(makewin, maxmenus)
char makewin;
int maxmenus;
{
   int i,wpath;

   if (makewin) {
      wpath=OpenFile("/w",S_IREAD+S_IWRITE); /* find window and open it */
   } 
   else {
      wpath=STDOUT;
   }

   /* turn off echo */

   _gs_opt(wpath,&sgbufold); /* save current status */
   _gs_opt(wpath,&sgbufnew); /* get copy to play with */
   sgbufnew.sg_echo=sgbufnew.sg_kbich=sgbufnew.sg_kbach=sgbufnew.sg_eofch=0;
   _ss_opt(wpath,&sgbufnew); /* update with modified values */

   if (makewin) {
#ifdef OSK
      DWSet(wpath,0,0,0,80,26,3,0,0); /* mm/1 screen */
#else
      DWSet(wpath,2,0,0,80,24,3,0,0); /* make win */
#endif
   }

   ReadCfg(wpath); /* open and read EthaWin.cfg file... */
   /* note ^^^ ReadCfg returns TRUE is .cfg found, FALSE if not (defaults) */

   FColor(wpath,clr[FORE]);
   BColor(wpath,clr[BACK]);
   Border(wpath,clr[BORDER]);
   Clear(wpath); /* clear screen */

   if (allocateMenus(maxmenus)==ERROR) /* allocate space for menus */
      exit(errno);

   TopText(wpath,"","","");
   MenuBar(wpath);

   Select(wpath);

#ifdef OSK
   for (i=0; i<16; i++) {
      Palette(wpath,i,pal_r[i],pal_g[i],pal_b[i]);
#else 
      for (i=0; i<8; i++) { /* set 8 palettes for CoCo */
         Palette(wpath,i,pal[i]);
         Palette(wpath,i+8,pal[i]);
#endif
      }

      if (mouseon) {
#ifndef OSK
         _ss_gip(wpath,mouseres,mouseport,255,255); /* set mouse parms */
#endif
         MouseOn(wpath); /* turn mouse scanning on if necessary */
      }

   intercept(SigHandler); /* setup signal handler */

   FColor(wpath,clr[FORE]);
   BColor(wpath,clr[BACK]);

   return wpath; /* return window path */
} /* end Startup() */

ShutDown(wpath) /* put things back like they were... */
int wpath;
{
   Clear(wpath); /* and clear the screen... */

   _ss_opt(wpath,&sgbufold); /* restore initial tmode settings */

   close(wpath); /* close & end window */
} /* end ShutDown() */

CalcXWH() /* calculate menu_x, menu_w, and menu_h */
{
   int i,j;
   int max_opts=strlen(menu_keys);

   menu_x[0]=9; /* first menu will be in 10th position */
   for (i=1;i<max_opts;i++) {
      menu_x[i]=menu_x[i-1]+strlen(menu_names[i-1])-2;
   }

   for (i=0;i<max_opts;i++) { /* loop through each menu, */
      menu_w[i]=0;
      menu_h[i]=strlen(menu_chars[i])+2;
      for (j=0;j<strlen(menu_chars[i]);j++) { /* and each option */
         if (strlen(menu_text[i][j])>menu_w[i]) {
            menu_w[i]=strlen(menu_text[i][j]);
         }
      }
   }
} /* end CalcXWH() */

MenuBar(wpath) /* display menu bar */
int wpath;
{
   int i;
   int max_opts=strlen(menu_keys); /* # of pull-down menus */

   CurXY(wpath,0,1);
   FColor(wpath,clr[MBFORE]);
   BColor(wpath,clr[MBBACK]);
   cwrite(wpath,"  \x1f\x22A\x1f\x23bout                                                                   \x1f\x22H\x1f\x23elp  ",128);
   CalcXWH(); /* calculate menu_x, menu_w, and menu_h... */
   for (i=0;i<max_opts;i++) {
      CurXY(wpath,menu_x[i],1);
      cwrite(wpath,menu_names[i],80);
   }
} /* end MenuBar() */

/* allocateMenus - version 1.0
 *            WB - Chris Hyde, 2/95
 *
 * Allocate space for the menu arrays; menu_x, menu_w, menu_h.
 * If allocation is successfull SUCCESS is returned, otherwise
 * ERROR is returned and the error number can be found in errno.
 *
 */

int allocateMenus(maxmenus)
int maxmenus;
{
 if ((menu_x=calloc(maxmenus, sizeof(int)))==NULL)
    return(ERROR);
 if ((menu_w=calloc(maxmenus, sizeof(int)))==NULL)
    return(ERROR);
 if ((menu_h=calloc(maxmenus, sizeof(int)))==NULL)
    return(ERROR);
 return(SUCCESS);
} /* end allocateMenus() */

/* end startup.c */

