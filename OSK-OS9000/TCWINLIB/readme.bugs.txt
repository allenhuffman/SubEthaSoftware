THIS IS A TEXT FILE!  NOT A C FILE.  :)

Ideas:	fix for brane dead terminals.  ANSI emulation (and maybe
others?) specifies codes to turn on individual attributes (blink,
underline, reverse) but NO codes to turn OFF individual attributes. 
Instead, the user is expected to turn off ALL attributes, then turn back
on the ones which were not meant to be shut off.  Thus, BRANE DEAD.

Concept would be for the termstuff portion to detect any matching
shutoff codes foor underline, reverse, and blink and then "know" it has
to turn certain ones back on. ie:

	blink_off 	= BLINKOFF;
	if (blink_off == reverse_off) blink_off = blink_off | REVERSE_OFF;
	if (blink_off == underine_off) blink_off = blink_off | UNDERLINE_OFF;

	reverse_off	= " "

	underline	= " "

Then it can use this mask to tell if things have changed!  :)
