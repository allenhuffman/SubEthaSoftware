/*
 * Misc. routines for MM/1 which are not part of it's library, but are
 * part of the CoCo 3 L2 library.
 */

#ifdef TCWIN /* hack */
#define STDOUT -1
#endif

cwrite(wpath,text,len) /* write null terminated text */
int wpath;
char *text;
int len;
{
#ifdef TCWIN
   if (strlen(text)>len) text[len]='\0';
   Text(STDOUT,text);
#else
   write(wpath,text,strlen(text));
#endif
}

cwriteln(wpath,text,len) /* write null terminated text */
int wpath;
char *text;
int len;
{
#ifdef TCWIN /* this one actually doesn't work as cwriteln did... */
   if (strlen(text)>len) text[len]='\0';
   Text(STDOUT,text);
#else
   write(wpath,text,strlen(text));
#endif
}

#ifndef TCWIN
BlnkOn(wpath) /* not supported under K-Windows */
int wpath;
{
}

BlnkOff(wpath) /* not supported under K-Windows */
int wpath;
{
}
#endif
