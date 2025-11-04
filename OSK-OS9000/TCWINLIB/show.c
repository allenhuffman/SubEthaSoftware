/*
 * Internal TCWin routine:  Redisplay working screen
 */
 
#include <stdio.h>
#include "tclib.h"

extern int jmplen;
extern char brane_dead;

ShowAll() /* reset delta buffer then force full refresh */
{
	int i;
	char *main_ptr,*main_attr,*main_color,*main_end;
	struct WINDOW *tmp;

	for ( i = 0 ; i < main_win->h ; i ++ ) { /* scan each line... */
		main_ptr = main_win->textbuf + main_win->w*i; /* point to start of line */
		main_attr = main_win->attrbuf + main_win->w*i;
		main_color = main_win->colorbuf + main_win->w*i;

		main_end = main_ptr + main_win->w;

		setcursor(main_win->x , main_win->y + i); /* move cursor */
		while ( main_ptr < main_end ) { /* while still on line */
			ChangeAttr(*main_attr);
			ChangeColor(*main_color);
			putchar( *main_ptr );

			main_ptr++;
			main_attr++;
			main_color++;
		}
	}
	tmp = cur_win;
	if (tmp == NULL) tmp = main_win;
	setcursor( tmp->x + tmp->warea_x + tmp->cur_x ,
			tmp->y + tmp->warea_y + tmp->cur_y);
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
	char *temp_ptr;
	
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

	for ( i = 0 ; i < tmp->h ; i ++ ) { /* scan each line... */
		main_ptr = tmp->textbuf + tmp->w*i; /* point to start of line */
		delta_ptr = tmp->prev->textbuf + tmp->w*i; /* point to delta's line too */

		main_attr = tmp->attrbuf + tmp->w*i;
		delta_attr= tmp->prev->attrbuf + tmp->w*i;

		main_color= tmp->colorbuf + tmp->w*i;
		delta_color=tmp->prev->colorbuf + tmp->w*i;

		main_end = main_ptr + tmp->w;
		temp_ptr = 0;
		j=0;
		last_x = -2; /* a-lug */
		while ( main_ptr < main_end ) { /* while still on line */
			if ( ( *main_ptr != *delta_ptr ) ||
			     ( *main_attr != *delta_attr) ||
			     ( *main_color != *delta_color ) ) { /* change? */

				if ( j > last_x ) { /* moved? */
					if ( main_ptr-temp_ptr > jmplen ) { /* moved far? */
						setcursor( tmp->x + j , tmp->y + i);
						last_x = j;
					} else { /* otherwise we just wanna output the chars */
						while ( temp_ptr < main_ptr ) {
							putchar( *temp_ptr );
							temp_ptr++;
						}
					}
				}
				ChangeAttr(*main_attr);
				ChangeColor(*main_color);
				putchar( *main_ptr );
				last_x++;
				temp_ptr = main_ptr+1;
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
	tmp = cur_win;
	if (tmp == NULL) tmp = main_win;
	setcursor( tmp->x + tmp->warea_x + tmp->cur_x ,
			tmp->y + tmp->warea_y + tmp->cur_y);
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
		memcpy( main_win->colorbuf + j , tmp->colorbuf + k , tmp->w );
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

ChangeAttr(attr) /* slightly less brane dead */
char attr;
{
	char changes, turnon, onmask;

	changes = last_attr ^ attr; /* mask to show only those that have changed */
	turnon = 0; /* start off with everything marked as clear */
	onmask = last_attr; /* what's currently on */

	if (changes==0) return 0;
	if (changes & BLINK) { /* if underline has changed */
		if (attr & BLINK) { /* if turning it on */
			turnon = turnon | BLINK; /* mark it on */
		} else {
			blinkoff();
			if (brane_dead) {
				onmask = onmask & ~BLINK;
			}
		}
	}
	if (changes & REVERSE) { /* if underline has changed */
		if (attr & REVERSE) { /* turning it on? */
			turnon = turnon | REVERSE; /* flag to turn it on */
		} else {
			reverseoff();
			if (brane_dead) {
				onmask = onmask & ~REVERSE;
			}
		}
	}
	if (changes & UNDERLINE) { /* if underline has changed */
		if (attr & UNDERLINE) { /* turning it on? */
			turnon = turnon | UNDERLINE;
		} else {
			underlineoff();
			if (brane_dead) {
				onmask = onmask & ~UNDERLINE;
			}
		}
	}
	if (changes & BOLD) { /* if underline has changed */
		if (attr & BOLD) { /* turning it on? */
			turnon = turnon | BOLD;
		} else {
			boldoff();
			if (brane_dead) {
				onmask = onmask & ~BOLD;
			}
		}
	}
	/* here we have a 'turnon' reflecting exactly what to turn on */
	turnon = turnon | onmask; /* things to re-on for brane dead terminals */
	if ( (brane_dead) && (onmask!=last_attr) ) {
		foreground(last_color>>4);
		background(last_color & 15);
	}
	if (turnon & BLINK) blinkon(); /* now turn on what needs to be ... */
	if (turnon & REVERSE) reverseon();
	if (turnon & UNDERLINE) underlineon();
	if (turnon & BOLD) boldon();

	last_attr = attr; /* set flag telling what the last color code sent was */
}

ChangeColor(color)
char color;
{
	if ( (color & 0xF0 ) != (last_color & 0xF0) ) {
		foreground( color>>4 );
	}
	if ( (color & 15) != (last_color & 15) ) { 
		background( color & 15 );
	}
	last_color = color; /* ditto... */
}
