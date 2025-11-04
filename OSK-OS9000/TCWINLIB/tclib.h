#include "tcwin.h"

/* Global variables potentially referenced by TCWin routines... */

extern struct WINDOW *cur_win; 	/* currently used Window or NULL if none */
extern struct WINDOW *main_win; /* main (background) Window pointer */
extern char win_update;
extern char last_attr,
			last_color; 		/* global screen attributes (for reference) */

/* Some TCWin defines... */

#define FILLCHAR 32
#define ALL_WIN TRUE
#define CUR_WIN FALSE

/* Attribute byte (8 1-bit on/off flags):
 *
 * 	bit	val		meaning
 *	0	1		blinking
 *	1	2		reverse video
 *	2	4		underline
 *	3	8		bold (standout)
 *	4	16
 *	5	32
 *	6	64
 *	7	128		reserved...
 */		

#define BLINK 		1<<0
#define REVERSE 	1<<1
#define UNDERLINE	1<<2
#define BOLD		1<<3

/* Color byte (2 4-bit nibbles)
 *
 * Format is:  $FB = where F is foreground and B is background
 */

/* Some function prototypes (to keep Ultra-C happy) */

struct WINDOW *PathToWin();

