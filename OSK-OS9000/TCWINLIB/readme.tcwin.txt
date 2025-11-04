TCWinLib ReadMe
---------------

Welcome to the wonderful world of termcap windowing environments!  TCWin is
a library of routines which create a system of overlay device "windows"
on any termcap capable system.  The windowing functions are based on the
CGFX calls from OS-9 Level 2 on the Color Computer and K-Windows on the MM/1.
Library calls are provided to initialize the windowing system, create
and remove overlay windows, as well as displaying text to them.  Other
functions allow you to position the cursor, change colors and text
attributes.  (Note:  Color and attribute commands function only on
terminal types which support them.)

While wrapper code is provided to allow "first on, first off" creation
and removeable of overlay windows, the system itself allows you to
access any overlay individually - similar to device windows under CGFX. 
Thus, a user's application can display several overlay windows - even
overlapping - and messages and be displayed to any of them, selectably,
even if the windows are completely are partially hidden by windows on
top of them.  Moving of windows is also supported.  (Note that these
features are only available by using the TCWin calls direct.  The CGFX
wrapper calls still perform as their original counterparts did.)

Full screen scrolling - though slow depending on terminal speed - is
supported within overlays and on the main window.  Also, Most other CGFX text
mode commands are or will be supported by this library (such as BlnkOn, ReVOn,
etc.) on terminals that can handle them.

Porting Notes:
--------------

If you are porting existing CGFX text applications over to TCWin, there
are certain limitations you must be aware of.  First, since TCWin has to
track all text output in order to maintain a "virtual window", you MUST
use the provided text output functions of TCWin.  THIS MEANS you cannot
directly "write", "writeln", "printf" or ANY OTHER screen output
function and still expect TCWin to track the window text.  Instead, you
MUST use the provided TCWin text functions, Print and PrintAt (with more
possibly being added in the future).  Replace most code is fairly simple
and both of these functions support the C-ism "\n" to insert carraige returns.

	printf("this is my text\n");			write(STDOUT,"hello",5);

would become:

	Print(STDOUT,"this is my text\n");		Print(STDOUT,"hello",5);

It is left up to you to convert any and all text output in order to work
with this library.  If you have already written applications that run
under the EthaWin interface, you will find most of it's text output
routines translate over to TCWin with minimum effort.

Currently support things:

	The entire EthaWin library (excluding mouse support) is currently
available in a TCWin version.  This would allow EthaWin applications
(correctly written) to run on a CoCo 3, MM/1 (under K-Windows), generic
OSK box (using Termcap) or even OS-9000 (also using termcap).

The FUTURE:

	TCWin and the EthaWin libraries will continue to evolve.  Full
source code is provided so you may make any changes you feel necessary. 
It is possible, using both of these libraries, to have you application
run under virtually any OS-9/OS-9000 platform, at one level or another.
External serial port mouse drivers may also allow EthaWin to eventually
support mouse control on a terminal based OSK system!  If you make any
significant changes to these libraries or find any bugs, please submit
them to the author and attempts will be made to incorporate them in the
official distribution.

This library is provided AS IS with no support from or connection to ANY
commercial organization.  Use at your own risk.
