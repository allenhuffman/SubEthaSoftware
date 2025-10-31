/*
 * EthaDemo.c V1.01 by Allen C. Huffman
 * Copyright (C) 1993,94 by Sub-Etha Software
 *
 * 1.00 10/09/93 - Re-done version to actually use the library
 * 1.01 11/13/94 - Modified to use new library screen setup thingy call
 */

/* #define MAXMENUS 4*/ /* number of pull-down menus in this program */

#include <stdio.h>
#include <modes.h>

#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaglobals.h>

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i;
   int wpath,menu,option;
   char ch,click;
   char makewin;

   makewin=TRUE;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
         /* v1.01 */
         if (argv[i][1]=='s') { /* -s (same screen) */
            makewin=FALSE; /* so don't create a new window to run on */
         }
      }
   }
   /* First, ya gotta initialize the EthaWin interface for this program */

   wpath=Startup(makewin); /* read config, create window, menu bar, etc. */

   /* Then display some stuff that you might want to display */

   TopText(wpath,"Left","EthaWin DEMO - By Allen Huffman","Right");
   About(wpath); /* programmer's "about" routine */

   /* For the demo, we print the status of some of the .cfg settings... */

   CurXY(wpath,10,23);
   if (hotkey==TRUE) {
      cwrite(wpath,"Hotkeys ON",80);
   } 
   else {
      cwrite(wpath,"Hotkeys OFF",80);
   }
#ifndef OSK
   CurXY(wpath,25,23);
   if (mouseport==0) {
      cwrite(wpath,"MousePort LEFT",80);
   } 
   else {
      cwrite(wpath,"MousePort RIGHT",80);
   }
   CurXY(wpath,40,23);
   if (mouseres==0) {
      cwrite(wpath,"MouseRes LOW",80);
   } 
   else {
      cwrite(wpath,"MouseRes HIGH",80);
   }
#endif
   CurXY(wpath,55,23);
   if (mouseon==TRUE) {
      cwrite(wpath,"Mouse ON",80);
   } 
   else {
      cwrite(wpath,"Mouse OFF",80);
   }

   /* Fill the screen with mindless demo-babble... */

   PrintAt(wpath,4,3,"Greetings from the Sub-Etha!  This is a sample of the EthaWin text-based");
   PrintAt(wpath,4,4,"windowing interface currently under development by Sub-Etha Software.");
   PrintAt(wpath,4,7,"Please read the 'ethademo.doc' file included with this archive for more\n");
   PrintAt(wpath,4,8,"information on this interface and developing programs for it.  Usage:");
#ifndef OSK
   PrintAt(wpath,4,10,"MOUSE/JOYSTICK - Position text-cursor on the menu-bar and click button");
#else
   PrintAt(wpath,4,10,"MOUSE/TRACKBALL - Position mouse cursor on the menu-bar and click button");
#endif
   PrintAt(wpath,4,11,"to select a menu.  Once the menu is displayed, move up/down to choose an");
#ifndef OSK
   PrintAt(wpath,4,12,"item then click button to select.  To abort a pull-down menu, move below");
   PrintAt(wpath,4,13,"the options then click.  (Keyboard mouse may also be used.)");
#else
   PrintAt(wpath,4,12,"item then click button to select.  To abort a pull-down menu, move off");
   PrintAt(wpath,4,13,"the menu then click.");
#endif
   PrintAt(wpath,4,15,"KEYBOARD - Press ALT+UNDERLINED LETTER of the menu to select.  Once the");
   PrintAt(wpath,4,16,"menu is displayed, use up/down arrow keys to choose an item and press");
   PrintAt(wpath,4,17,"ENTER to select it.  You may also press the underlined letter of option");
   PrintAt(wpath,4,18,"you wish to choose.  Use left/right arrow keys to move from menu to");
#ifndef OSK
   PrintAt(wpath,4,19,"menu, or press BREAK to abort a menu.");
#else
   PrintAt(wpath,4,19,"menu, or press ESC to abort a menu.");
#endif

   /* Now, here are the "guts" of the program, using EthaWin... */

   while(1) {
      if (CheckMenu(wpath,&menu,&option,&ch,&click)) {

         /* if TRUE, then a menu/option was selected so process it */

         CurXY(wpath,0,23); /* display option that was chosen */
         cwrite(wpath,menu_text[menu][option],80);

         if (menu==2) { /* if Text menu selected, */
            if (menu_stat[menu][option]==0) { /* toggle the "flag" */
               menu_stat[menu][option]=2;
            } 
            else {
               menu_stat[menu][option]=0;
            }
         }

         /* check if QUIT */

         if ( (menu==0) && (option==5) ) { /* if QUIT, */
            if (YesNo(wpath,"QUIT DEMO")) break;
         }
      }
   }

   ShutDown(wpath); /* Goodbye, EthaWin... */
}

Usage()
{
   fputs("\nEthaDemo V1.01 by Allen Huffman (coco-sysop@genie.geis.com)\n",stderr);
   fputs("Copyright (C) 1994 by Sub-Etha Software\n\n",stderr);
   fputs("Syntax: EthaDemo [-opts]\n",stderr);
   fputs("Usage : Useless demonstration of the EthaWin interface.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   fputs("        -s = use existing screen.\n",stderr);
   exit(0);
}

About(wpath)
int wpath;
{
   PopUp(wpath,"ABOUT",20,6,40,13);

   PrintAt(wpath,2,1,"EthaDemo V1.01 by Allen C. Huffman");
   PrintAt(wpath,6,2,"Text-Based User Interface.");
   PrintAt(wpath,5,4,"For development information,");
   PrintAt(wpath,6,5,"contact Sub-Etha Software");
   PrintAt(wpath,2,6,"P.O. Box 152442 - Lufkin, TX 75915");
   PrintAt(wpath,5,7,"or COCO-SYSOP@GENIE.GEIS.COM");
   PrintAt(wpath,3,9,"Press Any Key or Click Button...");

   Wait(wpath);
   EndWin(wpath);
}

Help(wpath)
int wpath;
{
   PopUp(wpath,"HELP",20,6,40,13);

   PrintAt(wpath,5,5,"Sorry, no help available...");

   Wait(wpath);
   EndWin(wpath);
}
