rem /**
rem  * \file build.bat
rem  * \brief compilar as fontes usando o CCS
rem  */
del *.err
del *.hex

set CCS="C:\Program Files\PICC\CCSC.exe"

rem 	PIC16 +FM
rem 	PIC18 +FH
rem 	+P pausa
set D1= +FH +PE +A

set LIB="C:\Program Files\PICC\Devices;C:\Program Files\PICC\Drivers"

%CCS% %D1% I=%LIB% +EXPORT _delay.c
%CCS% %D1% I=%LIB% +EXPORT _matriz.c
%CCS% %D1% I=%LIB% +EXPORT _lcd.c
%CCS% %D1% I=%LIB% +EXPORT _teclado.c
%CCS% %D1% I=%LIB% +EXPORT _funcoes_gerais.c
%CCS% %D1% I=%LIB% +EXPORT _analogico.c

%CCS% %D1% I=%LIB% +EXPORT main.c

%CCS% %D1% LINK="cp.hex=_matriz.o,_analogico.o,_delay.o,_funcoes_gerais.o,_teclado.o,_lcd.o,main.o"

del c:\c.log

del *.lst
rem del *.STA
del *.TRE
del *.COF
del *.esym
del *.osym
del *.o
del *.cod
del *.sym
del *.pjt
