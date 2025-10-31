Button(wpath,x,y,text)
int wpath,x,y;
char *text;
{
   int stx,sty,len;
   stx=x*8-4;
   sty=y*8;
   len=strlen(text)*8+6;
 
   FColor(wpath,2); /* light grey */
   SetDPtr(wpath,stx,sty);
   Bar(wpath,stx+len,sty+7);
 
   FColor(wpath,3); /* white */
   SetDPtr(wpath,stx,sty-1);
   Line(wpath,stx+len,sty-1);
   SetDPtr(wpath,stx-1,sty);
   Line(wpath,stx-1,sty+7);
 
   FColor(wpath,1); /* dark grey */
   SetDPtr(wpath,stx+len+1,sty);
   Line(wpath,stx+len+1,sty+7);
   SetDPtr(wpath,stx,sty+8);
   Line(wpath,stx+len,sty+8);
 
   FColor(wpath,0); /* black */
   CurXY(wpath,x,y);
   TCharSw(wpath,1);
   write(wpath,text,strlen(text));
   TCharSw(wpath,0);
   FColor(wpath,3); /* white */
}
