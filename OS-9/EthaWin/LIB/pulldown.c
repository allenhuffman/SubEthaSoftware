/*
 * PullDown.c
 *
 * 1.00 3/10/95 - change clr from "char" to "int", MM/1 updates
 *
 */

#include <ETHAWIN/ethawin.h>

extern int clr[COLORS];

PullDown(wpath,x,y,w,h) /* another window maker routine... */
int wpath,x,y,w,h;
{
   OWSet(wpath,1,x+1,y+1,w,h,0,clr[PDSHAD]);
   OWSet(wpath,1,x,y,w,h,clr[PDFORE]+8,clr[PDBACK]);
#ifdef OSK
   ScaleSw(wpath,0); /* turn scaling off */
   FColor(wpath,clr[PDSHAD]);
   SetDPtr(wpath,7,3);
   Line(wpath,(w-1)*8,3);
   Line(wpath,7,(h-1)*8+4);
   FColor(wpath,clr[PDFORE]);
   SetDPtr(wpath,(w-1)*8,(h-1)*8+4);
   Line(wpath,(w-1)*8,4);
   Line(wpath,8,(h-1)*8+4);
#endif
   CWArea(wpath,1,1,w-1,h-1);
}
