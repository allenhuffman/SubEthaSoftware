/* stdinc.h
 *
 * My standard include file.
 *
 */

#define NULL        0
#define ERROR       (-1)
#define TRUE        1
#define FALSE       0
#define SUCCESS     1
#define FAIL        0

/* usefull macros */

#define newline(pn)         writeln(pn, "\n", 1)
#define Cls(pn)             writeln(pn, "\x0C", 1)
#define CurHome(pn)         writeln(pn, "\x01", 1)
#define CurOn(pn)           writeln(pn, "\x05\x21", 2)
#define CurOff(pn)          writeln(pn, "\x05\x20", 2)
#define CurRgt(pn)          writeln(pn, "\x06", 1)
#define CurLft(pn)          writeln(pn, "\x08", 1)
#define CurUp(pn)           writeln(pn, "\x09", 1)
#define CurDown(pn)         writeln(pn, "\x0A", 1)
#define ErEOScrn(pn)        writeln(pn, "\x0B", 1)
#define ErEOLine(pn)        writeln(pn, "\x03", 1)
#define ErLine(pn)          writeln(pn, "\x04", 1)
#define DelLine(pn)         writeln(pn, "\x1F\x31", 2)
#define InsLine(pn)         writeln(pn, "\x1F\x30", 2)
#define Bell(pn)            writeln(pn, "\x07", 1)
#define RevOn(pn)           writeln(pn, "\x1F\x20", 2)
#define RevOff(pn)          writeln(pn, "\x1F\x21", 2)
#define UndlnOn(pn)         writeln(pn, "\x1F\x22", 2)
#define UndlOff(pn)         writeln(pn, "\x1F\x23", 2)
#define BlnkOn(pn)          writeln(pn, "\x1F\x24", 2)
#define BlnkOff(pn)         writeln(pn, "\x1F\x25", 2)
#define BoldOn(pn)          writeln(pn, "\x1B\x3D\x01", 3)
#define BoldOff(pn)         writeln(pn, "\x1B\x3D\x00", 3)
#define TransOn(pn)         writeln(pn, "\x1B\x3C\x01", 3)
#define TransOff(pn)        writeln(pn, "\x1B\x3C\x00", 3)
#define PropOn(pn)          writeln(pn, "\x1B\x3F\x01", 3)
#define PropOff(pn)         writeln(pn, "\x1B\x3F\x00", 3)
