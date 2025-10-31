/*
 * Menu stuff for ADFEdit.c
 */

#include "ethawin.h"

/* pull-down menu data */

char *menu_names[] = { /* names of menus (menu-bar) */
   "\x1f\x22F\x1f\x23ile"
};

char *menu_keys = { /* hot-keys for pull-down menus */
   "F"
};

int menu_stat[][MAXOPTS]= { /* status flags for up to 8 options, 3 menus */
   0,0,0,0,0,0,0,0  /* Bit 0=selectable? */
                    /* Bit 1=flagged? */
                    /* Bits 2-7=reserved for future or user use... */
};

char *menu_chars[]= { /* hot-keys for up to 8 options, 3 menus */
   "ENQ"
};

char *menu_text[][MAXOPTS]= { /* menu text... */
   "\x1f\x22E\x1f\x23dit",
   "\x1f\x22N\x1f\x23ew",
   "\x1f\x22Q\x1f\x23uit",
   "","","","",""
};

About(wpath)
int wpath;
{
   PopUp(wpath,"ABOUT",18,4,44,17);

   PrintAt(wpath,14,7,"P.O. Box 152442");
   PrintAt(wpath,13,6,"Sub-Etha Software");
   PrintAt(wpath,13,8,"Lufkin, TX  75915");
   PrintAt(wpath,20,5,"and");
   PrintAt(wpath,3,4,"Copyright (C) 1994  by Allen Huffman");
   PrintAt(wpath,1,10,"MAY BE FREELY DISTRIBUTED AS LONG AS THE");
   PrintAt(wpath,8,2,"A Useful (?) Disk Utility.");
   PrintAt(wpath,5,11,"COMPLETE ARCHIVE REMAINS INTACT.");
   PrintAt(wpath,11,1,"/) ADFedit  V0.00 (\\");
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
