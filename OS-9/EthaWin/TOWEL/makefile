#
# This is a Makefile which will create Towel.  Note that it does use the
# replacement CC front end and does require cgfx.l and the EthaWin library.
#

CFLAGS = -r

towel: drivepath.r iobox.r main.r process.r system.r towelcfg.r towelvars.r
 cc -f=Towel drivepath.r iobox.r main.r process.r system.r\
  towelcfg.r towelvars.r -l=/dd/lib/ethawin.l -l=/dd/lib/cgfx.l

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
