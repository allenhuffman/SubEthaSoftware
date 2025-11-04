/* error - version 1.0
 *    WB - Chris Hyde, 2/95
 *
 * This function pops up a titled 40 x 4 overlay window and displays an error
 * message followed by the error code.  The error message is given by s1 and
 * s2 and the error code is given by code.
 *
 */

#include <stdinc.h>

error(path, title, s1, s2, code)
int  path;
char *title,
     *s1, *s2;
int  code;
{
 char t[4];

/* use a length of 6 to get a 4 line window */
 PopUp(path, title, 19, 9, 42, 6);
 newline(path);
 write(path, " ", 1);
 write(path, s1, strlen(s1));
 write(path, s2, strlen(s2));
 newline(path);
 write(path, " Error # ", 9);
 write(path, itoa(code, t), 3);
 Bell(path);
 Wait(path);
 EndWin(path);
} /* end error() */

/* end error.c */
