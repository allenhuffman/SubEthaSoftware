/*
 * megabanvars.c - pull-down menu data for MegaBanners
 */

#include <ETHAWIN/ethawin.h>

/* pull-down menu data */

char *menu_names[] = { /* names of menus (menu-bar) */
   "\x1f\x22F\x1f\x23ile",
   "\x1f\x22L\x1f\x23ayout",
   "\x1f\x22T\x1f\x23ext"
};

char *menu_keys = { /* hot-keys for pull-down menus */
   "FLT"
};

int menu_stat[][MAXOPTS]= { /* status flags for up to 8 options, 3 menus */
   0,1,1,1,0,0,0,0, /* Bit 0=selectable?, 1=flagged? */
   2,0,1,2,0,0,0,0,
   1,1,0,0,0,0,0,0
};

char *menu_chars[]= { /* hot-keys for up to 8 options, 3 menus */
   "NOSAPQ",
   "LP-C14U",
   "NCDEFV"
};

char *menu_text[][MAXOPTS]= { /* menu text... */
   "\x1f\x22N\x1f\x23ew",
   "\x1f\x22O\x1f\x23pen",
   "\x1f\x22S\x1f\x23ave",
   "Save \x1f\x22A\x1f\x23s...",
   "\x1f\x22P\x1f\x23rint",
   "\x1f\x22Q\x1f\x23uit",
   "","",

   "\x1f\x22L\x1f\x23andscape",
   "\x1f\x22P\x1f\x23ortrait",
   "----------",
   "\x1f\x22C\x1f\x23ontinuous",
   "8 1/2 x 1\x1f\x221\x1f\x23",
   "8 1/2 x 1\x1f\x224\x1f\x23",
   "C\x1f\x22u\x1f\x23stom",
   "",
   
   "Ce\x1f\x22n\x1f\x23ter",
   "\x1f\x22C\x1f\x23olor",
   "\x1f\x22D\x1f\x23elete",
   "\x1f\x22E\x1f\x23nter",
   "\x1f\x22F\x1f\x23ont",
   "\x1f\x22V\x1f\x23iew",	
   "",""
};

About(wpath)
int wpath;
{
   PopUp(wpath,"ABOUT",18,4,44,17);

   PrintAt(wpath,14,7,"P.O. Box 152442");
   PrintAt(wpath,13,6,"Sub-Etha Software");
   PrintAt(wpath,13,8,"Lufkin, TX  75915");
   PrintAt(wpath,20,5,"and");
   PrintAt(wpath,3,4,"Copyright (C) 1995 by Allen Huffman");
#ifndef OSK
   PrintAt(wpath,1,10,"Support the future of OS-9 and the CoCo.");
#else
   PrintAt(wpath,1,10,"Support the future of OS-9 and the MM/1.");
#endif
   PrintAt(wpath,9,2,"An MFont Banner Printer.");
   PrintAt(wpath,4,11,"Please do not pirate this program.");
   PrintAt(wpath,9,1,"/) MegaBanners  V1.01 (\\");
   PrintAt(wpath,5,13,"Press Any Key or Click Button...");

   Wait(wpath);
   EndWin(wpath);
}

Help(wpath)
int wpath;
{
   PopUp(wpath,"HELP",9,3,62,19);

   Wait(wpath);
   EndWin(wpath);
}

