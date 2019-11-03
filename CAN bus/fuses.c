/*
 * File:   fuses.c
 * Authors:
 * Ricardo de Freitas 1023007
 * configura PIC conforme o modelo
 */

#if defined(__PCB__)
#include <16c56.h>
#fuses HS,NOWDT,NOPROTECT
#use delay(clock=20000000)

#elif defined(__PCM__)
#include <16F877A.h>
#include <.\regs_16.h>
#FUSES NOWDT
#FUSES HS
#FUSES NOPUT
#FUSES NOPROTECT
#FUSES NODEBUG
#FUSES NOBROWNOUT
#FUSES NOLVP
#FUSES NOCPD
#FUSES NOWRT
#use delay(clock=20000000)

#elif defined(__PCH__)
#include <18F4520.h>
/*
        #FUSES LP,XT,HS,RC,EC,EC_IO,H4,RC_IO,PROTECT,NOPROTECT
        #FUSES BROWNOUT_NOSL,BROWNOUT_SW,NOBROWNOUT,BROWNOUT,WDT1,WDT2,WDT4
        #FUSES WDT8,WDT16,WDT32,WDT64,WDT128,WDT,NOWDT,BORV25,BORV27,BORV42
        #FUSES BORV45,PUT,NOPUT,CPD,NOCPD,NOSTVREN,STVREN,NODEBUG,DEBUG
        #FUSES NOLVP,LVP,WRT,NOWRT,WRTD,NOWRTD,IESO,NOIESO,FCMEN,NOFCMEN
        #FUSES PBADEN,NOPBADEN,CCP2B3,CCP2C1,WRTC,NOWRTC,WRTB,NOWRTB,EBTR
        #FUSES NOEBTR,EBTRB,NOEBTRB,CPB,NOCPB,LPT1OSC,NOLPT1OSC,MCLR,NOMCLR
        #FUSES XINST,NOXINST,INTRC,INTRC_IO,WDT256,WDT512,WDT1024,WDT2048
        #FUSES WDT4096,WDT8192,WDT16384,WDT32768
 */
#FUSES NOIESO,HS,NOFCMEN
#FUSES NOBROWNOUT,NOWRT
#FUSES NOWDT,NOPROTECT
#FUSES PBADEN,NOLPT1OSC,MCLR
#FUSES NODEBUG,NOSTVREN,NOLVP,NOXINST
#FUSES NOWRTC,NOWRTB,NOWRTD,NOPBADEN
#FUSES NOCPD,NOCPB
#FUSES NOPUT,NOEBTR,NOEBTRB
#use delay(clock=20000000)
#endif

#zero_RAM

#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)

#include <stdio.h>
#include <stdlib.h>

/*
        Just add the main C source file to the MPLAB project.
        Don't add any other source files to the project.
        Then use #include statements in the main C source file
        to include all other .c and .h files.
        This method works.
 */