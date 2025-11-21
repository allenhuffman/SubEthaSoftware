/* Terry’s RS-DOS MultiBoot code V1.12… 512 Bytes */
    LDX     3800
    LDY     2600
    LDD     81
    STD     A1
    CMPX    3900
    BCS     F7
    LBSR    0198
    LBSR    01AB

/* At this point it goes into ASCII text; and may even have gone into it earlier. */