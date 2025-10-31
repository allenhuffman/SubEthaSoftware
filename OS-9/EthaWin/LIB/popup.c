/*
 * PopUp.c
 *
 * 1.00 3/10/95 - change clr from "char" to "int", MM/1 update
 */

#include <ETHAWIN/ethawin.h>

extern int clr[COLORS];

#ifdef OSK
#include <CGFX/buffs.h>
#endif

PopUp(wpath,title,x,y,w,h) /* pop-up shaded dialog box... */
int wpath;
char *title;
int x,y,w,h;
{
   OWSet(wpath,1,x+1,y+1,w,h,0,clr[PUSHAD]);
   OWSet(wpath,1,x,y,w,h,clr[PUFORE],clr[PUBORD]);

   CurOff(wpath); /* turn cursor off */
   CurXY(wpath,w/2-strlen(title)/2,0);
#ifdef OSK
   Font(wpath,GRP_FONT,FNT_G8X8); /* graphic font */
#endif
   cwrite(wpath,title,80);
#ifdef OSK
   Font(wpath,GRP_FONT,FNT_S8X8); /* normal font */

   ScaleSw(wpath,0); /* turn scaling off */
   FColor(wpath,clr[PUSHAD]); /* lowered text area (maybe) */
   SetDPtr(wpath,7,7);
   Line(wpath,(w-1)*8,7);
   Line(wpath,7,(h-1)*8);
   FColor(wpath,clr[PUFORE]);
   SetDPtr(wpath,0,0);
   Line(wpath,w*8-1,0);
   Line(wpath,0,h*8-1);
   SetDPtr(wpath,(w-1)*8,(h-1)*8);
   Line(wpath,(w-1)*8,8);
   Line(wpath,8,(h-1)*8);
#endif

   CWArea(wpath,1,1,w-2,h-2);
   BColor(wpath,clr[PUBACK]);
   Clear(wpath);
}
