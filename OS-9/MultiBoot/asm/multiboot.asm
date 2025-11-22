; f9dasm: M6800/1/2/3/8/9 / H6309 Binary/OS9/FLEX9 Disassembler V1.83
; Loaded binary file multiboot.bin

;****************************************************
;* Used Labels                                      *
;****************************************************

M0001   EQU     $0001
RSTFLG  EQU     $0071
CURPOS  EQU     $0088
DCOPC   EQU     $00EA
DCTRK   EQU     $00EC
DCBPT   EQU     $00EE
DCSTA   EQU     $00F0
M0155   EQU     $0155
M0156   EQU     $0156
M01DA   EQU     $01DA
M0200   EQU     $0200
M04A3   EQU     $04A3
M04A4   EQU     $04A4
M05C0   EQU     $05C0
OS      EQU     $2600
DOS     EQU     $2602
M3800   EQU     $3800
M3B00   EQU     $3B00
M3F00   EQU     $3F00
DLOAD   EQU     $8C1B
VDGPATCH EQU     $95C9
POLCAT  EQU     $A000
CHROUT  EQU     $A002
CLS     EQU     $A928
DSKCON  EQU     $C004
IOERROR EQU     $D709
COL32   EQU     $F652
VDGPIA  EQU     $FF22
FOREPAL32 EQU     $FFBC

;****************************************************
;* Program Code / Data Areas                        *
;****************************************************

        ORG     $3900

START   LDX     #M3800                   ;3900: 8E 38 00       '.8.'
        LDY     #OS                      ;3903: 10 8E 26 00    '..&.'
CPYLOOP LDD     ,X++                     ;3907: EC 81          '..'
        STD     ,Y++                     ;3909: ED A1          '..'
        CMPX    #START                   ;390B: 8C 39 00       '.9.'
        BCS     CPYLOOP                  ;390E: 25 F7          '%.'
        LBSR    PATCH32                  ;3910: 17 01 98       '...'
        LBSR    PRTSTACK                 ;3913: 17 01 AB       '...'
        FCC     "        MultiBoot V1."  ;3916: 20 20 20 20 20 20 20 20 4D 75 6C 74 69 42 6F 6F 74 20 56 31 2E '        MultiBoot V1.'
        FCC     "12"                     ;392B: 31 32          '12'
        FCB     $0D                      ;392D: 0D             '.'
        FCC     " by Terry Todd "        ;392E: 20 62 79 20 54 65 72 72 79 20 54 6F 64 64 20 ' by Terry Todd '
        FCB     $26                      ;393D: 26             '&'
        FCC     " Allen Huffman"         ;393E: 20 41 6C 6C 65 6E 20 48 75 66 66 6D 61 6E ' Allen Huffman'
        FCB     $0D                      ;394C: 0D             '.'
        FCC     "     Copyright (C) 19"  ;394D: 20 20 20 20 20 43 6F 70 79 72 69 67 68 74 20 28 43 29 20 31 39 '     Copyright (C) 19'
        FCC     "93 by"                  ;3962: 39 33 20 62 79 '93 by'
        FCB     $0D                      ;3967: 0D             '.'
        FCC     "       Sub-Etha Softw"  ;3968: 20 20 20 20 20 20 20 53 75 62 2D 45 74 68 61 20 53 6F 66 74 77 '       Sub-Etha Softw'
        FCC     "are"                    ;397D: 61 72 65       'are'
        FCB     $0D,$00                  ;3980: 0D 00          '..'
        LDY     #M05C0                   ;3982: 10 8E 05 C0    '....'
        STY     CURPOS                   ;3986: 10 9F 88       '...'
        LBSR    PRTSTACK                 ;3989: 17 01 35       '..5'
        FCC     "     Use UP/DOWN to s"  ;398C: 20 20 20 20 20 55 73 65 20 55 50 2F 44 4F 57 4E 20 74 6F 20 73 '     Use UP/DOWN to s'
        FCC     "croll"                  ;39A1: 63 72 6F 6C 6C 'croll'
        FCB     $0D                      ;39A6: 0D             '.'
        FCC     "[ENTER] Selects   [BR"  ;39A7: 5B 45 4E 54 45 52 5D 20 53 65 6C 65 63 74 73 20 20 20 5B 42 52 '[ENTER] Selects   [BR'
        FCC     "EAK] Quits"             ;39BC: 45 41 4B 5D 20 51 75 69 74 73 'EAK] Quits'
        FCB     $00                      ;39C6: 00             '.'
Z39C7   LDB     DELAY2                   ;39C7: F6 3A EB       '.:.'
        LBSR    Z3ADD                    ;39CA: 17 01 10       '...'
        LDU     #M04A4                   ;39CD: CE 04 A4       '...'
        LDB     CHOICES                  ;39D0: F6 3A FF       '.:.'
        LBEQ    GODOS                    ;39D3: 10 27 00 D1    '.'..'
        SUBB    DELAY2                   ;39D7: F0 3A EB       '.:.'
        CMPB    #$08                     ;39DA: C1 08          '..'
        BCS     Z39E0                    ;39DC: 25 02          '%.'
        LDB     #$08                     ;39DE: C6 08          '..'
Z39E0   PSHS    X,B                      ;39E0: 34 14          '4.'
        STU     CURPOS                   ;39E2: DF 88          '..'
        JSR     PRINT                    ;39E4: BD 3A D0       '.:.'
        PULS    X,B                      ;39E7: 35 14          '5.'
        LEAX    $20,X                    ;39E9: 30 88 20       '0. '
        LEAU    $20,U                    ;39EC: 33 C8 20       '3. '
        DECB                             ;39EF: 5A             'Z'
        BNE     Z39E0                    ;39F0: 26 EE          '&.'
Z39F2   LDA     DELAY1                   ;39F2: B6 3A EA       '.:.'
        SUBA    DELAY2                   ;39F5: B0 3A EB       '.:.'
        LDB     #$20                     ;39F8: C6 20          '. '
        MUL                              ;39FA: 3D             '='
        ADDD    #M04A3                   ;39FB: C3 04 A3       '...'
        TFR     D,X                      ;39FE: 1F 01          '..'
        LDA     #$6A                     ;3A00: 86 6A          '.j'
        STA     ,X                       ;3A02: A7 84          '..'
        LDY     DELAY                    ;3A04: 10 BE 3A FD    '..:.'
DLYLOOP LEAY    -$01,Y                   ;3A08: 31 3F          '1?'
        BNE     GETKEY                   ;3A0A: 26 05          '&.'
        LEAS    $02,S                    ;3A0C: 32 62          '2b'
        LBRA    GODOS                    ;3A0E: 16 00 97       '...'
GETKEY  JSR     [POLCAT]                 ;3A11: AD 9F A0 00    '....'
        BEQ     DLYLOOP                  ;3A15: 27 F1          ''.'
        LDB     #$60                     ;3A17: C6 60          '.`'
        STB     ,X                       ;3A19: E7 84          '..'
        LDB     #$FF                     ;3A1B: C6 FF          '..'
        CMPA    #$5E                     ;3A1D: 81 5E          '.^'
        BEQ     UP                       ;3A1F: 27 11          ''.'
        CMPA    #$0A                     ;3A21: 81 0A          '..'
        BEQ     DOWN                     ;3A23: 27 24          ''$'
        CMPA    #$0D                     ;3A25: 81 0D          '..'
        BEQ     ENTER                    ;3A27: 27 3E          ''>'
        CMPA    #$03                     ;3A29: 81 03          '..'
        BNE     Z39F2                    ;3A2B: 26 C5          '&.'
        CLR     RSTFLG                   ;3A2D: 0F 71          '.q'
        JMP     DLOAD                    ;3A2F: 7E 8C 1B       '~..'
UP      LDA     DELAY1                   ;3A32: B6 3A EA       '.:.'
        BEQ     Z39F2                    ;3A35: 27 BB          ''.'
        STB     M0155                    ;3A37: F7 01 55       '..U'
        DECA                             ;3A3A: 4A             'J'
        STA     DELAY1                   ;3A3B: B7 3A EA       '.:.'
        CMPA    DELAY2                   ;3A3E: B1 3A EB       '.:.'
        BCC     Z3A46                    ;3A41: 24 03          '$.'
        DEC     DELAY2                   ;3A43: 7A 3A EB       'z:.'
Z3A46   LBRA    Z39C7                    ;3A46: 16 FF 7E       '..~'
DOWN    LDA     DELAY1                   ;3A49: B6 3A EA       '.:.'
        INCA                             ;3A4C: 4C             'L'
        CMPA    CHOICES                  ;3A4D: B1 3A FF       '.:.'
        BCC     Z39F2                    ;3A50: 24 A0          '$.'
        STB     M0156                    ;3A52: F7 01 56       '..V'
        STA     DELAY1                   ;3A55: B7 3A EA       '.:.'
        LDA     DELAY2                   ;3A58: B6 3A EB       '.:.'
        ADDA    #$07                     ;3A5B: 8B 07          '..'
        CMPA    DELAY1                   ;3A5D: B1 3A EA       '.:.'
        BCC     Z3A46                    ;3A60: 24 E4          '$.'
        INC     DELAY2                   ;3A62: 7C 3A EB       '|:.'
        BRA     Z3A46                    ;3A65: 20 DF          ' .'
ENTER   LDB     DELAY1                   ;3A67: F6 3A EA       '.:.'
        INCB                             ;3A6A: 5C             '\'
        BSR     Z3ADD                    ;3A6B: 8D 70          '.p'
        LEAX    -$05,X                   ;3A6D: 30 1B          '0.'
        PSHS    X                        ;3A6F: 34 10          '4.'
        LDX     #DCOPC                   ;3A71: 8E 00 EA       '...'
        LDD     #M0200                   ;3A74: CC 02 00       '...'
        STD     ,X   ; (DCOPC-DCOPC)     ;3A77: ED 84          '..'
        LDD     #M0001                   ;3A79: CC 00 01       '...'
        STD     DCTRK-DCOPC,X            ;3A7C: ED 02          '..'
        LDD     #M01DA                   ;3A7E: CC 01 DA       '...'
        STD     DCBPT-DCOPC,X            ;3A81: ED 04          '..'
        JSR     [DSKCON]                 ;3A83: AD 9F C0 04    '....'
        TST     DCSTA-DCOPC,X            ;3A87: 6D 06          'm.'
        BNE     GOIOERR                  ;3A89: 26 4F          '&O'
        LDY     DCBPT-DCOPC,X            ;3A8B: 10 AE 04       '...'
        LEAY    $15,Y                    ;3A8E: 31 A8 15       '1..'
        PULS    U                        ;3A91: 35 40          '5@'
        LDB     #$05                     ;3A93: C6 05          '..'
CPYLOOP2 LDA     ,U+                      ;3A95: A6 C0          '..'
        STA     ,Y+                      ;3A97: A7 A0          '..'
        DECB                             ;3A99: 5A             'Z'
        BNE     CPYLOOP2                 ;3A9A: 26 F9          '&.'
        LDA     #$03                     ;3A9C: 86 03          '..'
        STA     ,X   ; (DCOPC-DCOPC)     ;3A9E: A7 84          '..'
        JSR     [DSKCON]                 ;3AA0: AD 9F C0 04    '....'
        TST     DCSTA-DCOPC,X            ;3AA4: 6D 06          'm.'
        BNE     GOIOERR                  ;3AA6: 26 32          '&2'
GODOS   JMP     DOS                      ;3AA8: 7E 26 02       '~&.'
PATCH32 LDA     #$57                     ;3AAB: 86 57          '.W'
        STA     VDGPATCH                 ;3AAD: B7 95 C9       '...'
        LDA     #$10                     ;3AB0: 86 10          '..'
        STA     VDGPIA                   ;3AB2: B7 FF 22       '.."'
        LDD     #M3F00                   ;3AB5: CC 3F 00       '.?.'
        STD     FOREPAL32                ;3AB8: FD FF BC       '...'
        JSR     COL32                    ;3ABB: BD F6 52       '..R'
        JMP     CLS                      ;3ABE: 7E A9 28       '~.('
PRTSTACK LDX     ,S                       ;3AC1: AE E4          '..'
PRTLOOP LDA     ,X+                      ;3AC3: A6 80          '..'
        BEQ     RETSTACK                 ;3AC5: 27 06          ''.'
        JSR     [CHROUT]                 ;3AC7: AD 9F A0 02    '....'
        BRA     PRTLOOP                  ;3ACB: 20 F6          ' .'
RETSTACK STX     ,S                       ;3ACD: AF E4          '..'
RETURN  RTS                              ;3ACF: 39             '9'
PRINT   LDA     ,X+                      ;3AD0: A6 80          '..'
        BEQ     RETURN                   ;3AD2: 27 FB          ''.'
        JSR     [CHROUT]                 ;3AD4: AD 9F A0 02    '....'
        BRA     PRINT                    ;3AD8: 20 F6          ' .'
GOIOERR JMP     IOERROR                  ;3ADA: 7E D7 09       '~..'
Z3ADD   LDX     #M3B00                   ;3ADD: 8E 3B 00       '.;.'
        TSTB                             ;3AE0: 5D             ']'
        BEQ     RETURN                   ;3AE1: 27 EC          ''.'
DOWN2   LEAX    $20,X                    ;3AE3: 30 88 20       '0. '
        DECB                             ;3AE6: 5A             'Z'
        BNE     DOWN2                    ;3AE7: 26 FA          '&.'
        RTS                              ;3AE9: 39             '9'
DELAY1  FCB     $00                      ;3AEA: 00             '.'
DELAY2  FCB     $00,$00,$00,$00,$00,$00  ;3AEB: 00 00 00 00 00 00 '......'
        FCB     $00,$00,$00,$00,$00,$00  ;3AF1: 00 00 00 00 00 00 '......'
        FCB     $00,$00,$00,$00,$00,$00  ;3AF7: 00 00 00 00 00 00 '......'
DELAY   FDB     $4242                    ;3AFD: 42 42          'BB'
CHOICES FCB     $00                      ;3AFF: 00             '.'

        END
