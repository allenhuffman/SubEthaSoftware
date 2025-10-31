/*
 * MultiBoot.c V0.04 by Allen C. Huffman
 * Copyright (C) 1993 by Sub-Etha Software
 *
 * OS9 portion of MultiBoot utility...  RS-DOS portion by Terry Todd.
 *
 * 0.01 7/06/93 - Initial test version
 * 0.02 7/12/93 - Changes for up to 16 bootfiles (OS9BOOT V1.1)
 * 0.03 7/15/93 - Addition of self-installation & bug bashing.
 * 0.04         - and removal of bulky default boot desc. (unused).
 */

#include <stdio.h>
#include <modes.h>
#include <dir.h>

#include "stdinc.h"

#define MAXBOOTS 16 /* maximum number of bootfiles to allow */
#define MAXDIRS 25 /* maximum number of directory entries to read */
#define DELAY 30 /* default delay value for menu (in seconds) */

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

char mb_sec34_1[] = { /* Terry's RS-DOS DOS startup code... 50 bytes */
   79,83,134,13,151,211,204,56,0,221,209,12,211,142,0,234,204,2,0,237,132,
   134,33,214,211,237,2,220,209,237,4,173,159,192,4,109,6,38,8,76,129,61,37,
   221,126,57,0,126,215,9
};

char mb_sec33_15[] = { /* Terry's RS-DOS MultiBoot code... 1024 Bytes */
   142,56,0,16,142,38,0,236,129,237,161,140,57,0,37,247,23,0,247,23,1,10,
   32,32,32,32,32,32,32,32,77,117,108,116,105,66,111,111,116,32,86,49,46,49,
   13,32,98,121,32,84,101,114,114,121,32,84,111,100,100,32,38,32,65,108,108,
   101,110,32,72,117,102,102,109,97,110,13,32,32,32,32,32,67,111,112,121,114,
   105,103,104,116,32,40,67,41,32,49,57,57,51,32,98,121,13,32,32,32,32,32,32,
   32,83,117,98,45,69,116,104,97,32,83,111,102,116,119,97,114,101,13,0,16,142,
   5,192,16,159,136,23,0,149,32,32,32,32,32,85,115,101,32,85,80,47,68,79,87,
   78,32,116,111,32,115,99,114,111,108,108,13,32,91,69,78,84,69,82,93,32,83,
   101,108,101,99,116,115,32,32,91,66,82,69,65,75,93,32,81,117,105,116,115,
   0,23,0,128,92,141,112,48,27,52,16,142,0,234,204,2,0,237,132,204,0,1,237,
   2,204,1,218,237,4,173,159,192,4,109,6,38,79,16,174,4,49,168,21,53,64,198,
   5,166,192,167,160,90,38,249,134,3,167,132,173,159,192,4,109,6,38,50,126,
   38,2,134,87,183,149,201,134,16,183,255,34,204,63,0,253,255,188,189,246,
   82,126,169,40,174,228,166,128,39,6,173,159,160,2,32,246,175,228,57,166,
   128,39,251,173,159,160,2,32,246,126,215,9,142,59,0,93,39,236,48,136,32,
   90,38,250,57,246,58,230,141,238,206,4,164,246,58,255,240,58,230,193,8,37,
   2,198,8,52,4,223,136,189,58,47,48,5,53,4,51,200,32,90,38,239,182,58,229,
   176,58,230,198,32,61,195,4,163,31,1,134,106,167,132,16,190,58,253,49,63,
   38,5,50,98,22,255,122,173,159,160,0,39,241,198,96,231,132,198,255,129,94,
   39,15,129,10,39,34,129,13,39,60,129,3,38,197,50,98,57,182,58,229,39,189,
   247,1,85,74,183,58,229,177,58,230,36,3,122,58,230,22,255,134,182,58,229,
   76,177,58,255,36,162,247,1,86,183,58,229,182,58,230,139,7,177,58,229,36,
   228,124,58,230,32,223,246,58,229,57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,66,66,0
/*                   ^ was 16... Deleted non-used default boot descs:

,32,49,46,32,79,114,105,103,105,110,97,108,32,79,83,
   45,57,32,66,111,111,116,32,32,32,13,0,0,0,0,0,0,32,50,46,32,82,101,115,
   101,114,118,101,100,32,66,111,111,116,32,84,119,111,32,32,32,32,13,0,0,
   0,0,0,0,32,51,46,32,82,101,115,101,114,118,101,100,32,66,111,111,116,32,
   84,104,114,101,101,32,32,13,0,0,0,0,0,0,32,52,46,32,82,101,115,101,114,
   118,101,100,32,66,111,111,116,32,70,111,117,114,32,32,32,13,0,0,0,0,0,0,
   32,53,46,32,82,101,115,101,114,118,101,100,32,66,111,111,116,32,70,105,
   118,101,32,32,32,13,0,0,0,0,0,0,32,54,46,32,82,101,115,101,114,118,101,
   100,32,66,111,111,116,32,83,105,120,32,32,32,32,13,0,0,0,0,0,0,32,55,46,
   32,82,101,115,101,114,118,101,100,32,66,111,111,116,32,83,101,118,101,110,
   32,32,13,0,0,0,0,0,0,32,56,46,32,82,101,115,101,114,118,101,100,32,66,111,
   111,116,32,78,105,110,101,32,32,32,13,0,0,0,0,0,0,32,57,46,32,82,101,115,
   101,114,118,101,100,32,66,111,111,116,32,78,105,110,101,32,32,32,13,0,0,0,
   0,0,0,49,48,46,32,82,101,115,101,114,118,101,100,32,66,111,111,116,32,84,
   101,110,32,32,32,32,13,0,0,0,0,0,0,49,49,46,32,82,101,115,101,114,118,101,
   100,32,66,111,111,116,32,69,108,101,118,101,110,32,13,0,0,0,0,0,0,49,50,46,
   32,82,101,115,101,114,118,101,100,32,66,111,111,116,32,84,119,101,108,118,
   101,32,13,0,0,0,0,0,0,49,51,46,32,82,101,115,101,114,118,101,100,32,66,111,
   111,116,32,84,104,105,114,116,101,101,13,0,0,0,0,0,0,49,52,46,32,82,101,
   115,101,114,118,101,100,32,66,111,111,116,32,70,111,117,114,116,101,101,13,
   0,0,0,0,0,0,49,53,46,32,82,101,115,101,114,118,101,100,32,66,111,111,116,
   32,70,105,102,116,101,101,110,13,0,0,0,0,0,0,49,54,46,32,82,101,115,101,
   114,118,101,100,32,66,111,111,116,32,83,105,120,116,101,101,110,13,0,0,0,
   0,0,0 */
};

/* define program... */

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,count,item;
   char ch;
   char filename[MAXDIRS][29]; /* up to 25 filenames, 29 chars max */
   long filestart[MAXDIRS]; /* up to 25 pointers to where file ID sct is */
   int fpath,sides; /* filepath/disk information */
   char drive[7];

   DIR *dp; /* directory file pointer */
   DIRECT *dirbuf; /* directory buffer */

   device[0]=NULL;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            About();
         }
      } else {
         strcpy(device,argv[i]);
      }
   }

/* rest of code goes here... */

   fputs("\nMultiBoot V0.04 by Allen Huffman (OS-9) and Terry Todd (RS-DOS).\n",stderr);
   fputs("Copyright (C) 1993 by Sub-Etha Software.\n\n",stderr);

   if (device[0]==NULL) {
      fputs("Enter path to disk containing bootfile(s) [ENTER=/D0] :",stdout);
      fflush(stdout);
      if (readln(STDIN,device,6)<1) { /* if error */
         fputs("Error reading keyboard.\n",stderr);
         exit(0);
      }
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

   BootCheck(fpath,sides); /* check for and install if necessary/possible */

   fprintf(stdout,"Attempting to open '%s'...",device);
   fflush(stdout);

   if ((dp=opendir(device))==NULL) {
      fputs("Unable to read directory...\n",stderr);
      close(fpath);
      exit(0);
   }
   count=0;
   while (count<MAXDIRS+1) { /* read up to MAXDIRS directory entries... */
      if ((dirbuf=readdir(dp))!=NULL) { /* if able to read entry, */
         if (dirbuf->d_name[0]=='.') continue; /* skip . entries */
         strcpy(filename[count],dirbuf->d_name); /* copy dirbuf to vars */
         filestart[count]=dirbuf->d_addr;
         count++;
      } else {
         break;
      }
   }
   fprintf(stdout,"%d directory entries processed...\n\n",count);
   closedir(dp);

   if (count==0) {
      fputs("No files were found.  Please copy bootfiles to an already cobbled\n",stdout);
      fputs("disk then re-run MultiBoot.\n",stdout);
      close(fpath);
      exit(0);
   }

/* here directory entries have been read...ask user stuff */

   setbuf(stdin,0);

   for (i=0,item=0;((i<count) && (item<MAXBOOTS+1));i++) {
      fprintf(stdout,"File Found: %29s - is this a bootfile? [y/N] :",filename[i]);
      ch=getchar(); /* get a character from user */
      ch=toupper(ch); /* make it uppercase */
      fprintf(stdout,"\n");
      if (ch=='Y') {
         if (Process(fpath,filestart[i],item)) {
            item++;
         }
      }
   }
   fflush(stdout);
   mb.delay=(0x1725*DELAY); /* default delay value */

   if (item>0) { /* if at least one file was selected, */
      fputs("\nEnter menu delay value (in seconds) [ENTER=10] : ",stdout);
      fflush(stdout); /* re-use drive variable...why make a new one? */
      if (readln(STDIN,drive,6)<1) { /* if error */
         fputs("Error reading keyboard.\n",stderr);
         close(fpath);
         exit(0);
      }
      if (drive[0]!=13) { /* if ENTER was not pressed, */
         mb.delay=atoi(drive)*1725; /* set delay */
      }
      fputs("\nUpdating MultiBoot information...",stdout);
      Update(fpath,sides);
   } else {
      fputs("\nError - No bootfiles to add...  No changes made.\n",stdout);
   }
   close(fpath);
}

About() /* programing information and usage message... */
{
   fputs("\nMultiBoot V0.04 by Allen Huffman (OS-9) and Terry Todd (RS-DOS).\n",stderr);
   fputs("Copyright (C) 1993 by Sub-Etha Software.\n\n",stderr);
   fputs("Syntax: MultiBoot [-opts] {device}\n",stderr);
   fputs("Usage : Creates an OS-9 boot disk with multiple boot files.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
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

   if (memcmp(buffer,mb_sec34_1,20)==0) { /* if MultiBoot installed, */
      return;
   } /* otherwise... */

   if (memcmp(buffer,sec34_1,20)!=0) { /* if normal BOOT not installed, */
      fputs("Error - MultiBoot can only be installed on an existing boot disk.\n",stderr);
      fputs("        Re-run on a bootable, cobbled disk.\n",stderr);
      close(fpath); /* close device, */
      exit(0); /* and exit... */
   } /* otherwise, */

   BootInstall(fpath,sides); /* install MultiBoot... */
}

/*
 * Note:  Need to fix this so it checks ALL the sectors and not
 *        just the FIRST one!!!
 */

BootInstall(fpath,sides)
int fpath,sides;
{
   char buffer[256];
   int i,lsn;
   int byte,bit;

   /* first, check to see if sectors are available... */
   fputs("Checking Sectors. ",stdout);
   SeekSector(fpath,1); /* seek to allocation map (sector 1) */
   ReadBuffer(fpath,buffer,256); /* read sector into "buffer" */
   lsn=TracktoLSN(33,14,sides); /* convert track/sector to LSN */
   for (i=0;i<5;i++) {
      Bitmap(lsn+i,&byte,&bit); /* convert lsn to byte/bit for bitmap */
      if ( (buffer[byte] & bit) >0 ) { /* if any bits are set, */
         fputs("Error - Required sectors already allocated.  Re-run on a freshly formatted\n",stderr);
         fputs("        and cobbled disk.\n",stderr);
         close(fpath);
         exit(0);
      }
   } /* if here, sectors must be available.  Install MultiBoot... */ 

   /* first, move OS9 KERNEL... */
   fputs("Moving KERNEL. ",stdout);
   lsn=TracktoLSN(34,1,sides); /* convert track/sector to LSN */
   SeekSector(fpath,lsn); /* seek to Track 34, Sector 1 */
   ReadBuffer(fpath,buffer,256); /* read sector into "buffer" */
   lsn=TracktoLSN(33,14,sides); /* convert again... */
   SeekSector(fpath,lsn); /* seek to Track 33, Sector 14 */
   WriteBuffer(fpath,buffer,256); /* and write it there */
   /* at this point, KERNEL has been moved...since we are already in */
   /* position, let's just write out Terry's code... */
   fputs("Writing MultiBoot. ",stdout);
   WriteBuffer(fpath,mb_sec33_15,512);
   lsn=TracktoLSN(34,1,sides); /* and convert once more... */
   SeekSector(fpath,lsn); /* seek to boot sector */
   WriteBuffer(fpath,mb_sec34_1,50); /* and write out Terry's loader */

   /* if here, installation done.  Allocate sectors... */
   fputs("Allocating Sectors. ",stdout);
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
      fputs("\nError - File fragmented.  This file cannot be added as a bootfile.\n",stderr);
      return(FALSE);
   } else { /* otherwise it IS contiguous... */
      fputs("\nEnter a description of this bootfile : ",stdout);
      fflush(stdout);

      if (readln(STDIN,mb.bf[item].desc,25)<1) { /* if error */
         fputs("Unable to read keyboard.\n",stderr);
         return(FALSE);
      }
      fputs("\n",stdout);

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
   char buffer[256];

   lsn=TracktoLSN(33,17,sides);
   if (lseek(fpath,(long)lsn*256-3,0)==-1) { /* if unable to seek to boot sector, */
      fputs("\nUpdate: Unable to seek to boot sector...\n",stderr);
      return(FALSE);
   }
   WriteBuffer(fpath,&mb.delay,515); /* write updated MultiBoot info */

   fputs("Boot sector updated...\n",stdout);
}
