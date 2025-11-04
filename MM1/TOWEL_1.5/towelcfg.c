#include <modes.h>
#ifdef TCWIN
#include "ethawin.h"
#else
#include <ETHAWIN/ethawin.h>
#endif

char *towelcfg [] = {
   "COPY",
   "DELETE",
   "FILEINFO",
   "RENAME",

   "BACKUP",
   "COBBLER",
   "DCHECK",
   "FORMAT",
   "DISKINFO",

   "DELDIR",
   "MAKEDIR",

   "USER1",
   "USER2",
   "USER3",
   "USER4",
   "USER5",
   "USER6",
   "USER7",
   "USER8",
   "LIST",
   "MOVE",
   "SORTDIR",
   "SHOWHIDDEN",
   ""
};
#define SORTDIR 21
#define SHOWHIDDEN 22
#define TOWELCFGS 23

char *defcmd[] = { /* default commands */
   "copy",
   "del",
   "ident",
   "rename",
   "backup",
   "cobbler",
   "dcheck",
   "format",
   "free",
   "deldir",
   "makdir",
   "dir",
   "dir",
   "dir",
   "dir",
   "dir",
   "dir",
   "dir",
   "dir",
   "list",
   "move"
};
#define DEFCMDS 21
char command[DEFCMDS][22];/* room for DEFCMDS 22 char commands */

char sortdir=FALSE;
char showhidden=FALSE;

int TowelCfg(wpath)
int wpath;
{
   int i,pos,start;
   int fpath;
   char line[80];

   for (i=0; i<DEFCMDS; i++) { /* copy defaults to working memory */
      strcpy(command[i],defcmd[i]);
      strcat(command[i]," "); /* v1.01 add space - bugfix */
   };

   fpath=open("/DD/SYS/ETHAWIN/Towel.cfg",S_IREAD); /* open cfg file */
   if (fpath==-1) {
      fpath=open("./Towel.cfg",S_IREAD); /* open cfg file */
      if (fpath == -1) return(FALSE);
   }

   while ( (pos=(readln(fpath,line,80)))>0 ) { /* while able to read, */
      /* pos points to last character of line (len) */
      line[pos-1]=0; /* NULL terminate line */
      if ((index(line,'=')==0)) continue; /* if no '=', ignore */
      /* v1.01 "-1" to fix space at start of line bugfix */
      for (i=0,start=-1;((i<pos) && (line[i]!='=')); line[i]=toupper(line[i]),i++) {
         /* scan line for "=", uppercase as we go */
         if (line[i]=='*') {
            start=pos; /* flag so we can get out for loop */
            break;
         }
         /* v1.01 bugfix, slightly optimized */
         if (start==-1) { /* if no start point has been set, */
            if (line[i]!=' ') start=i; /* if not space, set start */
         }
      }
      if (start==pos) continue; /* skip if didn't find nothin' */

      /* here start points to first character in the line (keyword) */
      /* i points to = */

      for (pos=0; pos<=TOWELCFGS; pos++) { /* search through config keywords */
         if (strncmp(line+start,towelcfg[pos],strlen(towelcfg[pos]))==0) {
            if (pos<DEFCMDS) {
               strncpy(command[pos],line+i+1,20);
               strcat(command[pos]," "); /* add space to end */
               break;
            }
            switch (pos) {
               case SORTDIR:
                  if (line[i+1]=='Y') sortdir=TRUE;
                  break;
               case SHOWHIDDEN:
                  if (line[i+1]=='Y') showhidden=TRUE;
                  break;
            }
         }
      }
   }
   close(fpath);
}
