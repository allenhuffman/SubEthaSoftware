; /* Terry’s RS-DOS DOS startup code… 50 bytes */

; lwasm dos.asm -fbasic -odos.bas --map --list

; Initial disassembly by Jerry "Super Basic" Stratton
; More work done using EDTASM+ Z-BUG

; Raw Bytes:
;
; 79,83,134,13,151,211,204,56,0,221,209,12,211,142,0,234,204,2,0,237,132,134,33,214,211,237,2,220,209,237,4,173,159,192,4,109,6,38,8,76,129,61,37,221,126,57,0,126,215,9,
;
; HEX Bytes:
; 4F,53,86,D,97,D3,CC,38,0,DD,D1,C,D3,8E,0,EA,CC,2,0,ED,84,0,86,21,D6,D3,ED,2,DC,D1,ED,4,AD,9F,C0,4,6D,6,26,8,4C,81,3D,25,0,DD,7E,39,0,7E,D7,9,
;

DSKCON  EQU     C004

; 7E - CBUFAD RMB 2 - CASSETTE LOAD BUFFER POINTER


        ORG     $25EF

L25EF   FCB     0
L25F0   FCB     0

        ORG     $2600

OS      FCB     'O
        FCB     'S

        LDA     #$D
        STA     $D3         ; CBUFAD
        LDD     #$3800
        STD     $D1         ; VD1 - scratch pad vars
        INC     $D3         ; VD3 - scratch pad vars
        LDX     #$EA        ; DCOPC - DSKCON op code 0-3
                            ; DCDRV - DSKCON drive number 0-3
        LDD     #$200       ; 512? or...
        STD     ,X          ; DCOPC=02, DCDRV=00
        NEG     $86         ; GRBLOK - block for set/reset/point
        BRN     L25EF
        ADDD    $ED         ; DSEC - DSKCON sector number 1-18
L261B   FCB     2           ; ??
        LDD     $D1         ; VD1
        STD     4,X         ; DCBPT - DSKCON data pointer
        JSR     [$C004]     ; DSKCON
        TST     6,X         ; DCSTA - DSKCON status byte
        BNE     L2630
        INCA
        CMPA    #$3D        ; 61
        BLO     L262D
L262D   STD     $7E         ; CBUFAD - cassete load buffer pointer
        RTS
L2630   NEG     $7E         ; CBUFAD - cassete load buffer pointer
        STB     $9          ; INPFLG - input flag (read=0, input<>0)
        NEG     $0          ; ENDFLG - stop/end flag (+ stop, - end)

        END
