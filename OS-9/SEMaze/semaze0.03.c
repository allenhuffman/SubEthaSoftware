/*
 * SEMaze.c V0.03 by Allen C. Huffman
 * Copyright (C) 1994 by Sub-Etha Software
 *
 * This serves as a starting place for writing 'C' code...
 *
 * 0.00 4/06/94 - Initial generic version
 * 0.01 4/06/94 - Re-write, now that I know what I'm doing...
 * 0.02 4/07/94 - Startup screen, map, etc.  Demo ready?
 * 0.03 4/08/94 - Better hallway draw, filter out bogus redraws...
 */
 
#include <stdio.h>
#include <modes.h>
#include <sgstat.h>
#include "stdinc.h"
 
/* Define the special control key thingies...and give Joel a break! */
 
#ifdef OSK
#define UP   16
#define DOWN 14
#define LEFT  2
#define RIGHT 6
#else
#define UP   12
#define DOWN 10
#define LEFT  8
#define RIGHT 9
#endif
 
#define BLACK 0
#define DKGREY 16
#define LTGREY 32
#define WHITE 63
 
int map[] = { /* 10 x 10 maze area, 1=wall, 0=space */
   1,1,1,1,1,1,1,1,1,1,
   1,0,0,0,0,1,0,0,0,1,
   1,0,1,1,0,1,0,1,0,1,
   1,0,0,0,0,0,0,1,0,1,
   1,0,1,1,0,1,0,1,0,1,
   1,0,1,0,0,1,0,1,0,1,
   1,0,1,0,1,0,0,1,0,1,
   1,0,1,0,1,0,1,1,0,1,
   1,0,0,0,1,0,0,0,0,1,
   1,1,1,1,1,1,1,1,1,1
};
/* int dots[100]; 100 dots? */

int step[] = { /* North, East, South, West */
   -10,1,10,-1
};
 
/* global place for tmode status...  I think... */
 
struct sgbuf sgbufold,sgbufnew;

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j,wpath;
   int loc; /* location in maze (0-99) */
   int dir; /* direction facing 0=North, 2=South, 3=West, 1=East */
   char ch,done,mono,validkey;
 
   mono=FALSE;
 
   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
         if (argv[i][1]=='c') { /* is it '-c' composite/monochrome? */
            mono=TRUE;
         }
      }
   }
   /* rest of code goes here... */
 
   loc=11; /* Start at top left of maze */
   dir=2;  /* Facing South */
   done=FALSE; /* Not done.  How could we be?  We just started! */
 
/*   for (i=0; i<99; i++) dots[i]=1; init dots */
 
   wpath=OpenDev("/W",S_IREAD+S_IWRITE); /* open path to a window */
 
   _gs_opt(wpath,&sgbufold); /* save current status */
   _gs_opt(wpath,&sgbufnew); /* get copy to play with */
   sgbufnew.sg_echo=sgbufnew.sg_kbich=sgbufnew.sg_kbach=sgbufnew.sg_eofch=0;
   _ss_opt(wpath,&sgbufnew); /* update with modified values */
 
#ifdef OSK
   DWSet(wpath,0,0,0,80,24,3,0,0); /* create OSK window */
#else
   DWSet(wpath,7,0,0,80,24,3,0,0); /* create OS9 window */
#endif
   Select(wpath); /* and switch to it... */
 
   PalOff(wpath);
   BColor(wpath,0); /* screen and border black */
   Border(wpath,0);
   CurOff(wpath); /* cursor go bye bye */
   ScaleSw(wpath,0); /* scaling off */
 
   if (mono==TRUE) { /* turn palettes on, correctly */
      PalOn(wpath);
   } else {
      PalOnRGB(wpath);
   }
 
   About(wpath);
   FColor(wpath,3);
 
   do { /* loop, loop, loop ... */
 
/*      if (dots[loc]==1) {
         Bell(wpath);
         dots[loc]=0;
      } */
 
      Clear(wpath);
 
      CurXY(wpath,38,1);
      write(wpath,"NorthEast SouthWest"+dir*5,5);
 
      /* find farthest wall, if any, down corridor */
      for (i=0; i<5 ; i++) {
         if (map[ loc+(i*step[dir]) ]==1) { /* wall found */
            SetDPtr(wpath,i*64,i*16); /* draw that end wall */
            Box(wpath,639-i*64,191-i*16);
            break;
         }
      }
 
      /* scan passages on the sides of the hallway and display them */
      for (j=1; j<=i; j++) {
         if (map[ loc+((j-1)*step[dir])+step[dir-1 & 3] ]==0) { /* left */
            SetDPtr(wpath,j*64,j*16);
            RLine(wpath,-64,0);
            RLine(wpath,0,(6-j)*32-1);
            SetDPtr(wpath,j*64,191-j*16);
            RLine(wpath,-64,0);
         } 
         else { /* no passage */
            SetDPtr(wpath,j*64,j*16);
            RLine(wpath,-64,-16);
            RLine(wpath,0,(6-j)*32-1);
            SetDPtr(wpath,j*64,191-j*16);
            RLine(wpath,-64,16);
         }
         if (map[ loc+((j-1)*step[dir])+step[dir+1 & 3] ]==0) { /* right */
            SetDPtr(wpath,639-j*64,j*16);
            RLine(wpath,64,0);
            RLine(wpath,0,(6-j)*32-1);
            SetDPtr(wpath,639-j*64,191-j*16);
            RLine(wpath,64,0);
         } else { /* no passage */
            SetDPtr(wpath,639-j*64,j*16);
            RLine(wpath,64,-16);
            RLine(wpath,0,(6-j)*32-1);
            SetDPtr(wpath,639-j*64,191-j*16);
            RLine(wpath,64,16);
         }
/* stuff for dots...kinda like a 3D pacman thing, eh?
         if (dots[ loc+((j-1)*step[dir]) ]==1) {
            SetDPtr(wpath,319,191-j*16+8);
            Circle(wpath,8);
         } */
      }
 
      do {
         validkey=TRUE;
         while(_gs_rdy(wpath)==-1) tsleep(10); /* sleep til char ready */
         read(wpath,&ch,1); /* read the character */
         ch=toupper(ch); /* uppercase it */
 
/* There is a bug in here somewhere.  BACK against a wall doesn't force a
   redraw, but FORWARD into a wall still clears the screen.  Odd... */
 
         switch (ch) { /* check for keypress matches */
         case UP:
            if (map[loc+step[dir]]!=1) { /* if there isn't a wall there */
               loc=loc+step[dir]; /* move there */
            } else {
               Bell(wpath);
               validkey=FALSE;
            }
            break;
         case DOWN:
            if (map[loc-step[dir]]!=1) {
                loc=loc-step[dir];
            } else {
               Bell(wpath);
               validkey=FALSE;
            }
            break;
         case LEFT:
            dir--;
            if (dir<0) dir=3;
            break;
         case RIGHT:
            dir++;
            if (dir>3) dir=0;
            break;
         case 'Q':
            done=TRUE;
            break;
         default:
            validkey=FALSE;
         }
      } while(!validkey); /* if validkey==FALSE, no good key so do again */
   } while(!done); /* if not done, redisplay maze and continue */

   Select(STDOUT); /* select original calling window */
 
   _ss_opt(wpath,&sgbufold); /* restore initial tmode settings */
 
   close(wpath); /* and close the window it made for this... */
}
 
Usage()
{
   fputs("\nSEMaze V0.03 by Allen Huffman of Sub-Etha Software (coco-sysop@genie.geis.com)\n",stderr);
   fputs("Syntax: SEMaze [-opts]\n",stderr);
   fputs("Usage : Test of 'maze' engine.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   fputs("        -c = select composite/monochrome colors.\n",stderr);
   exit(0);
}
 
About(wpath) /* display About overlay box */
int wpath;
{
   PopUp(wpath,"Sub-Etha Software Presents...",15,8,50,11);
   PrintAt(wpath,12,0,"M A Z E   D E M O   V0.03");
   PrintAt(wpath,15,1,"by Allen C. Huffman");
   /*   PrintAt(wpath,9,2,"MM1 Port by Joel Mathew Hegberg"); */
   PrintAt(wpath,5,3,"Copyright (C) 1994 by Sub-Etha Software");
   PrintAt(wpath,9,5,"Use ARROW KEYS to move and turn");
   PrintAt(wpath,19,6,"'Q' to Quit");
   Button(wpath,18,8,"Press Any Key");
   Wait(wpath);
   EndWin(wpath);
}
 
int OpenDev(filename,mode)
char filename[];
int mode;
{
   int fpath;
 
   if ((fpath=open(filename,mode))==-1) {
      fputs("Unable to open text file...\n",stderr);
      exit(0);
   }
   return(fpath);
}
 
Button(wpath,x,y,text)
int wpath,x,y;
char *text;
{
   int stx,sty,len;
   stx=x*8-4;
   sty=y*8;
   len=strlen(text)*8+6;
 
   FColor(wpath,2); /* light grey */
   SetDPtr(wpath,stx,sty);
   Bar(wpath,stx+len,sty+7);
 
   FColor(wpath,3); /* white */
   SetDPtr(wpath,stx,sty-1);
   Line(wpath,stx+len,sty-1);
   SetDPtr(wpath,stx-1,sty);
   Line(wpath,stx-1,sty+7);
 
   FColor(wpath,1); /* dark grey */
   SetDPtr(wpath,stx+len+1,sty);
   Line(wpath,stx+len+1,sty+7);
   SetDPtr(wpath,stx,sty+8);
   Line(wpath,stx+len,sty+8);
 
   FColor(wpath,0); /* black */
   CurXY(wpath,x,y);
   TCharSw(wpath,1);
   write(wpath,text,strlen(text));
   TCharSw(wpath,0);
   FColor(wpath,3); /* white */
}
 
PopUp(wpath,title,x,y,w,h)
int wpath,x,y,w,h;
char *title;
{
   int tlen;
   tlen=strlen(title);
 
   OWSet(wpath,1,x,y,w,h+1,3,2);
   CurXY(wpath,w/2-tlen/2,0);
   write(wpath,title,tlen);
   /* 
      FColor(wpath,3);
      SetDPtr(wpath,0,8);
      RBox(wpath,w*8-1,h*8-1); */
 
   FColor(wpath,1); /* make a "lowered text area" */
   SetDPtr(wpath,7,8);
   Line(wpath,(w-1)*8,8);
   Line(wpath,7,h*8+4);
   FColor(wpath,3);
   SetDPtr(wpath,(w-1)*8,h*8+4);
   Line(wpath,(w-1)*8,9);
   Line(wpath,8,h*8+4);
 
   CWArea(wpath,1,2,w-1,h-1);
}
 
EndWin(wpath)
int wpath;
{
   OWEnd(wpath);
}
 
PalOff(wpath) /* turn all 4 palettes to black */
int wpath;
{
#ifdef OSK
   Palette(wpath,0,0x00,0x00,0x00); /* OSK make everything dark... */
   Palette(wpath,1,0x00,0x00,0x00);
   Palette(wpath,2,0x00,0x00,0x00);
   Palette(wpath,3,0x00,0x00,0x00);
#else
   Palette(wpath,0,BLACK); /* make everything dark... */
   Palette(wpath,1,BLACK);
   Palette(wpath,2,BLACK);
   Palette(wpath,3,BLACK);
#endif
}
 
PalOn(wpath) /* turn on 4-level grey palettes */
int wpath;
{
#ifdef OSK
   Palette(wpath,0,0x00,0x00,0x00); /* OSK turn on 4 grey-scale palettes */
   Palette(wpath,1,0x50,0x50,0x50);
   Palette(wpath,2,0x90,0x90,0x90);
   Palette(wpath,3,0xff,0xff,0xff);
#else
   Palette(wpath,0,BLACK); /* turn on 4 grey-scale palettes */
   Palette(wpath,1,DKGREY);
   Palette(wpath,2,LTGREY);
   Palette(wpath,3,WHITE);
#endif
}
 
PalOnRGB(wpath) /* turn on 4-level grey palettes (RGB colors) */
int wpath;
{
   Palette(wpath,0,0); /* turn on 4 grey-scale palettes */
   Palette(wpath,1,7);
   Palette(wpath,2,56);
   Palette(wpath,3,63);
}
 
PrintAt(wpath,x,y,text)
int wpath,x,y;
char *text;
{
   CurXY(wpath,x,y);
   write(wpath,text,strlen(text));
}
 
Wait(wpath) /* wait for a keypress */
int wpath;
{
   char ch;
 
   while(_gs_rdy(wpath)==-1) tsleep(10);
   read(wpath,&ch,1); /* gobble character */
}
