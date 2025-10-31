/*
 * Towel.c V1.05 by Allen C. Huffman
 * Copyright (C) 1993-96 by Sub-Etha Software
 *
 * A Useful (?) Disk Utility!
 *
 * 0.00 09/18/93 - Initial version
 * 0.01 09/21/93 - Bug bash (to catch '*' remarks in .cfg files)
 * 0.02 09/22/93 - Bug bash (SHIFT DOWN stuff)
 * 1.00 09/27/93 - "Release Version"
 * 1.01 05/30/94 - Option to make new or use existing window (new .lib too)
 *                 and increase directory space to 500 entries (was 300).
 *      06/07/94   Shift-UP/DN change.  Add Ctrl-UP/DN.  Add LIST and MOVE.
 *      06/08/94   Fix bug (space in front of config entries not required)!!!
 *      06/13/94   Make shell calls BREAK abortable, double click UP/DOWN
 *      06/17/94   buttons now works.  Display number of files in directory.
 *                 Error # in Yes/No routine.  Add '$' to user menu options.
 *                 Make not check for mouse unless mouse is in use (duh!)
 * 1.02 09/13/94 - Test version to work with library update
 * 1.03 03/10/95 - MM/1 updates, filename mouse auto-follow, SortDir/ShowHidden
 * 1.04 09/07/95 - malloc space for directory names and stuff (FAST dir!), fix
 *                 ctrl-c/e abort of iobox functions, turn on cursor in iobox,
 * 1.05 09/27/95 - Modified for TCWIN and NOMOUSE, tagging files auto moves to
 *                 next file, removed unnecessary readdirs after forking cmds
 */

#include <stdio.h>
#include <modes.h>

#ifdef TCWIN
#include "ethawin.h"
#include "ethaglobals.h"
#else
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaglobals.h>
#endif

#ifdef TCWIN
#define LINES 18
#else
#ifndef OSK
#define LINES 18
#else
#define LINES 20
#endif
#endif

#define START 5  /* which line to start showing them at */
#define PLACE 3 /* column to print files in */

/* v1.04 gobal variable stuff */

char *dirbuf; /* pointer to where we store the entire directory */
char **filename; /* pointer to pointers to structure for file data */
char *fileflag; /* pointer to chars for file flags or something */
long *filestart;
int files;

extern char sortdir,showhidden;
extern int strcmp();
extern char *getpwd();

long _gs_size();
char *malloc();

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j,start,pointer,oldpointer;
   int wpath,menu,option;
   int fpath,dpath;
   char ch,click;
   char path[80];
   char loop,done,update;
#ifndef TCWIN
   int pn[3]; /* place for redirecting path stuffs... */
#endif
   char makewin;
   int dirsize;

   strcpy(path,"."); /* make default directory the current one */
   makewin=TRUE; /* make new window and use it */

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
#ifndef TCWIN
         /* v1.01 */
         if (argv[i][1]=='s') { /* if -s (same screen) */
            makewin=FALSE;
         }
#endif
      } else {
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
#ifndef TCWIN
   for (i=0; i<3; i++) { /* dup paths or something */
      pn[i]=dup(i);
      close(i);
      dup(wpath);
   }
#endif

   TopText(wpath,"Don't","Towel V1.05 - By Allen Huffman","Panic!");
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
   do { /* open and read directory here */
      if ((dpath=open(".",S_IREAD | S_IFDIR))==-1) { /* if able to open directory */
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

      dirsize=_gs_size(dpath); /* get size of directory */
      dirbuf = (char *)malloc(dirsize * sizeof(char));
      if (dirbuf==NULL) {
         EndWin(wpath);
         MallocError(wpath);
      }
      i=read(dpath,dirbuf,dirsize); /* read it ALL in...heh */
      close(dpath); /* close it */      
      EndWin(wpath); /* no longer reading so get rid of overlay... */

      dirsize=i/32; /* how many entries is it, really? */
      filename = (char **) malloc(dirsize * sizeof(char *));  
      if (filename==NULL) {
         free(dirbuf);
         MallocError(wpath);
      }
      fileflag=(char *)malloc(sizeof(char) * dirsize);
      if (fileflag==NULL) {
         free(dirbuf);
         free(filename);
         MallocError(wpath);
      }
      filestart=(long *)malloc(sizeof(long) * dirsize);
      if (filestart==NULL) {
         free(dirbuf);
         free(filename);
         free(fileflag);
         MallocError(wpath);
      }
      files=0;
      for (i=0; i<dirsize; i++) {
         if (dirbuf[i*32]!=0) { /* not deleted? */
            filename[files]=&dirbuf[i*32];
            fileflag[files]=FALSE; /* default to not flagged */
                        filestart[files]=0;
            for (j=0; j<28; j++) {
                if (filename[files][j] & 128) { /* high bit set? */
                    filename[files][j]=filename[files][j]-128;
                    filename[files][j+1]='\0'; /* make sure to null terminate */
                                        break;
                }
            }
            files++;
         }
      }
/*
      if (sortdir) {
         qsort(*filename,files,32,strcmp);
      }
*/
/*      if (files<LINES) {
         for (i=files; i<LINES; i++) {
            filename[i][0]='\0';
            fileflag[i]=FALSE;
         }
      }
/*
      /* "Done Processing" <- inside joke...see "Wave for Windows" */

      start=pointer=0; /* point to first file, first line */
      oldpointer=1;

      update=TRUE;
      loop=TRUE;

      sprintf(path,"%3.3d files.",files);
      PrintAt(wpath,PLACE+7+7,START-2,path);

      do {
         if (update==TRUE) {
#ifdef TCWIN
            RefreshOff();
#endif
            j=LINES;
            if (files<LINES) j=files;
            for (i=0; i<LINES ; i++) { /* display initial list of files */
               CurXY(wpath,PLACE-2,i+START);
               ErEOLine(wpath); /* wipe line clear */
               if (i<files) {
                  if ((fileflag[i+start])==TRUE) {
                     strcpy(path,"* "); /* start with CHECKFLAG char */
                  } else {
                         strcpy(path,"  "); /* else just a space */
                  }
/*                  if ((fileflag[i+start])==TRUE) PrintAt(wpath,PLACE-2,i+START,"*");
                  PrintAt(wpath,PLACE,i+START,filename[i+start]); */
                  strcat(path,filename[i+start]);
                  PrintAt(wpath,PLACE-2,i+START,path);
               }
            }
            update=FALSE;
#ifdef TCWIN
            RefreshOn();
#endif
         }
         if (pointer!=oldpointer) { /* update pointer if necessary */
            PrintAt(wpath,PLACE,oldpointer+START,filename[oldpointer+start]);
            ReVOn(wpath);
            PrintAt(wpath,PLACE,pointer+START,filename[pointer+start]);
            ReVOff(wpath);
            oldpointer=pointer;
         }
         if (CheckMenu(wpath,&menu,&option,&ch,&click)==TRUE) {
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
#ifndef NOMOUSE
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
#endif
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
brute_force:
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
            goto brute_force;
/*            break; */
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
         ch='\0';
      } while(loop); /* while still looping . . . */
      free(filename); /* free 'da space */
      free(fileflag);
      free(filestart);
      free(dirbuf);
   } while(!done); /* repeat while not done... */

   ShutDown(wpath);
#ifndef TCWIN
   for (i=0; i<3; i++) {
      close(i);
      dup(pn[i]);
      close(pn[i]);
   }
#endif
}

Usage()
{
   fputs("\nTowel V1.05 by Allen Huffman (coco-sysop@genie.geis.com)\n",stderr);
   fputs("Copyright (C) 1993,94,95 by Sub-Etha Software\n\n",stderr);
   fputs("Syntax: Towel [-opts] {drive/path}\n",stderr);
   fputs("Usage : Point n' click disk utility.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
#ifndef TCWIN
   fputs("        -s = use existing window.\n",stderr);
#endif
   exit(0);
}

MallocError(wpath)
int wpath;
{
   ShutDown(wpath);
   fputs("\nFATAL ERROR:  Towel was unable to allocate necessary memory to process\n",stderr);
   fputs(  "              this directory.\n",stderr);
   sleep(0);
   exit(0);
}
