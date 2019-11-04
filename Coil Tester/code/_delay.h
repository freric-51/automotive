/**
 * \file	_delay.h
 * \brief	protótipos para _delay.c
 *
 */

void Atraso_Milisegundos(unsigned int);
void delay(void);
void PULSO_tris(void);

/** Prescaler para  Timer 0 */
#define TMR_Prescaler_1ms 131

/** Sinal de comando para o chaveamento de corrente pelo FET */
#define PULSO_PIN LATE2

unsigned long int TH = 9999; /**< Tempo em Alto */
unsigned long int TL = 9999; /**< Tempo em Baixo */
