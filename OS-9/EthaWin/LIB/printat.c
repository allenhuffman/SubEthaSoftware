PrintAt(wpath,x,y,text)
int wpath,x,y;
char *text;
{
   CurXY(wpath,x,y);
   write(wpath,text,strlen(text));
}
