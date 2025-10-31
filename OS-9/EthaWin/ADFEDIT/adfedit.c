/*
 * ADFedit.c V0.00 by Allen C. Huffman
 * Copyright (C) 1994 by Sub-Etha Software
 *
 * SuperComm .adf Auto-Dialer Editor
 *
 * 0.00 09/14/94 - Initial version
 */

#define MAXMENUS 1 /* number of pull-down menus */

#include <stdio.h>
#include <modes.h>
#include <dir.h>

#include "ethawin.h"
#include "ethaglobals.h"
#include "stdinc.h"

     /* Used in Main() */

#define MAXFILES 42 /* up to MAXFILES files read from a directory */
#define LINES 18 /* how many filenames to show */
#define START 5  /* which line to start showing them at */
#define PLACE 3 /* column to print files in */

#define SPACE 32
#define SHIFTUP 28
#define SHIFTDOWN 26
#define CTRLUP 19
#define CTRLDOWN 18

     /* Used in Process() */

#define FILE 0

     /* Used in Main() and Process() */

char filename[MAXFILES][29]; /* global file variable stuff */
long filestart[MAXFILES];
int files;

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
   DIRECT *dirbuf; /* directory buffer */
   char loop,done,update;
   char makewin;

   strcpy(path,"/DD/SYS/DIAL"); /* point to default SuperComm directory */
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

   TopText(wpath,"Don't","ADFedit V0.00 - By Allen Huffman","Panic!");
   About(wpath);

   ReVOn(wpath);
   PrintAt(wpath,PLACE,START-2," UP ");
   PrintAt(wpath,PLACE+7,START-2,"DOWN");
   ReVOff(wpath);

   /* read directory ... */

   done=FALSE;
   do {
      if ((dp=opendir("."))==NULL) { /* if unable to open directory */
         PopUp(wpath,"ERROR",5,5,40,5);
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
            strcpy(filename[files],dirbuf->d_name); /* copy into array */
            filestart[files]=dirbuf->d_addr;
            if (patmatch("*.adf",filename[files],TRUE)==TRUE) {
               filename[files][strlen(filename[files])-4]=0;
               files++;
            }
         } 
         else {
            break; /* break if end of directory */
         }
      }
      closedir(dp);
      EndWin(wpath);
      if (files<LINES) { /* maybe brute force? */
         for (i=files; i<LINES; i++) {
            filename[i][0]=NULL;
         }
      }

      /* "Done Processing" <- inside joke...see "Wave for Windows" */

      start=pointer=0; /* point to first file, first line */
      oldpointer=1;

      update=TRUE;
      loop=TRUE;

      sprintf(path,"%3.3d entries.",files);
      PrintAt(wpath,PLACE+7+7,START-2,path);

      do {
         if (update==TRUE) {
            for (i=0; i<LINES; i++) { /* display initial list of files */
               CurXY(wpath,PLACE-2,i+START);
               ErLine(wpath); /* wipe line clear */
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
            if ( (menu==0) && (option==2) ) { /* if QUIT, */
               if (YesNo(wpath,"QUIT")) {
                  done=TRUE; /* we are done... */
                  break;
               }
            }
         }
         if (mouseon) { /* v1.01 bug fix, only do this if mouse is on */
            if (ms.pt_cbsa==TRUE) { /* is button down? */
               if (mouse_y==START-2) { /* on UP/DOWN line? */
                  if ((mouse_x>=PLACE) && (mouse_x<=PLACE+3)) {
                     if (ms.pt_ccta>0) { /* double click? */
                        ch=SHIFTUP;
                     } else {
                        ch=UP;
                     }
                  }
                  if ((mouse_x>=PLACE+7) && (mouse_x<=PLACE+10)) {
                     if (ms.pt_ccta>0) { /* double click? */
                        ch=SHIFTDOWN;
                     } else {
                        ch=DOWN;
                     }
                  }
               }
            }
            if (click==TRUE) { /* if click was intercepted in CheckMenu, */
               if ((mouse_y>=START) && (mouse_y<START+LINES)) {
                  i=strlen(filename[start+mouse_y-START])-1;
                  if ((mouse_x>=PLACE) && (mouse_x<=PLACE+i)) {
                     pointer=mouse_y-START;
                     if (ms.pt_ccta>0) { /* if more than one click, */
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
            update=TRUE;
            break;
         case CTRLDOWN: /* v1.01 this is kinda brute forced... */
            pointer=LINES-1;
            if (pointer>=files) {
               pointer=files-1;
               break;
            }
            if (start<files-LINES) { /* if not displaying last page, */
               start=files-LINES;
               update=TRUE;
            }
            break;
         case ENTER: /* change to DIR (if possible) */
            Bell(wpath);
            break;
         }
         ch=NULL;
      } 
      while(loop); /* while still looping . . . */
   } 
   while(!done); /* repeat while not done... */

   ShutDown(wpath);
}

Usage()
{
   fputs("\nADFedit V0.00 by Allen Huffman of Sub-Etha Software\n",stderr);
   fputs("Syntax: ADFedit [-opts] {path to .adf files}\n",stderr);
   fputs("Usage : SuperComm .adf Auto-Dialer editor.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   fputs("        -s = use existing window.\n",stderr);
   exit(0);
}
