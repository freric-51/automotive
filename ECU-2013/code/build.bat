::
:: \file build.bat
:: \brief compilar as fontes usando o CCS
::

del *.err
del *.hex

set CCS="C:\Program Files\PICC\CCSC.exe"

::	Valid options:
::	+FB	Select PCB (12 bit)
::	+FM	Select PCM (14 bit)
::	+FH	Select PCH (PIC18XXX)
::	+Yx	Optimization level x (0-9)
::	+ES	Standard error file
::	-T	Do not generate a tree file
::	+T	Create call tree (.TRE)
::	-A	Do not create stats file (.STA)
::	+A	Create stats file (.STA)
::	-EW	Suppress warnings (use with +EA)
::	+EW	Show warning messages
::	-E	Only show first error
::	+EA	Show all error messages and all warnings
::	+EX	Error/warning message format uses GCC's "brief format" (compatible with GCC editor environments)
::	+DF	Enables the output of an OFF debug file.
::	+PE	Keep status window up only if there are errors

set D1= +FH +PE +A

set LIB="C:\Program Files\PICC\Devices;C:\Program Files\PICC\Drivers"

%CCS% %D1% I=%LIB% +EXPORT _PIP.c
%CCS% %D1% I=%LIB% +EXPORT _ANA.c
%CCS% %D1% I=%LIB% +EXPORT _BaseTempo.c
%CCS% %D1% I=%LIB% +EXPORT _BotaoDisplay.c
%CCS% %D1% I=%LIB% +EXPORT _lcd.c
%CCS% %D1% I=%LIB% +EXPORT _pinagem_ecu.c
%CCS% %D1% I=%LIB% +EXPORT _funcoes_gerais.c
%CCS% %D1% I=%LIB% +EXPORT _estrategia.c

%CCS% %D1% I=%LIB% +EXPORT main.c

%CCS% %D1% LINK="ger201301.hex=_PIP.o,_ANA.o,_BaseTempo.o,_BotaoDisplay.o,_estrategia.o,_funcoes_gerais.o,_pinagem_ecu.o,_lcd.o,main.o"

del c:\c.log
del *.lst
del *.TRE
del *.COF
del *.esym
del *.osym
del *.o
del *.cod
del *.sym
del *.pjt