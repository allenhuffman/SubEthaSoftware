/*
 * TCWindows by Allen C. Huffman (coco-sysop@genie.geis.com)
 *
 * Termcap windows, based on CoCo L2/K-Windows CGFX library functions.
 *
 * 0.00 09/15/95 - initial work began in Princeton, NJ
 * 0.01 09/20/95 - more improvements, fixes in Des Moines, IA
 * 0.02 09/25/95 - converter to tcwinlib library in Atlanta, GA!!!
 */

/* Insert code to generate edition/revision in here */

#if 0

#asm

EdRev:		equ		000

_sysedit:	equ		EdRev/10
_sysattr:	equ		0x800 + ( EdRev - ( _sysedit * 10 ) )

#endasm

#endif