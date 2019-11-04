/**
 *  \file _analogico.c
 *  \brief Funcoes de leitura dos sinais analogicos
 */

#include	"_fuses.h"
#include	"_analogico.h"

unsigned char AD_Valor; /**< Último valor analógico lido */

/**
 *  \fn AD_LeituraFinalizada
 *  \brief Fim da conversao analogica
 */
#INT_AD
void AD_LeituraFinalizada(void) {
	AD_Valor = read_adc(ADC_READ_ONLY);
}

/**
 *  \brief Medida bruta do AD
 *  \return Último valor analógico lido
 */
unsigned char AD_Leitura(void){
	return AD_Valor;
}	

/**
 *  \fn AD_TRIS
 *  \brief Direcao da porta analogica como entrada
 */
void AD_TRIS(void) {

    TRISA = TRISA | 0b00000001; 
}

/**
 *  \fn AD_Start(char canal)
 *  \brief Inicio da conversao analogica
 *  \param[in] canal informar qual porta analogica sera iniciada
 */
void AD_Start(char canal){
	set_adc_channel(canal);
	read_adc(ADC_START_ONLY);
}
