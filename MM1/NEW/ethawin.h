/*
 * EthaWin.h - some defines for use with any EthaWin program.
 *
 * 1.00 9/18/93 - Initial version
 * 1.01 5/30/94 - Added STDIN, STDOUT, and STERR
 * *.** 2/20/95 - Added structures and defines for buttons and EwFPick(),
 *                  and user callable function prototypes.  changed the
 *                  MAXMENUS define to ifndef. - C. Hyde
 */

/* some logic defines */

#define TRUE  1
#define FALSE 0

/* some defines for reading the keyboard */

#ifdef OSK
#define UP   16
#define DOWN 14
#define LEFT  2
#define RIGHT 6
#define QUIT 27
#else
#define UP   12
#define DOWN 10
#define LEFT  8
#define RIGHT 9
#define QUIT 5
#endif
#define ENTER 13

/* some defines used by the windowing system */

#define CHECKFLAG 0xae

#define FORE 0
#define BACK 1
#define BORDER 2
#define HIGHLIGHTED 3
#define DESELECTED 4
#define MBFORE 5
#define MBBACK 6
#define TTFORE 7
#define TTBACK 8
#define PUFORE 9
#define PUBACK 10
#define PUSHAD 11
#define PUBORD 12
#define PDFORE 13
#define PDBACK 14
#define PDSHAD 15
#define COLORS 16 /* 15 customizeable color thingies */

#define MAXOPTS 8 /* maximum menu options */

#ifndef MAXMENUS /* the number of pull down menus used */
#define MAXMENUS 1 /* only 1 required (File) */
#endif

/* some defines for the mouse signal */

#ifdef OSK
#define MOUSESIG 444
#else
#define MOUSESIG 10 /* mouse click signal */
#endif

#define STDIN 0
#define STDOUT 1
#define STDERR 2

/* defines and struct used for buttons */

#define BUTTONNAMELEN 13

struct BUTTON
{
    char name[BUTTONNAMELEN+1]; /* include 1 for a null */
    int len; /* length of actual name */
    int posx; /* x position of left edge of button */
    int posy; /* y poition of button */
    char active; /* whether button is useable */
};

/* defines used for EwFPick(), types parameter */

#define ALLFILES    0xff
#define BOTH        0x180 /* include dirs automatically in BOTH */
#define OREAD       0x01 /* owner readable */
#define OWRITE      0x02 /* owner writable */
#define OEXEC       0x04 /* owner executable */
#define PREAD       0x08 /* public readable */
#define PWRITE      0x10 /* public writable */
#define PEXEC       0x20 /* public executable */
#define SHARE       0x40 /* sharable file */
#define DIRECTORY   0x80 /* directory attribute */

/* function prototypes */

/* void */ activateButton(/* int wpath, struct BUTTON *blist,
                             int button_number */);
/* void */ deactivateButton(/* int wpath, struct BUTTON *blist,
                               int button_number */);
/* void */ error(/* int wpath, char *title, char *string1, char *string2,
                    int error_code */);
/* void */ TopText(/* int wpath, char *left, char *title, char *right */);
/* void */ LineInput(/* int wpath, int x, int y, char *prompt, char *line,
                        int length */);
/* void */ PrintAt(/* int wpath, char *string */);
/* void */ PopUp(/* int wpath, char *title, int x, int y, int w, int h */);
/* void */ EndWin(/* int wpath */);
/* void */ Wait(/* int wpath */);
/* void */ Snooze(/* int wpath */);

int makeButton(/* int wpath, struct BUTTON *blist, int button_number,
                  char *name, int posx, int posy */);
int checkButton(/* struct BUTTON *blist, int number_of_buttons */);
int Startup(/* char makewin  int maxmenus */);
int ShutDown(/* int wpath */);
int YesNo(/* int wpath, char *title */);
int OpenFile(/* char *filename, int mode */);

char *EwFPick(/* int wpath, char *title, int types */);
char checkMouse(/* int wpath, char repeat */);
char CheckMenu(/* int wpath, int *menu, int *option, char *ch, char *click,
                  char repeat */);
char *getspwd(/* char *dir_name */);
char *getpwd(/* void */);

/* end ethawin.h */
