#define MAXMENUS 4

#include <stdio.h>
#include <ETHAWIN/ethawin.h>

#define FILE 0
#define DISK 1
#define DIRECTORY 2
#define USER 3

#define COPY 0
#define DELETE 1
#define FILEINFO 2
#define RENAME 3
#define BACKUP 4
#define COBBLER 5
#define DCHECK 6
#define FORMAT 7
#define DISKINFO 8
#define DELDIR 9
#define MAKEDIR 10
#define USER1 11
#define USER2 12
#define USER3 13
#define USER4 14
#define USER5 15
#define USER6 16
#define USER7 17
#define USER8 18
#define LIST 19
#define MOVE 20

extern char command[][22]; 
extern char **filename;
extern long *filestart;
extern char *fileflag;
extern int files;

int Process(wpath,menu,option)
int wpath,menu,option;
{
   int i;
   char cmd[80];
   char line[80];
   char readdir;

   readdir=FALSE; /* don't readdir upon return unless told to */

   if (menu==FILE) {
      for (i=0; i<files; i++) { /* look for at least one marked file */
         if (fileflag[i]==TRUE) break; /* found one? break out of loop */
      }
      if (i==files) { /* if no files are marked, */
         PopUp(wpath,"ERROR",27,9,26,5); /* tell user about it... */
         PrintAt(wpath,4,1,"No Files Marked!");
         Wait(wpath);
         EndWin(wpath);
         return(FALSE); /* exit */
      }
      switch(option) {
      case 0: /* if copy, */
         DrivePath(wpath,"COPY TO",line);
         if (line[0]!=NULL) {
            IOBox(wpath);
            for (i=0; i<files; i++) { /* loop through files */
               if (fileflag[i]==TRUE) {
                  strcpy(cmd,command[COPY]);
                  strcat(cmd,filename[i]);
                  strcat(cmd," ");
                  strcat(cmd,line);
                  strcat(cmd,"/");
                  strcat(cmd,filename[i]);
                  cwriteln(wpath,cmd,80);
                  CrRtn(wpath);
                  CurDwn(wpath);
                  if (System(wpath,cmd)) break;
               }
            }
            EndWin(wpath);
         }
         break;
      case 1: /* if delete, */
         if (YesNo(wpath,"DELETE FILES")) {
            IOBox(wpath);
            for (i=0; i<files; i++) {
               if (fileflag[i]==TRUE) {
                  readdir=TRUE;
                  strcpy(cmd,command[DELETE]);
                  strcat(cmd,filename[i]);
                  cwriteln(wpath,cmd,80);
                  CrRtn(wpath);
                  CurDwn(wpath);
                  if (System(wpath,cmd)) {
                     EndWin(wpath);
                     break;
                  }
               }
            }
            EndWin(wpath);
         }
         break;
      case 2: /* if info */
         IOBox(wpath);
         for (i=0; i<files; i++) {
            if (fileflag[i]==TRUE) {
               strcpy(cmd,command[FILEINFO]);
               strcat(cmd,filename[i]);
               cwriteln(wpath,cmd,80);
               CrRtn(wpath);
               CurDwn(wpath);
               if (System(wpath,cmd)) {
                  EndWin(wpath);
                  break;
               }
            }
         }
         Wait(wpath);
         EndWin(wpath);
         break;
         /* v1.01 */
      case 3: /* if list */
         IOBox(wpath);
         for (i=0; i<files; i++) {
            if (fileflag[i]==TRUE) {
               strcpy(cmd,command[LIST]);
               strcat(cmd,filename[i]);
               cwriteln(wpath,cmd,80);
               CrRtn(wpath);
               CurDwn(wpath);
               if (System(wpath,cmd)) {
                  EndWin(wpath);
                  break;
               }
               Wait(wpath); /* wait at end of each file */
            }
         }
         EndWin(wpath);
         break;
         /* v1.01 */
      case 4: /* if move */
         DrivePath(wpath,"MOVE TO",line);
         if (line[0]!=NULL) {
            IOBox(wpath);
            for (i=0; i<files; i++) { /* loop through files */
               if (fileflag[i]==TRUE) {
                  readdir=TRUE;
                  strcpy(cmd,command[MOVE]);
                  strcat(cmd,filename[i]);
                  strcat(cmd," ");
                  strcat(cmd,line);
                  cwriteln(wpath,cmd,80);
                  CrRtn(wpath);
                  CurDwn(wpath);
                  if (System(wpath,cmd)) break;
               }
            }
            EndWin(wpath);
         }
         break;
      case 5: /* if rename */
         if (YesNo(wpath,"RENAME FILES")) {
            IOBox(wpath);
            PrintAt(wpath,27,1,"Press [ENTER] to Abort");
            for (i=0; i<files; i++) {
               if (fileflag[i]==TRUE) {
                  PrintAt(wpath,1,3,"Old Name: ");
                  ErEOLine(wpath);
                  cwrite(wpath,filename[i],80);
                  LineInput(wpath,1,5,"New Name: ",line,29);
                  if (line[0]!=NULL) {
                     readdir=TRUE;
                     strcpy(cmd,command[RENAME]);
                     strcat(cmd,filename[i]);
                     strcat(cmd," ");
                     strcat(cmd,line);
                     if (System(wpath,cmd)) break;
                  }
               }
            }
            EndWin(wpath);
            break;
         }
      }
   }
   if (menu==DISK) {
      switch(option) {
      case 0: /* backup */
         DrivePath(wpath,"BACKUP SOURCE",line);
         if (line[0]!=NULL) {
            strcpy(cmd,command[BACKUP]);
            strcat(cmd,line);
            strcat(cmd," ");
            DrivePath(wpath,"DESTINATION",line);
            if (line[0]!=NULL) {
               IOBox(wpath);
               strcat(cmd,line);
               if (System(wpath,cmd)) {
                  EndWin(wpath);
                  break;
               }
               EndWin(wpath);
            }
         }
         break;
      case 1: /* cobbler */
         DrivePath(wpath,"COBBLE TO",line);
         if (line[0]!=NULL) {
            strcpy(cmd,command[COBBLER]);
            strcat(cmd,line);
            if (YesNo(wpath,"COBBLER")) {
               IOBox(wpath);
               if (System(wpath,cmd)) {
                  EndWin(wpath);
                  break;
               }
               readdir=TRUE;
            }
            EndWin(wpath);
         }
         break;
      case 2: /* dcheck */
         DrivePath(wpath,"DCHECK",line);
         if (line[0]!=NULL) {
            strcpy(cmd,command[DCHECK]);
            strcat(cmd,line);
            if (YesNo(wpath,"DCHECK")) {
               IOBox(wpath);
               if (System(wpath,cmd)) {
                  EndWin(wpath);
                  break;
               }
               Wait(wpath);
               EndWin(wpath);
            }
         }
         break;
      case 3: /* format */
         DrivePath(wpath,"FORMAT",line);
         if (line[0]!=NULL) {
            strcpy(cmd,command[FORMAT]);
            strcat(cmd,line);
            if (YesNo(wpath,"FORMAT")) { /* fix this to show path later */
               readdir=TRUE;
               IOBox(wpath);
               if (System(wpath,cmd)) {
                  EndWin(wpath);
                  break;
               }
               EndWin(wpath);
            }
         }
         break;
      case 4: /* Info */
         DrivePath(wpath,"DISK INFO",line);
         if (line[0]!=NULL) {
            strcpy(cmd,command[DISKINFO]);
            strcat(cmd,line);
            IOBox(wpath);
            if (System(wpath,cmd)) {
               EndWin(wpath);
               break;
            }
            Wait(wpath);
            EndWin(wpath);
         }
         break;
      }
   }
   if (menu==DIRECTORY) {
      switch(option) {
      case 0: /* change */
         DrivePath(wpath,"CHDIR",line);
         if (line[0]!=NULL) { /* if path returned, */
            chdir(line); /* change and update */
            readdir=TRUE;
         }
         break;
      case 1: /* deldir */
         if (YesNo(wpath,"DELDIR")) {
            IOBox(wpath);
            for (i=0; i<files; i++) {
               if (fileflag[i]==TRUE) {
                  strcpy(cmd,command[DELDIR]);
                  strcat(cmd,filename[i]);
                  cwrite(wpath,cmd,80);
                  CrRtn(wpath);
                  CurDwn(wpath);
                  if (System(wpath,cmd)) {
                     break;
                  }
                  readdir=TRUE;
               }
            }
            EndWin(wpath);
         }
         break;
      case 2: /* makedir */
         IOBox(wpath);
         PrintAt(wpath,27,1,"Press [ENTER] to Abort");
         LineInput(wpath,1,4,"Make Directory: ",line,30);
         if (line[0]!=NULL) {
            readdir=TRUE;
            strcpy(cmd,command[MAKEDIR]);
            strcat(cmd,line);
            System(wpath,cmd); /* make dir (no error check) */
         }
         EndWin(wpath);
         break;
      }
   }
   if (menu==USER) { /* if User menus */
      readdir=TRUE; /* no telling what user command may do... */
      IOBox(wpath);
      if (command[USER1+option][0]=='$') {
         for (i=0; i<files; i++) {
            if (fileflag[i]==TRUE) { /* if tagged file, */
               strcpy(cmd,command[USER1+option]+1); /* build command */
               strcat(cmd,filename[i]); /* add filename */
               cwrite(wpath,cmd,80);
               CrRtn(wpath);
               CurDwn(wpath);
               System(wpath,cmd);
            }
         }
      } 
      else {
         cwrite(wpath,command[USER1+option],80);
         CrRtn(wpath);
         CurDwn(wpath);
         System(wpath,command[USER1+option]);
      }
      Wait(wpath);
      EndWin(wpath);
   }
   return(readdir);
}
