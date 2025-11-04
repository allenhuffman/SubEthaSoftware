/* getpwd - version 1.0
 *      WB - Chris Hyde, 11/94
 *
 * This function returns a pointer to a string that contains the pathlist
 * to the current data directory.  The string is static and is overwritten
 * each call.  If there is an error then NULL is returned and the error number
 * can be found in errno.
 *
 * The function repeatedly calls getspwd() which gets the name of the current
 * data directory.  After each call it changes to the current directory's
 * parent before calling again.
 *
 * The final pathlist is built up by concatenating the strings returned by
 * getspwd() together (including '/' where appropriate).
 *
 * Rather than declare a static string large enough to hold any legal pathlist
 * the string is dynamically allocated and freed as each name is added.
 *
 */

#ifndef OSK
#include <string.h>
#else
#include <strings.h>
#endif
#include <stdinc.h>

char *getspwd();

char *getpwd()
{
 char cname[30], /* name of current directory */
      *s;        /* temp pointer for allocated memory */
 static char *plist; /* the final path list */
 int len1, len2; /* lengths of cname and plist, used to build plist */

 if (plist)
    free(plist);
 for (len1=0, len2=0, plist=NULL;
      (getspwd(cname)!=NULL) && (cname[0]!='/') && (cname[0]!='\0');
      chdir(".."), len1=strlen(plist))
 {
    len2=strlen(cname);
    if ((s=calloc(len1+len2+2, 1))==NULL)
    {
       if (plist)
          free(plist);
       return(NULL);
    }
    *s='/';
    strcpy(s+1, cname);
    if (plist)
       strcpy(s+len2+1, plist);
    *(s+len1+len2+2)='\0';

    if (plist)
       free(plist);
    plist=s;
 } /* end for */
 if (cname[0]=='\0') /* problem getting part of path */
    return(NULL);

 len2=strlen(cname);
 if ((s=calloc(len1+len2+2, 1))==NULL)
 {
    if (plist)
       free(plist);
    return(NULL);
 }
 strcpy(s, cname);
 if (plist)
    strcpy(s+len2, plist);
 *(s+len1+len2+1)='\0';
 if (plist)
    free(plist);
 plist=s;
 if (plist)
    chdir(plist); /* change back to original directory */
 return(plist);
} /* end getpwd() */

/* end getpwd.c */
