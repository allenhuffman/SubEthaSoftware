/*
 * MiniPub.c V0.02 by Allen C. Huffman
 * Copyright (C) 1993 by Sub-Etha Software
 *
 * Processes ASCII text file into a two column 'newsletter'...
 *
 * 0.00 8/16/93 - Initial version
 * 0.01 8/18/93 - Process one screen at a time (reduce buffer size req.)
 * 0.02 8/20/93 - It works!  It works!
 */

#include <stdio.h>
#include <modes.h>
#include "stdinc.h"

#define BUFLEN 1024 /* bytes in disk buffer */
#define LINES 56 /* lines per page */
#define TOP 0 /* top margin (lines) */

char text[LINES*2][44+1]; /* room for LINES xx character lines */
char topleft[31]="Vol. 1, No. 3";
char topright[31]="October '94";
char title[70]="S U B - E T H A   S O F T W A R E";
char subtitle[70]="Company History and Profile";

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
   int page,pagelen;

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
/*   buflen=BUFLEN-36; */
   buflen=BUFLEN-44;
   pagelen=LINES-7; /* page 1 (less 5 lines for header) */
   lines=0;
   page=0;
   do {
      while ((i=read(fpath,buffer+bufst,bufend))>0) { /* while able to read */
         if (i<bufend) { /* if less than expected chars read (EOF) */
            done=TRUE;
            buflen=bufst+i;
            buffer[buflen]=0; /* null terminate */
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
         while (bufst<buflen) {
            for (bufend=bufst;bufend<bufst+44;bufend++) { /* look for NULL */
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
            Justify(line,44,text[lines]);
            lines++;
            if (lines>=pagelen*2) {
               Output(page,pagelen); /* go to routine to output stuff... */
               if (page==0) pagelen=pagelen+4; /* brute force */
               page++;
               lines=0;
            }
            bufst=i+1;
         }
         for (i=0;i+bufst<=BUFLEN;i++) {
            buffer[i]=buffer[i+bufst];
         }
         bufst=i-1;
         bufend=BUFLEN-bufst;
      }
      for (;lines<pagelen*2;lines++) { /* pad any remaining text lines */
         text[lines][0]=0;
      }
      Output(page,pagelen);
      page++;
      done=TRUE; /* done processing */
   } while(!done);
}

Usage()
{
   fputs("\nMiniPub V0.02 by Allen Huffman of Sub-Etha Software\n",stderr);
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
      fputs("Unable to open text file...\n",stderr);
      exit(0);
   }
   return(fpath);
}

Output(page,pagelen)
int page,pagelen;
{
   int i;

   if (page==0) {
      Line();
      Header();
   }

   Line();
   for (i=0;i<pagelen;i++) {
      printf("| %44.44s | %44.44s |\n",text[i],text[i+pagelen]);
   }
   Line();
/*         | .................................... | ..................... */
   printf("                                            Page %02.2d\n",page+1);
   putchar(0x0c);
}

Header()
{
   int i;
   char ttl[70]; /* temp (centered) strings */
   char sttl[70];

   for (i=0;i<70;i++) { /* set 'em both to spaces */
      ttl[i]=' ';
      sttl[i]=' ';
   }
   strcpy(ttl+(70-strlen(title))/2,title);
   strcpy(sttl+(70-strlen(subtitle))/2,subtitle);

   printf("| %-30.30s                               %30.30s |\n",topleft,topright);
   printf("|            %-70.70s           |\n",ttl);
   printf("|            %-70.70s           |\n",sttl);
}

Line()
{
   printf(" ---------------------------------------------------------------------------------------------\n");
}
