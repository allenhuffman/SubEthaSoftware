/*
 * SEMaze.c V0.01 by Allen C. Huffman
 * Copyright (C) 1994 by Sub-Etha Software
 *
 * This serves as a starting place for writing 'C' code...
 *
 * 0.00 4/06/94 - Initial generic version
 * 0.01 4/06/94 - Re-write, now that I know what I'm doing...
 */

#include <stdio.h>

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

int map[] = { /* 10 x 10 maze area, 1=wall, 0=space */
   1,1,1,1,1,1,1,1,1,1,
   1,0,0,0,0,0,0,1,0,1,
   1,0,1,1,1,1,0,1,0,1,
   1,0,1,0,1,0,0,0,0,1,
   1,0,1,0,1,0,1,1,0,1,
   1,0,1,0,1,1,1,1,1,1,
   1,0,1,0,0,0,0,0,0,1,
   1,0,1,0,1,1,0,1,0,1,
   1,0,1,0,0,0,0,0,0,1,
   1,1,1,1,1,1,1,1,1,1
};
int step[] = { /* North, East, South, West */
   -10,1,10,-1
};
int dirs[] = { /* Directions...duh... */
   "North",
   "East",
   "South",
   "West"
};

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j;
   int loc; /* location in maze (0-99) */
   int dir; /* direction facing 0=North, 2=South, 3=West, 1=East */
   char ch,done;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
      }
   }
/* rest of code goes here... */

   loc=11; /* Start at top left of maze */
   dir=2;  /* Facing South */
   done=FALSE; /* Not done.  How could we be?  We just started! */

   setbuf(stdin,0);

 do { /* loop, loop, loop ... */

   Clear(STDOUT);

   CurXY(STDOUT,38,0);
   puts(dirs[dir]);
   
   /* find farthest wall, if any, down corridor */
   for (i=0; i<5 ; i++) {
      if (map[ loc+(i*step[dir]) ]==1) { /* wall found */
         SetDPtr(STDOUT,i*64,i*16); /* draw that end wall */
         Box(STDOUT,639-i*64,191-i*16);
         break;
      }
   }

   /* scan passages on the left side of the hallway and display them */
   for (j=1; j<=i; j++) {
      if (map[ loc+((j-1)*step[dir])+step[dir-1 & 3] ]==0) { /* passage */
         SetDPtr(STDOUT,j*64,j*16);
         RLine(STDOUT,-64,0);
         RLine(STDOUT,0,(6-j)*32-1);
         SetDPtr(STDOUT,j*64,191-j*16);
         RLine(STDOUT,-64,0);
      } else { /* no passage */
         SetDPtr(STDOUT,j*64,j*16);
         RLine(STDOUT,-64,-16);
         RLine(STDOUT,0,(6-j)*32-1);
         SetDPtr(STDOUT,j*64,191-j*16);
         RLine(STDOUT,-64,16);
      }
   }
   /* and now scan for right side passages */
   for (j=1; j<=i; j++) {
      if (map[ loc+((j-1)*step[dir])+step[dir+1 & 3] ]==0) { /* passage */
         SetDPtr(STDOUT,639-j*64,j*16);
         RLine(STDOUT,64,0);
         RLine(STDOUT,0,(6-j)*32-1);
         SetDPtr(STDOUT,639-j*64,191-j*16);
         RLine(STDOUT,64,0);
      } else { /* no passage */
         SetDPtr(STDOUT,639-j*64,j*16);
         RLine(STDOUT,64,-16);
         RLine(STDOUT,0,(6-j)*32-1);
         SetDPtr(STDOUT,639-j*64,191-j*16);
         RLine(STDOUT,64,16);
      }
   }
   fflush(stdout);
   ch=getchar();
   ch=toupper(ch);

   switch (ch) {
      case UP:
         if (map[loc+step[dir]]!=1) loc=loc+step[dir];
         break;
      case LEFT:
         dir--;
         if (dir<0) dir=3;
         break;
      case RIGHT:
         dir++;
         if (dir>3) dir=0;
         break;
      case DOWN:
         if (map[loc-step[dir]]!=1) loc=loc-step[dir];
         break;
      case 'Q':
         done=TRUE;
         break;
   }
 } while(!done);

}


Usage()
{
   fputs("\nSEMaze V0.00 by Allen Huffman of Sub-Etha Software (SysOp@Delta/StG)\n",stderr);
   fputs("Syntax: SEMaze [-opts]\n",stderr);
   fputs("Usage : Test of 'maze' engine.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   exit(0);
}
