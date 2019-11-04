/**
 *  \file _PIP.c
 *  \brief Tratamento do sinal do PIP
 */

#include 	"_fuses.h"
#include 	"_pinagem_ecu.h"
#include 	"_MemoryMap.h"
#include 	"_ANA.h"
#include 	"_matriz.h"
#include	"_BaseTempo.h"

/**
 * \fn Trata_int_RB0
 * \brief 	Funcao de tratamento da interrupcao externa em RB0,
 * 			\n sensivel a borda de descida ou subida do sinal do sensor Hall/PIP.
 */
#int_ext
void Trata_int_RB0(void){
	unsigned long int CALCULO;

	if (INTEDG0){
		/* 1  = rising edge */

		/* Medir Tempo do PIP */
		CALCULO = MemoryMap.RPM_RAW;
		CALCULO <<= 2;
		MemoryMap.PIP_TL = CALCULO;
		MemoryMap.PIP_TH = MemoryMap.PIP_T_TOT - MemoryMap.PIP_TL;

		/* As leituras analógicas estao sincronizadas com o movimento do pistão */
		if(!MemoryMap.ANA_UpdateCicle){
			MemoryMap.ANA_UpdateCicle = 20;
			AD_Next_Port_Choice();
		}

	} else {

		/* Avanco de Ignicao */
		CALCULO = MemoryMap.PIP_TL;
		if (CALCULO > MemoryMap.IGN_AVANCO)
			CALCULO -= MemoryMap.IGN_AVANCO;
		if(!CALCULO) CALCULO++;
		CALCULO *= 150;
		CALCULO  = 0x00 - CALCULO;
		set_timer1(CALCULO);
		enable_interrupts(int_timer1);

		/* A injecao comeca com a descida do PIP */
		if(MemoryMap.PIP_SimNaoComandar){
			if(MemoryMap.PIP_SimNao){
				Injetor_out = 1;
				LED_Injetor = 0;
			}
			MemoryMap.PIP_SimNao = !MemoryMap.PIP_SimNao;
		} else{
			Injetor_out = 1;
			LED_Injetor = 0;
		}

		/* Sinal para o eletroinjetor */
		setup_timer_2(T2_DIV_BY_16, MemoryMap.INJ_T2_set, 15);
		enable_interrupts(int_timer2);

		/* 0 = falling edge
		 * Limitado valor de 1 a 60 => 100 a 6000 RPMs
		 */
		CALCULO = MemoryMap.RPM_RAW;
		MemoryMap.RPM_RAW = 0;
		CALCULO <<= 2;
		if(!CALCULO){
			//Não está girando, apenas a K15 está acionada
			MemoryMap.RPM = 0;
			MemoryMap.RPM_pointer=15;
			MemoryMap.PIP_T_TOT = 0;
		} else {
			if(CALCULO > 3000) 	CALCULO = 3000;
			if(CALCULO < 50) 	CALCULO = 50;

			/* Medir Tempo do PIP */
			MemoryMap.PIP_T_TOT = CALCULO;
			/* RPM */
			CALCULO = 3000/CALCULO;
			MemoryMap.RPM_pointer = CALCULO - 1;
			MemoryMap.RPM = CALCULO * 100;
		}
	}

	/* Troca int0 ext borda descida/subida */
	INTEDG0 = !INTEDG0;
}
