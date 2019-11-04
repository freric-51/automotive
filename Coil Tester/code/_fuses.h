/**
 * \file	_fuses.h
 * \brief	programa o PIC conforme o modelo
 */

// Bibliotecas
#include <18F4520.h>
#include "regs_P18F4520.h"

// FUSES
#FUSES NOIESO,HS,NOFCMEN
#FUSES NOBROWNOUT,NOWRT
#FUSES NOWDT,NOPROTECT
#FUSES PBADEN,NOLPT1OSC,MCLR
#FUSES NODEBUG,NOSTVREN,NOLVP,NOXINST
#FUSES NOWRTC,NOWRTB,NOWRTD,NOPBADEN
#FUSES NOCPD,NOCPB
#FUSES NOPUT,NOEBTR,NOEBTRB

#DEVICE ADC=8 // conversor AD em 8 bits
#use delay (clock = 16000000)
#ZERO_RAM

// inicializacao dos port's
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)

#priority timer1,timer0,ad  // Estabelece a prioridade das interrupcoes 
