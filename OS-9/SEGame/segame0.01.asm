*
* Game demo thingy.
*

TypeLang equ $11    module type / data type
AttRev   equ $81    module attributes / revision
Edition  equ $00    module edition
Stack    equ $200   stack size

 psect Test,TypeLang,AttRev,Edition,Stack,start

 vsect

buffer rmb 10       temporary buffer stuff

path rmb 1          path to window
screen rmb 2        start location of screen memory
end rmb 2           end location of screen memory
block rmb 2         start block of screen
number rmb 1        number of 8K blocks used by screen
offset rmb 2        offset from start block to start of screen

locx rmb 2          x position of ship
maxleft rmb 2
maxright rmb 2

enemyx rmb 20       room for 10 enemy locations
movecount rmb 1     how fast enemies move (delay)

starloc rmb 30      location of stars
starbyte rmb 1

missiles rmb 20     location of missiles (5 of 'em)
firecount rmb 1     how fast we can fire (delay)

 endsect

StdIn equ 0
StdOut equ 1
StdErr equ 2

* Start.  Begin.  Proceed.  Initiate.

start
 clrb
 lda ,x+
 cmpa #13
 lbne info

 lda #3             read/write
 leax nextwin,pcr   location of /W string
 os9 I$Open
 lbcs error          exit if error
 sta path,u
 ldy #mwinlen       load length
 leax makewin,pcr   point to display data
 os9 I$Write        output it

 lda #StdIn
 os9 I$Close
 lda path
 os9 I$Dup

 lda path         find out about current screen
 ldb #$8f (SS.ScInf)
 pshs u
 os9 I$GetStt
 puls u
 stx offset,u       save offset of window start in first block
 sta number,u       store number of blocks in use
 stb block+1,u      store starting block number

 ldx block          get starting block number
 ldb number         get number of blocks we want to map in
 pshs u             save u
 os9 F$MapBlk       map them into user space
 lbcs info          exit if error
 tfr u,d            put u in d (u=address of first block)
 puls u             restore u
 addd offset        add offset to d
 std screen,u       save start of screen
 tfr d,x            put d in x (use in a moment)
 lda number         load a with number of 8K blocks we have
endloop
 leax 8*1024,x      add 8k each time
 deca
 bne endloop
 stx end,u          save end position of screen

* Screen is mapped in.  Ready to proceed.

 ldx screen         clear screen loop
 ldd #0
clear std ,x++
 cmpx end
 blo clear

 lda #20            initialize missiles
 leay missiles,u
mclear clr ,y+
 deca
 bne mclear

 ldx screen         get start of screen
 leax 170*80,x      150 rows down or so...far left
 stx maxleft,u      save this as max left
 leax 76,x          add 76
 stx maxright,u     save this as max right
 leax -36,x         go back to center
 stx locx,u         save initial ship location

 lda #10            initialize enemies
 sta buffer
 leax enemies,pcr
 leay enemyx,u
estart ldd ,x++
 addd screen
 std ,y++
 dec buffer
 bne estart

 lda #15
 sta buffer,u
 leax stars,pcr
 leay starloc,u
starloop
 ldd ,x++
 addd screen
 std ,y++
 dec buffer
 bne starloop
 
 lbra moveskip

game
 ldx #2             snooze for 2 ticks (too fast if we don't!)
 os9 F$Sleep

 lda #15            draw stars
 leay starloc,u
putstars
 ldx ,y
 clr ,x             store it
 leax 80,x
 cmpa #5
 ble xxx
 leax 80,x
 cmpa #10
 ble xxx
 leax 80,x
xxx
 cmpx end
 ble starcont
 leax -80*180-1,x
starcont inc starbyte
 ldb starbyte
 andb #%00001111
 stb ,x
 stx ,y++
 deca
 bne putstars

 lda #10
 leay missiles,u    point x to start of missile packet
mloop ldx ,y        get first missile location
 cmpx #0            if location is 0, no missile to fire
 beq mskip
 clr ,x
 leax 3,x
 clr ,x
 leax -163,x
 cmpx screen        is it at top of screen?
 bge mcont          if not, continue. else
 ldx #0
 stx ,y
 bra mskip
mcont ldb #%00111100
 stb ,x
 leax 3,x
 stb ,x
 leax -3,x
 stx ,y             * put collision check in here...
 ldb #10            10 baddies
mskip
 leay 2,y
 deca
 bne mloop

 ldb #$27
 os9 I$GetStt
 cmpa #0
 lbeq moveskip

 bita #%10000000    space bar
 bne fire
 
 ldx locx
 lbsr eraseobj

 bita #%00100000    left arrow
 bne moveleft
 bita #%01000000    right arrow
 bne moveright
 lbra shutdown       neither...

fire dec firecount
 lda firecount
 anda #3
 bne moveskip
 lda #10
 leay missiles,u
floop ldx ,y++
 cmpx #0
 beq fireit
 deca
 bne floop
 bra moveskip
fireit ldx locx
 stx -2,y

 lda path           make sound
 ldb #$98
 ldx #$3f00
 ldy #3800
 os9 I$SetStt
 bra moveskip

moveleft
 ldx locx
 cmpx maxleft
 ble moveskip
 leax -1,x
 stx locx,u
 bra moveskip
moveright ldx locx
 cmpx maxright
 bge moveskip
 leax 1,x
 stx locx,u

moveskip
 ldx locx           put ship
 leay ship,pcr
 bsr putobj

* here we need to erase and move the bad guys...

 lda #10            Put 10 bad guys
 leax enemyx,u
 pshs u
 tfr x,u
eploop
 leay enemy,pcr
 ldx ,u++
 beq epskip
 bsr putobj
epskip
 deca
 bne eploop
 puls u

 lbra game

putobj ldb #16      *load x with location, point y to data
 pshs u
putloop
 ldu ,y++
 stu ,x++
 ldu ,y++
 stu ,x
 leax 78,x
 decb
 bne putloop
 puls u
 rts

eraseobj ldb #16     load x with location of object to clear
 ldy #$0000
erase2loop
 sty ,x
 leax 2,x
 sty ,x
 leax 78,x
 decb
 bne erase2loop
 rts

* End.  Map screen back out and return to OS-9.

shutdown
 ldb number         get number of blocks
 pshs u             save u
 ldu block          get starting block number
 os9 F$ClrBlk
 puls u             restore u

 lda #path
 os9 I$Close

 lda #StdOut
 ldy #2
 leax select,pcr
 os9 I$Write

exit clrb
error os9 F$Exit

info lda #StdOut
 ldy #helpsize
 leax helpmsg,pcr
 os9 I$Write
 bra error

helpmsg fcb $d,$a
 fcc "SEGame 0.01 by Allen Huffman of Sub-Etha Software (coco-sysop@genie.geis.com)"
 fcb $d,$a
 fcc "Syntax: SEGame"
 fcb $d,$a
 fcc "Usage : You must have NitrOS9 (SS.ScInf GetStt call) to run this DEMO!"
 fcb $d,$a
 fcc "        Use LEFT/RIGHT to Move. SPACE to Fire.  UP/DOWN/CTRL/ALT/SHIFT to Quit."
 fcb $d,$a
 fcc "Opts  : None at All."
 fcb $d,$a
helpsize equ *-helpmsg

ship
     fdb %0000110000000000,%0000000000110000
     fdb %0000110000000000,%0000000000110000
     fdb %0000110000001111,%1111000000110000
     fdb %0000110011110010,%1000111100110000
     fdb %0000111100000010,%1000000011110000
     fdb %0011000000000010,%1000000000001100
     fdb %0011000000000010,%1000000000001100
     fdb %1100000000000010,%1000000000000011
     fdb %1100000000000110,%1001000000000011
     fdb %1100000000010110,%1001010000000011
     fdb %1100000001010010,%1000010100000011
     fdb %0011000101000010,%1000000101001100
     fdb %0011110100000010,%1000000001111100
     fdb %0000111100000010,%1000000011110000
     fdb %0000000011110010,%1000111100000000
     fdb %0000000000001111,%1111000000000000

enemy
     fdb %0000000000001111,%1111000000000000
     fdb %0000000011110010,%1000111100000000
     fdb %0000111100000010,%1000000011110000
     fdb %0011000000001000,%0010000000001100
     fdb %0011000000001000,%0010000000001100
     fdb %1100000000100000,%0000100000000011
     fdb %1100000000100000,%0000100000000011
     fdb %1100000010101010,%1000001000000011
     fdb %1100000010000000,%0010101000000011
     fdb %0011001000000000,%0000000010001100
     fdb %0011111000000000,%0000000010011100
     fdb %0000101100000000,%0000000011100000
     fdb %0000100011110000,%0000111100100000
     fdb %0010000000001111,%1111000000001000
     fdb %0001000000000000,%0000000000000100
     fdb %0001000000000000,%0000000000000100

stars fdb 100,1343,1123,7574,1234,6794,5346,12039,8574,9293,7283,3546,11023
     fdb 9675,2387

enemies fdb 1600,1610,1620,1630,1640
        fdb 3200,3210,3220,3230,3240

nextwin fcc "/W"
 fcb 13

makewin fcb $1b,$20,$6,0,0,40,24,3,0,0,$1b,$32,3,$1b,$33,0,$1b,$34,0
        fcb $1b,$31,0,0,$1b,$31,1,$10,$1b,$31,2,$20,$1b,$31,3,$3f,$05,$20
select fcb $1b,$21
mwinlen equ *-makewin

 endsect
