/*
 * config.c - EthaWin config file routines.
 *
 * 0.00 9/18/93 - Initial version
 * 0.01 9/21/93 - Bug bash (to catch '*' remarks in .cfg file)
 * 0.02 6/08/94 - Fix bug (space in front of config entry not required)
 *                (and fix some other bugs in this routine too...)
 * 0.03 6/17/94 - Move setpal and mouse routines into startup.c
 * 1.00 3/10/95 - MM/1 updates
 */

#include <modes.h>
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>

char *config[] = { /* Keywords in .cfg file */
   "COLOR",

   "FOREGROUND",
   "BACKGROUND",
   "BORDER",
   "HIGHLIGHTED",
   "DESELECTED",
   "MENUBARFORE",
   "MENUBARBACK",
   "TOPTEXTFORE",
   "TOPTEXTBACK",
   "POPUPFORE",
   "POPUPBACK",
   "POPUPSHADOW",
   "POPUPBORDER",
   "PULLDOWNFORE",
   "PULLDOWNBACK",
   "PULLDOWNSHADOW",

   "USEMOUSE", /* mouse config stuff */
#ifndef OSK
   "MOUSEPORT",
   "MOUSERES",
#endif
   "HOTKEYS", /* use hotkeys? */
   ""
};
#define USEMOUSE 17
#ifdef OSK
#define HOTKEYS 18
#define KEYWORDS 19 /* 19 keywords for OSK version */
#else
#define MOUSEPORT 18
#define MOUSERES 19
#define HOTKEYS 20
#define KEYWORDS 21 /* 21 config keywords */
#endif

int ReadCfg(wpath) /* read EthaWin.cfg file to set config stuff... */
int wpath;
{
   int i,pos,start;
   int fpath;
   char line[80];

   fpath=open("/DD/SYS/ETHAWIN/EthaWin.cfg",S_IREAD); /* open cfg file */
   if (fpath==-1) return(FALSE); /* return (use defaults) if not found */

   while ( (pos=(readln(fpath,line,80)))>0 ) { /* while able to read, */
      /* pos points to last character of line (len) */
      line[pos-1]=0; /* NULL terminate line */
      if (index(line,'=')==0) continue; /* if no '=', ignore */
      /* v0.02 fix space at start of line bug */
      for (i=0,start=-1;((i<pos) && (line[i]!='=')); line[i]=toupper(line[i]),i++) {
         /* scan line for "=", uppercase as we go */
         if (line[i]=='*') {
            start=pos; /* flag so we can get out of loop */
            break; 
         }

         if (start==-1) { /* if no start point has been set, */
            if (line[i]!=' ') start=i; /* if not space, set start */
         }
      }
      if (start==pos) continue; /* skip if didn't find nothin' */

      /* here start points to first character in the line (keyword) */
      /* i points to = */

      for (pos=0; pos<KEYWORDS+1; pos++) { /* search thru config keywords */
         if (strncmp(line+start,config[pos],strlen(config[pos]))==0) {
            EWParse(pos,line+start,line+i);
            break;
         }
      }
   }
   close(fpath);

   return(TRUE);
}

EWParse(key,line,pos) /* if here, must have matched a keyword */
int key;
char *line;
char *pos;
{
   int i,temp;
   char *param; /* pointer to a character */

   param=pos+1;

   if (key>0 && key<COLORS+1) { /* if one of first 16 keywords, */
      clr[key-1]=atoi(param); /* set which palette slot to use for items */
      return 0;
   }
   switch (key) {
      case (0): /* Color## (set palette slots) */
      /*         if ((line[5]>='0') && (line[5]<='7')) {*/ /* if valid Colorslot #, */
      i=atoi(line+5);	
#ifdef OSK
      if (i>=0 && i<16) {
         pal_r[i]=atoi(param); /* MM/1 3-byte palette info. */
         if ((temp=findstr(1,param,","))!=0) param=param+temp;
         pal_g[i]=atoi(param);
         if ((temp=findstr(1,param,","))!=0) param=param+temp;
         pal_b[i]=atoi(param);
         break;
      }
#else
      if (i>=0 && i<8) {
         pal[i]=atoi(param); /* CoCo palettes */
         break;
      }
#endif
      break;
      case (USEMOUSE): /* usemouse */
      if (param[0]=='N') {
         mouseon=FALSE; /* default is TRUE */
         break;
      }
      mouseon=TRUE;
      break;
#ifndef OSK
      case (MOUSEPORT): /* mouseport */
      if (param[0]=='L') {
         mouseport=0; /* default is 1 */
         break;
      }
      mouseport=1;
      break;
      case (MOUSERES): /* mouseres */
      if (param[0]=='L') {
         mouseres=0; /* default is 1 */
         break;
      }
      mouseres=1;
      break;
#endif
      case (HOTKEYS): /* hotkeys */
      if (param[0]=='Y') {
         hotkey=TRUE; /* default is FALSE */
         break;
      }
      hotkey=FALSE;
      break;
   }
}

