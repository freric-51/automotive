/**
 * \file	_fuses.h
 * \brief	Configura o PIC conforme o modelo
 */

/* Bibliotecas */
/* Microcontrolador utilizado */
#include <18F4550.h>
/* Registros 18F */
#include "regs_18F4550.h"

/*
 * Definições
 * BANCOS DE RAM
 * COMUTAR ENTRE OS BANCOS DE MEMORIA.
 */
#DEFINE	BANK1	BSF	STATUS,RP0	/* SELECIONA BANK1 DA MEMORIA RAM */
#DEFINE	BANK0	BCF	STATUS,RP0	/* SELECIONA BANK0 DA MEMORIA RAM */

/*
 * FUSES
 * Configuracoes de gravacao, cristal de 20MHz para operar a 48MHz \n
 * http://www.ccsinfo.com/forum/viewtopic.php?t=47528 \n
 * http://www.ccsinfo.com/forum/viewtopic.php?t=37929&start=3 \n
 * HSPLL High-Speed Crystal/Resonator with PLL enabled	\n
 * prescaler 1,2,3,4,5,6,10,12	\n
 * postscaler 2,3,4,6	\n
 * Input Oscillator Frequency = 20 MHz
 * PLL Division = ÷5 (100)
 * Clock Mode = HSPLL, ECPLL, ECPIO
 * MCU Clock Division = ÷2 (00)
 * Microcontroller Clock Frequency = 48 MHz
 */
#fuses HSPLL, NOWDT, NOPROTECT, NOLVP, NODEBUG, USBDIV, PLL5, CPUDIV1, VREGEN
#fuses PUT, BROWNOUT
/* Conversor AD em 10 ou 8 bits */
#device  ADC = 8
#device CONST = ROM
#ZERO_RAM
#use delay(clock=48000000)
/* Estabelece a prioridade das interrupções */
#priority ext,timer0,timer1,timer2,ad,ext1,ext2,eeprom

/*
 * inicialização dos PORTS
 */
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)
