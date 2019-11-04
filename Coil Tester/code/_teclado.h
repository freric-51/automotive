/**
 * \file	_teclado.h
 * \brief	protótipos para _teclado.c
 */

unsigned long int KB_TempoVarredura = 500; /**< VALOR GRANDE apenas para que demore a primeira varredura do teclado */

// prototipos
void KB_tris(void);
void KB_Reset(void);
void KB_trata_teclado(void);
char KB_Tecla_Nova(void);

/**
 *  @{
 *  Códigos ASCII
 */
#define KB_Tecla_Nenhuma 0
#define KB_Tecla_0 48
#define KB_Tecla_1 49
#define KB_Tecla_2 50
#define KB_Tecla_3 51
#define KB_Tecla_4 52
#define KB_Tecla_5 53
#define KB_Tecla_6 54
#define KB_Tecla_7 55
#define KB_Tecla_8 56
#define KB_Tecla_9 57

#define KB_Tecla_A 65
#define KB_Tecla_B 66
#define KB_Tecla_C 67
#define KB_Tecla_D 68

#define KB_Tecla_Estrela 42
#define KB_Tecla_Velha 35
/** @} */
