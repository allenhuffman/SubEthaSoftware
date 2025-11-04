IOBox(wpath)
int wpath;
{
/*   PopUp(wpath,"Shell Monitor",1,5,77,15); */
#ifdef TCWIN
   foreground(7);
   background(0);
   clearscreen();
#else
   PopUp(wpath,"Shell Monitor",0,2,79,21);
#endif
}

EndIOBox(wpath)
int wpath;
{
#ifdef TCWIN
   clearscreen();
   ShowAll();
#else
   OWEnd(wpath);
   OWEnd(wpath);
#endif
}

