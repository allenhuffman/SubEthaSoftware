/*
 * startup.c - EthaWin startup routines.
 *
 * 0.00 09/18/93 - Initial version
 * 0.01 05/30/94 - Pass TRUE/FALSE to startup to control whether or not
 *                 ethawin will create it's own screen or use existing one.
 * 0.02 06/13/94 - Make not switch to new screen until palettes are set
 * 0.03 06/17/94 - Move setpal and mouse init routines here from readcfg.c
 * 0.04 09/13/94 - Make stdin/stdout go to wpath, add MenuBar routine, fix
 *                 STDOUT/wpath define (close, dup)
 * 1.00 03/10/95 - change "char" arrays to "ints", MM/1 updates, add pt_ccta
 *                "click counter" for MM/1 in SigHandler, MM/1 QUIT box
 * 1.01 09/26/95 - Updates for TCWIN and NOMOUSE
 */

#include <modes.h>
#ifdef TCWIN
#include "ethawin.h"
#include "ethaext.h"
int tcwinpath;
#else
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>
#include <CGFX/buffs.h>
#endif

#ifndef NOMOUSE
#ifdef OSK
#include <time.h>
/* how fast to qualify for a double click, in tenths of a second */
#define CLICKSPEED 2 /* 2/10ths of a second repeat time? */
clock_t clk_tck; /* number of clicks per second will go here... */
clock_t click_time,old_click_time; /* system click timer thingies */
int pt_ccta; /* click counter for MM/1 to use */
#endif
#endif

SigHandler(sig) /* any signal (such as a mouse click) jumps here... */
int sig;
{
   sigcode=sig; /* save signal in global variable */

#ifndef NOMOUSE
#ifdef OSK
   if (sigcode==MOUSESIG) { /* only check if it's a click */
      click_time=clock()/clk_tck;
      if (click_time>old_click_time+CLICKSPEED) { /* too long since last? */
         pt_ccta=1; /* reset click counter */
      } else {
         pt_ccta++; /* else increment click counter */
      }
      old_click_time=click_time;
   }
#endif
#endif
}

int Startup(makewin) /* setup screen, init vars, turn on mouse, etc... */
char makewin;
{
   int i,wpath;

#ifndef NOMOUSE
/* test stuff */
#ifdef OSK
   clk_tck=CLK_TCK/10; /* take seconds per tick and make tenths of secs */
#endif
/* end of test stuff */
#endif

#ifdef TCWIN
   makewin = FALSE; /* TCWin takes care of this itself */
#endif
   if (makewin) {
      wpath=open("/w",S_IREAD+S_IWRITE); /* find window and open it */
   } else {
      wpath=STDOUT;
   }

   /* turn off echo */

   _gs_opt(wpath,&sgbufold); /* save current status */
   _gs_opt(wpath,&sgbufnew); /* get copy to play with */
   sgbufnew.sg_echo=sgbufnew.sg_kbich=sgbufnew.sg_kbach=sgbufnew.sg_eofch=0;
   _ss_opt(wpath,&sgbufnew); /* update with modified values */

#ifdef TCWIN
   tcwinpath = WinStartup(0,0,80,24,3,0); /* initialize TCWindow */
#else
   if (makewin) { /* do we need to make a window? */
#ifdef OSK
      DWSet(wpath,0,0,0,80,26,3,0,0); /* mm/1 screen */
#else
      DWSet(wpath,2,0,0,80,24,3,0,0); /* make win */
#endif
   }
#endif

#ifdef TCWIN
   wpath = -1; /* from now on, it means current TCWin screen! */
   RefreshOff();
#endif

   ReadCfg(wpath); /* open and read EthaWin.cfg file... */
   /* note ^^^ ReadCfg returns TRUE is .cfg found, FALSE if not (defaults) */

   FColor(wpath,clr[FORE]);
   BColor(wpath,clr[BACK]);
   Border(wpath,clr[BORDER]);
#ifndef TCWIN
   Clear(wpath); /* clear screen to config colors */
#endif

   TopText(wpath,"","","");
   MenuBar(wpath);

#ifndef TCWIN
   Select(wpath);
#else
   RefreshOn();
#endif

#ifndef TCWIN
#ifdef OSK
   for (i=0; i<16; i++) {
      Palette(wpath,i,pal_r[i],pal_g[i],pal_b[i]);
   }
#else 
   for (i=0; i<8; i++) { /* set 8 palettes for CoCo */
      Palette(wpath,i,pal[i]);
      Palette(wpath,i+8,pal[i]);
   }
#endif
#endif

#ifndef NOMOUSE
   if (mouseon) {
#ifndef OSK
      _ss_gip(wpath,mouseres,mouseport,255,255); /* set mouse parms */
#endif
      MouseOn(wpath); /* turn mouse scanning on if necessary */
   }
#endif

   intercept(SigHandler); /* setup signal handler */

   FColor(wpath,clr[FORE]); /* why am I doing this in two places??? */
   BColor(wpath,clr[BACK]);

   return(wpath); /* return window path */
}

ShutDown(wpath) /* put things back like they were... */
int wpath;
{
#ifndef TCWIN
   Clear(wpath); /* and clear the screen... */
#else
   wpath = STDOUT; /* back here */
#endif

   _ss_opt(wpath,&sgbufold); /* restore initial tmode settings */

#ifdef TCWIN
   WinShutdown(tcwinpath);
#else
   close(wpath); /* close & end window */
#endif
}

CalcXWH() /* calculate menu_x, menu_w, and menu_h */
{
   int i,j;
   int max_opts=strlen(menu_keys);

   menu_x[0]=9; /* first menu will be in 10th position */
   for (i=1;i<max_opts;i++) {
#ifdef TCWIN
      menu_x[i]=menu_x[i-1]+strlen(menu_names[i-1])+2;
#else
      menu_x[i]=menu_x[i-1]+strlen(menu_names[i-1])-2;
#endif
   }

   for (i=0;i<max_opts;i++) { /* loop through each menu, */
      menu_w[i]=0;
      menu_h[i]=strlen(menu_chars[i])+2;
      for (j=0;j<strlen(menu_chars[i]);j++) { /* and each option */
         if (strlen(menu_text[i][j])>menu_w[i]) {
            menu_w[i]=strlen(menu_text[i][j]);
         }
      }
#ifdef TCWIN
      menu_w[i]=menu_w[i]+4; /* account for no embedded escape codes */
#endif
   }
}

MenuBar(wpath) /* display menu bar */
int wpath;
{
   int i;
   int max_opts=strlen(menu_keys); /* # of pull-down menus */

   CurXY(wpath,0,1);
   FColor(wpath,clr[MBFORE]);
   BColor(wpath,clr[MBBACK]);
#ifdef TCWIN
   cwrite(wpath,"  About                                                                   Help  ",80);
#else
   cwrite(wpath,"  \x1f\x22A\x1f\x23bout                                                                   \x1f\x22H\x1f\x23elp  ",88);
#endif
#ifndef TCWIN
#ifdef OSK
   CurXY(wpath,0,1); /* put close box on MM/1 screen */
   Font(wpath,GRP_FONT,FNT_G8X8);
   cwrite(wpath,ICN_CLOS,1);
   Font(wpath,GRP_FONT,FNT_S8X8);
#endif
#endif
   CalcXWH(); /* calculate menu_x, menu_w, and menu_h... */
   for (i=0;i<max_opts;i++) {
      CurXY(wpath,menu_x[i],1);
      cwrite(wpath,menu_names[i],strlen(menu_names[i]));
   }
}
