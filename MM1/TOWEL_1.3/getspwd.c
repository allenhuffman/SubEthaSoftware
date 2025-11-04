/* getspwd - version 1.0
 *      WB - Chris Hyde, 11/94
 *
 * Gets the name of the current directory - ie DATA and places it in
 * the string passed in (a pointer to the string is returned) if there was an
 * error then NULL is returned and the string returned will be empty.
 *
 * The address of the current dir dot (.) is passed in.  The function
 * opens the parent (..) and searches it for an entry with the address
 * passed in.  It then copies the name of this entry into s, which needs
 * to be a string large enough to hold it (29 chars).  The string s is
 * passed in.
 *
 * If the value of s is "." then we've reached the root, so s is replaced
 * by the current device number.
 *
 */

#include <stdinc.h>
#ifndef OSK
#include <string.h>
#include <lowio.h>
#else
#include <strings.h>
#include <modes.h>
union {
   char text[4];
   long numb;
} temp;
#endif

char *getspwd(s)
char *s;
{
 char tmp[32]; /* buffer to hold device name */
 long addr, /* address (LSN) to search for */
      addr2; /* current address */
 int  dpath; /* directory path */
 char done;

 s[0]='\0';

#ifndef OSK
 if ((dpath=open(".", READ | DIR))==ERROR)
#else
 if ((dpath=open(".", S_IREAD))==ERROR)
#endif
    return(NULL);

/* find the dot '.' entry */
 done=FALSE;
 while (!done)
 {
    if (read(dpath, tmp, 32)<32)
    { /* disk error */
       close(dpath);
       return(NULL);
    }
    if (tmp[0]==0xae) /* $ae == '.' with high bit set */
       done=TRUE;
 }
 close(dpath);
/* l3tol(&addr, tmp+29, 1); */
   memcpy(temp.text,&tmp[28],4); /* copy file id over */
   addr=temp.numb;

#ifndef OSK
 if ((dpath=open("..", READ | DIR))==ERROR)
#else
 if ((dpath=open("..", S_IREAD))==ERROR)
#endif
    return(NULL);

/* now match it with a name in it's parent (..) */
 done=FALSE;
 while (!done)
 {
    if (read(dpath, tmp, 32)<32)
    {
       close(dpath);
       return(NULL);
    }
/*    l3tol(&addr2, tmp+29, 1); */
   memcpy(temp.text,&tmp[28],4); /* copy file id over */
   addr2=temp.numb;
    if (addr==addr2)
       done=TRUE;
 }

/* when we get here either dentry is what we want or NULL */
 strhcpy(s, tmp);
 if ((s[0]=='.'))
 {
    _gs_devn(dpath, tmp);
    strhcpy(&s[1], tmp);
    s[0]='/';
 }
 close(dpath);
 return(s);
} /* end getspwd() */

/* end getspwd.c */
