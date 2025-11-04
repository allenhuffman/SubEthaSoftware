/*
 * Internal TCWin routine:  Redisplay working screen
 */
 
#include <stdio.h>
#include "tclib.h"

ShowAll() /* reset delta buffer then force full refresh */
{
	int i;
	char *main_ptr,*main_attr,*main_color,*main_end;

	for ( i = 0 ; i < main_win->h ; i ++ ) { /* scan each line... */
		main_ptr = main_win->textbuf + main_win->w*i; /* point to start of line */
		main_attr = main_win->attrbuf + main_win->w*i;
		main_color= main_win->colorbuf + main_win->w*i;

		main_end = main_ptr + main_win->w;

		setcursor(main_win->x , main_win->y + i); /* move cursor */
		while ( main_ptr < main_end ) { /* while still on line */
			if ( *main_attr != last_attr ) {
				ChangeAttr(*main_attr);
				last_attr = *main_attr;
			}
			if ( *main_color != last_color ) {
				ChangeColor(*main_color);
				last_color = *main_color;
			}
			putchar( *main_ptr );

			main_ptr++;
			main_attr++;
			main_color++;
		}
	}
	fflush(stdout);
	DupMainBuffer();
}

/* SPAM  */

ShowChanges(mode) /* show any changes to main buffer then update */
char mode; /* TRUE = update all, FALSE = update only current window */
{
	int i,j;
	char *main_ptr,*delta_ptr,*main_end;
	char *main_attr,*delta_attr;
	char *main_color,*delta_color;
	struct WINDOW *tmp;
	int last_x; /* chug... */
	
	if (win_update == FALSE) return(ERROR);

/* this is an attempt to speed it up by not rescanning the entire screen if
 * we are only using the top window (ie, not printing to a hidden window).
 * To eliminate, take out the condition check and variable, and always do
 * the AllWinToBuf() at the start...
 */
	
	if (mode==ALL_WIN) {
		AllWinToBuf(); /* update main display buffer first */
		tmp = main_win;
	} else {
		tmp = cur_win;
	}

	for ( i = 0 ; i < main_win->h ; i ++ ) { /* scan each line... */
		main_ptr = tmp->textbuf + tmp->w*i; /* point to start of line */
		delta_ptr = tmp->prev->textbuf + tmp->w*i; /* point to delta's line too */

		main_attr = tmp->attrbuf + tmp->w*i;
		delta_attr= tmp->prev->attrbuf + tmp->w*i;

		main_color= tmp->colorbuf + tmp->w*i;
		delta_color=tmp->prev->colorbuf + tmp->w*i;

		main_end = main_ptr + tmp->w;
		j=0;
		last_x = -2; /* a-lug */
		while ( main_ptr < main_end ) { /* while still on line */
			if ( ( *main_ptr != *delta_ptr ) ||
			     ( *main_attr != *delta_attr) ||
			     ( *main_color != *delta_color ) ) { /* change? */

				if (j > last_x) { /* moved? */
					setcursor( tmp->x + j , tmp->y + i);
					last_x = j;
				}
				
				if ( *main_attr != last_attr ) {
					ChangeAttr(*main_attr);
					last_attr = *main_attr;
				}

				if ( *main_color != last_color ) {
					ChangeColor(*main_color);
					last_color = *main_color;
				}
				putchar( *main_ptr );
				last_x++;
			} /* otherwise just keep scanning... */
			main_ptr++;
			delta_ptr++;
			main_attr++;
			delta_attr++;
			main_color++;
			delta_color++;
			j++;
		}
	}
	fflush(stdout);
	DupMainBuffer();
}

int AllWinToBuf()
{
	struct WINDOW *tmp;

	if (cur_win==NULL) return(ERROR);

	tmp=cur_win;	
	do {
		tmp = tmp->next;
		WinToBuf(tmp);
	} while (tmp != cur_win);
}

WinToBuf(tmp)
struct WINDOW *tmp;
{
	int i,j,k;

	for ( i=0 ; i < tmp->h ; i++ ) {
		j = tmp->x + ((tmp->y+i) * main_win->w);
		k = tmp->w * i;
		memcpy( main_win->textbuf + j , tmp->textbuf + k , tmp->w );
		memcpy( main_win->attrbuf + j , tmp->attrbuf + k , tmp->w );
		memcpy( main_win->colorbuf + j , tmp->colorbuf + k , tmp->w );							/* copy SIZE */
	}
}

DupMainBuffer()
{	
	int size;

	size = main_win->w * main_win->h;

	memcpy(main_win->prev->textbuf , main_win->textbuf , size);
	memcpy(main_win->prev->attrbuf , main_win->attrbuf , size);
	memcpy(main_win->prev->colorbuf , main_win->colorbuf , size);
}

ChangeAttr(attr) /* brane dead */
char attr;
{
	if (attr & UNDERLINE) {
		underlineon();
	} else {
		underlineoff();
	}
	if (attr & REVERSE) {
		reverseon();
	} else {
		reverseoff();
	}
}

ChangeColor(color)
char color;
{
	foreground( color>>4 );
	background( color & 15 );
}
