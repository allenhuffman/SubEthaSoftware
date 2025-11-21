;****************************************************
;* Used Labels                                      *
;****************************************************

VD1     EQU     $00D1
VD3     EQU     $00D3
DCOPC   EQU     $00EA
DCTRK   EQU     $00EC
DCBPT   EQU     $00EE
DCSTA   EQU     $00F0
M0200   EQU     $0200
M3800   EQU     $3800
Z3900   EQU     $3900
ZC004   EQU     $C004
IOERROR EQU     $D709

;****************************************************
;* Program Code / Data Areas                        *
;****************************************************

        ORG     $2600

        FCC     "OS"                     ;2600: 4F 53          'OS'
        LDA     #$0D                     ;2602: 86 0D          '..'
        STA     VD3                      ;2604: 97 D3          '..'
        LDD     #M3800                   ;2606: CC 38 00       '.8.'
Z2609   STD     VD1                      ;2609: DD D1          '..'
        INC     VD3                      ;260B: 0C D3          '..'
        LDX     #DCOPC                   ;260D: 8E 00 EA       '...'
        LDD     #M0200                   ;2610: CC 02 00       '...'
        STD     ,X   ; (DCOPC-DCOPC)     ;2613: ED 84          '..'
        LDA     #$21                     ;2615: 86 21          '.!'
        LDB     VD3                      ;2617: D6 D3          '..'
        STD     DCTRK-DCOPC,X            ;2619: ED 02          '..'
        LDD     VD1                      ;261B: DC D1          '..'
        STD     DCBPT-DCOPC,X            ;261D: ED 04          '..'
        JSR     [ZC004]                  ;261F: AD 9F C0 04    '....'
        TST     DCSTA-DCOPC,X            ;2623: 6D 06          'm.'
        BNE     Z262F                    ;2625: 26 08          '&.'
        INCA                             ;2627: 4C             'L'
        CMPA    #$3D                     ;2628: 81 3D          '.='
        BCS     Z2609                    ;262A: 25 DD          '%.'
        JMP     Z3900                    ;262C: 7E 39 00       '~9.'
Z262F   JMP     IOERROR                  ;262F: 7E D7 09       '~..'

        END