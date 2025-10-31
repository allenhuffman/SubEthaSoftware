/*
 * Menu/Options for EthaDemo ... note that all the "\x1f\x22" stuff is just
 * control codes to underline a letter.  It looks messy, but works great.
 * Hopefully this should be self-explanitory.  This file and ethawin.c are
 * compiled and linked with the ethawin.l library...and it's just that easy.
 */

#include "ethawin.h"

/* pull-down menu data */

char *menu_names[] = { /* names of menus (menu-bar) */
   "\x1f\x22F\x1f\x23ile",
   "\x1f\x22E\x1f\x23dit",
   "\x1f\x22T\x1f\x23ext"
};

char *menu_keys = { /* hot-keys for pull-down menus */
   "FET"
};

int menu_stat[][MAXOPTS]= { /* status flags for up to 8 options, 3 menus */
   0,0,1,0,0,0,0,0, /* Bit 0=selectable? */
   1,0,0,0,0,0,0,0, /* Bit 1=flagged? */
   0,0,0,0,0,0,0,0  /* Bits 2-7=reserved for future or user use... */
};

char *menu_chars[]= { /* hot-keys for up to 8 options, 3 menus */
   "NOSAPQ",
   "UCOPD",
   "BIU"
};

char *menu_text[][MAXOPTS]= { /* menu text... */
   "\x1f\x22N\x1f\x23ew",
   "\x1f\x22O\x1f\x23pen",
   "\x1f\x22S\x1f\x23ave",
   "Save \x1f\x22A\x1f\x23s...",
   "\x1f\x22P\x1f\x23rint",
   "\x1f\x22Q\x1f\x23uit",
   "","",

   "\x1f\x22U\x1f\x23ndo",
   "\x1f\x22C\x1f\x23ut",
   "C\x1f\x22o\x1f\x23py",
   "\x1f\x22P\x1f\x23aste",
   "\x1f\x22D\x1f\x23elete",
   "","","",

   "\x1f\x22B\x1f\x23old",
   "\x1f\x22I\x1f\x23talics",
   "\x1f\x22U\x1f\x23nderline",
   "","","","",""
};
