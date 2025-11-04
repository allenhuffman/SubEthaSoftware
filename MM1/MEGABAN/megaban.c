/* MegaBanners.c V1.01 by Allen C. Huffman
 * Copyright (C) 1995 by Sub-Etha Software
 *
 * 0.00 03/21/95 - In the beginning. . .
 *		03/25/95 - There was code . . . that worked!  :)  [sorta...]
 *		03/29/95 - Installation of font Chooser() routine! (updated .l, too)
 * 1.00 04/03/95 - L/R/C added, NEW, Print, TAB select
 * 1.01 05/19/95 - Updates: LAYOUT menu, print usage on invalid option,
 *		07/13/96   more work to make it work...
 */

#include <stdio.h>
#include <modes.h>
#include <dir.h>

#include "mfonts.h"
#include <ETHAWIN/ethawin.h>
#include <ETHAWIN/ethaglobals.h>

#define ERROR -1

#define FILE 0
#define FILE_NEW 0
#define FILE_OPEN 1
#define FILE_SAVE 2
#define FILE_SAVE_AS 3
#define FILE_PRINT 4
#define FILE_QUIT 5

/* v1.01 */
#define LAYOUT 1
#define LAYOUT_LANDSCAPE 0
#define LAYOUT_PORTRAIT 1
#define LAYOUT_NOTHING 2
#define LAYOUT_CONTINUOUS 3
#define LAYOUT_8x11 4
#define LAYOUT_8x14 5
#define LAYOUT_CUSTOM 6

#define TEXT 2
#define TEXT_CENTER 0
#define TEXT_COLOR 1
#define TEXT_DELETE 2
#define TEXT_ENTER 3
#define TEXT_FONT 4
#define TEXT_VIEW 5

#define START_X 0
#define START_Y 31
#define WIDTH (639-START_X)
#define HEIGHT (207-START_Y)
#define MAX 20
#define MAXWIDTH 135
#define MAXFONTS 40
#define LINELEN 80

#define LANDSCAPE 0
#define PORTRAIT 1

#define MAX_LANDSCAPE WIDTH
#define MAX_PORTRAIT HEIGHT

int text_x[MAX],text_y[MAX],text_w[MAX],text_h[MAX];
int text_font[MAX],text_j[MAX];
char text[MAX][MAXWIDTH];
char fontname[MAXFONTS][25];

extern char orgfont[],printdev[];
extern char bannerchar,layout,linefeeds;
extern int pagelength,printwidth,spacing;

main(argc,argv)
int argc;			 /* argc = # of arguments on command line */
char *argv[];		/* argv[1-?] = argurments */
{
	int i,j,k,x,y;
	int opath,wpath,menu,option;
	char ch,click;
	char makewin,done,update;
	int obj,sel;
	static char line[LINELEN];
	DIR *dp;
	struct direct *dirbuf;
	int font,fonts;
	int box_x,box_y,box_w,box_h;

	makewin=TRUE;
	linefeeds=TRUE;
	update=TRUE;

	for(i=1 ; i<argc ; i++) {
		if (argv[i][0]=='-') { /* is it a '-' option? */
			if (argv[i][1]=='?') {
				Usage();
			}
			/* v1.01 */
			if (argv[i][1]=='s') { /* -s (same screen) */
				makewin=FALSE; /* so don't create a new window to run on */
				continue; /* v1.01 */
			}
			Usage(); /* 1.01 else unknown option, print usage */
		}
	}
	wpath=Startup(makewin); /* read config, create window, menu bar, etc. */

	/* Then display some stuff that you might want to display */
 
	TopText(wpath,"","MegaBanners V1.01 - By Allen Huffman","");
	About(wpath); /* programmer's "about" routine */

	PopUp(wpath,"",26,9,28,5);
	PrintAt(wpath,1,1,"Searching for MFonts...");
	if ((dp=opendir("/DD/SYS/.BITMAPS/MFONTS/."))==NULL) {
		PrintAt(wpath,1,2,"Error Opening Directory");
		Bell(wpath);
		Wait(wpath);
		EndWin(wpath);
		ShutDown(wpath);
	}
	fonts=0;
	while (fonts<MAXFONTS) {
		if ((dirbuf=readdir(dp))!=NULL) {
			if (_cmpnam(dirbuf->d_name,"*.mfont",7)==0) {
				strcpy(fontname[fonts],dirbuf->d_name);
				fontname[fonts][strlen(fontname[fonts])-6]=0;
				fonts++;
			}
		} else {
			break;
		}
	}
	if (fonts==0) {
		PrintAt(wpath,1,1,"  No MFonts Installed  ");
		Bell(wpath);
		Wait(wpath);
		EndWin(wpath);
		ShutDown(wpath);
	}
	EndWin(wpath);

	PopUp(wpath,"",26,9,28,5);
	BlnkOn(wpath);
	PrintAt(wpath,1,1,"Reading Configuration...");
	BlnkOff(wpath);
	MBanCfg(wpath);
	EndWin(wpath);

	font=0;
	obj=-1;
	sel=-1;
	done=FALSE;
	if (printwidth>132) printwidth=132;
	
	if (LoadMFont(wpath,orgfont)==-1) {
		LoadMFont(wpath,fontname[font]);
	}

	menu_stat[FILE][FILE_PRINT]=1; /* deselect option */
	menu_stat[FILE][FILE_NEW]=1; /* same here */

	FColor(wpath,clr[FORE]);
	SetDPtr(wpath,box_x,box_y); /* erase old box */
	RBox(wpath,box_w,box_h);
	do {
		if (update==TRUE) { /* time to update the screen */
			FColor(wpath,clr[FORE]);
			LSet(wpath,3);
			SetDPtr(wpath,box_x,box_y); /* erase old box */
			RBox(wpath,box_w,box_h);
			switch (layout) {
				case PORTRAIT:
					if (pagelength==0) {
						box_h = MAX_PORTRAIT;
					} else {
						box_h = pagelength;
					}
					box_x = (START_X)+((WIDTH)/2)-printwidth/2;
					box_y = START_Y;
					box_w = printwidth;
					break;
				case LANDSCAPE:
					if (pagelength==0) {
						box_w = MAX_LANDSCAPE;
					} else {
						box_w = pagelength;
					}
					box_x = (START_X)+((WIDTH)/2)-box_w/2;
					box_y = START_Y;
					box_h = printwidth;
					break;
			}
			SetDPtr(wpath,box_x,box_y);
			RBox(wpath,box_w,box_h);
			LSet(wpath,0);
			update=FALSE;
		} /* end of "if (update==TRUE)" */
		if (CheckMenu(wpath,&menu,&option,&ch,&click)) {
			if ( (menu==FILE) && (option==FILE_QUIT) ) {
				if (YesNo(wpath,"QUIT")) {
					done=TRUE;
					break;
				}
			}
/*			if (ch==4) {
				menu=TEXT;
				option=TEXT_DELETE;
			} */
			if (menu==FILE) {
				switch(option) {
				case FILE_NEW:
					if (YesNo(wpath,"CLEAR MEMORY")) {
						TextClear(wpath,0); /* erase all */
						obj=-1; /* no objets */
						sel=-1; /* nothing selected */
						menu_stat[FILE][FILE_PRINT]=1; /* deslectable */
						menu_stat[FILE][FILE_NEW]=1;
					}
					break;
				case FILE_PRINT:
					if (YesNo(wpath,"PRINT")) {
						opath=open(printdev,S_IWRITE);
						if (opath==-1) {
							opath=creat(printdev,S_IWRITE);
						}
						if (opath==-1) {
							PopUp(wpath,"",24,9,31,5);
							PrintAt(wpath,1,1,"Unable to open path/device.");
							Wait(wpath);
							EndWin(wpath);
							break;
						}
						if (sel!=-1) { /* if something selected already */
							TextBox(wpath,sel);
							sel=-1;
						}
						PrintAt(wpath,25,2,"[Press ESC or Click to Abort.]");

						 /* find start and end for either case */
						for (i=0,x=0,y=WIDTH; i<=obj; i++) {
							if (layout==LANDSCAPE) {
								if (text_x[i]>x) x=text_x[i];
								if (text_x[i]+text_w[i]<y) \
														y=text_x[i]+text_w[i];
							} else {
								if (text_y[i]>x) x=text_y[i];
								if (text_y[i]+text_h[i]<y) \
														y=text_y[i]+text_h[i];
							}
						}
						LSet(wpath,3);
						for (i=x; i<y; i++) {
							if (layout==LANDSCAPE) {
								SetDPtr(wpath,START_X+i,START_Y);
								RLine(wpath,0,printwidth);
							} else {
								SetDPtr(wpath,START_X,START_Y+i);
								RLine(wpath,printwidth,0);
							}
							for (j=0; j<printwidth; j++) {
								if (layout==LANDSCAPE) {
									k = _gs_point(wpath,START_X+i,START_Y+j);
								} else {
									k = _gs_point(wpath,START_X+j,START_Y+i);
								}
								if (k==0) {
									line[printwidth-1-j]=bannerchar;
								} else {
									line[printwidth-1-j]=' ';
								}
							}
							line[printwidth]=13; /* add carraige return */
							j=write(opath,line,printwidth+1); /* output line */
							if (layout==LANDSCAPE) {
								RLine(wpath,0,printwidth);
							} else {
								RLine(wpath,printwidth,0);
							}
							if (j==ERROR) {
								if (YesNo(wpath,"PRINT ERROR - ABORT?")) {
									close(opath);
									break;
								}
							}
							if (linefeeds) { /* if printer needs line feeds */
								write(opath,10,1);
							}
							if (MouseClick(wpath) ||
								((_gs_rdy(wpath)>0) && read(wpath,&ch,1) && ch==QUIT)) {
								LSet(wpath,0);
								if (YesNo(wpath,"ABORT PRINT")) break;
								LSet(wpath,3);
							}
						}
						LSet(wpath,0);
						close(opath);
						PrintAt(wpath,25,2,"										");
					}
					break;
				}
			} /* end of "if (menu==FILE)" */
			if (menu==LAYOUT) {
				update=TRUE; /* flag to re-draw box */
				switch(option) {
				case LAYOUT_LANDSCAPE:
					menu_stat[menu][LAYOUT_PORTRAIT]=0;
					menu_stat[menu][LAYOUT_LANDSCAPE]=2;
					layout=LANDSCAPE;
					break;
				case LAYOUT_PORTRAIT:
					menu_stat[menu][LAYOUT_LANDSCAPE]=0;
					menu_stat[menu][LAYOUT_PORTRAIT]=2;
					layout=PORTRAIT;
					break;
				case LAYOUT_CONTINUOUS:
					menu_stat[menu][LAYOUT_CONTINUOUS]=2;
					menu_stat[menu][LAYOUT_8x11]=0;
					menu_stat[menu][LAYOUT_8x14]=0;
					menu_stat[menu][LAYOUT_CUSTOM]=0;
					if (layout==LANDSCAPE) pagelength=MAX_LANDSCAPE;
					if (layout==PORTRAIT) pagelength=MAX_PORTRAIT;
					break;
				case LAYOUT_8x11:
					menu_stat[menu][LAYOUT_8x11]=2;
					menu_stat[menu][LAYOUT_CONTINUOUS]=0;
					menu_stat[menu][LAYOUT_8x14]=0;
					menu_stat[menu][LAYOUT_CUSTOM]=0;
					pagelength=66;
					break;
				case LAYOUT_8x14:
					menu_stat[menu][LAYOUT_8x14]=2;
					menu_stat[menu][LAYOUT_CONTINUOUS]=0;
					menu_stat[menu][LAYOUT_8x11]=0;
					menu_stat[menu][LAYOUT_CUSTOM]=0;
					pagelength=84;
					break;
				case LAYOUT_CUSTOM:
					PopUp(wpath,"Enter Page Length",30,8,20,5);
					LineInput(wpath,1,1,">",line,3);
					EndWin(wpath);
					i=atoi(line);
					if (layout==PORTRAIT) {
						j=MAX_PORTRAIT;
					} else {
						j=MAX_LANDSCAPE;
					}
					if (i>0 && i<j) { /* if they entered a valid number */
						menu_stat[menu][LAYOUT_CUSTOM]=2;
						menu_stat[menu][LAYOUT_CONTINUOUS]=0;
						menu_stat[menu][LAYOUT_8x11]=0;
						menu_stat[menu][LAYOUT_8x14]=0;
						pagelength=i;
						break;
					}
				}
			} /* end of "if (menu==LAYOUT)" */
			if (menu==TEXT) {
				switch(option) {
				case TEXT_CENTER:
					if (YesNo(wpath,"CENTER")) {
						
					}
				case TEXT_DELETE:
					if ((sel!=-1) && (obj!=-1)) { /* if something selected */
						if (YesNo(wpath,"DELETE LINE")) {
							if (sel==obj) { /* if deleting last object */
								SetDPtr(wpath,text_x[obj]+START_X,text_y[obj]+START_Y);
								FColor(wpath,clr[BACK]);
								RBar(wpath,text_w[obj],text_h[obj]);
								FColor(wpath,clr[FORE]);
								obj--;
								sel=-1;
								if (obj==-1) { /* no objects? */
									menu_stat[FILE][FILE_PRINT]=1; /* deslectable */
									menu_stat[FILE][FILE_NEW]=1;
								}
								break;
							}
							j=text_h[sel];
							for (i=sel; i<obj; i++) {
								text_x[i]=text_x[i+1];
								text_y[i]=text_y[i+1]-j-spacing;
								text_w[i]=text_w[i+1];
								text_h[i]=text_h[i+1];
								text_j[i]=text_j[i+1];
								text_font[i]=text_font[i+1];
								strcpy(text[i],text[i+1]); /* pointers? */
							}
							obj--;
							if (obj==-1) {
								menu_stat[FILE][FILE_PRINT]=1;
								menu_stat[FILE][FILE_NEW]=1;
							}
							TextView(wpath,obj,font,fonts,sel);
							sel=-1;
						}
					}
					break;
				case TEXT_ENTER:
					while (obj<MAX) { /* room to add another? */
						obj++;
						if (obj>0) { /* if more than one obj exists */
							text_y[obj]=text_y[obj-1]+text_h[obj-1]+spacing;
						} 
						else {
							text_y[obj]=0;
						}
						text_font[obj]=font;
						text_h[obj]=Query_MFont_Height();
						if (text_y[obj]+text_h[obj]>box_h) {
							PopUp(wpath,"ALERT",19,10,42,6);
							PrintAt(wpath,1,1,"No more text will fit in the working");
							PrintAt(wpath,5,2,"area using the current font.");
							Wait(wpath);
							EndWin(wpath);
							obj--;
							break;
						}
						PopUp(wpath,"Enter Banner Text - [ENTER] to Quit",2,16,75,6);
						LineInput(wpath,1,1,">",text[obj],69);
						if (text[obj][0]!=NULL) { /* something typed? */
							text_w[obj]=MText_Length(text[obj]);
							if (text_w[obj]>box_w) {
								EndWin(wpath); /* kill text box */
								PopUp(wpath,"ALERT",26,10,28,5);
								PrintAt(wpath,1,1,"Text too long to fit...");
								Wait(wpath);
								EndWin(wpath);
								obj--;
								continue; /* go back and get another line... */
							}
							LineInput(wpath,1,2,"[C)enter, [L)eft, or [R)ight Justify?",&ch,1);
							EndWin(wpath); /* kill text box */
							ch=toupper(ch);
							switch (ch) {
								case 'L': /* left */
									text_j[obj]=LEFT_JUSTIFY;
									text_x[obj]=box_x;
									break;
								case 'R': /* right */
									text_j[obj]=RIGHT_JUSTIFY;
									text_x[obj]=box_x+box_w-text_w[obj];
									break;
								default: /* anything else (center) */
									text_j[obj]=CENTER_JUSTIFY;
									text_x[obj]=(WIDTH)/2-text_w[obj]/2;
									break;
							}
							MCurXY(wpath,text_x[obj]+START_X, \
										text_y[obj]+START_Y);
							MPrint(wpath,text[obj]);
							menu_stat[FILE][FILE_PRINT]=0; /* selectable */
							menu_stat[FILE][FILE_NEW]=0;
						} else {
							obj--;
							EndWin(wpath);
							break; /* break out of loop */
						}
					} /* end of "while (obj<MAX)" */
					break;
				case TEXT_FONT:
					i=Chooser(wpath,"SELECT FONT",15,fontname,fonts);
					if ((i>=0) && (i<=fonts)) { /* valid number? */
						if (i!=font) { /* different font? */
							LoadMFont(wpath,fontname[i]);
							font=i;
						}
					}
					break;
				case TEXT_VIEW:
					TextView(wpath,obj,font,fonts,0); /* redraw all */
					sel=-1;
					break;
				}
			} /* end of "if (menu==TEXT)" */
		} /* end of "if (CheckMenu)" */
		if ((ch==QUIT) && (sel!=-1)) { /* ESC clear selected object */
			TextBox(wpath,sel);
			sel=-1;
		}
		if ((ch==9) && (obj!=-1)) { /* TAB and object available ? */
			if (sel!=-1) { /* if something already selected */
				TextBox(wpath,sel);
			}
			sel++; /* move to next object to select */
			if (sel>obj) { /* past last object? */
				sel=0; /* back to first */
			}
			TextBox(wpath,sel);
		}
		if (mouseon) {
			if (click>0) { /* did we click on somethin'? */
				x=ms.pt_wrx;
				y=ms.pt_wry;
				if (obj!=-1) {
					if (sel!=-1) { /* if something selected already */
						TextBox(wpath,sel);
						sel=-1;
					}
					for (i=0; i<=obj; i++) {
						if ((x>=text_x[i]+START_X) && (x<=text_x[i]+START_X+text_w[i]) &&
							 (y>=text_y[i]+START_Y) && (y<=text_y[i]+START_Y+text_h[i])) {
							Bell(wpath);
							TextBox(wpath,i);
							sel=i;
							break;
						}
					}
				} /* end of "if (obj!=-1)" */
			} /* end of "if (click>0)" */
		} /* end of "if (mouseon)" */
	}
	while(!done);

	Deiniz_MFont(wpath);
	ShutDown(wpath); /* Goodbye, EthaWin... */
}

TextBox(wpath,i) /* box/unbox object 'n' */
int wpath,i;
{
	SetDPtr(wpath,text_x[i]+START_X,text_y[i]+START_Y);
	LSet(wpath,3);
	RBox(wpath,text_w[i],text_h[i]);
	LSet(wpath,0);
}

TextView(wpath,obj,font,fonts,start) /* update view area from 'start' */
int wpath,obj,font,fonts,start;
{
	int i,j,last_font;

	if (obj!=-1) { /* if objects available */
		TextClear(wpath,start);
		last_font=font;
		for (i=0; i<=fonts; i++) { /* loop through fonts */
			for (j=start; j<=obj; j++) { /* loop through lines */
				if (text_font[j]==i) { /* match font? */
					if (last_font!=i) { /* and it's not the one we are using */
						/*						Iniz_MFont(wpath,fontname[i]);*/ /* then load it */
						LoadMFont(wpath,fontname[i]);
						last_font=i; /* and update font var. */
					}
					MCurXY(wpath,text_x[j]+START_X,text_y[j]+START_Y);
					MPrint(wpath,text[j]);
				}
			}
		}
		if (last_font!=font) { /* if at a different font */
			LoadMFont(wpath,fontname[font]);
		}
	}	
}

TextClear(wpath,start) /* clear all or part of text area */
int wpath,start;
{
	if (start<1) { /* if clear all */
		SetDPtr(wpath,START_X,START_Y);
	} 
	else {
		SetDPtr(wpath,START_X,text_y[start]+START_Y);
	}
	FColor(wpath,clr[BACK]);
	Bar(wpath,START_X+WIDTH,START_Y+printwidth);
	FColor(wpath,clr[FORE]);
}

int LoadMFont(wpath,fontname)
int wpath;
char *fontname;
{
	int status;
						
	PopUp(wpath,"",29,9,22,5);
	PrintAt(wpath,2,1,"Loading Font ...");
	status=Iniz_MFont(wpath,fontname);
	EndWin(wpath);
	return(status);
}

Usage()
{
	fputs("\nMegaBanners V1.00 by Allen Huffman (coco-sysop@genie.geis.com)\n",stderr);
	fputs("Copyright (C) 1995 by Sub-Etha Software\n\n",stderr);
	fputs("Syntax: MegaBanners [-opts]\n",stderr);
	fputs("Usage : EthaWin/MFonts banner making program.\n",stderr);
	fputs("Opts  : -? = display this message.\n",stderr);
	fputs("		  -s = use existing screen.\n",stderr);
	exit(0);
}
