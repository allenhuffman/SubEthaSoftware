/*
 * EthaDemo.c V1.01 by Allen C. Huffman
 * Copyright (C) 1993,94 by Sub-Etha Software
 *
 * 1.00 10/09/93 - Re-done version to actually use the library
 * 1.01 11/13/94 - Modified to use new library screen setup thingy call
 * *.** 3/95     - Added buttons to demo and activate some File menu items,
 *                  they now call EwFPick() - C.Hyde
 */

#include <stdio.h>
#include <modes.h>
#include <ethawin.h>
#include <ethaglobals.h>
#include <stdinc.h>

#define MAXMENUS 4 /* number of pull-down menus */

#define NUMBUTTONS  5
#define HOTKEYS     0
#define MOUSE       1
#define MOUSERES    2
#define MOUSEPORT   3
#define CHANGEDIR   4

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i;
   int wpath,menu,option;
   char ch,click, *cp;
   char makewin;
   struct BUTTON buttons[NUMBUTTONS];

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
/* read config, create window, menu bar, etc. */
   wpath=Startup(makewin, MAXMENUS);

/* Then display some stuff that you might want to display */

   TopText(wpath,"Left","EthaWin DEMO - By Allen Huffman","Right");
   About(wpath); /* programmer's "about" routine */

/* now lets create the buttons */
 makeButton(wpath, buttons, HOTKEYS, "HotKeys", 10, 23);
 makeButton(wpath, buttons, MOUSE, "Mouse", 25, 23);
 makeButton(wpath, buttons, MOUSERES, "MouseRes", 40, 23);
 makeButton(wpath, buttons, MOUSEPORT, "MousePort", 55, 23);
 makeButton(wpath, buttons, CHANGEDIR, "Change Dir", 40, 21);

/* For the demo, we print the status of some of the .cfg settings... */

   CurXY(wpath,18,23);
   if (hotkey==TRUE) {
      cwrite(wpath,"ON",2);
   } else {
      cwrite(wpath,"OFF",3);
   }
   CurXY(wpath,31,23);
   if (mouseon==TRUE) {
      cwrite(wpath,"ON",2);
   } else {
      cwrite(wpath,"OFF",3);
   }
   CurXY(wpath,49,23);
   if (mouseres==0) {
      cwrite(wpath,"LOW",3);
   } else {
      cwrite(wpath,"HIGH",4);
   }
   CurXY(wpath,65,23);
   if (mouseport==0) {
      cwrite(wpath,"LEFT",4);
   } else {
      cwrite(wpath,"RIGHT",5);
   }

/* Fill the screen with mindless demo-babble... */

   PrintAt(wpath,4,3,"Greetings from the Sub-Etha!  This is a sample of the EthaWin text-based");
   PrintAt(wpath,4,4,"windowing interface currently under development by Sub-Etha Software.");
   PrintAt(wpath,4,7,"Please read the 'ethademo.doc' file included with this archive for more\n");
   PrintAt(wpath,4,8,"information on this interface and developing programs for it.  Usage:");
   PrintAt(wpath,4,10,"MOUSE/JOYSTICK - Position text-cursor on the menu-bar and click button");
   PrintAt(wpath,4,11,"to select a menu.  Once the menu is displayed, move up/down to choose an");
   PrintAt(wpath,4,12,"item then click buttom to select.  To abort a pull-down menu, move below");
   PrintAt(wpath,4,13,"the options then click.  (Keyboard mouse may also be used.)");
   PrintAt(wpath,4,15,"KEYBOARD - Press ALT+UNDERLINED LETTER of the menu to select.  Once the");
   PrintAt(wpath,4,16,"menu is displayed, use up/down arrow keys to choose an item and press");
   PrintAt(wpath,4,17,"ENTER to select it.  You may also press the underlined letter of option");
   PrintAt(wpath,4,18,"you wish to choose.  Use left/right arrow keys to move from menu to");
   PrintAt(wpath,4,19,"menu, or press BREAK to abort a menu.");

/* Now, here are the "guts" of the program, using EthaWin... */

   while(1) {
      if (CheckMenu(wpath,&menu,&option,&ch,&click,FALSE)) {

/* if TRUE, then a menu/option was selected so process it */

         CurXY(wpath,0,22); /* display option that was chosen */
         ErEOLine(wpath);
         cwrite(wpath,menu_text[menu][option],80);

         if ((menu==0) && ((option==0) || (option==1) || (option==3)))
         {
            cp=EwFPick(wpath, menu_text[menu][option], BOTH | OREAD | PREAD);
            CurXY(wpath,0,22);
            ErEOLine(wpath);
            cwrite(wpath, cp, strlen(cp));
         }
         if (menu==2) { /* if Text menu selected, */
            if (menu_stat[menu][option]==0) { /* toggle the "flag" */
               menu_stat[menu][option]=2;
            } else {
               menu_stat[menu][option]=0;
            }
         }

/* check if QUIT */

         if ( (menu==0) && (option==5) ) { /* if QUIT, */
            if (YesNo(wpath,"QUIT DEMO")) break;
         } /* if quit */
      } /* end if menu selected */
      else /* menu was not selected so see if a button was selected */
      {
         if (click)
         {
            switch(checkButtons(buttons, NUMBUTTONS))
            {
               case HOTKEYS:     /* HotKeys */
                                   CurXY(wpath,18,23);
                                   if (hotkey==TRUE) {
                                      hotkey=FALSE;
                                      cwrite(wpath,"OFF",3);
                                   } else {
                                      hotkey=TRUE;
                                      cwrite(wpath,"ON ",3);
                                   }
                                   CurXY(wpath, mouse_x, mouse_y);
                                   break;
               case MOUSE:     /* Mouse */
                                   CurXY(wpath,31,23);
                                   if (mouseon==TRUE) {
                                      mouseon=FALSE;
                                      cwrite(wpath,"OFF",3);
                                   } else {
                                      mouseon=TRUE;
                                      cwrite(wpath,"ON ",3);
                                   }
                                   CurXY(wpath, mouse_x, mouse_y);
                                   break;
               case MOUSERES:     /* MouseRes */
                                   CurXY(wpath,49,23);
                                   if (mouseres==0) {
                                      mouseres=1;
                                      cwrite(wpath,"HIGH",4);
                                   } else {
                                      mouseres=0;
                                      cwrite(wpath,"LOW ",4);
                                   }
                                   CurXY(wpath, mouse_x, mouse_y);
                                   break;
               case MOUSEPORT:     /* MousePort */
                                   CurXY(wpath,65,23);
                                   if (mouseport==0) {
                                      mouseport=1;
                                      cwrite(wpath,"RIGHT",5);
                                   } else {
                                      mouseport=0;
                                      cwrite(wpath,"LEFT ",5);
                                   }
                                   CurXY(wpath, mouse_x, mouse_y);
                                   break;
               case CHANGEDIR:     /* change the current directory */
                                   cp=EwFPick(wpath, "Change Folder", 
                                              DIRECTORY | OREAD | PREAD);
                                   CurXY(wpath,0,22);
                                   ErEOLine(wpath);
                                   cwrite(wpath, cp, strlen(cp));
                                   break;
            } /* end switch for buttons */
         } /* end click */
      } /* end else menu not selected */
   } /* end while */
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

