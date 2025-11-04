#
# This MakeFile will create the EthaWin library, placing it in the location
# pointed to by the LDIR macro below.  It will also create a sample program
# using it called "EthaDemo", which will compile to your current execution
# directory.  Note:  You will need to have a sub-directory RELS inside the
# directory which contains all of the source files for EthaWin.
#
# Note:  ENDAS uses "-eas" for Ultra-C, or "-r" for K&R (old) compiler.
#        COMMON uses "-bc" for Ultra-C, or may use "-t=/r0" for K&R w/ramdisk.
#        (Ultra-C defaults to use TMPDIR env. variable as work dir.)
#
#
# Also:  Pass in the appropriate -dLABEL for the version you wish to make!
#
CC			= cc
RDIR		= RELS
#ENDAS		= -eas=$(RDIR)
ENDAS		= -r
DEBUG		= -g
#COMMON		= $(DEBUG) -bc
COMMON		= -t=/r0
#CFLAGS		= $(COMMON) -dTCWIN -dNOMOUSE -dOSK
CFLAGS		= $(COMMON) -dOSK
LFLAGS		= $(COMMON)
CGFXLIB	= cgfx.l #/dd/lib/cgfx.l
#CGFXLIB		= /h0/lib/tcwinlib.l
TERMLIB		= termlib.l
LDIR		= /h0/lib
LNAME		= ethawin.l

RFILES		= chooser.r config.r endwin.r lineinput.r menu.r\
				misc.r mouse.r popup.r printat.r pulldown.r snooze.r\
				startup.r toptext.r wait.r yesno.r

all: $(LDIR)/$(LNAME) ethademo
	@echo All done...
#
# Create the library by merging all the .r files together
#
$(LDIR)/$(LNAME): $(RFILES)
	chd $(RDIR);\
	merge $(RFILES) >-$(LDIR)/$(LNAME)
#
# EthaDemo program stuff (compile to .r files then link with the libraries)
#
#ethademo: ethademo.r ethademovars.r $(LDIR)/$(LNAME)
#	chd $(RDIR);\
#	$(CC) $(LFLAGS) -f=EthaDemo ethademo.r ethademovars.r\
#		-l=$(LDIR)/$(LNAME) -l=$(CGFXLIB) -l=$(TERMLIB)
#
#ethademo.c: ethademo.r
#	$(CC) $(CFLAGS) $(ENDAS) ethademo.c
#
#ethademovars.c: ethademovars.r
#	$(CC) $(CFLAGS) $(ENDAS) ethademovars.c
#
# EthaWin routines... (compile all down to .r relocatable files)
#
chooser.r: chooser.c
	$(CC) $(CFLAGS) $(ENDAS) chooser.c

config.r: config.c
	$(CC) $(CFLAGS) $(ENDAS) config.c

endwin.r: endwin.c
	$(CC) $(CFLAGS) $(ENDAS) endwin.c

lineinput.r: lineinput.c
	$(CC) $(CFLAGS) $(ENDAS) lineinput.c

menu.r: menu.c
	$(CC) $(CFLAGS) $(ENDAS) menu.c

misc.r: misc.c
	$(CC) $(CFLAGS) $(ENDAS) misc.c

mouse.r: mouse.c
	$(CC) $(CFLAGS) $(ENDAS) mouse.c

popup.r: popup.c
	$(CC) $(CFLAGS) $(ENDAS) popup.c

printat.r: printat.c
	$(CC) $(CFLAGS) $(ENDAS) printat.c

pulldown.r: pulldown.c
	$(CC) $(CFLAGS) $(ENDAS) pulldown.c

snooze.r: snooze.c
	$(CC) $(CFLAGS) $(ENDAS) snooze.c

startup.r: startup.c
	$(CC) $(CFLAGS) $(ENDAS) startup.c

toptest.r: toptext.c
	$(CC) $(CFLAGS) $(ENDAS) toptext.c

wait.r: wait.c
	$(CC) $(CFLAGS) $(ENDAS) wait.c

yesno.r: yesno.c
	$(CC) $(CFLAGS) $(ENDAS) yesno.c
