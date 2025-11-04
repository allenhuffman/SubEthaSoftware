/*
 * mbancfg.c - EthaWin config file routines.
 *
 * 0.00 3/31/95 - Initial version
 */

#include <modes.h>
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaext.h>

char *mbancfg[] = { /* Keywords in .cfg file */
   "BANNERCHAR",
   "LINEFEEDS",
   "LINESPACING",
   "MFONT",
   "PRINTERDEVICE",
   "PRINTERWIDTH"
};
#define BANNERCHAR 0
#define LINEFEEDS 1
#define LINESPACING 2
#define MFONT 3
#define PRINTERDEVICE 4
#define PRINTERWIDTH 5
#define KEYWORDS 6

#define LANDSCAPE 0
#define PORTRAIT 1

char bannerchar=42;
char layout=LANDSCAPE;
char linefeeds=TRUE;
int pagelength=0;
int printwidth=80;
int spacing=1;
char orgfont[25]="Futura24";
char printdev[25]="/P";

int MBanCfg(wpath) /* read EthaWin.cfg file to set config stuff... */
int wpath;
{
   int i,pos,start;
   int fpath;
   char line[80];

   fpath=open("./MegaBan.cfg",S_IREAD); /* check current directory first */
	if (fpath==-1) { /* if not there, check SYS directory next */
		fpath=open("/DD/SYS/ETHAWIN/MegaBan.cfg",S_IREAD); /* open cfg file */
		if (fpath==-1) return(FALSE); /* return (use defaults) if not found */
	}

   while ( (pos=(readln(fpath,line,80)))>0 ) { /* while able to read, */
      if (index(line,'=')==0) continue; /* if no '=', ignore */
      /* pos points to last character of line (len) */
      line[pos-1]=0; /* NULL terminate line */
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
         if (strncmp(line+start,mbancfg[pos],strlen(mbancfg[pos]))==0) {
            MBanParse(pos,line+start,line+i);
            break;
         }
      }
   }
   close(fpath);

   return(TRUE);
}

MBanParse(key,line,pos) /* if here, must have matched a keyword */
int key;
char *line;
char *pos;
{
   int i,temp;
   char *param; /* pointer to a character */

   param=pos+1;

   switch (key) {
      case (BANNERCHAR):
         i=atoi(param);
         if (i>32 && i<=255) {
            bannerchar=i;
         }
         break;
      case (LINEFEEDS):
         if (param[0]=='N') {
            linefeeds=FALSE;
         }
         break;
      case (LINESPACING):
         if (param[0]>='0') { /* if number... */
            spacing=atoi(param);
         }
         break;
      case (MFONT):
         strncpy(orgfont,param,25);
         break;
      case (PRINTERWIDTH):
         i=atoi(param);
         if ((i>=32) && (i<=255)) { /* if valid range */
            printwidth=i;
         }
         break;
      case (PRINTERDEVICE):
         strncpy(printdev,param,25);
         break;
   }
}
