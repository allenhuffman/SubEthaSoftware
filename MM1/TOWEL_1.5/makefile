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
CC			= cc
RDIR		= RELS
#ENDAS		= -eas=$(RDIR)
ENDAS		= -r=$(RDIR)
DEBUG		= -g
#COMMON		= $(DEBUG) -bc
COMMON		= -t=/r0
#CFLAGS		= $(COMMON) -dTCWIN -dNOMOUSE -dOSK
CFLAGS		= $(COMMON)
LFLAGS		= $(COMMON)
LDIR		= /dd/lib
CGFXLIB		= -l=$(LDIR)/cgfx.l
#CGFXLIB	= -l=$(LDIR)/tcwinlib.l
TERMLIB		= 
#TERMLIB		= -l=$(LDIR)/termlib.l
ETHALIB		= -l=$(LDIR)/ethawin.l

ODIR		= /dd/cmds
ONAME		= Towel

RFILES		= drivepath.r iobox.r main.r process.r system.r towelcfg.r\
				towelvars.r
#
# Towel program stuff
#
towel: $(RFILES)
	chd RELS;\
	$(CC) $(CFLAGS) -f=$(ODIR)/$(ONAME) \
									$(RFILES) $(ETHALIB) $(CGFXLIB) $(TERMLIB)

drivepath.r: drivepath.c
	$(CC) $(CFLAGS) $(ENDAS)  drivepath.c

iobox.r: iobox.c
	$(CC) $(CFLAGS) $(ENDAS)  iobox.c

main.r: main.c ethaglobals.h
	$(CC) $(CFLAGS) $(ENDAS)  main.c

process.r: process.c
	$(CC) $(CFLAGS) $(ENDAS)  process.c

system.r: system.c
	$(CC) $(CFLAGS) $(ENDAS)  system.c

towelcfg.r: towelcfg.c
	$(CC) $(CFLAGS) $(ENDAS)  towelcfg.c

towelvars.r: towelvars.c
	$(CC) $(CFLAGS) $(ENDAS)  towelvars.c
