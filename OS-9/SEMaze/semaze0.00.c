/*
 * SEMaze.c V0.00 by Allen C. Huffman
 * Copyright (C) 1994 by Sub-Etha Software
 *
 * This serves as a starting place for writing 'C' code...
 *
 * 0.00 4/06/94 - Initial generic version
 */

#include <stdio.h>

#include "stdinc.h"

#define WIDTH 10
#define HEIGHT 10

int map[] = { /* 10 x 10 maze area, 1=wall, 0=space */
   1,1,1,1,1,1,1,1,1,1,
   1,0,0,0,0,0,0,1,0,1,
   1,0,1,1,0,1,0,0,0,1,
   1,0,1,0,0,0,1,1,0,1,
   1,0,0,0,1,0,0,0,0,1,
   1,0,1,0,1,1,1,0,1,1,
   1,0,1,0,0,0,1,0,0,1,
   1,0,1,1,1,0,1,1,0,1,
   1,0,0,0,0,0,1,0,0,1,
   1,1,1,1,1,1,1,1,1,1
};
int step[] = { /* North, South, West, and East */
   -10,1,10,-1
};


main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j;
   int loc; /* location in maze (0-99) */
   int dir; /* direction facing 0=North, 2=South, 3=West, 1=East */
   char ch;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
      }
   }
/* rest of code goes here... */

   loc=88; /* start in bottom right corner */
   dir=0;  /* facing North */

   setbuf(stdin,0);

for (;;) { /* endless, mindless looping */

   Clear(STDOUT);
   /* find farthest wall, if any, down corridor */
   for (i=0; i<6 ; i++) {
      if (map[ loc+(i*step[dir]) ]==1) { /* wall found */
         break;
      }
   }
   SetDPtr(STDOUT,i*64,i*16);
   Box(STDOUT,640-i*64,192-i*16);

   CurXY(STDOUT,0,0);
   fflush(stdout);
   printf("Room %d, Facing %d, Wall at %d spaces.",loc,dir,i);

   for (j=0; j<i; j++) { /* draw vertical lines */
      SetDPtr(STDOUT,j*64,j*16);
      Line(STDOUT,j*64,192-j*16);
      SetDPtr(STDOUT,640-j*64,j*16);
      Line(STDOUT,640-j*64,192-j*16);
   }
   SetDPtr(STDOUT,i*64,i*16);
   RLine(STDOUT,640-i*64,0);
   SetDPtr(STDOUT,i*64,192-i*16);
   RLine(STDOUT,640-i*64,0);

   /* scan passages on the left side of the hallway and display them */
   for (j=0; j<=i; j++) {
      if (map[ loc+(j*step[dir])+step[dir-1] ]==0) { /* opening found */
         SetDPtr(STDOUT,j*64,j*16);
         RLine(STDOUT,-63,-15);
         SetDPtr(STDOUT,j*64,192-j*16);
         RLine(STDOUT,-63,15);
      } else {
         SetDPtr(STDOUT,j*64,j*16);
         RLine(STDOUT,-63,0);
         SetDPtr(STDOUT,j*64,192-j*16);
         RLine(STDOUT,-63,0);
      }
   }
   /* and now scan for right side passages */
   for (j=0; j<=i; j++) {
      if (map[ loc+(j*step[dir])+step[dir+1] ]==0) {
         SetDPtr(STDOUT,640-j*64,j*16);
         RLine(STDOUT,63,-15);
         SetDPtr(STDOUT,640-j*64,192-j*16);
         RLine(STDOUT,63,15);
      } else {
         SetDPtr(STDOUT,640-j*64,j*16);
         RLine(STDOUT,63,0);
         SetDPtr(STDOUT,640-j*64,192-j*16);
         RLine(STDOUT,63,0);
      }
   }
   printf(" - Direction?  (N, W, E) :");
   fflush(stdout);
   ch=getchar();

   if (ch=='N') {
      loc=loc+step[dir];
   }
   if (ch=='W') {
      dir--;
      if (dir<0) dir=3;
   }
   if (ch=='E') {
      dir++;
      if (dir>3) dir=0;
   }
 }

}


Usage()
{
   fputs("\nSEMaze V0.00 by Allen Huffman of Sub-Etha Software (SysOp@Delta/StG)\n",stderr);
   fputs("Syntax: SEMaze [-opts]\n",stderr);
   fputs("Usage : Test of 'maze' engine.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   exit(0);
}
