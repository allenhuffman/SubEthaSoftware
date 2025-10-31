/*
 * EthaWin.h - some defines for use with any EthaWin program.
 *
 * 1.00 9/18/93 - Initial version
 * 1.01 5/30/94 - Added STDIN, STDOUT, and STERR
 * 1.02 3/29/95 - Added Ctrl/Shift UP/DOWN, SPACE, ENTER, and NULL
 */
 
/* some logic defines */
 
#define TRUE  1
#define FALSE 0
#define ERROR -1

/* some defines for reading the keyboard */

#ifdef OSK
#define UP   16
#define DOWN 14
#define LEFT  2
#define RIGHT 6
#define SHIFTUP 0x1a /* Page Up on MM/1 */
#define SHIFTDOWN 0x16 /* Page Down */
#define CTRLUP 1 /* Home */
#define CTRLDOWN 5 /* End */
#define QUIT 27
#define SCREENLEN 26
#else
#define UP   12
#define DOWN 10
#define LEFT  8
#define RIGHT 9
#define SHIFTUP 28 /* CoCo keyboard definitions */
#define SHIFTDOWN 26
#define CTRLUP 19
#define CTRLDOWN 18
#define QUIT 5
#define SCREENLEN 24
#endif
#define ENTER 13
#define SPACE 32

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

/* some defines for the mouse signal */

#ifdef OSK
#define MOUSESIG 444
#else
#define MOUSESIG 10 /* mouse click signal */
#endif
#define STDIN 0
#define STDOUT 1
#define STDERR 2
