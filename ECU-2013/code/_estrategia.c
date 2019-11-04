/**
 *  \file _estrategia.c
 *  \brief cálculos para controle do tempo de injeção e avanço de ignição
 */

#include 	"_fuses.h"
#include 	"_estrategia.h"
#include 	"_MemoryMap.h"
#include 	"_matriz.h"

 /**
 *  \brief Define qual o regime sendo processado
 */
 void Calcular_Estrategia(void){
	MOp = 0;
	// escolha da estratégia 
	// Partida 
	if (MemoryMap.RPM < 700){
		if (MemoryMap.ECT < 50){
			MOp_PartidaFrio = 1;
			MemoryMap.Estrategia_ID=0;
		}else{
			MOp_PartidaQuente = 1;
			MemoryMap.Estrategia_ID=1;
		}
		return;
	}
	// Aquecendo / marcha lenta 
	if (MemoryMap.RPM < 1300){
		if (MemoryMap.ECT < 50){
			MOp_Aquecimento = 1;
			MemoryMap.Estrategia_ID=2;
		} else {
			MOp_MarchaLenta = 1;
			MemoryMap.Estrategia_ID=3;
		}
		return;
	}

	// Rodando 
	if (MemoryMap.TPS < 50){
		if (!MemoryMap.TPS){
			MOp_Desaceleracao = 1;
			MemoryMap.Estrategia_ID=7;
		} else {
			MOp_CargaParcial = 1;
			MemoryMap.Estrategia_ID=4;
		}	
	}else{
		if (!MemoryMap.TPS_Aceleracao){
			MOp_AceleracaoRapida = 1;
			MemoryMap.Estrategia_ID=6;
		} else {
		MOp_PlenaCarga = 1;
		MemoryMap.Estrategia_ID=5;
		}
	}
}

/**
 *  \fn Calcular_Avanco_IGN
 *  \brief Calcula o tempo (Timer 1) para o avanço de ignição
 *
 * \anchor Figura_10_2
 * \ref Figuras "Lista de Figuras"
 * \addindex Figura_10_2
 * \image 	latex ControleIgnicao.eps "Fluxograma geral usado como base para comando do sinal de ignição" width=14cm
 */
void Calcular_Avanco_IGN(void) {
	unsigned long int CALCULO;
	// Avanco de Ignicao
	CALCULO = MemoryMap.RPM_pointer;
	CALCULO >>= 2;
	CALCULO &= 0x000F;
	MemoryMap.IGN_AVANCO = M_IgnAvanco_RPM16_TPS[CALCULO][MemoryMap.TPS_pointer];
	// Timer 1
	CALCULO  = MemoryMap.IGN_AVANCO;
	if(!CALCULO) CALCULO++;
	CALCULO *= 150;
	CALCULO  = 0x00 - CALCULO;
	MemoryMap.IGN_T1_set = CALCULO;
}

/**
 *  \brief Calcula o tempo (Timer 2) no eletro injetor onde + tempo se Vbat cair e + tempo se carga (TPS) aumentar \n
 *  Considerado 1=20us 100=2ms 255[max]=5.1ms
 */
void Calcular_Injecao(void){
	unsigned long int CALCULO;
	CALCULO = Padrao_Inj_OffSet;
	/** \todo Acerto do tempo de injeção conforme tensão da bateria */
	// CALCULO += M_INJ_BAT
	/** \todo Acerto do tempo de injeção conforme carga no motor */
	// CALCULO += M_INJ_TPS

	/** \todo Acerto do tempo de injeção conforme estratégia / regime no motor, onde a soma dos tempos não pode passar de 255*/
	if (MOp_PartidaFrio){
		// Injetor = f(Vbat)
		// Injetor = f(ECT)
		CALCULO += 150;
		MemoryMap.PIP_SimNaoComandar=0;
	}
	if (MOp_PartidaQuente){
		CALCULO += 0;
		MemoryMap.PIP_SimNaoComandar=0;
	}
	if (MOp_Aquecimento){
		CALCULO += 0;
		MemoryMap.PIP_SimNaoComandar=0;
	}
	if (MOp_MarchaLenta){
		CALCULO += 0;
		MemoryMap.PIP_SimNaoComandar=0;
	}
	if (MOp_CargaParcial){
		CALCULO += 0;
		MemoryMap.PIP_SimNaoComandar=1;
	}
	if (MOp_PlenaCarga){
		CALCULO += 0;
		MemoryMap.PIP_SimNaoComandar=1;
	}
	if (MOp_AceleracaoRapida){
		CALCULO += 0;
		MemoryMap.PIP_SimNaoComandar=1;
	}
	if (MOp_Desaceleracao){
		CALCULO += 0;
		MemoryMap.PIP_SimNaoComandar=1;
	}

	// Bomba de aceleração	
	CALCULO += MemoryMap.TPS_Aceleracao;
	// Timer 2
	MemoryMap.INJ_T2_set = CALCULO;
	// Animação no display
	CALCULO <<= 1;
	CALCULO /= 10;
	MemoryMap.INJ = CALCULO;
}

/**
 *  \brief Derivada do sinal do TPS como referência a um mapa
 */
void Calcular_Aceleracao(void){
	char tps_p;
	// 50 * 10ms = 0.5s
	// TPS 0 [0] a 100 [15]
	if (MemoryMap.TPS_pointer > MemoryMap.TPS_Anterior)
		tps_p = MemoryMap.TPS_pointer - MemoryMap.TPS_Anterior;
	else
		tps_p = 0;
	
	MemoryMap.TPS_Aceleracao = M_TPS_ACELERACAO_TPS_ECT[tps_p][MemoryMap.ECT_pointer];
	MemoryMap.TPS_Anterior = MemoryMap.TPS_pointer;
}
