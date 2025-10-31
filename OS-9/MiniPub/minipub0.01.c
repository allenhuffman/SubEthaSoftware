/*
 * MiniPub.c V0.00 by Allen C. Huffman
 * Copyright (C) 1993 by Sub-Etha Software
 *
 * Processes ASCII text file into a two column 'newsletter'...
 *
 * 0.00 8/16/93 - Initial version
 * 0.01 8/18/93 - Process one screen at a time (reduce buffer size req.)
 */

#include <stdio.h>
#include <modes.h>
#include "stdinc.h"

#define BUFLEN 256 /* bytes in disk buffer */
#define LINES 53 /* lines per page */
#define TOP 0 /* top margin (lines) */

char text[LINES][36]; /* room for LINES 36 character lines */

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j,lines,fpath;
   char filename[80];
   char buffer[BUFLEN+1];
   char line[37];
   int bufst,bufend;
   int done;
   int buflen;

   filename[0]=0;
   buffer[BUFLEN]=0; /* null terminate for testing */

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

   bufst=0;
   bufend=BUFLEN;
   done=FALSE;
   buflen=BUFLEN-36;

   do {
      while ((i=read(fpath,buffer+bufst,bufend))>0) { /* while able to read */
         if (i<bufend) { /* if less than expected chars read (EOF) */
            done=TRUE;
            buflen=bufst+i;
            printf("modified buflen = %d\n",buflen);
         }
         for (i=bufst;i<BUFLEN;i++) {
            if (buffer[i]==13) { /* if CR, */
               if (buffer[i+1]==13) { /* followed by another one */
                  while( (buffer[i]==13) && (i<BUFLEN) ) {
                     buffer[i]=0; /* make it a null */
                     i++;
                  }
               } else {
                  buffer[i]=' '; /* otherwise make it a space */
               }
            }
         }
         bufst=0;
         while (bufst<buflen) { /* ??? was BUFLEN-36 */
            for (bufend=bufst;bufend<bufst+36;bufend++) { /* look for NULL */
               if (buffer[bufend]==0) {
                  i=bufend;
                  break; /* and break if found */
               }
            }
            if (bufend-bufst>34) { /* word wrap only if necessary, */
               for (i=bufend;i>bufst;i--) {
                  if (buffer[i]==' ') {
                     break; /* break when i points to space */
                  }
               }
            }
            strncpy(line,buffer+bufst,i); /* copy from bufst to last word */
            line[i-bufst]=0; /* null terminate string */
            printf(">%s\n",line);
            bufst=i+1;
         }
         for (i=0;i+bufst<=BUFLEN;i++) {
            buffer[i]=buffer[i+bufst];
         }
         bufst=i-1;
         bufend=BUFLEN-bufst;
      }
      done=TRUE; /* done processing */
   } while(!done);
}

Usage()
{ /*
   fputs("\nGENERIC V0.00 by Allen Huffman of Sub-Etha Software (SysOp@Delta/StG)\n",stderr);
   fputs("Syntax: GENERIC [-opts] {filename}\n",stderr);
   fputs("Usage : Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   exit(0); */
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
      fputs("Unable to open text file...\n",stderr);
      exit(0);
   }
   return(fpath);
}

Header()
{
   Line();
   printf("| Vol 01, No 01                                                     August 93 |\n");
   printf("|               M I N I P U B   b y   A L L E N   H U F F M A N               |\n");
   printf("|                A Simple Newsletter Printer by Allen Huffman.                |\n");
   Line();
}

Line()
{
   printf(" -----------------------------------------------------------------------------\n");
}
