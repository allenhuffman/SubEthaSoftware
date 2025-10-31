/*
 * SEMaze2.c V0.01 by Allen C. Huffman
 * Copyright (C) 1994 by Sub-Etha Software
 *
 * 0.00 4/14/94 - Starting over . . .
 * 0.01 4/20/94 - Working on more stuff...
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
 
/* Not sure if these are used in here or not, actually. . . */
 
#define BLACK 0
#define DKGREY 16
#define LTGREY 32
#define WHITE 63
 
char *map[] = { /* new map data format (strings) */
   "XXXXXXXXXXXXXXXXXXXXXXX",
   "X..........X..........X",
   "XoXXX.XXXX.X.XXXX.XXXoX",
   "X.....................X",
   "X.XXX.X.XXXXXXX.X.XXX.X",
   "X.....X....X....X.....X",
   "XXXXX.XXXX X XXXX.XXXXX",
   "    X.X         X.X    ",
   "XXXXX.X XXX-XXX X.XXXXX",
   "     .  X     X  .     ",            /* ...map[y][x]... */
   "XXXXX.X XXXXXXX X.XXXXX",
   "    X.X         X.X    ",
   "XXXXX.X XXXXXXX X.XXXXX",
   "X..........X..........X",
   "X.XXX.XXXX.X.XXXX.XXX.X",
   "Xo..X......$......X..oX",
   "XXX.X.X.XXXXXXX.X.X.XXX",
   "X.....X....X....X.....X",
   "X.XXXXXXXX.X.XXXXXXXX.X",
   "X.....................X",
   "XXXXXXXXXXXXXXXXXXXXXXX"
};
int width=23; /* maze diameters */
int height=21;

/* global place for tmode status...  I think... */
 
struct sgbuf sgbufold,sgbufnew;
 
main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j,wpath[2];
   int loc_x,loc_y; /* location in maze (x,y) */
   int temp_x,temp_y;
   int move_x,move_y; /* movement stuff */
   int dir; /* direction facing 0=North, 1=South, 2=West, 3=East */
   int window; /* current window */
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

   /* find starting location in maze by scanning for '$' */
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         if (map[i][j]=='$') break;
      }
   }
   if (i==width && j==height) { /* default starting position */
      loc_x=1; /* top left of the maze, assuming border */
      loc_y=1;
   } else {
      loc_x=i; /* may need to reverse this... */
      loc_y=j;
   }
 
   dir=3;  /* Facing East */
   move_x=1;
   move_y=0;
   window=0; /* first window */
   done=FALSE; /* Not done.  How could we be?  We just started! */
 
   for (i=0; i<2; i++) {
      wpath[i]=OpenDev("/W",S_IREAD+S_IWRITE); /* open path to a window */
   }
#ifdef OSK
   DWSet(wpath[0],0,0,0,80,24,3,0,0); /* create OSK window */
   DWSet(wpath[1],0,0,0,80,24,3,0,0); /* create another OSK window */
#else
   DWSet(wpath[0],7,0,0,80,24,3,0,0); /* create OS9 window */
   DWSet(wpath[1],7,0,0,80,24,3,0,0); /* create another OS9 window */
#endif
 
   _gs_opt(wpath[0],&sgbufold); /* save current status */
   _gs_opt(wpath[0],&sgbufnew); /* get copy to play with */
   sgbufnew.sg_echo=sgbufnew.sg_kbich=sgbufnew.sg_kbach=sgbufnew.sg_eofch=0;
   _ss_opt(wpath[0],&sgbufnew); /* update with modified values */
   _ss_opt(wpath[1],&sgbufnew); /* ...for both windows */
 
   Select(wpath[0]); /* and switch to main window... */
 
   for (i=0; i<2; i++) {
      PalOff(wpath[i]);
      BColor(wpath[i],0); /* screen and border black */
      Border(wpath[i],0);
      CurOff(wpath[i]); /* cursor go bye bye */
      ScaleSw(wpath[i],0); /* scaling off */
 
      if (mono==TRUE) { /* turn palettes on, correctly */
         PalOnCMP(wpath[i]);
      } else {
         PalOnRGB(wpath[i]);
      }
      FColor(wpath[i],3);
   }
 
   About(wpath[0]);
 
   do { /* loop, loop, loop ... */
 
      window=(window+1 & 1); /* 0 to 1, to 0, to 1 ... */
      Clear(wpath[window]);
 
      CurXY(wpath[window],38,1);
      write(wpath[window],"NorthEast SouthWest"+dir*5,5);

      /* calculate steps to move... */
      temp_x=loc_x;
      temp_y=loc_y;

      /* find farthest wall, if any, down corridor */
      for (i=0; i<5 ; i++) {
         if (map[temp_x][temp_y]=='X') {
            SetDPtr(wpath[window],i*64,i*16); /* draw that end wall */
            Box(wpath[window],639-i*64,191-i*16);
            break;
         }
         temp_x=temp_x+move_x; /* add stuff later */
         temp_y=temp_y+move_y;
      }
      temp_x=loc_x;

      /* scan passages on the sides of the hallway and display them */
      for (j=1; j<=i; j++) {

         if (map[temp_y-(dir>1)][temp_x-(dir<2)]!='X') {
            SetDPtr(wpath[window],j*64,j*16);
            RLine(wpath[window],-64,0);
            RLine(wpath[window],0,(6-j)*32-1);
            SetDPtr(wpath[window],j*64,191-j*16);
            RLine(wpath[window],-64,0);
         } 
         else { /* no passage */
            SetDPtr(wpath[window],j*64,j*16);
            RLine(wpath[window],-64,-16);
            RLine(wpath[window],0,(6-j)*32-1);
            SetDPtr(wpath[window],j*64,191-j*16);
            RLine(wpath[window],-64,16);
         }
         if (map[temp_y+(dir>1)][temp_x+(dir<2)]!='X') {
            SetDPtr(wpath[window],639-j*64,j*16);
            RLine(wpath[window],64,0);
            RLine(wpath[window],0,(6-j)*32-1);
            SetDPtr(wpath[window],639-j*64,191-j*16);
            RLine(wpath[window],64,0);
         } else { /* no passage */
            SetDPtr(wpath[window],639-j*64,j*16);
            RLine(wpath[window],64,-16);
            RLine(wpath[window],0,(6-j)*32-1);
            SetDPtr(wpath[window],639-j*64,191-j*16);
            RLine(wpath[window],64,16);
         }
         temp_x=temp_x+move_x; /* add boundry checking later */
         temp_y=temp_y+move_y;
      }
 
      Select(wpath[window]); /* select it */
 
      do {
         validkey=TRUE;
 
         i=0; /* these lines are to test something... */
         while(1) {
            if (_gs_rdy(wpath[i])>0) {
               read(wpath[i],&ch,1);
               break;
            }
            tsleep(10);
            i=(i+1 & 1);
         }
         ch=toupper(ch); /* uppercase it */
 
         switch (ch) { /* check for keypress matches */
         case UP:
            loc_x=loc_x+move_x;
            loc_y=loc_y+move_y;
            break;
         case DOWN:
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
         case '?':
            ShowMap(wpath[window]); /* heh heh heh... */
            break;
         default:
            validkey=FALSE;
         }
      } while(!validkey); /* if validkey==FALSE, no good key so do again */
   } while(!done); /* if not done, redisplay maze and continue */
 
   Select(STDOUT); /* select original calling window */
 
   for (i=0; i<2; i++) {
      _ss_opt(wpath[window],&sgbufold); /* restore initial tmode settings */
      close(i); /* and close the window it made for this... */
   }
}
 
Usage()
{
   fputs("\nSEMaze2 V0.01 by Allen Huffman of Sub-Etha Software (coco-sysop@genie.geis.com)\n",stderr);
   fputs("Syntax: SEMaze [-opts]\n",stderr);
   fputs("Usage : Second test of 'maze' engine.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
#ifndef OSK
   fputs("        -c = select composite/monochrome colors.\n",stderr);
#endif
   exit(0);
}
 
About(wpath) /* display About overlay box */
int wpath;
{
   PopUp(wpath,"Sub-Etha Software Presents...",15,8,50,11);
   PrintAt(wpath,10,0,"M A Z E   D E M O   2   V0.01");
   PrintAt(wpath,15,1,"by Allen C. Huffman");
#ifdef OSK
   PrintAt(wpath,9,2,"MM1 Port by Joel Mathew Hegberg");
#endif
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
 
PalOnCMP(wpath) /* turn on 4-level grey palettes */
int wpath;
{
   Palette(wpath,0,BLACK); /* turn on 4 grey-scale palettes */
   Palette(wpath,1,DKGREY);
   Palette(wpath,2,LTGREY);
   Palette(wpath,3,WHITE);
}
 
PalOnRGB(wpath) /* turn on 4-level grey palettes (RGB colors) */
int wpath;
{
#ifdef OSK
   Palette(wpath,0,0x00,0x00,0x00); /* OSK turn on 4 grey-scale palettes */
   Palette(wpath,1,0x50,0x50,0x50);
   Palette(wpath,2,0x90,0x90,0x90);
   Palette(wpath,3,0xff,0xff,0xff);
#else
   Palette(wpath,0,0); /* turn on 4 grey-scale palettes (RGB) */
   Palette(wpath,1,7);
   Palette(wpath,2,56);
   Palette(wpath,3,63);
#endif
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
 
ShowMap(wpath) /* show map on the screen <grin> */
int wpath;
{
   int i,j;
 
   PopUp(wpath,"The Maze",10,0,width*2,height+1); /* Place for map... */
 
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         if (map[i][j]=='X') {
            SetDPtr(wpath,8+j*16,i*8);
            RBox(wpath,16,8);
         }
      }
   }
   Wait(wpath);
   EndWin(wpath);
}
