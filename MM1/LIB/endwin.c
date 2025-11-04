EndWin(wpath) /* kill overlay window */
int wpath;
{
   OWEnd(wpath);
#ifndef TCWIN
   OWEnd(wpath);
#endif
}
