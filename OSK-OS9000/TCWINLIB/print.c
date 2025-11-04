/*
 * User callable TCWin routine:  Text (text output)
 */
 
#include <stdio.h>
#include "tclib.h"

int TextAt(wpath,x,y,text)
int wpath,x,y;
char *text;
{
	struct WINDOW *tmp;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	tmp->cur_x = x;
	tmp->cur_y = y;
	return( Text(wpath,text) );
}

int Text(wpath,text) /* print and update cursor, window relative */
int wpath;
char *text;
{
	struct WINDOW *tmp;
	int i;
	char ch;
	char *line,*scan,*start;

	tmp = PathToWin(wpath);
	if (tmp == NULL) return(ERROR); /* unable to locate window */

	line = (char *) malloc(strlen(text)+1);
	if (line == NULL) return(ERROR);
	strcpy(line,text);

	for ( start=scan=line ; *scan != '\0' ; scan++ ) {
		if ( (scan-start) >= (tmp->warea_w - tmp->cur_x) ) {
			ch = *scan;
			*scan = 0;
			Output(tmp, start, TRUE);
			*scan = ch;
			start = scan;
		}
		if (*scan == '\n') {
			*scan = 0;
			Output(tmp, start, TRUE);
			start = scan + 1;
		}
		if (*scan < ' ') { /* filter out non printable characters */
			*scan = ' ';
		}
	}
	if (*start != '\0' ) {
		Output(tmp, start, FALSE);
	}

	free(line);

	if (wpath == cur_win->number) {
		ShowChanges(CUR_WIN); /* update screen */
	} else {
		ShowChanges(ALL_WIN);
	}
}

Output(tmp, text, cr) /* output string to window buffer */
struct WINDOW *tmp;
char *text;
char cr; /* TRUE == append carraige return */
{
	int i,len;
	
	i = (tmp->warea_x + tmp->cur_x) + (tmp->warea_y + tmp->cur_y) * tmp->w;
	len = strlen(text);

	memcpy( tmp->textbuf + i , text , len );
	memset( tmp->attrbuf + i , tmp->attr , len ) ;
	memset( tmp->colorbuf + i , tmp->color , len );

	if ( cr == TRUE ) {
		tmp->cur_x = 0;
		if ( tmp->cur_y < tmp->warea_h-1 ) {
			tmp->cur_y++;
		} else {
			ScrollWindow(tmp);
		}
	} else {
		tmp->cur_x = tmp->cur_x + len;
	}
}

ScrollWindow(tmp)
struct WINDOW *tmp;
{
	int i,j;

	for ( i = 0 ; i < tmp->warea_h-1 ; i++ ) {
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
