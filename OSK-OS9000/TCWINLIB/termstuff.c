/*
 * Termcap setup configuration stuff.
 */

#include <termcap.h>
#include <stdio.h>
#include "tcwin.h"

/* Termcap specific globals */

char *termPtr,*cm,*cl,*ti,*te,*ce,*so,*se,*us,*ue;
char *fc[16],*bc[16]; /* foreground and background */
char *termType;
char termbuff[2048];
char capbuff[2048];
char temp[256];
char *BC, *UP, PC_;
short ospeed;
int jmplen;
char brane_dead = FALSE;

/* Global max size of termcap screen we are using */

int termsizeX,termsizeY;

void outchar();

setupTermcap()
{
	int i;
	char *tempPtr;
	char text[5];

	termPtr = capbuff;
	if ((termType = (char*)getenv("TERM")) == NULL) {
		appExit("TERM environment variable must be set.\n");
	}

	if (tgetent(termbuff, termType) == 0) {
		appExit("Cannot find your terminal capability.\n");
	}

	/* these codes wil be retreived if available... */

	cl = tgetstr("cl", &termPtr); /* clear */
	ce = tgetstr("ce", &termPtr); /* clear to end? */

	if ((cm = tgetstr("cm", &termPtr)) == NULL) { /* cursor movement */
		appExit("'cm' capability needed\n");
	}
	jmplen = strlen(tgoto(cm , 1 , 1));

/* screen text attributes */

	so = tgetstr("so", &termPtr); /* stand out (rev) */
	se = tgetstr("se", &termPtr); /* stand end (revoff) */

	us = tgetstr("us", &termPtr); /* underline */
	ue = tgetstr("ue", &termPtr); /* underline end */

/* do check for brane dead emulations (like ANSI) */

	if (se!=NULL && ue!=NULL) {
		if (strcmp(se,ue)==0) brane_dead = TRUE;
	}

/* Note: library can support 16 colors but we'll define only 8*/

	for ( i=0; i<8; i++ ) {
		sprintf(text,"c%d",i);
		fc[i] = tgetstr(text , &termPtr);
		fc[i+8]=fc[i];
		sprintf(text,"d%d",i);
		bc[i] = bc[i+8] = tgetstr(text , &termPtr);
		bc[i+8]=bc[i];
	}

	/* not vital, but useful capabilities if available */

	if ((ti = tgetstr("ti", &termPtr)) != NULL) { /* save main screen */
		tputs(ti, 1, outchar);	/* save screen */
		fflush(stdout);
	} else { /* no screen save? */
		clearscreen();
	}		
	te = tgetstr("te", &termPtr); /* restore screen */

	termPtr = termbuff;

	/* set screen size variables */

	if ((tempPtr = (char*)getenv("LINES")) != NULL) { /* check environ first */
		termsizeY = atoi(tempPtr);
	} else { /* else read it from termcap */
		termsizeY = tgetnum("li");
	}
	if ((tempPtr = (char*)getenv("COLUMNS")) != NULL) { /* check environ first */
		termsizeX = atoi(tempPtr);
	} else { /* else read from termcap entry */
		termsizeX = tgetnum("co");
	}
}

destructTermcap()
{
	reverseoff(); /* make sure all attribute codes are shut off */
	underlineoff();
	blinkoff();
	if (te != NULL) {
		tputs(te, 1, outchar); /* restore screen if we can, */
	} else {
		tputs(cl, 1, outchar); /* else just clear and exit */
	}
	fflush(stdout);
}

appExit(text)
char *text;
{
	fputs(text, stdout);
	exit(0);
}

void outchar( c ) /* output one character */
{
	putchar(c);
}

clearscreen()
{
	tputs(cl, 1, outchar);
    fflush(stdout);
}

getXY( x , y ) /* return terminal max screen size */
int *x, *y;
{
	*x = termsizeX;
	*y = termsizeY;
}

setcursor( x , y ) /* raw cursor position */
int x,y;
{
	tputs(tgoto(cm , x , y), 1, outchar);
}

foreground(c) /* user must ensure not to call color higher than max!!! */
{
	tputs( fc[c & 15], 1, outchar);
}

background(c)
{
	tputs( bc[c & 15], 1, outchar);
}

underlineon()
{
	tputs( us, 1, outchar);
}

underlineoff()
{
	tputs( ue, 1, outchar);
}

boldon()
{
	tputs( so, 1, outchar);
}

boldoff()
{
	tputs( se, 1, outchar);
}

int reverseon()
{
	tputs(so, 1, outchar);
}

int reverseoff()
{
	tputs(se, 1, outchar);
}

int blinkon()
{}

int blinkoff()
{}
