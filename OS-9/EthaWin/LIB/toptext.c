/*
 * TopText.c
 *
 * 1.00 3/10/95 - change clr from "char" to "int", MM/1 updates
 *
 */

#include <ETHAWIN/ethawin.h>
#ifdef OSK
#include <CGFX/buffs.h>
#endif

extern int clr[COLORS];

TopText(wpath,left,title,right) /* display text on top line */
int wpath;
char *left,*title,*right;
{
   CurXY(wpath,0,0);
   FColor(wpath,clr[TTFORE]);
   BColor(wpath,clr[TTBACK]);
   ErLine(wpath); /* erase old line */
#ifdef OSK
   CurXY(wpath,0,0);
   Font(wpath,GRP_FONT,FNT_G8X8);  /* Set to graphic font */
   write(wpath,ICN_MOVE,1);  /* Move-window icon */
   CurXY(wpath,79,0);
   write(wpath,ICN_BACK,1);  /* Back-window icon */
   Font(wpath,GRP_FONT,FNT_S8X8);  /* Back to standard font */
#endif
   CurXY(wpath,2,0);
   write(wpath,left,strlen(left));
   CurXY(wpath,40-strlen(title)/2,0);
   write(wpath,title,strlen(title));
   CurXY(wpath,78-strlen(right),0);
   write(wpath,right,strlen(right));
   FColor(wpath,clr[FORE]);
   BColor(wpath,clr[BACK]);
}
