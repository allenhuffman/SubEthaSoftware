/*
 * LaborVars.c - Menu/Options for Labor.c
 */

#include <ETHAWIN/ethawin.h>

/* pull-down menu data */

char *menu_names[] = { /* names of menus (menu-bar) */
   "\x1f\x22F\x1f\x23ile",
   "\x1f\x22E\x1f\x23dit",
   "\x1f\x22D\x1f\x23ay"
};

char *menu_keys = { /* hot-keys for pull-down menus */
   "FED"
};

int menu_stat[][MAXOPTS]= { /* status flags for up to 8 options, 3 menus */
   1,0,1,1,1,0,0,0, /* Bit 0=selectable? */
   0,0,0,0,0,0,0,0, /* Bit 1=flagged? */
   0,0,0,0,0,0,0,0
};

char *menu_chars[]= { /* hot-keys for up to 8 options, 3 menus */
   "NOSAPQ",
   "ADES",
   "SMTWHFA"
};

char *menu_text[][MAXOPTS]= { /* menu text... */
   "\x1f\x22N\x1f\x23ew",
   "\x1f\x22O\x1f\x23pen",
   "\x1f\x22S\x1f\x23ave",
   "Save \x1f\x22A\x1f\x23s...",
   "\x1f\x22P\x1f\x23rint",
   "\x1f\x22Q\x1f\x23uit",
   "","",

   "\x1f\x22A\x1f\x23dd",
   "\x1f\x22D\x1f\x23elete",
   "\x1f\x22E\x1f\x23dit",
   "\x1f\x22S\x1f\x23ystem",
   "","","","",

   "\x1f\x22S\x1f\x23unday",
   "\x1f\x22M\x1f\x23onday",
   "\x1f\x22T\x1f\x23uesday",
   "\x1f\x22W\x1f\x23ednesday",
   "T\x1f\x22h\x1f\x23ursday",
   "\x1f\x22F\x1f\x23riday",
   "S\x1f\x22a\x1f\x23turday",
   ""
};

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

