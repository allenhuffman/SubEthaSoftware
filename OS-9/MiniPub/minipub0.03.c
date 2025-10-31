/* MiniPub.c V0.03 by Allen C. Huffman
 * Copyright (C) 1994 by Sub-Etha Software
 *
 * Processes ASCII text file into a two column 'newsletter'...
 *
 * 0.00 8/16/93 - Initial version
 * 0.01 8/18/93 - Process one screen at a time (reduce buffer size req.)
 * 0.02 8/20/93 - It works!  It works!
 * 0.03 4/17/94 - Rewrite
 */
 
#include <stdio.h>
#include <modes.h>
#include "stdinc.h"
 
#define BUFLEN 1024
   /* bytes in disk buffer */
#define LINES 53
   /* lines per page */
#define TOP 0
   /* top margin (lines) */
 
char text[LINES*2][37]; /* room for LINES 36 character lines */
char topleft[31]="Vol. 1, No. 1";
char topright[31]="August '93";
char title[70]="M I N I P U B   b y   A L L E N   H U F F M A N";
char subtitle[70]="A Simple Newsletter Printing Utility";

char buffer[BUFLEN]; /* space for disk buffer */
int bufptr=0; /* pointer to where we currently are in the buffer */
 
main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j,fpath;
   char filename[80],ch;
   int done,enter;
   char line[37]; /* line read from file */
   int lineptr; /* pointer to current character in line */
   int lines; /* # of text lines */

   filename[0]=0;
   lines=0;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
      } else {
         strcpy(filename,argv[i]);
         break; /* found filename, break out of this routine... */
      }
   }
   if (filename[0]==0) {
      fputs("\nMiniPub: No filename specified...\n",stderr);
      exit(0);
   }
 
/* rest of code goes here... */
   
   fpath=OpenFile(filename,S_IREAD); /* open file for reading */

   done=FALSE; /* start out as NOT done */
   enter=FALSE; /* start out as no ENTER (line break) in text file */
   lineptr=0;
   line[36]=0;

   do {
      if (read(fpath,&ch,1)<1) { /* read one character from the file */
         done=TRUE; /* end */
         printf("Done!\n");
         break;
      }

      if (ch=='^') { /* if character is a control character */
         /* then just ignore it for now */
      }

      if (ch==13) { /* if ENTER */
         ch=' '; /* turn it into a SPACE */
         enter=TRUE;
      }

      line[lineptr++]=ch; /* build the line */

      if (lineptr>=36) { /* if at end of line */
         printf("\n\n> %s\n  ",line);

         /* find space at end of line for word wrap */

         for (i=36; i>0 && line[i]!=' '; i--); /* find space */
for(j=0;j<i;printf(" "),j++); printf("^");
         printf("space found at %d position\n",i);
         /* here, i points to the last space in the original read line */
         line[i]=0; /* null terminate */
         strcpy(text[lines],line); /* copy it over */

         strcpy(line,line+i-1);
         lineptr=36-i;
         printf("setting line ptr to %d\n",lineptr);

         printf("New Line: %s\n",text[lines]);
         lines++;
      }

   } while (!done);
   close(fpath);
}
 
Usage()
{
   fputs("\nMiniPub V0.03 by Allen Huffman of Sub-Etha Software\n",stderr);
   fputs("Syntax: MiniPub [-opts] {filename}\n",stderr);
   fputs("Usage : Processes a text file into two-column newsletter format.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   exit(0);
}
 
/* accepts in string then returns out string justified to length */
 
Justify(in,length,out)
char *in;
int length;
char *out;
{
   int i;
 
   for (i=0;i<length;i++) out[i]=' ';
   strcpy(out,in);
   out[strlen(in)]=' ';
   out[length]=0;
}
 
int OpenFile(filename,mode)
char filename[];
int mode;
{
   int fpath;
 
   if ((fpath=open(filename,mode))==-1) {
      fputs("\nMiniPub: Unable to open text file...\n",stderr);
      exit(0);
   }
   return(fpath);
}

ReadCh(fpath,ch)
int fpath;
char *ch;
{
   int i;

   if (bufptr>=BUFLEN) { /* if at end of buffer, */
      bufptr=0;
      i=(read(fpath,buffer,BUFLEN));
      printf("bytes read=%d\n",i);
      if ( i>0 ) { /* if able to read, */
         printf("\n%s\n",buffer);
         if (i<BUFLEN) { /* if less than buffer characters read, */
            buffer[i]=0; /* null terminate buffer packet */
         }
      } else { /* if unable to read */
         ch=0;
         return(0); /* return 0 on error or end of file */
      }
   }
   ch=buffer[bufptr++];
   return(0);
}
