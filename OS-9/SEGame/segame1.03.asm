*
* SEGame V1.03 by Allen Huffman and Robert Gault
*
* Allen's code modified by R.Gault to remove NitrOS9 call, use direct
* page notation, unsigned tests, and cleaned code.
*
* Code to map in screen using NitrOS9 is still here, but commented out.
* This version should work on a stock OS-9 system.
* 
* 1.00 08/25/94 - Robert submits corrections/updates
* 1.01 08/29/94 - Further source comments for OS9 Underground
* 1.02 05/16/95 - Redo screen map code
* 1.03 07/20/96 - Slight code cleanup
*

TypeLang equ $11    module type / data type
AttRev   equ $81    module attributes / revision
Edition  equ $00    module edition
Stack    equ $200   stack size

 psect SEGame,TypeLang,AttRev,Edition,Stack,start

 vsect

path rmb 1          path to new window
oldpath rmb 1       path to original window
buffer rmb 10       temporary buffer stuff
screen rmb 2        start location of screen memory
end rmb 2           end location of screen memory
block rmb 2         start block of screen
number rmb 1        number of 8K blocks used by screen
offset rmb 2        offset from start block to start of screen

locx rmb 2          x position of ship
maxleft rmb 2       max left of ship
maxright rmb 2      max right of ship
starloc rmb 30      location of stars
starbyte rmb 1      star

missiles rmb 20     location of missiles (10 of 'em)
firecount rmb 1     fire delay

 endsect

StdIn equ 0
StdOut equ 1
StdErr equ 2

* Start.  Begin.  Proceed.  Initiate.

start
 clrb               scan command line
 lda ,x+
 cmpa #13
 lbne info          if something, print usage

 clra               dup std. in
 os9 I$Dup
 sta <oldpath       save it
 lda #3             read/write
 leax nextwin,pcr   location of /W string
 os9 I$Open         open path to new window
 lbcs error         exit if error
 sta <path          save path to new window
 leax makewin,pcr   point to display data
 ldy #mwinlen       load length
 os9 I$Write        output it (creates new window)

 lda #StdIn         close old stdin and stdout
 os9 I$Close
 lda #StdOut
 os9 I$Close
 lda <path          dup new window twice making it
 os9 I$Dup          stdin and stdout
 lda <path
 os9 I$Dup

* NitrOS-9 code to find out which blocks our screen is using

nitrostart
 lda <path          NitrOS-9 screen map routine
 ldb #$8f           SS.ScInf
 pshs u             save u (modified by system call)
 os9 I$GetStt       get info on screen
 puls u             restore u
 bcs stockstart     if error, NitrOS9 must not be in use
 stx <offset        save offset of window start in first block
 sta <number        store number of blocks in use
 stb <block+1       store starting block number
 bra mapin

* Stock OS-9 code to find out which blocks the visible screen is using
stockstart
idle lda <path      test to see if screen is showing by asking mouse
 ldb #$89           SS.Mouse; this call is supposed to be good for
*                   CC3IO, GrfInt, and WindInt; see manual
 leax buffer,u
 ldy #0             automatic port selection
 os9 I$GetStt       get mouse status
 tst ,x             is Pt.Valid good? 0=no 1=yes
 bne OKscr          if not 0 (pointer valid), we must be looking at screen
 ldx #3             otherwise, sleep for a bit...
 os9 F$Sleep
 bra idle           check again for desired screen as active screen
OKscr ldx #3
 os9 F$Sleep        let system settle
*************************************************************************
 ldd #0             find out about current screen using stock OS-9
 std <buffer
 leax >buffer,u
 tfr x,d
******************* change for >512K systems; stock OS-9 ****************
 ldx #$9b           was $9D changed to $9B for >512K systems RG
 pshs u
 leau >block,u      was block+1 RG
 ldy #3             was 1       RG
 os9 F$CpyMem
 puls u
 lda <block         was lsr <block+1
 ldb <block+2           lsr <block+1
 lsra
 rorb
 lsra
 rorb
 std <block        16bit number $9B*256+$9D divided by 4; result stored RG
 ldx #0
 stx <offset       save offset of window start in first block
 lda #4
 sta <number       store number of blocks in use
* Map screen into program's address space

mapin
 ldx <block         get starting block number
 ldb <number        get number of blocks we want to map in
 pshs u             save u
 os9 F$MapBlk       map them into user space
 lbcs info          exit if error
 tfr u,d            put u in x (u=address of first block)
 puls u             restore u
 addd <offset       add offset to d
 std <screen        save start of screen
 addd #191*160      end of screen
 std <end           save end position of screen

* Screen is mapped in.  Ready to proceed.

 lda #20            initialize missiles to 0
 leay missiles,u
mclear clr ,y+
 deca
 bne mclear

 ldx <screen        get start of screen
 leax 170*160,x      150 rows down or so...far left
 stx <maxleft       save this as max left
 leax 80*2-4*2,x          add 76
 stx <maxright      save this as max right
 leax -36*2,x         go back to center
 stx <locx          save initial ship location

 lda #15            initialize star locations
 sta <buffer
 leax stars,pcr     point x to star init loc.
 leay starloc,u     point y to star data area
starloop
 ldd ,x++           get init loc.
 addd <screen       add screen offset
 std ,y++           save it in data area
 dec <buffer
 bne starloop       loop until done.
 
 lbra moveskip      display ship

game
 ldx #2             snooze for 2 ticks (too fast if we don't!)
 os9 F$Sleep

 lda #15            draw stars
 leay starloc,u     point to star locations
putstars
 ldx ,y             get location
 clr ,x             clear what is on screen
 leax 160,x          move down one line
 cmpa #5            5th star?
 bls xxx            no, skip
 leax 160,x          else move again
 cmpa #10           10th star?
 bls xxx            no, skip
 leax 160,x          else move again
xxx
 cmpx end           at end of screen?
 bls starcont       no, skip
 leax -160*180-1,x   else reset star
starcont
 inc <starbyte
 ldb <starbyte
 andb #$0F
 stb ,x             put star on screen
 stx ,y++           update star location
 deca               more stars?
 bne putstars       if so, do more

 lda #10            10 missiles to check
 leay missiles,u    point x to start of missile packet
mloop ldx ,y        get first missile location
 cmpx #0            if location is 0, no missile to fire
 beq mskip          so skip
 clr ,x             erase old missile
 clr 5,x
 leax -160,x        move up two rows
 cmpx screen        is it at top of screen?
 bhs mcont          if not, continue. else
 ldx #0
 stx ,y             else reset missile
 bra mskip
mcont
 ldb #$FF
 stb ,x             put missile on screen
 stb 5,x
 stx ,y
mskip
 leay 2,y           go to next missile ptr
 deca               more missiles?
 bne mloop          if so, do 'em

 ldb #$27           key sense syscall
 os9 I$GetStt
 cmpa #0            0?  No key
 beq moveskip       so skip

 bita #%10000000    check space bar
 bne fire
 
 ldb #16            else erase old ship
 pshs u             save u
 ldu locx           get byte at x
 ldy #$0000         y=0000
eraseloop
 sty ,u             erase old ship
 sty 2,u
 sty 4,u
 sty 6,u
 leau 160,u
 decb
 bne eraseloop
 puls u             restore u

 bita #%00100000    left arrow
 bne moveleft
 bita #%01000000    right arrow
 bne moveright
 bra shutdown       neither...

fire dec <firecount
 lda <firecount
 anda #3
 bne moveskip       time to fire?
 lda #10
 leay missiles,u
floop ldx ,y++      get missile loc.
 cmpx #0            0? not moving
 beq fireit         so fire it
 deca
 bne floop
 bra moveskip
fireit ldx <locx
 leax 1,x
 stx -2,y           store missile loc

 lda <path          make sound
 ldb #$98
 ldx #$3f00
 ldy #3800
 os9 I$SetStt

 bra moveskip

moveleft
 ldx <locx          get loc.
 cmpx <maxleft      at left?
 bls moveskip       yes, skip
 leax -1,x          else move left
 stx <locx          update loc.
 bra moveskip
moveright ldx <locx get loc.
 cmpx <maxright     at right?
 bhs moveskip       yes, skip
 leax 1,x           else move right
 stx <locx          update loc.

moveskip
 ldb #16            16 rows of ship data
 leax ship,pcr      point x to ship data
 pshs u             save u
 ldu <locx          point u to where ship will go
shiploop
 ldy ,x++           get 2 bytes of ship data
 sty ,u  `          store it on screen
 ldy ,x++           get next byte
 sty 2,u            store it on screen
 ldy ,x++
 sty 4,u
 ldy ,x++
 sty 6,u
 leau 160,u          next line
 decb               more data?
 bne shiploop       yes, do more
 puls u             restore u

 lbra game          do it all again

* End.  Map screen back out and return to OS-9.

shutdown
 ldb <number        get number of blocks
 pshs u             save u
 ldu <block         get starting block number
 os9 F$ClrBlk
 puls u             restore u

 lda #StdIn         close stdin and out
 os9 I$Close        on new screen
 lda #StdOut
 os9 I$Close
 lda <oldpath       restore original paths
 os9 I$Dup
 lda <oldpath
 os9 I$Dup

 lda #StdIn
 ldy #2             select original screen
 leax select,pcr
 os9 I$Write

exit clrb           clear b (no errors)
error os9 F$Exit    exit

info lda #StdOut    output help message
 ldy #helpsize
 leax helpmsg,pcr
 os9 I$Write
 bra error

helpmsg fcb $d,$a
 fcc "SEGame 1.03 by Allen Huffman of Sub-Etha Software (coco-sysop@genie.geis.com)"
 fcb $d,$a
 fcc "            with modifications by Robert Gault (ab282@detroit.freenet.org)."
 fcb $d,$a,$d,$a
 fcc "Syntax: SEGame"
 fcb $d,$a
 fcc "Usage : Simple little space game demo thingy."
 fcb $d,$a
 fcc "        Use LEFT/RIGHT to Move. SPACE to Fire.  UP/DOWN/CTRL/ALT/SHIFT to Quit."
 fcb $d,$a
 fcc "Opts  : None at All."
 fcb $d,$a
helpsize equ *-helpmsg

ship
     fdb $00F0,$0000,$0000,$0F00
     fdb $00F0,$0000,$0000,$0F00
     fdb $00F0,$00FF,$FF00,$0F00
     fdb $00F0,$FF01,$10FF,$0F00
     fdb $00FF,$0001,$1000,$FF00
     fdb $0F00,$0001,$1000,$00F0
     fdb $0F00,$0001,$1000,$00F0
     fdb $F000,$0001,$1000,$000F
     fdb $F000,$0012,$2100,$000F
     fdb $F000,$0112,$2110,$000F
     fdb $F000,$1101,$1011,$000F
     fdb $0F01,$1001,$1001,$10F0
     fdb $0FF1,$0001,$1000,$1FF0
     fdb $00FF,$0001,$1000,$FF00
     fdb $0000,$FF01,$10FF,$0000
     fdb $0000,$00FF,$FF00,$0000

* Initial star locations on scren.

stars fdb 100,1343,1123,7574,1234,6794,5346,12039,8574,9293,7283,3546,11023
     fdb 9675,2387

nextwin fcc "/W"
 fcb 13

makewin fcb $1b,$20,$8,0,0,40,24,3,0,0           create type 8 graphics screen
 fcb $1b,$32,3,$1b,$33,0,$1b,$34,0               set fore/back/border colors
 fcb $1b,$31,$0,0,$1b,$31,$1,$04                   set palette slots 1, 2
 fcb $1b,$31,$2,$06,$1b,$31,$3,$0a                 3 and 4
 fcb $1b,$31,$4,$0e,$1b,$31,$5,$13                 5, 6
 fcb $1b,$31,$6,$17,$1b,$31,$7,$1b                 7, 8
 fcb $1b,$31,$8,$1f,$1b,$31,$9,$24                 9, 10
 fcb $1b,$31,$a,$28,$1b,$31,$b,$2c                 11, 12
 fcb $1b,$31,$c,$32,$1b,$31,$d,$36                 13, 14
 fcb $1b,$31,$e,$3a,$1b,$31,$f,$3f                 15, 16
 fcb $05,$20                                     cursor off
select fcb $1b,$21                               select window
mwinlen equ *-makewin

 endsect
