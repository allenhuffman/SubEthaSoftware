#
# Make 'Towel'
#
CC		= cc

RDIR	= RELS

RFILES	= drivepath.r iobox.r main.r process.r system.r towelcfg.r towelvars.r

CFLAGS	= -r=$(RDIR) -t=/r0 -o
LFLAGS	= -t=/r0 -l=/dd/lib/ethawin.l -l=/dd/lib/cgfx.l

towel: $(RFILES)
	chd $(RDIR);\
	cc $(LFLAGS) $(RFILES) -f=Towel

drivepath.r: drivepath.c
	cc $(CFLAGS) drivepath.c

iobox.r: iobox.c
	cc $(CFLAGS) iobox.c

main.r: main.c
	cc $(CFLAGS) main.c

process.r: process.c
	cc $(CFLAGS) process.c

system.r: system.c
	cc $(CFLAGS) system.c

towelcfg.r: towelcfg.c
	cc $(CFLAGS) towelcfg.c

towelvars.r: towelvars.c
	cc $(CFLAGS) towelvars.c
