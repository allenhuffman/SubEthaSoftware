/*
 * Misc. extra CGFX crap to be added later (or not)
 */

#include <stdio.h>
#include "tclib.h"
 
Bell() /* ding */
{
}

Border(wpath) /* not supported */
int wpath;
{
}

CurDwn(wpath) /* cursor down */
int wpath;
{
	struct WINDOW *tmp;
	int i;
	
	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	i = tmp->cur_x; /* save current vertical position */
	Text(wpath,"\n"); /* send a CR (next line) */
	tmp->cur_x = i; /* restore */
}

CurHome(wpath) /* move cursor to home position */
int wpath;
{
	struct WINDOW *tmp;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	tmp->cur_x = 0; /* home cursor */
	tmp->cur_y = 0;
}

CurLft(wpath) /* move cursor left */
int wpath;
{
	struct WINDOW *tmp;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */
	
	tmp->cur_x--; /* move cursor left */
	if (tmp->cur_x < 0) tmp->cur_x = tmp->warea_w; /* bounds check */
}

CurOn(wpath) /* not supported */
int wpath;
{
}

CurOff(wpath) /* not supported */
int wpath;
{
}

CurRgt(wpath) /* cursor right */
int wpath;
{
	struct WINDOW *tmp;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */
	
	tmp->cur_x++; /* move cursor left */
	if (tmp->cur_x >= tmp->warea_w) tmp->cur_x = 0; /* bounds check */
}

CrRtn(wpath) /* carraige return with no line feed */
int wpath;
{
	struct WINDOW *tmp;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	tmp->cur_x = 0; /* c/r with no line feed */
}

CurUp(wpath) /* move cursor up */
int wpath;
{
	struct WINDOW *tmp;
	
	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR);
	
	tmp->cur_y--;
	if (tmp->cur_y < 0) tmp->cur_y = tmp->warea_h;
}

DefColr(wpath) /* not supported...maybe not needed? */
int wpath;
{
}

DelLine(wpath) /* delete line cursor is on, moving rest up */
int wpath;
{
	struct WINDOW *tmp;
	int i,j;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	for ( i = tmp->cur_y ; i < tmp->warea_h-1 ; i++ ) {
        j = tmp->warea_x + ((tmp->warea_y + i) * tmp->w);
		memcpy( tmp->textbuf + j , tmp->textbuf + j + tmp->w , tmp->warea_w );
		memcpy( tmp->attrbuf + j , tmp->attrbuf + j + tmp->w , tmp->warea_w );
		memcpy( tmp->colorbuf + j , tmp->colorbuf + j + tmp->w , tmp->warea_w );
	}
	j = j + tmp->w; /* move pointer to next line */
	memset(tmp->textbuf + j , FILLCHAR , tmp->warea_w );
	memset(tmp->attrbuf + j , tmp->attr , tmp->warea_w );
	memset(tmp->colorbuf + j , tmp->color , tmp->warea_w );
}

DWProtSw(wpath, toggle) /* not inplimented */
int wpath,toggle;
{
}

ErEOLine(wpath) /* blank out from cursor to end of line */
int wpath;
{
	struct WINDOW *tmp;
	int j;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	j = tmp->w * (tmp->cur_y + tmp->warea_y) + tmp->warea_x + tmp->cur_x;
	memset(tmp->textbuf + j , FILLCHAR , tmp->warea_w - tmp->cur_x ); /* zero it out */
	memset(tmp->attrbuf + j , tmp->attr , tmp->warea_w - tmp->cur_x);
	memset(tmp->colorbuf + j , tmp->color , tmp->warea_w - tmp->cur_x);
}

ErEOScrn(wpath) /* erase to end of screen */
int wpath;
{
	struct WINDOW *tmp;
	int i,j;
	
	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	for ( i = tmp->warea_h-1 ; i >= tmp->cur_y ; i-- ) {
		j = tmp->warea_x + ((tmp->warea_y + i) * tmp->w);
		memset(tmp->textbuf + j , FILLCHAR , tmp->warea_w ); /* zero it out */
		memset(tmp->attrbuf + j , tmp->attr , tmp->warea_w );
		memset(tmp->colorbuf + j , tmp->color , tmp->warea_w );
	}
}

ErLine(wpath) /* blank out line cursor is on */
int wpath;
{
	struct WINDOW *tmp;
	int j;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	j = tmp->w * (tmp->cur_y + tmp->warea_y) + tmp->warea_x;
	memset(tmp->textbuf + j , FILLCHAR , tmp->warea_w ); /* zero it out */
	memset(tmp->attrbuf + j , tmp->attr , tmp->warea_w );
	memset(tmp->colorbuf + j , tmp->color , tmp->warea_w );
}

InsLin(wpath) /* insert blank line at current cursor position */
int wpath;
{
    struct WINDOW *tmp;
	int i,j;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	for ( i = tmp->warea_h-2 ; i >= tmp->cur_y ; i-- ) {
        j = tmp->warea_x + ((tmp->warea_y + i) * tmp->w);
		memcpy( tmp->textbuf + j + tmp->w, tmp->textbuf + j , tmp->warea_w );
		memcpy( tmp->attrbuf + j + tmp->w, tmp->attrbuf + j  , tmp->warea_w );
		memcpy( tmp->colorbuf + j + tmp->w, tmp->colorbuf + j , tmp->warea_w );
	}

	memset(tmp->textbuf + j , FILLCHAR , tmp->warea_w );
	memset(tmp->attrbuf + j , tmp->attr , tmp->warea_w );
	memset(tmp->colorbuf + j , tmp->color , tmp->warea_w );
}

Palette(wpath,palette,color) /* not used, yet */
int wpath,palette,color;
{
}
