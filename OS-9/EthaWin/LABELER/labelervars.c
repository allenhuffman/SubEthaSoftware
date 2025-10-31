/*
 * Menu/Options for LABELER
 *
 */

#include "ethawin.h"

/* pull-down menu data */

char *menu_names[] = { /* names of menus (menu-bar) */
   "\x1f\x22F\x1f\x23ile",
   "\x1f\x22M\x1f\x23isc"
};

char *menu_keys = { /* hot-keys for pull-down menus */
   "FM"
};

int menu_stat[][MAXOPTS]= { /* status flags for up to 8 options, 3 menus */
   0,0,0,0,0,0,0,0, /* Bit 0=selectable? */
   0,0,0,0,0,0,0,0  /* Bit 1=flagged? */
};

char *menu_chars[]= { /* hot-keys for up to 8 options, 3 menus */
   "OSAQ",
   "ABCD"
};

char *menu_text[][MAXOPTS]= { /* menu text... */
   "\x1f\x22O\x1f\x23pen", /* File menu */
   "\x1f\x22S\x1f\x23ave",
   "Save \x1f\x22A\x1f\x23s...",
   "\x1f\x22Q\x1f\x23uit",
   "","","","",

   "\x1f\x22A\x1f\x23 aaa", /* Misc menu */
   "\x1f\x22B\x1f\x23 bbb",
   "\x1f\x22C\x1f\x23 ccc",
   "\x1f\x22D\x1f\x23 ddd",
   "","","",""
};

About(wpath)
int wpath;
{
   PopUp(wpath,"ABOUT",18,4,44,17);

   PrintAt(wpath,14,7,"P.O. Box 152442");
   PrintAt(wpath,13,6,"Sub-Etha Software");
   PrintAt(wpath,13,8,"Lufkin, TX  75915");
   PrintAt(wpath,20,5,"and");
   PrintAt(wpath,2,4,"Copyright (C) 1993 by Allen C. Huffman");
   PrintAt(wpath,1,10,"Support the future of OS-9 and the CoCo.");
   PrintAt(wpath,8,2,"A Labeler Program ... Duh!");
   PrintAt(wpath,4,11,"Please do not pirate this program.");
   PrintAt(wpath,11,1,"/) Labeler  V0.00 (\\");
   PrintAt(wpath,5,13,"Press Any Key or Click Button...");

   Wait(wpath);
   EndWin(wpath);
}

Help(wpath)
int wpath;
{
   PopUp(wpath,"HELP!",9,3,62,19);

   Wait(wpath);
   EndWin(wpath);
}
