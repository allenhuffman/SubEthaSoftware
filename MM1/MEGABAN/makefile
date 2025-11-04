#
# Make 'MegaBan'
#
CC		= cc

RDIR	= RELS

RFILES	= chooser.r mbancfg.r megaban.r megabanvars.r

CFLAGS	= -r=$(RDIR) -t=/r0 -o
LFLAGS	= -t=/r0 -l=/dd/lib/mfonts.l -l=/dd/lib/ethawin.l -l=/dd/lib/cgfx.l

megaban: $(RFILES)
	chd $(RDIR);\
	cc $(LFLAGS) $(RFILES) -f=megaban

megaban.r: megaban.c
	cc $(CFLAGS) megaban.c

megabanvars.r: megabanvars.c
	cc $(CFLAGS) megabanvars.c

mbancfg.r: mbancfg.c
	cc $(CFLAGS) mbancfg.c

chooser.r: chooser.c
	cc $(CFLAGS) chooser.c

