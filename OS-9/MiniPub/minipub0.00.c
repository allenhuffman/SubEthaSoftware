/*
 * MiniPub.c V0.00 by Allen C. Huffman
 * Copyright (C) 1993 by Sub-Etha Software
 *
 * Processes ASCII text file into a two column 'newsletter'...
 *
 * 0.00 8/17/93 - Initial version
 *
 */

#include <stdio.h>
#include <modes.h>
#include "stdinc.h"

#define MAXPAGES 20 /* total # of pages to have room for */
#define PAGE1 49 /* text lines on first page */
#define PAGE2 53 /* text lines on extra pages */
#define MAX PAGE1*2+(PAGE2*(MAXPAGES-1)) /* how many total lines to read */

char text[MAX][36]; /* room for MAX 37 character lines */

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j,lines,fpath;
   char line[80];
   char filename[80];
   char buffer[256];

   filename[0]=0;

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
   
   fpath=OpenFile(filename,S_IREAD); /* open file */
   lines=ReadLines(fpath); /* read lines into text array */
   close(fpath); /* done reading, close file... */

   Header(); /* print document header */
   for (i=0;i<PAGE1;i++) {
      printf("| %36.36s | %36.36s |\n",text[i],text[i+PAGE1]);
   }
   Line();

   j=i*2;
   while ((lines>j) && ((j+PAGE2)<MAX)) {
      printf("%c",12);
      Line();
      for (i=j;i<j+PAGE2;i++) {
         printf("| %36.36s | %36.36s |\n",text[i],text[i+PAGE2]);
      }
      Line();
      j=j+(PAGE2*2);
   }
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

int ReadLines(fpath)
int fpath;
{
   int i,j,ln;
   char buffer[256];
   char line[80];

   j=0;
   ln=0;
   while ( (read(fpath,&buffer[j],37-j)>0) && (ln<MAX) ) { /* while able to read */
      for (i=0;i<37;i++) {
         if (buffer[i]==0) break;
         if (buffer[i]==13) {
            buffer[i]=' ';
            if (buffer[i+1]==' ') { /* CR followed by space */
               buffer[i]=0;
               break;
            }
            if (buffer[i+1]==13) {
               buffer[i]=0;
               buffer[i+1]=0;
               break;
            }
         }
      }
      strcpy(line,buffer);
      if (i>36) {
         for (i=36;i>1;i--) {
            if (line[i]==' ') {
               line[i]=0;
               break;
            }
         }
      }
      Justify(line,36,text[ln]);
/*      printf("%2d -> '%s'\n",ln,text[ln]); */
      ln++;
      for (j=0;i<36;j++,i++) {
         buffer[j]=buffer[i+1];
      }
      i=j;
      for ( ;i<36;i++) {
         buffer[i]=' ';
      }
   }
   for (i=ln;i<MAX;i++) { /* null out any unused lines... */
      text[i][0]=0;
   }
   return(ln);
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
