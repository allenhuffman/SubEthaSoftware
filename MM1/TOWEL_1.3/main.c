/*
 * Towel.c V1.03 by Allen C. Huffman
 * Copyright (C) 1993,1994,1995 by Sub-Etha Software
 *
 * A Useful (?) Disk Utility!
 *
 * 0.00 9/18/93 - Initial version
 * 0.01 9/21/93 - Bug bash (to catch '*' remarks in .cfg file)
 * 0.02 9/22/93 - Bug bash (SHIFT DOWN stuff)
 * 1.00 9/27/93 - "Release Version"
 * 1.01 5/30/94 - Option to make new or use existing window (new .lib too)
 *                and increase directory space to 500 entries (was 300).
 *      6/07/94   Shift-UP/DN change.  Add Ctrl-UP/DN.  Add LIST and MOVE.
 *      6/08/94   Fix bug (space in front of config entries not required)!!!
 *      6/13/94   Make shell calls BREAK abortable, double click UP/DOWN
 *      6/17/94   buttons now works.  Display number of files in directory.
 *                Error # in Yes/No routine.  Add '$' to user menu options.
 *                Make not check for mouse unless mouse is in use (duh!)
  * 1.02 9/13/94 - Test version to work with library update
 * 1.03 3/10/95 - MM/1 updates, filename mouse auto-follow, SortDir/ShowHidden
 */

#include <stdio.h>
#include <modes.h>
#include <dir.h>

#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaglobals.h>

#define MAXFILES 500 /* up to MAXFILES files read from a directory */

#ifndef OSK
#define LINES 18
#else
#define LINES 20
#endif

#define START 5  /* which line to start showing them at */
#define PLACE 3 /* column to print files in */

char filename[MAXFILES][29]; /* global file variable stuff */
long filestart[MAXFILES];
char fileflag[MAXFILES];
int files;

extern char sortdir,showhidden;
extern int strcmp();
extern char *getpwd();

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,start,pointer,oldpointer;
   int wpath,menu,option;
   int fpath;
   char ch,click;
   char path[80];
   DIR *dp; /* directory file pointer */
#ifndef OSK
   DIRECT *dirbuf; /* directory buffer */
#else
   struct direct *dirbuf; /* OSK dir buffer? */
#endif
   char loop,done,update;
   int pn[3]; /* place for redirecting path stuffs... */
   char makewin;

   strcpy(path,"."); /* make default directory the current one */
   makewin=TRUE; /* make new window and use it */

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
         /* v1.01 */
         if (argv[i][1]=='s') { /* if -s (same screen) */
            makewin=FALSE;
         }
      } 
      else {
         strcpy(path,argv[i]); /* copy cmdline path into 'path' */
      }
   }
   /* rest of code goes here... */
   if (chdir(path)==-1) {
      fputs("\nTowel: Unable to CHDIR to drive/path...\n",stderr);
      exit(0);
   }

   /* v1.01 */
   wpath=Startup(makewin);

   /* v1.02 */
   for (i=0; i<3; i++) { /* dup paths or something */
      pn[i]=dup(i);
      close(i);
      dup(wpath);
   }

   TopText(wpath,"Don't","Towel V1.03 - By Allen Huffman","Panic!");
   About(wpath);
   PopUp(wpath,"DON'T PANIC!",26,9,28,5);
   BlnkOn(wpath);
   PrintAt(wpath,1,1,"Reading Configuration...");
   BlnkOff(wpath);
   TowelCfg(wpath);
   EndWin(wpath);

   ReVOn(wpath);
   PrintAt(wpath,PLACE,START-2," UP ");
   PrintAt(wpath,PLACE+7,START-2,"DOWN");
   ReVOff(wpath);

   /* read directory ... */

   done=FALSE;
   do {
      if ((dp=opendir("."))==NULL) { /* if unable to open directory */
         PopUp(wpath,"ERROR",25,9,30,5);
         PrintAt(wpath,1,1,"Unable to open directory.");
         Wait(wpath);
         ShutDown(wpath); /* <-- restore paths, eh? */
         exit(0);
      }
      PopUp(wpath,"",27,9,26,5);
      BlnkOn(wpath);
      PrintAt(wpath,1,1,"Reading Directory. . .");
      BlnkOff(wpath);
      files=0;
      while (files<MAXFILES) { /* read up to MAXFILES directory entries */
         if ((dirbuf=readdir(dp))!=NULL) { /* if able to read entry, */
            if (!((dirbuf->d_name[0]=='.') && (dirbuf->d_name[1]!='.') && (!showhidden))) {
               strcpy(filename[files],dirbuf->d_name); /* copy into array */
               filestart[files]=dirbuf->d_addr;
               fileflag[files]=FALSE;
               files++;
            }
         } 
         else {
            break; /* break if end of directory */
         }
      }
      if (sortdir) {
         qsort(filename,files,29,strcmp);
      }
      closedir(dp);
      EndWin(wpath);
      if (files<LINES) { /* maybe brute force? */
         for (i=files; i<LINES; i++) {
            filename[i][0]=NULL;
            fileflag[i]=FALSE;
         }
      }

      /* "Done Processing" <- inside joke...see "Wave for Windows" */

      start=pointer=0; /* point to first file, first line */
      oldpointer=1;

      update=TRUE;
      loop=TRUE;

      sprintf(path,"%3.3d files.",files);
      PrintAt(wpath,PLACE+7+7,START-2,path);

      do {
         if (update==TRUE) {
            for (i=0; i<LINES; i++) { /* display initial list of files */
               CurXY(wpath,PLACE-2,i+START);
               ErLine(wpath); /* wipe line clear */
               if ((fileflag[i+start])==TRUE) PrintAt(wpath,PLACE-2,i+START,"*");
               PrintAt(wpath,PLACE,i+START,filename[i+start]);
            }
            update=FALSE;
         }
         if (pointer!=oldpointer) { /* update pointer if necessary */
            PrintAt(wpath,PLACE,oldpointer+START,filename[oldpointer+start]);
            ReVOn(wpath);
            PrintAt(wpath,PLACE,pointer+START,filename[pointer+start]);
            ReVOff(wpath);
            oldpointer=pointer;
         }
         if (CheckMenu(wpath,&menu,&option,&ch,&click)) {
            if ( (menu==0) && (option==6) ) { /* if QUIT, */
               if (YesNo(wpath,"QUIT")) {
                  done=TRUE; /* we are done... */
               }
               break;
            }
            if (Process(wpath,menu,option)) { /* process options */
               loop=FALSE; /* must re-read directory if here */
            }
            break;
         }
         if (mouseon) { /* v1.01 bug fix, only do this if mouse is on */

/* put a check in here to see if button has been held down longer than
   the click time... */

            if (ms.pt_cbsa==TRUE) { /* is button down? */
               if (mouse_y==START-2) { /* on UP/DOWN line? */
                  if ((mouse_x>=PLACE) && (mouse_x<=PLACE+3)) { /* UP? */
                     if (click>1) { /* double click? */
                        ch=SHIFTUP;
                     } 
                     else {
                        ch=UP;
                     }
                  } 
                  else { /* if not on UP, check if on DOWN */
                     if ((mouse_x>=PLACE+7) && (mouse_x<=PLACE+10)) {
                        if (click>1) { /* double click? */
                           ch=SHIFTDOWN;
                        } 
                        else {
                           ch=DOWN;
                        }
                     }
                  }
               }
            }
            if ((mouse_y>=START) && (mouse_y<START+LINES)) { /* if on file */
               i=strlen(filename[start+mouse_y-START])-1;
               if ((mouse_x>=PLACE) && (mouse_x<=PLACE+i)) {
                  pointer=mouse_y-START;
                  if (click>0) {
                     if (click>1) { /* if more than one click, */
                        ch=ENTER;
                     } 
                     else {
                        ch=SPACE;
                     }
                  }
               }
            }
         } /* end of mouse checking code */

         /* here, assume menu option has been executed OR char is in ch */

         switch(ch) {
         case UP:
            if (pointer>0) { /* if pointer not already at top, */
               pointer--; /* decrement it */
               break; /* and exit */
            } /* else assume marker is already at top, so */
            if (start>0) { /* if filelist is not already at top, */
               PrintAt(wpath,PLACE,START,filename[start]); /* unhighlight */
               start--; /* decrement it */
               CurXY(wpath,PLACE,START+LINES-1);
               ErLine(wpath);
               CurXY(wpath,PLACE,START); /* move to first filelist line */
               InsLin(wpath); /* insert a line there */
               if (fileflag[start]) PrintAt(wpath,PLACE-2,START,"*");
               ReVOn(wpath);
               PrintAt(wpath,PLACE,START,filename[start]);
               ReVOff(wpath);
            }
            break; /* done with this stuff */
         case DOWN:
            if ((pointer<LINES-1) && (pointer<files-1)) {
               pointer++;
               break;
            }
            if (start<files-LINES) {
               PrintAt(wpath,PLACE,START+LINES-1,filename[start+pointer]);
               start++;
               CurXY(wpath,PLACE,START);
               DelLine(wpath);
               if (fileflag[start+pointer]) PrintAt(wpath,PLACE-2,START+LINES-1,"*");
               ReVOn(wpath);
               PrintAt(wpath,PLACE,START+LINES-1,filename[start+pointer]);
               ReVOff(wpath);
            }
            break;
         case SHIFTUP:
            if (pointer>0) {
               pointer=0;
               break;
            }
            if (start>0) {
               start=start-LINES;
               if (start<0) start=0;
               /* v1.01, make work like ved */
               pointer=LINES-1;
               update=TRUE;
            } /* may have bug with oldpointer update...? */
            break;
         case SHIFTDOWN:
            if (pointer<LINES-1) {
               pointer=LINES-1;
               if (pointer>=files) pointer=files-1;
               break;
            }
            if (start<files-LINES) {
               start=start+LINES;
               if (start>files-LINES) start=files-LINES;
               /* v1.01, make work like ved */
               pointer=0;
               update=TRUE;
            }
            break;
         case CTRLUP: /* v1.01 */
            pointer=0;
            if (start==0) break; /* if less than screenfull */
            start=0;
            oldpointer=1;
            update=TRUE;
            break;
         case CTRLDOWN: /* v1.01 this is kinda brute forced... */
            pointer=LINES-1;
            oldpointer=0;
            if (pointer>=files) {
               pointer=files-1;
               break;
            }
            if (start<files-LINES) { /* if not displaying last page, */
               start=files-LINES;
               update=TRUE;
            }
            break;
         case SPACE:
            if (fileflag[start+pointer]==TRUE) {
               fileflag[start+pointer]=FALSE;
               PrintAt(wpath,PLACE-2,START+pointer," ");
            } 
            else {
               fileflag[start+pointer]=TRUE;
               PrintAt(wpath,PLACE-2,START+pointer,"*");
            }
            break;
         case ENTER: /* change to DIR (if possible) */
            PopUp(wpath,"",27,9,26,5);
            BlnkOn(wpath);
            PrintAt(wpath,1,1,"Attempting to CHDIR...");
            BlnkOff(wpath);
            if (chdir(filename[start+pointer])==0) { /* if successful, */
               loop=FALSE; /* break out of loop */
            }
            EndWin(wpath);
            break;
         }
         ch=NULL;
      } 
      while(loop); /* while still looping . . . */
   }
   while(!done); /* repeat while not done... */

   Select(STDOUT);
   ShutDown(wpath);
   for (i=0; i<3; i++) {
      close(i);
      dup(pn[i]);
      close(pn[i]);
   }
}

Usage()
{
   fputs("\nTowel V1.03 by Allen Huffman (coco-sysop@genie.geis.com)\n",stderr);
   fputs("Copyright (C) 1993,94,95 by Sub-Etha Software\n\n",stderr);
   fputs("Syntax: Towel [-opts] {drive/path}\n",stderr);
   fputs("Usage : Point n' click disk utility.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   fputs("        -s = use existing window.\n",stderr);
   exit(0);
}
