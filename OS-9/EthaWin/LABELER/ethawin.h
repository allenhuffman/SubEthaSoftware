/*
 * EthaWin.h - some defines for use with any EthaWin program.
 *
 * 1.00 9/18/93 - Initial version
 *
 */

/* some logic defines */

#define TRUE  1
#define FALSE 0

/* some defines for reading the keyboard */

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
#define ENTER 13
#define QUIT   5

/* some defines used by the windowing system */

#define CHECKFLAG 0xae

#define FORE 0
#define BACK 1
#define BORDER 2
#define HIGHLIGHTED 3
#define DESELECTED 4
#define MBFORE 5
#define MBBACK 6
#define TTFORE 7
#define TTBACK 8
#define PUFORE 9
#define PUBACK 10
#define PUSHAD 11
#define PUBORD 12
#define PDFORE 13
#define PDBACK 14
#define PDSHAD 15
#define COLORS 16 /* 15 customizeable color thingies */

#define MAXOPTS 8 /* maximum menu options */
/* #define MAXMENUS 3 */  /* number of pull down menus */

/* some defines for the mouse signal */

#ifdef OSK
#define MOUSESIG 444
#else
#define MOUSESIG 10 /* mouse click signal */
#endif
