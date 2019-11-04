/**
 * \file	_delay.c
 * \brief	Controle de tempo, temporizacao, atraso
 */

// Bibliotecas
#include	"_fuses.h"
#include	"_teclado.h"
#include	"_analogico.h"
#include	"_delay.h"

 /**
 * \fn	PULSO_tris(void)
 * \brief define se porta é entrada ou saída
 */
 void PULSO_tris(void) {
	TRISE = TRISE & 0b11111011; // forca saidas
}

/**
 * \fn  Trata_Timer0(void)
 * \brief	Interrupcao do timer 0. A cada 1ms.
 */

#int_timer0

void Trata_Timer0(void) {
    set_timer0(TMR_Prescaler_1ms + get_timer0());
    if (KB_TempoVarredura) KB_TempoVarredura--;
}

/**
 * \fn  Trata_Timer1(void)
 * \brief	Interrupcao do timer 1 a cada 200ns.
 */

#int_timer1

void Trata_Timer1(void) {
	static int1 NivelAtual = 0;
	static long int TL_ciclo;
	long int calculo;
	if(NivelAtual){
		//fim do tempo em alto, imediatamente
		//prepara o proximo nivel: BAIXO
		setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
		TL_ciclo = TL; //transfere TL para TL_Ciclo TH em ms
		set_timer1(65036 + get_timer1()); //ajustar para 1ms o T1
		AD_Start(0); // Ler a corrente antes de zerar a saida
		PULSO_PIN = 0;
		NivelAtual=0;
	} else {
		//aguardar fim do TL para armar o sinal em alto
		if (TL_ciclo){
			TL_ciclo --;
			set_timer1(65036 + get_timer1()); // ajustar para 1ms o T1
		} else {
			//prepara o proximo nivel: ALTO
			setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
			// 65136 = 115us > 65196 = 100us(-15us)
			// 45536 = 5016ms
			//TH em us
			if (TH > 15)
				calculo = (TH - 15);
			calculo	*= 4;
			calculo = 0x00 - calculo;
			calculo += get_timer1();
			set_timer1(calculo);
			PULSO_PIN = 1;
			NivelAtual=1;
		}
	}
}

/**
 * \fn delay
 * \brief	Gera delay de 4 nops.
 */
void delay(void) {
#asm
    NOP
    NOP
    NOP
    NOP
#endasm
}

/**
 * \fn	Atraso_Milisegundos(unsigned int valor)
 * \brief	Gera atraso em milisegundos.
 * \param [in]	valor tempo desejado em ms
 */
void Atraso_Milisegundos(unsigned int valor) {
    unsigned int i;
    unsigned char j;

    for (i = 0; i < valor; i++) {

        for (j = 0; j < 200; j++) {
#asm
            NOP
            NOP
            NOP
            NOP
            NOP
#endasm;
        }
    }
}
