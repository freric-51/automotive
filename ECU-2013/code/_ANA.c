/**
 *  \file _ANA.c
 *  \brief varredura e conversão das entradas analógicas
 */

#include "_fuses.h"
#include "_MemoryMap.h"
#include "_matriz.h"

/*
 *  Variables
 */
int1  			AD_END_Sample = 1; 	/**< \brief Fim da conversao analógica, comeca indicando que finalizou! */
unsigned char 	AD_Valor;			/**< \brief Valor da conversão da ultima porta selecionada */

/**
 *  \fn AD_LeituraFinalizada
 *  \brief Fim da conversao analogica
 */
#INT_AD
void AD_LeituraFinalizada(void) {
	AD_Valor = read_adc(ADC_READ_ONLY);
	AD_END_Sample = 1;
}

/**
 *  \fn AD_Start(char canal)
 *  \brief Inicio da conversao analogica
 *  \param[in] canal informar qual porta analogica sera iniciada
 *  
 */
void AD_Start(char canal){
	set_adc_channel(canal);	
	/*
	* Foi considerado o tempo de aquisição
	* TACQ = TAMP + TC + TCOFF
	* TACQ = 2.4us
	*/
	delay_us(3);
	read_adc(ADC_START_ONLY);
}

/**
 *  \name 	nFig1
 * @{
 * \section Figura_10_1 Conversor FxV para o sensor MAP
 * \ref Figuras "Lista de Figuras"
 * \n	R6 ajusta a excursao do sinal e R11 define o ganho
 * \addindex Figura_10_1
 * \image 	latex MAP_FxV.eps "Conversor frequência tensao para o sinal MAP" width=14cm
 * @}
 */
 
 /**
 * \fn 		AD_Next_Port_Choice
 * \brief 		Seleciona proxima entrada analogica
 *
 * \details 	Esta rotina é executada toda vez que ocorre o PIP
 *  			assim, as leituras ficam sincronizadas com o movimento do motor
 *  			para obter um valor de leitura mais estavel sem efeito pulsativo.	\n
 */
 
void AD_Next_Port_Choice (void){
	static char AD_RelativePort;

	if (!AD_END_Sample)
		return;

	AD_END_Sample = 0;

	switch (AD_RelativePort){
	case 0:
		//MemoryMap.XXX_RAW = AD_Valor;
		AD_RelativePort++;
		AD_Start (MemoryMap.TPS_Port);	// Start : proxima porta
		break;
	case 1:
		MemoryMap.TPS_RAW = AD_Valor; // RAW = Valor : transfere leitura para a ultima porta direcionada
		MemoryMap.TPS_pointer = (MemoryMap.TPS_RAW >> 4) & 0x0F;
		MemoryMap.TPS = M_TPS_ANA[MemoryMap.TPS_pointer];
		AD_RelativePort++;
		AD_Start (MemoryMap.ACT_Port); // Start : proxima porta
		break;
	case 2:
		MemoryMap.ACT_RAW = AD_Valor;
		MemoryMap.ACT_pointer = (MemoryMap.ACT_RAW >> 4) & 0x0F;
		MemoryMap.ACT = MemoryMap.ACT_RAW; // M_ACT[MemoryMap.ACT_pointer];
		AD_RelativePort++;
		AD_Start (MemoryMap.MAP_Port);
		break;
	case 3:
		/*
		 *  160 Hz = x.xxV = 0.00 bar \n
		 *  135 Hz = 3.70V = 0.79 bar \n
		 *  122 Hz = 3.52V = 0.54 bar \n
		 *  77 Hz  = 0.75V = 1.00 bar \n
		 *  bar = 1.1 - AD * 0.46/143
		 */
		MemoryMap.MAP_RAW = AD_Valor;
		MemoryMap.MAP_pointer = (MemoryMap.MAP_RAW >> 4) & 0x0F;
		MemoryMap.MAP = M_MAP_ANA[MemoryMap.MAP_pointer];
		AD_RelativePort++;
		AD_Start (MemoryMap.ECT_Port);
		break;
	case 4:
		MemoryMap.ECT_RAW = AD_Valor;
		MemoryMap.ECT_pointer = (MemoryMap.ECT_RAW >> 4) & 0x0F;	
		MemoryMap.ECT = M_ECT_ANA[MemoryMap.ECT_pointer];
		AD_RelativePort = 0;
		// AD_RelativePort++;
		// AD_Start (next port);
	default:
		AD_RelativePort = 0;
		AD_END_Sample = 1;
		break;
	}
}
