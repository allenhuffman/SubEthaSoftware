/*
 * Misc. routines for MM/1 which are not part of it's library, but are
 * part of the CoCo 3 L2 library.
 */

cwrite(wpath,text,len) /* write null terminated text */
int wpath;
char *text;
int len;
{
   write(wpath,text,strlen(text));
}

cwriteln(wpath,text,len) /* write null or CR terminated text */
int wpath;
char *text;
int len;
{
   writeln(wpath,text,strlen(text));
}

BlnkOn(wpath) /* not supported under K-Windows */
int wpath;
{
}

BlnkOff(wpath) /* not supported under K-Windows */
int wpath;
{
}

