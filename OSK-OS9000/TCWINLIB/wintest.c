#include <stdio.h>
#include "tcwin.h"

main()
{
	int wpath;								/* main window path number */
	int win[10],windows;					/* overlay window path array */
	int i,j;
	char text[80];

	wpath = WinStartup( 0, 0, 80, 24, 7, 0 );

	ReVOn(STDOUT);
	TextAt(STDOUT, 27, 0, "Termcap Windowing System:");
	ReVOff(STDOUT);
	UndlnOn(STDOUT);
	TextAt(STDOUT, 27, 2, "D E M O N S T R A T I O N");
	UndlnOff(STDOUT);
	
	for (i = 0 ; i < 10 ; i++ ) {
		FColor(STDOUT,i);
		TextAt(STDOUT, 8, i+4, "Welcome to the wonderful world of termcap windowing environments.");
		FColor(STDOUT,i+8);
		TextAt(STDOUT, 8, 23-i,"Welcome to the wonderful world of termcap windowing environments.");
		tsleep(10);
	}
	FColor(STDOUT,7);
	sleep(2);

	for (i=0 ; i<4 ; i++ ) {
		RefreshOff();
		win[i] = MakeWin(10+i*10, 6+i*2, 30, 10, i+2, 0);
		ReVOn(STDOUT);
		Text(STDOUT,    "+----------------------------+");
		for (j=0; j<8; j++) {
			Text(STDOUT,"|                            |");
		}
		Text(STDOUT,    "+----------------------------+");
		ReVOff(STDOUT);
		CWArea(STDOUT,1,1,28,8);
		BColor(STDOUT,0);
		Clear(STDOUT);
		TextAt(STDOUT,6,2,"Terminal Windows");
		UndlnOn(STDOUT);
		TextAt(STDOUT,10,5,"ON DEMAND!");
		UndlnOff(STDOUT);
		RefreshOn();
		sleep(2);
	}
	sleep(2);
	
	win[4] = MakeWin(15, 2, 50, 3, 7, 0);
	ReVOn(STDOUT);
	Clear(STDOUT);
	Text(STDOUT,"TCWin supports scrolling of windows currently\ndisplayed...\n");
	sleep(2);

	for (i=0; i<80; i++) {
		sprintf(text,"\n  [%2.2d] Scroll test... [%2.2d]",i,i);
		Text(win[3],text);
	}
	Text(STDOUT,"         ...or partially hidden in the background.");
	sleep(2);

	for (i=0; i<80; i++) {
		sprintf(text,"\n  [%2.2d] Scroll test... [%2.2d]",i,i);
		Text(win[1],text);
	}
	sleep(2);

	Text(STDOUT,"\n\n TCWin allows you to move exisiting windows, too!\n");
	sleep(2);

	for (i=0; i<10; i++) {
		MoveWin(STDOUT,15,2+i*2);
		sleep(1);
	}

	Text(STDOUT,"\n\n          And even cycle through them...\n");
	sleep(2);

	for (i=0; i<4; i++) {
		Select(win[i]);
		sleep(1);
	}
	Select(win[4]);

	MoveWin(STDOUT,15,2);
	Text(STDOUT,"\n\n     Main screen scrolling is also supported.\n");
	sleep(2);

	Clear(wpath);
	sleep(2);
		
	for (i=0; i<20; i++) {
		sprintf(text,"\n | | [%2.2d]      Now scrolling text on the main background window...     [%2.2d] | |",i,i);
		Text(wpath,text);
		Text(wpath, "\n |o|                                                                        |o|");
		Text(wpath, "\n | |------------------------------------------------------------------------| |");
	}
	sleep(2);

	Text(STDOUT,"\n\n  So say goodbye to boring terminal interfaces!\n");
	sleep(2);

	for (i=0; i<4; i++) {
		Clear(win[i]);
		TextAt(win[i],11,2,"Goodbye,");
		TextAt(win[i],7,4,"boring terminal");
		TextAt(win[i],9,5,"interfaces!");
	}
	sleep(2);
	
	for (i=0; i<4; i++) {
		KillWin(win[i]);
		sleep(2);
	}
	sleep(2);

	MoveWin(STDOUT,15,11);
	Text(STDOUT,"\n\n            And hello to ... TC Windows!\n");	
	sleep(5);

	KillWin(win[4]);
	sleep(2);
	WinShutdown(wpath);
}
