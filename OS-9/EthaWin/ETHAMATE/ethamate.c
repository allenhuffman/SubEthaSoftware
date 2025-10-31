/*
 * EthaMate.c 0.00 - 2/28/93
 * by Allen C. Huffman of Sub-Etha Software
 *
 * 
 */

#include <stdio.h>
#include <sgstat.h>

#include "stdinc.h"

char version[5]="0.00"; /* Version number... */

#define UP 12
#define DOWN 10
#define LEFT 8
#define RIGHT 9
#define ENTER 13

/* Some defines used by the windowing system... */

#define PATH 1
#define WHITE 0
#define BLACK 2
#define LGREY 4
#define DGREY 3
#define MAXOPTS 8 /* maximum menu options... */

/* Global variables for pull-down menus... */

int menu_x[]= { /* X positions for 3 menus */
   9,15,26
};

int menu_y[]= { /* Y positions for 3 menus */
   2,2,2
};

int menu_w[]= { /* Width for 3 menus */
   21,14,12
};

int menu_h[]= { /* Height for 3 menus */
   9,5,10
};

int menu_stat[][MAXOPTS]= { /* status flags for up to 8 options, 3 menus */
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   1,1,0,0,2,2,0,3
};

char *menu_chars[] = { /* hot-keys for up to 8 options, 3 menus */
   "CDGNRUE",
   "CDM",
   "12345678"
};

char *menu_text[][MAXOPTS] = { /* and option text... */
   "\x1f\x22C\x1f\x23opy...",
   "\x1f\x22D\x1f\x23elete...",
   "\x1f\x22G\x1f\x23et Info...",
   "Re\x1f\x22n\x1f\x23ame...",
   "\x1f\x22R\x1f\x23un...",
   "\x1f\x22U\x1f\x23pdate Screen  ^U",
   "\x1f\x22E\x1f\x23xit          ESC",
   "",

   "\x1f\x22C\x1f\x23hange...",
   "\x1f\x22D\x1f\x23elete...",
   "\x1f\x22M\x1f\x23akeDir...",
   "","","","","",

   "Option \x1f\x221\x1f\x23",
   "Option \x1f\x222\x1f\x23",
   "Option \x1f\x223\x1f\x23",
   "Option \x1f\x224\x1f\x23",
   "Option \x1f\x225\x1f\x23",
   "Option \x1f\x226\x1f\x23",
   "Option \x1f\x227\x1f\x23",
   "Option \x1f\x228\x1f\x23"
};

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i;
   int option;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            printf("\nEthaMate V%s by Allen Huffman of Sub-Etha Software (SysOp@Delta/StG)\n",version);
            printf("Syntax: EthaMate [-opts]\nUsage : Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah Blah\n");
            printf("Usage : I don't know yet...\n");
            printf("Opts  : -? = display this message.\n");
            exit(0);
         }
      }
   }
   /* Begin Program */
   setbuf(stdin,0); /* no input buffer...one char at a time */
   CurOff(PATH);
   system("tmode -echo");

   Clear(PATH);
   MenuBar();
   About();

/* Screen setup, scan for menu option... */

   i=0; /* first menu default */
   for (;;) { /* "endless" loop */
      option=DoMenu(i);
      if (option==40) {
         i--; /* go back... */
         if (i<0) i=2;
      }
      if (option==41) {
         i++; /* go next... */
         if (i>2) i=0;
      }
      if (option==0) break; /* select first option to quit this */
   }
   system("tmode echo");
   CurOn(PATH);
}

About()
{
   PopUp("ABOUT",20,5,40,8);

   getchar();
   EndWin();
}

MenuBar()
{
   FColor(PATH,0);
   ReVOn(PATH);
   write(PATH,"                       EthaMate V0.00 by Allen C. Huffman                       ",80);
   ReVOff(PATH);
   
   FColor(PATH,4);
   ReVOn(PATH);
   write(PATH,"  \x1f\x22A\x1f\x23bout  \x1f\x22F\x1f\x23ile  \x1f\x22D\x1f\x23irectory  Desk\x1f\x22t\x1f\x23op                                         \x1f\x22H\x1f\x23elp  ",100);
   ReVOff(PATH);
   FColor(PATH,0);
}

PopUp(title,x,y,w,h)
char *title;
int x,y,w,h;
{
   OWSet(PATH,1,x+1,y+1,w,h,0,DGREY);
   OWSet(PATH,1,x,y,w,h,BLACK,WHITE);
   CurXY(PATH,w/2-strlen(title)+2,0);
   cwrite(PATH,title,80);
   CWArea(PATH,1,1,w-2,h-2);
   FColor(PATH,WHITE);
   BColor(PATH,BLACK);
   Clear(PATH);
}

EndWin()
{
   OWEnd(PATH);
   OWEnd(PATH);
}

PullDown(x,y,w,h)
int x,y,w,h;
{
   OWSet(PATH,1,x+1,y+1,w,h,0,DGREY);
   OWSet(PATH,1,x,y,w,h,BLACK,WHITE);
   CWArea(PATH,1,1,w-1,h-1);
}

/*
 * Neat DoMenu Routine...
 */

int DoMenu(num)
int num;
{
   int i;
   int max_opts=strlen(menu_chars[num]); /* how many menu options */
   int option; /* selected option... */
   char select[1]; /* pointer to selected/deselected character */
   char ch; /* keypress... */
   int pos;
   int comnum;

   option=42; /* initial option not initialized... */

   PullDown(menu_x[num],menu_y[num],menu_w[num],menu_h[num]);
   for (i=0;i<max_opts;i++) {
      if (menu_stat[num][i] & 2) { /* if flagged item */
         select[0]=174;
      } else {
         select[0]=' ';
      }
      if (menu_stat[num][i] & 1) { /* if non-selectable item */
         FColor(PATH,LGREY); /* so make it dark...*/
         printf("%s%s\n",select,menu_text[num][i]);
         FColor(PATH,BLACK);
      } else {
         printf("%s%s\n",select,menu_text[num][i]);
         if (option==42) option=i;
      }
   }
   /* at this point, pull down menu has been displayed... */
   for (;;) { /* "endless" loop */
      CurXY(PATH,1,option); /* move to current option */
      ReVOn(PATH); /* reverse on */
      printf("%s",menu_text[num][option]); /* print highlighted option */
      fflush(stdout);
      ReVOff(PATH); /* return to normal */
   
      ch=getchar(); /* read a character */
      CurXY(PATH,1,option);
      printf("%s",menu_text[num][option]);
      fflush(stdout);
      if (ch==ENTER) break; /* ENTER pressed */
      if (ch==UP) { /* UP pressed */
       do {
         option--; /* decrement option */
         if (option==-1) option=max_opts-1;
       } while (menu_stat[num][option] & 1); /* find next available opt */
      }
      if (ch==DOWN) { /* DOWN pressed */
       do {
         option++; /* increment option */
         if (option==max_opts) option=0;
       } while (menu_stat[num][option] & 1); /* fine next available opt */
      }
      if (ch==LEFT) { /* LEFT pressed */
         option=40; /* flag for LEFT */
         break;
      }
      if (ch==RIGHT) { /* RIGHT pressed */
         option=41; /* flag for RIGHT */
         break;
      }
      /* If here, must not be a special character.  Check for hot-key... */
      toupper(ch); /* make sure it's uppercase */
      pos=index(menu_chars[num],ch); /* pos points to where 'ch' is */
      comnum=pos-menu_chars[num][0]; /* subtract from start of string */
      if (comnum>0) option=comnum;
   }
   EndWin();
   return(option);
}
