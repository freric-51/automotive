/**
 * \file _pinagem_ecu.c
 * \brief Arquivo de Configuracao
 */

#include "_fuses.h"
#include "_MemoryMap.h"
#include "_pinagem_ecu.h"

/**
 *  \fn init_Pinagens
 *  \brief Definicao das portas analógicas
 */
void init_Pinagens(void){
	/* Definicao das portas analógicas */
	MemoryMap.TPS_Port = 0 ;
	MemoryMap.ACT_Port = 1 ;
	MemoryMap.MAP_Port = 2 ;
	MemoryMap.ECT_Port = 3 ;
}
