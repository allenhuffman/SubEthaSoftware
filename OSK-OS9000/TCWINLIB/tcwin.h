#define TRUE 1
#define FALSE 0
#define OK TRUE
#define ERROR -1
#define FIRSTWIN 3
#define STDOUT -1
#define MINX 5
#define MINY 5

/*
 * Data structure for TCWindows
 */
 
struct WINDOW {
	struct WINDOW *next;	/* pointer to next window or NULL if last one */
	struct WINDOW *prev;	/* pointer to previous window or NULL if first */
	int			number;		/* window number (assigned on creation) */
	int			x;			/* start x position of window on screen */
	int			y;			/* start y position of window on screen */
	int			w;			/* width of this window */
	int			h;			/* height of this window */
	unsigned char attr;		/* current attributes */
	unsigned char color;	/* current foreground/background color */
	int			cur_x;		/* X cursor position */
	int			cur_y;		/* Y cursor position */
	int			warea_x;	/* working area X position */
	int			warea_y;	/* working area Y position */
	int			warea_w;	/* working area width */
	int			warea_h;	/* working area height */
	char	 	*textbuf;	/* pointer to text buffer for window */
	char		*attrbuf;	/* pointer to attribute buffer for window */
	char		*colorbuf;	/* pointer to color buffer for window */
};

