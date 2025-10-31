/*
 * Labeler.c V0.00 by Allen C. Huffman
 * Copyright (C) 1993 by Sub-Etha Software
 *
 * This serves as a starting place for writing 'C' code...
 *
 * 0.00 10/13/93 - Initial version
 */

#define MAXMENUS 2 /* number of pull-down menus */

#include <stdio.h>
#include <modes.h>
#include <dir.h>

#include "ethawin.h"
#include "ethaglobals.h"
#include "stdinc.h"

#define SPACE 32
#define SHIFTUP 28
#define SHIFTDOWN 26

#define FILE 0
#define MISC 1

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i;
   int wpath,menu,option;
   char ch,click;

   char loop,done,update;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
      }
   }
/* rest of code goes here... */
   wpath=Startup();

   TopText(wpath,"Don't","Labeler V0.00 - By Allen Huffman","Panic!");
   About(wpath);
/*   PopUp(wpath,"DON'T PANIC!",26,9,28,5);
   BlnkOn(wpath);
   PrintAt(wpath,1,1,"Reading Configuration...");
   PrinterCfg(wpath);
   BlnkOff(wpath);
   EndWin(wpath); */

   while (!CheckMenu(wpath,&menu,&option,&ch,&click)) { }

   ShutDown(wpath);
}

Usage()
{
   fputs("\nLabeler V0.00 by Allen Huffman of Sub-Etha Software\n",stderr);
   fputs("Syntax: Labeler [-opts]\n",stderr);
   fputs("Usage : A nifty label printer...\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   exit(0);
}
