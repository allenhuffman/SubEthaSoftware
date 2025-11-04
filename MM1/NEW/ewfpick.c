/* EwFPICK - version 1.0
 *      WB - Chris Hyde, 3/95
 *
 * The function opens the current directory (folder) and creates a list of the
 * files and or folders that match the type passed in.  It then allows the
 * user to select a file or directory and returns the name selected.  The
 * user can move between directories as welll as other options (see below).
 *
 * The types are defined in ethawin.h and are or'ed together to get the
 * desired type (file attributes).  The types defined are similar to those
 * used by chmod().  The function choses the files that at least match the
 * attributes passed in.  For example to get all the publicly readable files
 * the types should equal PREAD.  To get only those files that are readable
 * and writeable by the processes owner type should equal OREAD | OWRITE.
 * To get a list of directories the type should equal at least DIRECTORY,
 * this will get a list of all the subdirectories of the current directory,
 * regaurdless of their attributes.  Oring DIRECTORY and PREAD will get only
 * publically readable directories.  A listing of all the files and directories
 * can be gotten by using the type ALLFILES.  To get a restricted listing of
 * directories and files BOTH along with DIRECTORY and any other attributes -
 * ie BOTH | DIRECTORY | PREAD will get all the directories and files that
 * are publically readable.
 *
 * After the function creates the list, the first eight are displayed and
 * the user can scoll through the list by clicking on the 'Up' and 'Down'
 * buttons or using the arrow keys.  When the desired file is highlighted
 * they then click on the 'Okay' button to select it.  A pointer to the name
 * of the file is returned or NULL if there is an error or an empty string if
 * the user cancels.
 *
 * Other options available via buttons are;
 *
 *      moving to the current folder's parent
 *      changing to a new drive
 *      creating a new folder (d-ewrewr, and then chnaging to it)
 *      typing in the name of a file (a pointer to this is returned)
 *      opening the selected folder (if the currently selected name is a
 *          folder then this button is active and the user can open it and
 *          change to it)
 *
 */

#include <ethawin.h>
#ifdef OSK
#include <strings.h>
#else
#include <string.h>
#endif
#include <direct.h>
#include <dir.h>
#include <stdinc.h>

#include "ethaext.h"

#define NUMBUTTONS  9 /* the number of buttons defined */

#define UP          0
#define DOWN        1
#define DRIVE       2
#define PARENT      3
#define OPEN        4
#define NEWFOLDER   5
#define NEWFILE     6
#define CANCEL      7
#define OKAY        8

/* structure for each node in the file list */
struct FBUFF
{
   long address;
   char attr;
};

/* helpfull macros tp print things in the right place */
#define farea()     CWArea(path, 7, 5, 30, 8) /* filename area */
#define large()     CWArea(path, 1, 4, 62, 13) /* normal overlay area */
#define tarea()     CWArea(path, 1, 2, 62, 1) /* current folder name area */

/* predeclare functions */
DIR  *getfiles();
#ifndef OSK
DIRECT *newfilename(),
       *startagain();
#else
struct direct *newfilename(),
              *startagain();
#endif
char checkfile();

/* TEST */
extern int errno;

char *EwFPick(path, title, types)
int  path; /* window path */
char *title; /* window title */
int types; /* file attributes to search for */
{
 char done,     /* loop control flag */
      ch,       /* key pressed by user */
      *oldpl,   /* original directory pathlist */
      click;    /* mouse button flag */
 int line,      /* the display line currently highlighted */
     index,     /* the node number of the currently highlighted file */
     numfiles,  /* the total number of files in the list */
     bnum;      /* the button number of the button clicked on */
 DIR *dpath;    /* the directory stream for the current directory */
#ifndef OSK
 DIRECT *dentry;/* the current directory entry */
#else
 struct direct *dentry;
#endif
 static char fname[30]; /* the name of the file of folder selected */
 static struct FBUFF files[256]; /* the files list */
 struct BUTTON blist[NUMBUTTONS]; /* the button list */

/* open the overlay window */
 PopUp(path, title, 8, 3, 64, 18);
 CurOn(path); /* PopUp() turns the cursor off, so we need to turn it back on */

/* now draw the buttons */
 large();
 makeButton(path, blist, UP, "Up", 1, 0);
 makeButton(path, blist, DOWN, "Down", 1, 9);
 makeButton(path, blist, DRIVE, "Drive", 41, 1);
 makeButton(path, blist, PARENT, "Parent", 51, 1);
 makeButton(path, blist, OPEN, "Open", 41, 3);
 makeButton(path, blist, NEWFOLDER, "New Folder", 51, 3);
 makeButton(path, blist, NEWFILE, "New File", 41, 5);
 makeButton(path, blist, CANCEL, "Cancel", 51, 5);
 makeButton(path, blist, OKAY, "Okay", 41, 7);

/* open the current directory and search it for the desired file types */
 if ((dpath=getfiles(".", fname, &numfiles, files, types))==NULL)
 {
    EndWin(path);
    return(NULL);
 }

 oldpl=getpwd(); /* get the current pathlist to chd back to on cancel */

 /* write out first eight names */
 firsteight(path, numfiles, dpath, files, fname);
 deactivateButton(path, blist, UP); /* can't go up to begin with */
 if (numfiles<2) /* can't go down if there are 1 or none files */
    deactivateButton(path, blist, DOWN);

/* set up for the loop */
 line=0;
 index=0;
 done=FALSE;
 dentry=NULL;
 if (numfiles>0) /* if there's more than one file highlight the first one */
     dentry=newfilename(path, line, files[0].address, files[0].attr, dpath,
                        blist);

/* now loop until the user wants to leave */
 do
 {
    bnum=(-1);
    ch='a';

/* wait for user input */
    if (checkMouse(path, TRUE)) /* returns TRUE if mouse was clicked */
       bnum=checkButton(blist, NUMBUTTONS);
    if (_gs_rdy(path)>0)
       read(path, &ch, 1);

/* first check the common buttons and keys (they overlap functions) */
    if (((bnum==DOWN) || (ch==0x0A)) && (index<numfiles-1)) /* down arrow? */
    {
       unhighlight(path, dentry->d_name, line);
       activateButton(path, blist, UP);
       if (++index==numfiles-1) /* index is 0 based */
          deactivateButton(path, blist, DOWN);
       if (++line>7)
       { /* we're past the last line of the display */
          farea();
          CurHome(path);
          DelLine(path);
          line=7;
          large();
       }
       dentry=newfilename(path, line, files[index].address, files[index].attr,
                             dpath, blist);
       continue;
    } /* end if down arrow */

    if (((bnum==UP) || (ch==0x0C)) && (index>0)) /* up arrow? */
    {
       unhighlight(path, dentry->d_name, line);
       activateButton(path, blist, DOWN);
       if (--line<0)
       { /* we're past the first line of the display */
          farea();
          CurHome(path);
          InsLin(path);
          line=0;
          large();
       }
       if (--index==0)
          deactivateButton(path, blist, UP);
       dentry=newfilename(path, line, files[index].address, files[index].attr,
                             dpath, blist);
       continue;
    } /* end if up arrow */

    if ((bnum==OKAY) || (ch==0x0d)) /* enter? */
    {
       strcpy(fname, dentry->d_name);
       done=TRUE;
       break;
    } /* end if enter */

    if ((bnum==CANCEL) || (ch==0x05) || (ch==0x03)) /* cancel, abort, quit */
    {
       fname[0]='\0';
       chdir(oldpl); /* change back to original directory */
       done=TRUE;
    }

/* now check the remaining buttons */
    switch(bnum)
    {
       case DRIVE:          LineInput(path, 2, 11, "New Drive: ", fname, 3);
                            CurXY(path, 2, 11);
                            ErEOLine(path);
                            CurXY(path, mouse_x, mouse_y);
                            if (fname[0]=='\0')
                               break;
                            if (chdir(fname)==ERROR)
                            {
                               error(path, "DISK ERROR",
                                      " Could not open drive ", fname, errno);
                               break;
                            }
                            dentry=startagain(path, fname, dpath, &numfiles,
                                              files, types, &line, &index,
                                              blist);
                            break;

       case PARENT:         if (chdir("..")==ERROR)
                            {
                               error(path, "DISK ERROR",
                                      " Could not open Parent.", "", errno);
                               break;
                            }
                            dentry=startagain(path, fname, dpath, &numfiles,
                                              files, types, &line, &index,
                                              blist);
                            break;

       case OPEN:           if (chdir(dentry->d_name)==ERROR)
                            {
                               error(path, "DISK ERROR",
                                      " Could not open Directory ",
                                      dentry->d_name, errno);
                               break;
                            }
                            dentry=startagain(path, fname, dpath, &numfiles,
                                              files, types, &line, &index,
                                              blist);
                            break;
       case NEWFOLDER:      LineInput(path, 2, 11, "New Folder: ", fname, 29);
                            CurXY(path, 2, 11);
                            ErEOLine(path);
                            CurXY(path, mouse_x, mouse_y);
                            if (fname[0]=='\0')
                               break;
                            if (mknod(fname, 0x3F)==ERROR)
                            {
                               error(path, "DISK ERROR",
                                     " Could not create folder ", fname,
                                     errno);
                               break;
                            }
                            chdir(fname);
                            dentry=startagain(path, fname, dpath, &numfiles,
                                              files, types, &line, &index,
                                              blist);
                            break;

       case NEWFILE:        LineInput(path, 2, 11, "New File: ", fname, 29);
                            CurXY(path, 2, 11);
                            ErEOLine(path);
                            CurXY(path, mouse_x, mouse_y);
                            if (fname[0]!='\0')
                               if (access(fname, 0)==0)
                                  error(path, "FILE ERROR", fname,
                                        " already exists.", 218);
                               else
                                  done=TRUE;
    } /* end switch */
 } while (!done);
 EndWin(path);
 closedir(dpath);
 return(fname);    
} /* end EwFPick() */

/* getfiles - version 1.0
 *       WB - Chris Hyde, 3/95
 *
 * Open the directory passed in and get a listing of the files that
 * match the types past in - ie have at least types attributes.
 * The number of files loaded into the files array is returned in numfiles.
 * A pointer to the direcotry path is returned.
 */

DIR *getfiles(dname, fname, numfiles, files, types)
char *dname; /* the pathlist of the directory to open */
char *fname; /* the name of the current directory (returned) */
int *numfiles; /* the number of files in the files list */
struct FBUFF *files; /* the files list */
int types; /* the attributes to match */
{
 long cloc; /* the current location in the directory stream */
#ifndef OSK
 DIRECT *dentry; /* the current directory entry */
#else
   struct direct *dentry;
#endif
 DIR *dpath; /* the current directory stream */

/* get the name of the current directory */
    getspwd(fname);

 if ((dpath=opendir(dname))==NULL)
    return(NULL);
 
 for (*numfiles=0, cloc=telldir(dpath); /* intialize loop */
        /* while ther are still files to check */
         (*numfiles<256) && ((dentry=readdir(dpath))!=NULL);
        /* get the location of the entry about to be read */
         cloc=telldir(dpath))
 {
/* don't want to include . or .. in the list */
     if ((dentry->d_name[0]!='.') &&
           (files->attr=checkfile(dentry, types)))
     {
        files->address=cloc;
        (*numfiles)++;
        files++;
     } /* end if */
 } /* end for */
 return(dpath);
} /* end getfiles() */

/* checkfile - version 1.0
 *        WB - Chris Hyde, 3/95
 *
 * Checks the directory entry given it to see if it has at least the attributes
 * specified by types.  It does this by opening a path to the entry getting the
 * file descriptor and checking the file's attributes against types.  Returns
 * the file's attribute byte if there's a match, FALSE if not.  If there's an
 * error then FALSE is returned.
 *
 */

char checkfile(direntry, types)
#ifndef OSK
DIRECT *direntry;
#else
struct direct *direntry;
#endif
int types;
{
 struct fildes buff;
 int fd;

 if ((fd=open(direntry->d_name, 1))==ERROR)
 {
    if (types<0x80) /* 0x80 == directory */
       return(FALSE);
    if ((fd=open(direntry->d_name, 1 | DIRECTORY))==ERROR)
       return(FALSE);
 }

 if (_gs_gfd(fd, &buff, sizeof(buff))==ERROR)
 {
    close(fd);
    return(FALSE);
 }
 close(fd);

 if (types==ALLFILES)
    return(buff.fd_att);

 if (types>=BOTH)
 {
    types&=0xff; /* mask off BOTH bit */
    if ((buff.fd_att & types)==types) /* match directories */
       return(buff.fd_att);
    types&=0x7f; /* mask off DIRECTORY bit */
 }
 if ((buff.fd_att & types)==types) /* match files */
    return(buff.fd_att);
 return(FALSE);
} /* end checkfile() */

/* newfilename - version 1.0
 *          WB - Chris Hyde, 3/95
 *
 * Seeks to the directory entry given by index and writes the filename to
 * path in reverse video.  If the file seeked to is a directory then the
 * 'Open' button is activated, otherwise it is deactivated.
 *
 * A pointer to the new directory entry is returned if the seek and read are
 * successfull, otherwise NULL is returned.
 *
 */

#ifndef OSK
DIRECT *newfilename(path, line, index, attr, dpath, blist)
#else
struct direct *newfilename(path, line, index, attr, dpath, blist)
#endif
int path; /* window path */
int line; /* current line of display */
long index; /* location to seek to */
char attr; /* attributes of the current file */
DIR *dpath; /* directory stream to use */
struct BUTTON *blist; /* button list to use */
{
#ifndef OSK
 DIRECT *dentry; /* current directory entry */
#else
   struct direct *dentry;
#endif

 farea();
 seekdir(dpath, index);
 if ((dentry=readdir(dpath))==NULL)
    return(NULL);
 RevOn(path);
 PrintAt(path, 0, line, dentry->d_name);
 RevOff(path);
 large();
 if (attr<0) /* directories have attributes < 0 */
    activateButton(path, blist, OPEN);
 else
    deactivateButton(path, blist, OPEN);
 return(dentry);
} /* end newfilename() */

/* unhighlight - version 1.0
 *          WB - Chris Hyde, 3/95
 *
 * Unhighlights the string given on line (turns off reverse video and
 * writes the name).
 *
 */

unhighlight(path, s, line)
int path; /* window path */
char *s; /* string to write */
int line; /* line to write it on */
{
 farea();
 RevOff(path);
 PrintAt(path, 0, line, s);
 large();
} /* end unhighlight() */

/* firsteight - version 1.0
 *         WB - Chris Hyde, 3/95
 *
 * Displays the first eight file names (if there are eight) contained in 
 * the files array.  It also displays the name of the current foldler.
 *
 */

firsteight(path, numfiles, dpath, files, fname)
int path; /* window path */
int numfiles; /* number of files in the list */
DIR *dpath; /* directory stream to use */
struct FBUFF *files; /* list of files */
char *fname; /* name of current folder */
{
 int i;
#ifndef OSK
 DIRECT *dentry; /* curent directory entry */
#else
   struct direct *dentry;
#endif

/* put the name of the current directory in the overlay */
 tarea();
 PrintAt(path, 1, 0, "Folder:                                ");
 PrintAt(path, 9, 0, fname);

 farea();
 Cls(path);
 for (i=0; (i<8) && (i<numfiles); i++)
 {
    seekdir(dpath, files[i].address);
    dentry=readdir(dpath);
    PrintAt(path, 0, i, dentry->d_name);
 } /* end for */
 large();
} /* end firsteight() */


/* startagain - version 1.0
 *         WB - Chris Hyde, 3/95
 *
 * Resets variables and such when moving to a new directory.
 *
 * Closes the current directory, opens the new one (we already chddir()
 * to it), loads the files array, prints the first eight filenames and
 * highlights the first one.
 *
 * Returns a pointer to dirctory entry for the first filename if successfull,
 * NULL, otherwise.
 *
 */

#ifndef OSK
DIRECT *startagain(path, fname, dpath, numfiles, files, types, line,
                   index, blist)
#else
struct direct *startagain(path, fname, dpath, numfiles, files, types, line,
                          index, blist)
#endif
int path; /* window path */
char *fname; /* name of the current directory */
DIR *dpath; /* directory stream */
int *numfiles; /* number of files in the files list */
struct FBUFF *files; /* list of files */
int types; /* attributes to look for */
int *line, /* current display line highlighted */
    *index; /* current file highlighted */
struct BUTTON *blist;
{
/* close the current directory and open the new one (we've already chd'd
 * to the new directory.
 */
 closedir(dpath);
 if ((dpath=getfiles(".", fname, numfiles, files, types))==NULL)
    return(NULL);
 firsteight(path, *numfiles, dpath, files, fname);
 *line=0;
 *index=0;
 deactivateButton(path, blist, UP); /* can't go up to begin with */
 if (*numfiles<2) /* can't go down unless there is more than 1 file */
    deactivateButton(path, blist, DOWN);
 else
    activateButton(path, blist, DOWN); /* just incase it was deactivated */
 if (*numfiles>0)
     return(newfilename(path, line, files[0].address, files[0].attr, dpath,
                        blist));
 return(NULL);
} /* end startagain() */

/* end ewfpick.c */
