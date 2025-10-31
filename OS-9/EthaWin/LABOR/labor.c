/*
 * Labor.c V0.00 by Allen C. Huffman
 * Copyright (C) 1995 by Sub-Etha Software
 *
 * 0.00 04/08/95 - Start.
 */

#include <stdio.h>
#include <modes.h>
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaglobals.h>

int *names[] = { "Jeremy", "Allen", "Kirsten", "Scott", "Reyna" };
#define NAMES 5

int *days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
#define DAYS 7

#define NAMELEN 9
#define NAMEROOM 10
struct {
   char name[NAMELEN]; /* name */
   int availin[7]; /* availability for 7 days in the week */
   int availout[7];
   int timein[7]; /* actual schedule */
   int timeout[7];
   char open[7]; /* can they open/close on this day? */
   char close[7];
   char set[7]; /* set schedule? */
   int minweek; /* min. hours needed per week */
   int maxweek; /* max */
   int minshift; /* min. hours per shift */
   int maxshift; /* max */
} emp[NAMEROOM];

char *items[] = { /* employee info prompt thingies */
   "Employee :",
   "Min/Week :",
   "Max/Week :",
   "Min/Shift:",
   "Max/Shift:"
};
#define ITEMS 5

#define FILE 0

#define EDIT 1
#define EDITADD 0
#define EDITDELETE 1
#define EDITEDIT 2
#define EDITSYSTEM 3

#define DAY 2
#define DAYSUNDAY 0
#define DAYMONDAY 1
#define DAYTUESDAY 2
#define DAYWEDNESDAY 3
#define DAYTHURSDAY 4
#define DAYFRIDAY 5
#define DAYSATURDAY 6

/* where things go on the screen */

#define DAYX 13
#define DAYY 3
#define DAYW 10
#define NAMEX 0
#define GRIDX 11
#define GRIDY 6

/* global system configuration variable thingies */

int minwarning;

main(argc,argv)
int argc;          /* argc = # of arguments on command line */
char *argv[];      /* argv[1-?] = argurments */
{
   int i,j,temp,start,day,oldday,name,oldname,employees;
   int wpath,fpath,menu,option;
   char ch,click;
   char makewin;
   char done,update;

   makewin=TRUE;

   for(i=1 ; i<argc ; i++) {
      if (argv[i][0]=='-') { /* is it a '-' option? */
         if (argv[i][1]=='?') {
            Usage();
         }
         /* v1.01 */
         if (argv[i][1]=='s') { /* -s (same screen) */
            makewin=FALSE; /* so don't create a new window to run on */
         }
      }
   }
   /* First, ya gotta initialize the EthaWin interface for this program */

   wpath=Startup(makewin); /* read config, create window, menu bar, etc. */

   /* Then display some stuff that you might want to display */

   TopText(wpath,"","Labor V0.00 - By Allen Huffman","");
   About(wpath); /* programmer's "about" routine */

   for (i=0; i<NAMES; i++) { /* initialize data... */
      strcpy(emp[i].name,names[i]);
      for (j=0; j<7; j++) {
         emp[i].availin[j]=9*60+30; /* available to come in at 9:30am */
         emp[i].availout[j]=21*60+30; /* can work until 9:30pm */
         emp[i].timein[j]=0;
         emp[i].timeout[j]=0;
         emp[i].set[j]=FALSE;
         emp[i].open[j]=TRUE; /* yes, can open */
         emp[i].close[j]=TRUE; /* yes, can close */
      }
      emp[i].minweek=20*60;
      emp[i].maxweek=40*60;
      emp[i].minshift=3*60;
      emp[i].maxshift=9*60;
   }
   employees=NAMES;

   for (i=0; i<DAYS; i++) { /* display days o' the week */
      PrintAt(wpath,DAYX+i*DAYW,DAYY,days[i]);
   }

   day=name=0;
   oldday=oldname=1;

   PrintAt(wpath,GRIDX,GRIDY-1," 7.. 8.. 9..10..11..12..13..14..15..16..17..18..19..20..21..22..23..");
   start=7*60; /* start at 7am */
   minwarning=1; /* alert if fewer than this many people sched. at a time */
   ShowEmp(wpath,employees);

   update=TRUE;
   do {
      ShowGraph(wpath,day,start,employees,minwarning);
      done=FALSE;
      for (i=0; i<employees; i++) { /* calc status thingies */
         temp=emp[i].timeout[day]-emp[i].timein[day];
         if (temp>emp[name].maxshift) {
            PrintAt(wpath,NAMEX,GRIDY+i,"+"); /* above max week */
         } else {
            if (temp<emp[name].minshift) {
               PrintAt(wpath,NAMEX,GRIDY+i,"-"); /* below min week */
            } else {
               PrintAt(wpath,NAMEX,GRIDY+i," "); /* just right... */
            }
         }
         temp=0; /* show over/under employees */
         for (j=0; j<DAYS; j++) {
            temp=temp+(emp[i].timeout[j]-emp[i].timein[j]);
         }
         if (temp>emp[i].maxweek) {
            PrintAt(wpath,NAMEX+NAMELEN+1,GRIDY+i,"+");
         }
         if (temp<emp[i].minweek) {
            PrintAt(wpath,NAMEX+NAMELEN+1,GRIDY+i,"-");
         }
      }
      do { /* and we are not done */
         if (oldday!=day) { /* if day has changed */
            PrintAt(wpath,DAYX+oldday*DAYW,DAYY,days[oldday]);
            ReVOn(wpath);
            PrintAt(wpath,DAYX+day*DAYW,DAYY,days[day]);
            ReVOff(wpath);
            oldday=day;
         }
         if (oldname!=name) { /* if name has changed */
            PrintAt(wpath,NAMEX+1,GRIDY+oldname,emp[oldname].name);
            ReVOn(wpath);
            PrintAt(wpath,NAMEX+1,GRIDY+name,emp[name].name);
            ReVOff(wpath);
            oldname=name;
         }   
         if (CheckMenu(wpath,&menu,&option,&ch,&click)) {
            if ( (menu==0) && (option==5) ) { /* if QUIT, */
               if (YesNo(wpath,"QUIT LABOR")) {
                  done=TRUE;
                  update=FALSE;
                  break;
               }
            }
            if (menu==EDIT) {
               switch(option) {
                  case EDITADD:
                     if (employees>=NAMEROOM) {
                        PopUp(wpath,"ALERT",30,8,20,5);
                        PrintAt(wpath,1,1,"No more room...");
                        Wait(wpath);
                        EndWin(wpath);
                        break;
                     }
                     EditEmp(wpath,employees);
                     employees++;
                     ShowEmp(wpath,employees);
                     oldname++;
                     if (oldname>=employees) oldname=0;
                     done=TRUE;
                     break;
                  case EDITDELETE:
                     if (employees<2) break;
                     if (!YesNo(wpath,"DELETE")) break;
                     if (name==employees-1) { /* if last employee */
                        employees--;
                     } else {
                        for (i=name; i<employees; i++) {
                           strcpy(emp[i].name,emp[i+1].name);
                           emp[i].minweek=emp[i+1].minweek;
                           emp[i].maxweek=emp[i+1].maxweek;
                           emp[i].minshift=emp[i+1].minshift;
                           emp[i].maxshift=emp[i+1].maxshift;
                           for (j=0; j<DAYS; j++) {
                              emp[i].availin[j]=emp[i+1].availin[j];
                              emp[i].availout[j]=emp[i+1].availout[j];
                              emp[i].timein[j]=emp[i+1].timein[j];
                              emp[i].timeout[j]=emp[i+1].timeout[j];
                              emp[i].open[j]=emp[i+1].open[j];
                              emp[i].close[j]=emp[i+1].close[j];
                           }
                        }
                        employees--;
                     }
                     ShowEmp(wpath,employees);
                     CurXY(wpath,NAMEX,GRIDY+employees);
                     ErLine(wpath);
                     CurXY(wpath,NAMEX,GRIDY+employees+1);
                     ErLine(wpath);
                     name--;
                     if (name<0) name=0;
                     oldname=name;
                     ReVOn(wpath);
                     PrintAt(wpath,NAMEX+1,GRIDY+name,emp[name].name);
                     ReVOff(wpath);
                     done=TRUE;
                     break;
                  case EDITEDIT:
                     EditEmp(wpath,name);
                     ShowEmp(wpath,employees);
                     oldname++;
                     if (oldname>=employees) oldname=0;
                     done=TRUE;
                     break;
                  case EDITSYSTEM:
                     EditSystem(wpath);
                     done=TRUE;
                     break;
               }
            }
            if (menu==DAY) {
               day=option;
               done=TRUE; /* update screen */
               break;
            }
         } /* end of "if (CheckMenu)" */
         switch(ch) {
            case LEFT:
               day--;
               if (day<0) day=DAYS-1;
               done=TRUE;
               break;
            case RIGHT:
               day++;
               if (day>DAYS-1) day=0;
               done=TRUE;
               break;
            case UP:
               name--;
               if (name<0) name=employees-1;
               break;
            case DOWN:
               name++;
               if (name>employees-1) name=0;
               break;
            case ENTER:
               GetHours(wpath,name,day);
               done=TRUE;
               break;
         }
      } while(!done); /* end of "do" */
   } while(update); /* update? */

   ShutDown(wpath); /* Goodbye, EthaWin... */
}

ShowEmp(wpath,employees)
{
   int i;

   for (i=0; i<employees; i++) { /* display employee names */
      PrintAt(wpath,NAMEX+1,GRIDY+i,"         "); /* erase, erase, erase */
      PrintAt(wpath,NAMEX+1,GRIDY+i,emp[i].name);
   }   
}

EditSystem(wpath)
int wpath;
{
   char line[4];
   
   PopUp(wpath,"Edit System",10,5,60,5);
   LineInput(wpath,1,1,"Minimum Employees Desired On-Duty at a time? ",line,2);
   if (line[0]!=NULL) minwarning=atoi(line);
   EndWin(wpath);
}

EditEmp(wpath,name)
int wpath,name;
{
   int i;
   char line[NAMELEN];

   PopUp(wpath,"Edit Employee",10,5,60,12);
   PrintAt(wpath,20,1,"Day  In-Avail-Out  Set?  Open? Close?");
   for (i=0; i<ITEMS; i++) {
      PrintAt(wpath,1,i+2,items[i]);
   }
   PrintAt(wpath,11,2,emp[name].name);
   PrintAt(wpath,11,3,IntToTime(emp[name].minweek,line));
   PrintAt(wpath,11,4,IntToTime(emp[name].maxweek,line));
   PrintAt(wpath,11,5,IntToTime(emp[name].minshift,line));
   PrintAt(wpath,11,6,IntToTime(emp[name].maxshift,line));
   
   for (i=0; i<DAYS; i++) {
      PrintAt(wpath,20,i+2,days[i]);
      PrintAt(wpath,25,i+2,IntToTime(emp[name].availin[i],line));
      PrintAt(wpath,32,i+2,IntToTime(emp[name].availout[i],line));
      if (emp[name].set[i]) {
         PrintAt(wpath,40,i+2,"Yes");
      } else {
         PrintAt(wpath,40,i+2,"No");
      }
      if (emp[name].open[i]) {
         PrintAt(wpath,46,i+2,"Yes");
      } else {
         PrintAt(wpath,46,i+2,"No");
      }
      if (emp[name].close[i]) {
         PrintAt(wpath,52,i+2,"Yes");
      } else {
         PrintAt(wpath,52,i+2,"No");
      }
   }
   LineInput(wpath,11,2,"",line,NAMELEN);
   if (line[0]!=NULL) strcpy(emp[name].name,line);
   InputTime(wpath,11,3,&emp[name].minweek);
   InputTime(wpath,11,4,&emp[name].maxweek);
   InputTime(wpath,11,5,&emp[name].minshift);
   InputTime(wpath,11,6,&emp[name].maxshift);

   for (i=0; i<DAYS; i++) {
      InputTime(wpath,25,i+2,&emp[name].availin[i]);
      InputTime(wpath,32,i+2,&emp[name].availout[i]);
      InputYesNo(wpath,40,i+2,&emp[name].set[i]);
      InputYesNo(wpath,46,i+2,&emp[name].open[i]);
      InputYesNo(wpath,52,i+2,&emp[name].close[i]);      
   }
   EndWin(wpath);
}

InputYesNo(wpath,x,y,bool)
int wpath,x,y;
char bool;
{
   char ch;

   LineInput(wpath,x,y,"",&ch,1);
   if (ch!=NULL) {
      ch=toupper(ch);
      if (ch=='Y') {
         bool=TRUE;
         PrintAt(wpath,x,y,"Yes");
      } else {
         bool=FALSE;
         PrintAt(wpath,x,y,"No ");
      }
   }
}

InputTime(wpath,x,y,value)
int wpath,x,y,*value;
{
   char line[6];

   LineInput(wpath,x,y,"",line,5);
   if (line[0]!=NULL) {
      *value=TimeToInt(line);
      PrintAt(wpath,x,y,IntToTime(*value,line));
   }
}

IntToTime(time,text)
int time;
char *text;
{
   int i,hours,minutes;
   
   strcpy(text,"00:00");
   hours=time/60;
   minutes=time-hours*60;

   i=minutes;
   while (i>0) {
      if (text[4]=='9') {
         text[3]++;
         text[4]='0';
      } else {
         text[4]++;
      }
      i--;
   }
   i=hours;
   while (i>0) {
      if (text[1]=='9') {
         text[0]++;
         text[1]='0';
      } else {
         text[1]++;
      }
      i--;
   }
   return(text); /* return a pointer to the string */
}

int TimeToInt(text)
char *text; /* pointer to ASCII time string */
{
   int i,hour,minute;
   char time[6];

   strncpy(time,text,6); /* copy over */
   for (i=0; i<6; i++) {
      if ((time[i]<'0') || (time[i]>'9')) { /* found delimeter? */
         time[i]=0; /* null */
         hour=atoi(time);
         minute=atoi(time+i+1);
         break;
      }
   }
   return (hour*60+minute);
}

GetHours(wpath,name,day)
int wpath,name,day;
{
   int i;
   char line[6];

   PopUp(wpath,"Enter Schedule",30,10,20,6);
   PrintAt(wpath,1,1,"Time In  :");
   PrintAt(wpath,12,1,IntToTime(emp[name].timein[day],line));
   PrintAt(wpath,1,2,"Time Out :");
   PrintAt(wpath,12,2,IntToTime(emp[name].timeout[day],line));
   InputTime(wpath,12,1,&emp[name].timein[day]);
   InputTime(wpath,12,2,&emp[name].timeout[day]);

   EndWin(wpath);
}

ShowGraph(wpath,day,start,employees,minwarning)
int wpath,day,start,employees,minwarning;
{
   int i,j;
   char count;

   for (j=0; j<68; j++) {
      count='0';
      for (i=0; i<employees; i++) {
      CurXY(wpath,GRIDX+j,GRIDY+i);
         if ((emp[i].timein[day]<=start+j*15) && (emp[i].timeout[day]>start+j*15)) {
            write(wpath,"X",1);
            if (count<='9') {
               count++;
            }
         } else {
            write(wpath,".",1);
         }
      }
      CurXY(wpath,GRIDX+j,GRIDY+i);
      if (count<'0'+minwarning) {
         ReVOn(wpath);
         write(wpath,&count,1);
         ReVOff(wpath);
      } else {
         write(wpath,&count,1);
      }
   }
}

Usage()
{
   fputs("\nLabor V0.00 by Allen Huffman (coco-sysop@genie.geis.com)\n",stderr);
   fputs("Copyright (C) 1995 by Sub-Etha Software\n\n",stderr);
   fputs("Syntax: Labor [-opts]\n",stderr);
   fputs("Usage : EthaWin employee employee schedule maker.\n",stderr);
   fputs("Opts  : -? = display this message.\n",stderr);
   fputs("        -s = use existing screen.\n",stderr);
   exit(0);
}
