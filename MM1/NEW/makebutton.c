#include <stdinc.h>
#include <ethawin.h>

extern int mouse_x, mouse_y;

/* makeButton - version 1.0
 *         WB - Chris Hyde, 2/95
 *
 * This function loads information about a button into the button list pasted
 * in and displays the button on wpath.  All new button are active.  If the 
 * name of the button is longer than BUTTONNAMELEN it will be truncated.
 *
 */

int makeButton(wpath, blist, bnum, name, cpx, cpy)
int wpath; /* window path */
struct BUTTON *blist; /* button list to add new button to */
int bnum; /* location of new button in list */
char *name; /* name of new button */
int cpx, /* x position of left edge of button */
    cpy; /* y position of button */
{
 blist+=bnum;
 strncpy(blist->name, name, BUTTONNAMELEN);
 blist->name[BUTTONNAMELEN]='\0';
 blist->len=strlen(blist->name);
 blist->posx=cpx;
 blist->posy=cpy;
 blist->active=TRUE;

 RevOn(wpath);
 CurXY(wpath, cpx, cpy);
 write(wpath, blist->name, blist->len);
 RevOff(wpath);
 CurXY(wpath, mouse_x, mouse_y);
 return(1);
} /* end makeButton() */

/* checkButton - version 1.0
 *          WB - Chris Hyde, 2/95
 *
 * This function checks the list of buttons passed in to see if the mouse was
 * pointing to one when the mouse button was clicked.  If it was and the
 * button is active the number of the button is returned, otherwise -1 is
 * returned.
 *
 */

int checkButton(blist, bnum)
struct BUTTON *blist;
int bnum;
{
 int i;

 for (i=0; i<bnum; i++, blist++)
 {
    if ((blist->posy==mouse_y) &&
        ((mouse_x>=blist->posx) && (mouse_x<=blist->posx+blist->len)) &&
        (blist->active==TRUE))
       return(i);
 }
 return(-1); /* no button pressed */
} /* end checkButton() */

/* activateButton - Version 1.0
 *             WB - Chris Hyde, 2/95
 *
 * This function actives the button given by bnum in blist.  It does this
 * by setting the active flag of the button and by writing it out to wpath
 * in reverse video.
 *
 */

activateButton(wpath, blist, bnum)
int wpath; /* window path */
struct BUTTON *blist; /* button list */
int bnum; /* position of button in list */
{
 blist+=bnum;
 blist->active=TRUE;
 RevOn(wpath);
 CurXY(wpath, blist->posx, blist->posy);
 write(wpath, blist->name, blist->len);
 RevOff(wpath);
 CurXY(wpath, mouse_x, mouse_y);
} /* end activateButton() */

/* deactivateButton - version 1.0
 *               WB - Chris Hyde, 2/95
 *
 * This function deactivates the button given by bnum in blist.  This is done
 * by setting the active flage to FALSE and writting the name of the button
 * in normal video.
 *
 */

deactivateButton(wpath, blist, bnum)
int wpath; /* window path */
struct BUTTON *blist; /* button list */
int bnum; /* position of button in list */
{
 blist+=bnum;
 blist->active=FALSE;
 CurXY(wpath, blist->posx, blist->posy);
 RevOff(wpath);
 write(wpath, blist->name, blist->len);
 CurXY(wpath, mouse_x, mouse_y);
} /* end deactivateButton() */

/* redrawButtons - version 1.0
 *            WB - Chris Hyde, 2/95
 *
 * This function redraws all the buttons in the button list given.  If the
 * button is active it'n name is written in reverse video, otherwise it is
 * written in normal video.
 *
 */

redrawButtons(path, blist, numb)
int path; /* window path */
struct BUTTON *blist; /* button list */
int numb; /* total number of button in button list */
{
 int i;

 for (i=0; i<numb; i++, blist++)
 {
    RevOff(path);
    CurXY(path, blist->posx, blist->posy);
    if (blist->active)
       RevOn(path);
    write(path, blist->name, blist->len);
 }
 CurXY(path, mouse_x, mouse_y);
} /* end redrawButtons() */

/* end makebutton.c */
