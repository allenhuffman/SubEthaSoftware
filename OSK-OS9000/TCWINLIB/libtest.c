/*
 * Library Test Routine...
 */

#include <stdio.h>
#include "tcwin.h"

main()
{
	int i;
	int wpath;								/* main window path number */

	wpath = WinStartup( 0, 0, 80, 24, 7, 0 ); /* create main window */

	for (i = 0 ; i < 24 ; i++ ) {
		FColor(STDOUT,i/3);
		TextAt(STDOUT, 1, i, "..............................................................................");
	}
	FColor(STDOUT,7);

	TextAt(STDOUT,5,5,"Erase to End of Line:");
	sleep(2);
	ErEOLine(STDOUT);
	Refresh();
	sleep(2);

	TextAt(STDOUT,5,7,"Erase entire Line:");
	sleep(2);
	ErLine(STDOUT);
	Refresh();
	sleep(2);

	TextAt(STDOUT,5,3,"DELETE line:");
	sleep(2);
	DelLine(STDOUT);
	Refresh();
	sleep(2);
	
	TextAt(STDOUT,5,7,"Cursor UP, DOWN, LEFT, RIGHT...");
	sleep(2);
	for(i=0;i<2;i++) { CurUp(STDOUT); Refresh(); sleep(1); }
	for(i=0;i<2;i++) { CurDwn(STDOUT); Refresh(); sleep(1); }
	for(i=0;i<2;i++) { CurLft(STDOUT); Refresh(); sleep(1); }
	for(i=0;i<2;i++) { CurRgt(STDOUT); Refresh(); sleep(1); }

	TextAt(STDOUT,5,9,"Home Cursor");
	sleep(2);
	CurHome(STDOUT);
	Refresh();	
	sleep(2);

	TextAt(STDOUT,5,11,"Erase to End of Screen:");
	sleep(2);
	ErEOScrn(STDOUT);
	Refresh();
	sleep(2);

	sleep(10);
	WinShutdown(wpath);
}
