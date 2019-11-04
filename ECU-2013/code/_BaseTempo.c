/**
 *  \file _BaseTempo.c
 *  \brief Trata a interrupção dos timers
 */

#include 	"_fuses.h"
#include 	"_BaseTempo.h"
#include 	"_MemoryMap.h"
#include 	"_pinagem_ecu.h"

/**
 * \fn  Trata_Timer0(void)
 * \brief	Timer 0 = base de tempo fixo em 250us
 */
#int_timer0
void Trata_Timer0(void) {
	static char cnt10ms;
	// Tempo Fixo
    set_timer0(TMR_0_Prescaler + get_timer0());
	/*
	 *  timer 0 é usado para medir rotacao
	 *  Limitado valor de 1 a 60 => 100 a 6000 RPMs
	 *  [100RPM] 50 <= RPM_RAW <= 3000 [6000RPM]
	 *  3001 p/ 100us
	 *  751  p/ 400us
	 *  O PIP_TH e PIP_TL são processados em _PIP.C
	 */
	if (MemoryMap.RPM_RAW < 751)	MemoryMap.RPM_RAW++;

	/* Base de 10ms */
	if ( cnt10ms) 	cnt10ms--;
	if (!cnt10ms){
		cnt10ms = 25;
		// LCD
		if(MemoryMap.LCD_SHOW) 	MemoryMap.LCD_SHOW--;
		// Sinais Analogicos
		if(MemoryMap.ANA_UpdateCicle) 	MemoryMap.ANA_UpdateCicle--;
		// Estrategias
		if(MemoryMap.Estrategias_CicloCalc) 	MemoryMap.Estrategias_CicloCalc--;
		// Aceleração
		if(MemoryMap.TPS_CicloCalc) 	MemoryMap.TPS_CicloCalc--;		
	}
}

/**
 * \fn  Trata_Timer1(void)
 * \brief	Timer 1 Tempo de avanco de ignicao SPOUT, base de tempo fixo em 250us
 */
#int_timer1
void Trata_Timer1(void) {
	static char	PIP_PASSO;
	unsigned long int CALCULO;

	switch(PIP_PASSO){
		case 0:
			/* Ligar a ignicao se a rotação > 200 RPM */
			//if ( MemoryMap.RPM_pointer > 1) {
				SPout = 0;
				LED_Ign = 0;
			//}
			
			if ( MemoryMap.RPM_pointer > 4){
				CALCULO = MemoryMap.PIP_TH;
			} else {
				CALCULO = 200;
			}
			if(!CALCULO) CALCULO++;
			
			CALCULO *= 150;
			CALCULO  = 0x00 - CALCULO;
			//CALCULO += get_timer1();
			set_timer1(CALCULO);
			PIP_PASSO = 1;
			break;
		case 1:
			/* desligar a ignicao e aguardar sincronismo do PIP */
			SPout = 1;
			LED_Ign = 1;
			set_timer1(1); /* MAX > 43ms */
			disable_interrupts(int_timer1);
			PIP_PASSO = 0;
			break;
		default:
			PIP_PASSO = 0;
			break;
		}
}

/**
 * \fn  Trata_Timer2(void)
 * \brief	Timer 2 Tempo de injecao
 */
#int_timer2
void Trata_Timer2(void) {
	/* Desliga o injetor */
	Injetor_out = 0;
	LED_Injetor = 1;
	/* só liga no proximo PIP/Hall */
	setup_timer_2(T2_DISABLED, Padrao_Inj_OffSet, 15);
}
