IOBox(wpath)
int wpath;
{
#ifdef TCWIN
   clearscreen();
#else
#ifdef OSK
   PopUp(wpath,"Shell Monitor",0,3,79,21);
#else
   PopUp(wpath,"Shell Monitor",0,3,79,19);
#endif
#endif
}

EndIOBox(wpath)
int wpath;
{
#ifdef TCWIN
   ShowAll();
#else
   OWEnd(wpath);
   OWEnd(wpath);
#endif
}

