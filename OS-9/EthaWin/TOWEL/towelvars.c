/*
 * Menu/Options for TOWEL
 *
 * 1.01 5/30/94 - Change version # and update (C) notice.  Add LIST, MOVE.
 * 1.02 9/14/94 - Update version #
 * 1.03 3/10/95 - Update version # and (C), MM/1 ifdefs
 * 1.04 9/08/95 - Update version #
 * 1.05 9/27/95 - Updates for TCWIN and NOMOUSE (no ctrl chars in strings)
 */

#ifdef TCWIN
#include "ethawin.h"
#else
#include <ETHAWIN/ethawin.h>
#endif

/* pull-down menu data */

char *menu_names[] = { /* names of menus (menu-bar) */
#ifdef TCWIN
   "File","Disk","dIrectory","User"
#else
   "\x1f\x22F\x1f\x23ile",
   "\x1f\x22D\x1f\x23isk",
   "D\x1f\x22i\x1f\x23rectory",
   "\x1f\x22U\x1f\x23ser"
#endif
};

char *menu_keys = { /* hot-keys for pull-down menus */
   "FDIU"
};

int menu_stat[][MAXOPTS]= { /* status flags for up to 8 options, 3 menus */
   0,0,0,0,0,0,0,0, /* Bit 0=selectable? */
   0,0,0,0,0,0,0,0, /* Bit 1=flagged? */
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0
};

char *menu_chars[]= { /* hot-keys for up to 8 options, 3 menus */
   "CDILMRQ",
   "BCDFI",
   "CDM",
   "12345678",
   ""
};

char *menu_text[][MAXOPTS]= { /* menu text... */
#ifdef TCWIN
   "Copy","Delete","Info","List","Move","Rename","Quit",
#else
   "\x1f\x22C\x1f\x23opy", /* File menu */
   "\x1f\x22D\x1f\x23elete",
   "\x1f\x22I\x1f\x23nfo",
   "\x1f\x22L\x1f\x23ist", /* v1.01 */
   "\x1f\x22M\x1f\x23ove", /* v1.01 */
   "\x1f\x22R\x1f\x23ename",
   "\x1f\x22Q\x1f\x23uit",
#endif
   "",

#ifdef TCWIN
   "Backup","Cobbler","Dcheck","Format","Info",
#else
   "\x1f\x22B\x1f\x23ackup", /* Disk menu */
   "\x1f\x22C\x1f\x23obbler",
   "\x1f\x22D\x1f\x23check",
   "\x1f\x22F\x1f\x23ormat",
   "\x1f\x22I\x1f\x23nfo",
#endif
   "","","",

#ifdef TCWIN
   "Change","Deldir","Makedir",
#else
   "\x1f\x22C\x1f\x23hange", /* Directory menu */
   "\x1f\x22D\x1f\x23elDir",
   "\x1f\x22M\x1f\x23akeDir",
#endif
   "","","","","",

#ifdef TCWIN
   "1 user","2 user","3 user","4 user","5 user","6 user","7 user","8 user",
#else
   "\x1f\x221\x1f\x23 User", /* User menu */
   "\x1f\x222\x1f\x23 User",
   "\x1f\x223\x1f\x23 User",
   "\x1f\x224\x1f\x23 User",
   "\x1f\x225\x1f\x23 User",
   "\x1f\x226\x1f\x23 User",
   "\x1f\x227\x1f\x23 User",
   "\x1f\x228\x1f\x23 User", /* v1.01 add #8 and null terminate */
#endif

   ""
};

About(wpath)
int wpath;
{
   PopUp(wpath,"ABOUT",18,4,44,17);

   PrintAt(wpath,14,7,"P.O. Box 152442");
   PrintAt(wpath,13,6,"Sub-Etha Software");
   PrintAt(wpath,13,8,"Lufkin, TX  75915");
   PrintAt(wpath,20,5,"and");
   PrintAt(wpath,2,4,"Copyright (C) 1993-96 by Allen Huffman");
#ifndef OSK
   PrintAt(wpath,1,10,"Support the future of OS-9 and the CoCo.");
#else
#ifdef TCWIN
   PrintAt(wpath,1,10,"Support the future of desktop OS-9(000).");
#else
   PrintAt(wpath,1,10,"Support the future of OS-9 and the MM/1.");
#endif
#endif
   PrintAt(wpath,8,2,"A Useful (?) Disk Utility.");
   PrintAt(wpath,4,11,"Please do not pirate this program.");
   PrintAt(wpath,12,1,"/) Towel  V1.05 (\\");
   PrintAt(wpath,5,13,"Press Any Key or Click Button...");

   Wait(wpath);
   EndWin(wpath);
}

Help(wpath)
int wpath;
{
   PopUp(wpath,"HELP!",9,3,62,19);

   PrintAt(wpath,24,9,"Using Menus:");
#ifndef OSK
   PrintAt(wpath,1,7,"selected directory.  Also:  Shift Up/Down and Ctrl Up/Down.");
#else
   PrintAt(wpath,1,7,"selected directory.  Also:  Page Up/Down, Home and End.");
#endif
   PrintAt(wpath,1,10,"Press ALT+Underlined menu letter or Click on menu name to");
   PrintAt(wpath,1,6,"Press ENTER or Double Click on filename to change into the");
   PrintAt(wpath,1,11,"activate menu.  Use UP/DOWN arrow keys or mouse to choose");
   PrintAt(wpath,1,12,"an option.  Press ENTER or Click to select it.  Moving to");
   PrintAt(wpath,1,5,"highlighter.  Press SPACE or Click on filename to flag it.");
   PrintAt(wpath,1,4,"Use UP/DOWN arrows or Click on UP/DOWN buttons to move the");
#ifndef OSK
   PrintAt(wpath,1,13,"the bottom of the menu or pressing  BREAK  will abort it.");
#else
   PrintAt(wpath,1,13,"the bottom of the menu or pressing ESCAPE  will abort it");
#endif
   PrintAt(wpath,22,3,"Selecting Files:");
   PrintAt(wpath,1,14,"You may also press the Underlined  letter of an option to");
   PrintAt(wpath,5,1,"A few things to say on the subject of using Towel:");
   PrintAt(wpath,1,15,"to choose it.");

   Wait(wpath);
   Clear(wpath);

   PrintAt(wpath,23,9,"Directory Menu:");
   PrintAt(wpath,1,7,"These options will prompt you for the drive to operate on.");
   PrintAt(wpath,1,10,"These options will ask you to select a drive or type in a");
   PrintAt(wpath,25,6,"Disk Menu:");
   PrintAt(wpath,1,11,"complete PATH.");
   PrintAt(wpath,1,4,"These options will effect selected files in sequence.");
   PrintAt(wpath,25,13,"User Menu:");
   PrintAt(wpath,25,3,"File Menu:");
   PrintAt(wpath,1,14,"The commands these options execute are settable in the");
   PrintAt(wpath,3,1,"A few more things to say on the subject of using Towel:");
   PrintAt(wpath,1,15,"'Towel.cfg' file located in /DD/SYS/ETHAWIN.");

   Wait(wpath);
   EndWin(wpath);
}
