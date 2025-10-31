MiniBanners is a printer utility that prints banners. It debuted at the first Atlanta CoCoFest in 1990. It is significant because, unlike other banner progarms of the day, it could work on ANY printer, not just dot matrix graphics printers. It would make use of solid block characters, if available, or could print the banner using an ASCII character like "X" or "#". As far as I know, it was the only commercial banner printing program that would work on printers like the TP-10 (32 columns wide) or daisy wheel printers.

It was written in BASIC, but we did a sneaky trick to make it appear to be an assembly language program. First, the program was "scrambled" and placed as data in a small assemblty language program that would unscramble it and place it in BASIC memory then "RUN" it. The result was a file that you would "LOADM" and then EXEC. The program would disable the BREAK key as well.

Another trick was done with how the screens were displayed. The 40/80 column screen on the CoCo 3 was slow and you could really tell it was BASIC by how the lines "printed" to the screen (as well as the visible cursor). To get around that, MiniBanners would set the screen colors to black on black, print the screen, then change the palette to make the screen appear instantly. This gave the illusion of it being an assembly program.

The assembly code I used for this - SCRAM.ASM - is also included. I have memory of how it all worked, but there is a .BAS file that seems to give some instructions on how to scramble and save.

There was a secret backdoor in the program, which Nick Johnson figured out immediately. This would break out of the program into BASIC. As far as I know, he is the only one that ever discovered this. Nick created his own N*BANNER program which used much higher resolution fonts for printing. I hope to track him down sometime so we can get his programs included in the archive (at least the ones that we distributed for him).

More to come...
