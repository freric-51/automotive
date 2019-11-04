/**
 *  \file _BotaoDisplay.c
 *  \brief Botão usado para trocar tela no display LCD
 */

#include 	"_fuses.h"
#include 	"_pinagem_ecu.h"
#include 	"_MemoryMap.h"
#include	"_BotaoDisplay.h"
/**
 * \fn Trata_int_RB1
 * \brief 	Funcao de tratamento da interrupcao externa em RB1,
 * 			\n debouce por hardware com capacitor em paralelo ao botão.
 */
#int_ext1
void Trata_int_RB1(void){
	MemoryMap.LCD_Pagina++;
}
