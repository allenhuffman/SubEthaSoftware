/*
 * MultiBoot.c V1.04 by Allen C. Huffman
 * Copyright (C) 1993,1994 by Sub-Etha Software
 *
 * OS9 portion of MultiBoot utility...  RS-DOS portion by Terry Todd.
 *
 * 0.01 7/06/93 - Initial test version
 * 0.02 7/12/93 - Changes for up to 16 bootfiles (OS9BOOT V1.1)
 * 0.03 7/15/93 - Addition of self-installation & bug bashing.
 * 0.04         - and removal of bulky default boot desc. (unused).
 * 0.05         - Incorporation of RS-DOS code version 1.12
 * 0.06 7/17/93 - Fix MAXxxx+1 bug (crash when more than MAXxxx read) and
 *                unbuffer output.
 * 0.07 7/21/93 - fflush() alot of text, more room for DIR entries...
 * 1.00 8/16/93 - Release version
 * 1.01 10/5/93 - fix buffered output bug ("enter description") and make
 *                it check for "OS" instead of first 20 bytes of Kernel
 *                for compatiblity with non-Tandy kernels.
 * 1.02 5/09/94 - Fix STACK OVERFLOW problems when using stock shell...blah!
 * 1.03 5/25/94 - Fix to properly recognize other Kernels (NitrOS9, etc.)
 *                and to make work after breaking in 1.01...
 * 1.04 7/11/94 - Add -e (edit) option, fix minor fflush errors
 */

#include <stdio.h>
#include <modes.h>
#include <dir.h>

#include "stdinc.h"

#define MAXBOOTS 16
   /* maximum number of bootfiles to allow */
#define MAXDIRS 50
   /* maximum number of directory entries to read */
#define DELAY 30
   /* default delay value for menu (in seconds) */

#define TIMER 1725
   /* multiplier... DELAY*TIMER = seconds */

/* define a few structures... */

typedef struct { /* setup bootfile entry format... */
   char desc[27]; /* room for bootfile desc. */
   char info[5]; /* start/length of bootfile info */
} BF;

typedef struct { /* setup multiboot sector format... */
   unsigned delay; /* 2 byte delay value */
   char choices; /* # of boot files (1-MAXBOOTS) */
   BF bf[MAXBOOTS]; /* space for MAXBOOTS bootfile entries */
} MB;

/* define global variables... */

char device[6]; /* default bootfile drive */
MB mb; /* room for multiboot sector data */

/* define global variable to contain RS-DOS MultiBoot data... */

char sec34_1[] = { /* First 20 bytes of Original OS-9 KERNEL Boot Code... */
   79,83,32,91,18,5,135,205,1,42,0,13,193,129,211,0,89,0,0,82
};
#define CMPLEN 5
/* above is number of bytes to compare when checking for cobbled disk. */

char mb_sec34_1[] = { /* Terry's RS-DOS DOS startup code... 50 bytes */
   79,83,134,13,151,211,204,56,0,221,209,12,211,142,0,234,204,2,0,237,132,
   134,33,214,211,237,2,220,209,237,4,173,159,192,4,109,6,38,8,76,129,61,37,
   221,126,57,0,126,215,9
};

char mb_sec33_15[] = { /* Terry's RS-DOS MultiBoot code V1.12... 512 Bytes */
   142,56,0,16,142,38,0,236,129,237,161,140,57,0,37,247,23,1,152,23,1,171,
   32,32,32,32,32,32,32,32,77,117,108,116,105,66,111,111,116,32,86,49,46,49,
   50,13,32,98,121,32,84,101,114,114,121,32,84,111,100,100,32,38,32,65,108,
   108,101,110,32,72,117,102,102,109,97,110,13,32,32,32,32,32,67,111,112,121,
   114,105,103,104,116,32,40,67,41,32,49,57,57,51,32,98,121,13,32,32,32,32,
   32,32,32,83,117,98,45,69,116,104,97,32,83,111,102,116,119,97,114,101,13,
   0,16,142,5,192,16,159,136,23,1,53,32,32,32,32,32,85,115,101,32,85,80,47,
   68,79,87,78,32,116,111,32,115,99,114,111,108,108,13,91,69,78,84,69,82,93,
   32,83,101,108,101,99,116,115,32,32,32,91,66,82,69,65,75,93,32,81,117,105,
   116,115,0,246,58,235,23,1,16,206,4,164,246,58,255,16,39,0,209,240,58,235,
   193,8,37,2,198,8,52,20,223,136,189,58,208,53,20,48,136,32,51,200,32,90,
   38,238,182,58,234,176,58,235,198,32,61,195,4,163,31,1,134,106,167,132,16,
   190,58,253,49,63,38,5,50,98,22,0,151,173,159,160,0,39,241,198,96,231,132,
   198,255,129,94,39,17,129,10,39,36,129,13,39,62,129,3,38,197,15,113,126,
   140,27,182,58,234,39,187,247,1,85,74,183,58,234,177,58,235,36,3,122,58,
   235,22,255,126,182,58,234,76,177,58,255,36,160,247,1,86,183,58,234,182,
   58,235,139,7,177,58,234,36,228,124,58,235,32,223,246,58,234,92,141,112,
   48,27,52,16,142,0,234,204,2,0,237,132,204,0,1,237,2,204,1,218,237,4,173,
   159,192,4,109,6,38,79,16,174,4,49,168,21,53,64,198,5,166,192,167,160,90,
   38,249,134,3,167,132,173,159,192,4,109,6,38,50,126,38,2,134,87,183,149,
   201,134,16,183,255,34,204,63,0,253,255,188,189,246,82,126,169,40,174,228,
   166,128,39,6,173,159,160,2,32,246,175,228,57,166,128,39,251,173,159,160,
   2,32,246,126,215,9,142,59,0,93,39,236,48,136,32,90,38,250,57,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,66,66,0
};

/* define program... */

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,count,item;
   char ch;
/* make these two static to avoid STACK OVERFLOW errors... */
   static char filename[MAXDIRS][29]; /* up to 25 filenames, 29 chars max */
   static long filestart[MAXDIRS]; /* up to 25 ptrs to where file ID sct is */
/* ah... much better. */
   int fpath,sides; /* filepath/disk information */
   char drive[7];
   char edit;

   DIR *dp; /* directory file pointer */
   DIRECT *dirbuf; /* directory buffer */

   device[0]=NULL;
   edit=FALSE;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            About();
         }
         if (argv[i][1]=='e') {
            edit=TRUE;
         }
      } else {
         strcpy(device,argv[i]);
      }
   }

/* rest of code goes here... */

   fputs("\nMultiBoot V1.04 by Allen Huffman (OS-9) and Terry Todd (RS-DOS).\n",stderr);
   fputs("Copyright (C) 1993,1994 by Sub-Etha Software.\n\n",stderr);

   setbuf(stdin,0);

   if (device[0]==NULL) {
      fputs("Enter path to disk containing bootfile(s) [ENTER=/D0] :",stdout);
      fflush(stdout);
      readln(STDIN,device,6);
      fputs("\n",stdout);
      for (i=1;i<6;i++) {
         if (device[i]==13) device[i]=0;
      }
   }
   if (device[0]==13) strcpy(device,"/D0");

   strcpy(drive,device); /* copy "device" to "drive" */
   strcat(drive,"@"); /* then add "@" to it for direct disk access */

   fpath=OpenDevice(S_IREAD+S_IWRITE); /* open disk... */

   sides=GetSides(fpath);

   if (BootCheck(fpath,sides)==FALSE) { /* if no boot present */
      if (edit==TRUE) {
         fputs("MultiBoot not installed.  Nothing to edit.\n",stderr);
         close(fpath);
         exit(0);
      }
      BootInstall(fpath,sides);
   }

   fprintf(stdout,"Attempting to open '%s'...",device);
   fflush(stdout);

   if (edit==TRUE) {
      BootEdit(fpath,sides);
      close(fpath);
      exit(0);
   }

   if ((dp=opendir(device))==NULL) {
      fputs("Unable to read directory...\n",stderr);
      close(fpath);
      exit(0);
   }
   count=0;
   while (count<MAXDIRS) { /* read up to MAXDIRS directory entries... */
      if ((dirbuf=readdir(dp))!=NULL) { /* if able to read entry, */
         if (dirbuf->d_name[0]=='.') continue; /* skip . entries */
         strcpy(filename[count],dirbuf->d_name); /* copy dirbuf to vars */
         filestart[count]=dirbuf->d_addr;
         count++;
      } else {
         break;
      }
   }
   fprintf(stdout,"%d directory entries processed...\n",count);
/*   fflush(stdout); */
   closedir(dp);

   if (count==0) {
      fputs("\nNo files were found.  Please copy bootfiles to an already cobbled\n",stdout);
      fputs("disk then re-run MultiBoot.\n",stdout);
      close(fpath);
      exit(0);
   }

/* here directory entries have been read...ask user stuff */

   for (i=0,item=0;((i<count) && (item<MAXBOOTS));i++) {
      fprintf(stdout,"\nFile Found: %29s - is this a bootfile? [y/N] :",filename[i]);
      ch=getchar(); /* get a character from user */
      ch=toupper(ch); /* make it uppercase */
      if (ch=='Y') {
         if (Process(fpath,filestart[i],item)) {
            item++;
         }
      }
   }
/*   fflush(stdout); */
   mb.delay=(TIMER*DELAY); /* default delay value */

   if (item>0) { /* if at least one file was selected, */
      fputs("\n\nEnter menu delay value (1-30 seconds) [ENTER=10] : ",stdout);
      fflush(stdout); /* re-use drive variable...why make a new one? */
      readln(STDIN,drive,6);
      if (drive[0]!=13) { /* if ENTER was not pressed, */
         mb.delay=atoi(drive)*TIMER; /* set delay */
      }
      fputs("\nUpdating MultiBoot information...",stdout);
      fflush(stdout);
      Update(fpath,sides);
   } else {
      fputs("\n\nMultiBoot: No bootfiles to add...  No changes made.\n",stdout);
   }
   close(fpath);
}

About() /* programing information and usage message... */
{
   fputs("\nMultiBoot V1.04 by Allen Huffman (OS-9) and Terry Todd (RS-DOS).\n",stderr);
   fputs("Copyright (C) 1993,1994 by Sub-Etha Software.\n\n",stderr);
   fputs("Syntax: MultiBoot [-opts] {device}\n",stderr);
   fputs("Usage : Creates an OS-9 boot disk with multiple boot files.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   fputs("        -e = edit existing MultiBoot information.\n",stderr);
   exit(0);
}

int OpenDevice(mode)
unsigned mode;
{
   int fpath;
   char drive[7];

   strcpy(drive,device); /* make "drive" = "device" */
   strcat(drive,"@"); /* append "@" ... */ 

   if ((fpath=open(drive,mode))==-1) { /* if not able to open disk... */
      fputs("OpenDevice: Unable to open device.\n",stderr);
      exit(0);
   }
   return(fpath);
}

int GetSides(fpath)
int fpath;
{
   char buffer[256];

   SeekSector(fpath,0); /* seek to disk ID sector */
   ReadBuffer(fpath,buffer,256); /* and read in the sector... */
   return((buffer[0x10] & 1)+1); /* return determined # of sides of disk */
}

SeekSector(fpath,sector) /* seek to sector */
int fpath,sector;
{
   if (lseek(fpath,sector*256l,0)==-1) { /* if not able to seek, */
      fputs("SeekSector: Unable to seek...\n",stderr);
      close(fpath);
      exit(0);
   }
}

ReadBuffer(fpath,buffer,buflen) /* read buffer */
int fpath;
char *buffer[];
int buflen;
{
   if (read(fpath,buffer,buflen)<1) { /* if unable to read, */
      fputs("ReadBuffer: Unable to read device...\n",stderr);
      close(fpath);
      exit(0);
   }
}

WriteBuffer(fpath,buffer,buflen)
int fpath;
char *buffer[];
int buflen;
{
   if (write(fpath,buffer,buflen)==-1) { /* if unable to write, */
      fputs("WriteBuffer: Unable to write to device...\n",stderr);
      close(fpath);
      exit(0);
   }
}

Allocate(fpath,track,sector,sides,range)
int fpath,track,sector,sides,range;
{
   char buffer[256];
   int i,lsn;
   int byte,bit;

   SeekSector(fpath,1); /* seek to bitmap sector */
   ReadBuffer(fpath,buffer,256); /* read first 256 bytes of bitmap */

/*   _dump("Bitmap BEFORE:",buffer,256); */
   lsn=TracktoLSN(track,sector,sides); /* convert track/sector to LSN */
   for (i=0;i<range;i++) { /* count through range */
      Bitmap(lsn+i,&byte,&bit); /* convert LSN to byte/bit */
      buffer[byte]=(buffer[byte] | bit); /* allocate bits */
   }
/*   _dump("Bitmap AFTER:",buffer,256); */
   SeekSector(fpath,1); /* seek back to bitmap sector */
   WriteBuffer(fpath,buffer,256); /* write back modified bitmap */
}

int TracktoLSN(track,sector,sides) /* return LSN from RS-DOS track/sector */
int track,sector,sides;
{
   return (track*18*(sides)+sector-1);
}

Bitmap(lsn,byte,bit)
int lsn,*byte,*bit;
{
   *byte=lsn/8;
   *bit=1;
   *bit=*bit<<(7-(lsn & 7));
}

BootCheck(fpath,sides)
{
   int i;
   char buffer[256];
   int lsn;

/* Check to see if disk is a bootable, cobbled disk... */

   lsn=TracktoLSN(34,1,sides); /* convert track/sector to LSN */
   SeekSector(fpath,lsn); /* seek to Track 34, Sector 1 (Boot Sector) */
   ReadBuffer(fpath,buffer,256); /* read entire track into "buffer" */

/* First check to see if MultiBoot code is alread installed... */

   if (memcmp(buffer,mb_sec34_1,CMPLEN)==0) { /* if MultiBoot installed, */
      return(TRUE); /* boot exists */
   } /* otherwise... */

/* If here, MB must not be.  Check to see if disk is bootable by looking for
   "OS" at start of boot sector. */

   if (memcmp(buffer,sec34_1,2)!=0) { /* if normal BOOT not installed, */
      fputs("BootCheck: MultiBoot can only be installed on an existing boot disk.\n",stderr);
      fputs("           Re-run on a bootable, cobbled disk.\n",stderr);
      close(fpath); /* close device, */
      exit(0); /* and exit... */
   } /* otherwise, */

   return(FALSE); /* no boot present */
}

BootInstall(fpath,sides)
int fpath,sides;
{
   char buffer[256];
   int i,lsn;
   int byte,bit;

   /* first, check to see if sectors are available... */
   fputs("Checking Sectors. ",stdout);
   fflush(stdout);
   SeekSector(fpath,1); /* seek to allocation map (sector 1) */
   ReadBuffer(fpath,buffer,256); /* read sector into "buffer" */
   lsn=TracktoLSN(33,14,sides); /* convert track/sector to LSN */
   for (i=0;i<5;i++) {
      Bitmap(lsn+i,&byte,&bit); /* convert lsn to byte/bit for bitmap */
      if ( (buffer[byte] & bit) >0 ) { /* if any bits are set, */
         fputs("BootInstall: Required sectors already allocated.  Re-run on a freshly formatted\n",stderr);
         fputs("             and cobbled disk.\n",stderr);
         close(fpath);
         exit(0);
      }
   } /* if here, sectors must be available.  Install MultiBoot... */ 

   /* first, move OS9 KERNEL... */
   fputs("Moving KERNEL. ",stdout);
   fflush(stdout);
   lsn=TracktoLSN(34,1,sides); /* convert track/sector to LSN */
   SeekSector(fpath,lsn); /* seek to Track 34, Sector 1 */
   ReadBuffer(fpath,buffer,256); /* read sector into "buffer" */
   lsn=TracktoLSN(33,14,sides); /* convert again... */
   SeekSector(fpath,lsn); /* seek to Track 33, Sector 14 */
   WriteBuffer(fpath,buffer,256); /* and write it there */
   /* at this point, KERNEL has been moved...since we are already in */
   /* position, let's just write out Terry's code... */
   fputs("Writing MultiBoot. ",stdout);
   fflush(stdout);
   WriteBuffer(fpath,mb_sec33_15,512);
   lsn=TracktoLSN(34,1,sides); /* and convert once more... */
   SeekSector(fpath,lsn); /* seek to boot sector */
   WriteBuffer(fpath,mb_sec34_1,50); /* and write out Terry's loader */

   /* if here, installation done.  Allocate sectors... */
   fputs("Allocating Sectors. ",stdout);
   fflush(stdout);
   Allocate(fpath,33,14,sides,5); /* allocate 5 sectors before boot track */
   /* if here, allocation map has been updated... */
   fputs("Done.\n\n",stdout);
}

int Process(fpath,lsn,item)
int fpath;
long lsn;
int item;
{
   int i;
   char buffer[256];

   for (i=0; i<256; i++) { /* clear out buffer before reading sectors */
      buffer[i]=0;
   }

   SeekSector(fpath,(int)lsn); /* seek to file ID sector */
   ReadBuffer(fpath,buffer,256); /* read sector in */
   if ((buffer[0x17]!=0) || (buffer[0x19]!=0)) { /* if not contig, */
      fputs("\nProcess: File fragmented.  This file cannot be added as a bootfile.\n",stderr);
      return(FALSE);
   } else { /* otherwise it IS contiguous... */
      fputs("\n\nEnter a description of this bootfile : ",stdout);
      fflush(stdout);

      readln(STDIN,mb.bf[item].desc,25);

      if ((mb.bf[item].desc[0])==13) return(FALSE); /* if no desc... */
  
      mb.choices=item+1; /* update # of bootfile choices */
/* Start of yucky code to compensate for Terry's print routine... */
      for (i=24; i>0; i--) {
         if (mb.bf[item].desc[i]==13) break;
         mb.bf[item].desc[i]=' '; /* pad with spaces */
      }
      mb.bf[item].desc[i]=' '; /* pad over user's ENTER */
/* End of yucky code to compensate for Terry's print routine... */
      mb.bf[item].desc[25]=13; /* add carraige return */
      mb.bf[item].desc[26]=0; /* and terminate with a NULL */
      mb.bf[item].info[0]=buffer[0x10]; /* copy START to info */ 
      mb.bf[item].info[1]=buffer[0x11];
      mb.bf[item].info[2]=buffer[0x12];
      mb.bf[item].info[3]=buffer[0x0b]; /* copy LENGTH to info */
      mb.bf[item].info[4]=buffer[0x0c];
      return(TRUE);
   }
}

Update(fpath,sides)
int fpath,sides;
{
   int lsn;

   lsn=TracktoLSN(33,17,sides);
   if (lseek(fpath,(long)lsn*256-3,0)==-1) { /* if unable to seek to boot, */
      fputs("\nUpdate: Unable to seek to boot sector...\n",stderr);
      return(FALSE);
   }
   WriteBuffer(fpath,&mb.delay,515); /* write updated MultiBoot info */

   fputs("Boot sector updated...\n",stdout);
}

BootEdit(fpath,sides)
int fpath,sides;
{
   int i,lsn,seconds,from,to;
   char input[3];
   char temp[32];
   char update;

   update=FALSE; /* start out with no update */

   fputs("Editing existing MultiBoot information...\n",stdout);

   lsn=TracktoLSN(33,17,sides);
   if (lseek(fpath,(long)lsn*256-3,0)==-1) {
      fputs("\nBootEdit: Unable to seek to boot sector...\n",stderr);
      return(FALSE);
   }
   ReadBuffer(fpath,&mb.delay,515); /* read all the stuff there... */

   if (mb.choices==0) return(FALSE);

   seconds=mb.delay/TIMER; /* convert back into seconds */

   fprintf(stdout,"\nEnter menu delay value (1-30 seconds) [ENTER=%d] : ",seconds);
   fflush(stdout);
   readln(STDIN,input,3);
   if (input[0]!=13) { /* if ENTER was not pressed, */
      mb.delay=atoi(input)*TIMER; /* set delay */
      update=TRUE;
   }

   while(mb.choices>1) {
      fputs("\n",stdout);
      for (i=0; i<mb.choices; i++) {
         printf("%2.2d - %s",i+1,mb.bf[i].desc);
      }
   
      fputs("\nEnter bootfile # to move [ENTER=Quit] :",stdout);
      fflush(stdout);
      readln(STDIN,input,3);
      from=atoi(input);
      if (from<1 || from>mb.choices) break;
   
      fputs("Exchange with bootfile # [ENTER=Quit] :",stdout);
      fflush(stdout);
      readln(STDIN,input,3);
      to=atoi(input);
      if (to<1 || to>mb.choices) break;

/* if here, swap them */
      from--;
      to--;

      memcpy(temp,mb.bf[to].desc,32); /* copy TO to TEMP */
      memcpy(mb.bf[to].desc,mb.bf[from].desc,32); /* copy FROM to TO */
      memcpy(mb.bf[from].desc,temp,32); /* copy TEMP to TO */
      update=TRUE; /* yes, we did change something */
   }

   if (update==TRUE) {
      fputs("\nWrite changes back to disk? [y/N] :",stdout);
      fflush(stdout);
      input[0]=getchar();
      input[0]=toupper(input[0]);

      if (input[0]=='Y') { /* if yes */
         fputs("\n\nUpdating MultiBoot information...",stdout);
         fflush(stdout);
         Update(fpath,sides);
         return(TRUE);
      }
   }
   fputs("\n\nMultiBoot not updated...\n",stdout);
   return(FALSE);
}
