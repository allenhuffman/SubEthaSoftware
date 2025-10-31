#
# This is a Makefile which will create the Ethawin library.  Note that it does
# use the replacement CC front end and does require cgfx.l.
#

CFLAGS = -r

# the .r files that make up the library

RFILES = chooser.r config.r endwin.r lineinput.r menu.r mouse.r\
         popup.r printat.r pulldown.r snooze.r startup.r toptext.r wait.r\
         yesno.r

# to make the library, just merge the .r files together...

ethalib.l: $(RFILES)
     echo Merging EthaWin.r files into EthaWin.l file . . . hang on . . .
     merge $(RFILES) >-/dd/lib/ethawin.l

# these files are made in the following manner:

chooser.r: chooser.c
     cc $(CFLAGS) chooser.c

config.r: config.c
     cc $(CFLAGS) config.c

endwin.r: endwin.c
     cc $(CFLAGS) endwin.c

lineinput.r: lineinput.c
     cc $(CFLAGS) lineinput.c

menu.r: menu.c
     cc $(CFLAGS) menu.c

misc.r: misc.c
     cc $(CFLAGS) misc.c

mouse.r: mouse.c
     cc $(CFLAGS) mouse.c

popup.r: popup.c
     cc $(CFLAGS) popup.c

printat.r: printat.c
     cc $(CFLAGS) printat.c

pulldown.r: pulldown.c
     cc $(CFLAGS) pulldown.c

snooze.r: snooze.c
     cc $(CFLAGS) snooze.c

startup.r: startup.c
     cc $(CFLAGS) startup.c

toptext.r: toptext.c
     cc $(CFLAGS) toptext.c

wait.r: wait.c
     cc $(CFLAGS) wait.c

yesno.r: yesno.c
     cc $(CFLAGS) yesno.c
