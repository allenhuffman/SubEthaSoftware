Address   Byte   Opcode/Operand   Mnemonic/Comment
--------------------------------------------------
$2600:    79     79 83            LEA   (OS)   ; ASCII 'O','S'
$2602:    134    134 13           LDA   #$0D
$2604:    151    151 211          STA   $D3
$2606:    204    204 56 00        LDD   $3800
$2609:    221    221 209          STU   $D1
$260B:    12     12 211           CLR   $D3
$260D:    142    142 00 234       LDS   #$00EA
$2610:    204    204 02 00        LDD   $0200
$2613:    237    237 132          STD   $84
$2615:    134    134 33           LDA   #$21
$2617:    214    214 211          ORA   $D3
$2619:    237    237 02           STD   $02
$261B:    220    220 209          LDB   $D1
$261D:    237    237 04           STD   $04
$261F:    173    173 159 192      JMP   $C0
$2623:    4      109 6            ???   (likely part of multi-byte op)
$2625:    38     8                BRA   $262F
$2626:    76     129              JMP   $2181
$2628:    61     37               ???   (likely part of multi-byte op)
$262A:    221    126              STU   $7E
$262C:    57     0                ???   (likely part of multi-byte op)
$262E:    126    215              LDU   $D7
$2630:    9                      ???   (likely part of multi-byte op)