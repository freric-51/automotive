/**
 * \file	_teclado.c
 * \brief	Comandos de leitura de teclado 16 teclas
 */

 /**
 *  \name    nFig1
 * @{
 * \section Fig1 Teclado
 * \ref Figuras "Lista de Figuras"
 * \addindex Fig1
 * \image    latex keypad_diagram.eps "Teclado" height=9cm
 * @}
 */
 
// Bibliotecas
#include	"_fuses.h"
#include	"_teclado.h"

// Definicoes
#define KB_IN PORTC
#define KB_OUT LATD

#define KB_Coluna_Nao 0x0F
#define KB_Coluna_0 0b00001110
#define KB_Coluna_1 0b00001101
#define KB_Coluna_2 0b00001011
#define KB_Coluna_3 0b00000111

#define KB_linha_Nao	0x0F
#define KB_linha_0	0b00001110
#define KB_linha_1	0b00001101
#define KB_linha_2	0b00001011
#define KB_linha_3	0b00000111

// Variaveis
unsigned char KB_UltimaLida = '\0'; /**< Última tecla em ASCII, tipo One Shot */
short KB_Dado_Novo = FALSE; /**< Sinal tipo One Shot */

/**
 * \fn	KB_tris(void)
 * \brief	Configura Portas para entrada ou saida
 */
void KB_tris(void) {
    // Hex 0x1F
    // Binary 0b010010

    TRISC = TRISC | 0b00011110; // forca entradas / RC0 é usada pelo TIMER_1
    TRISD = TRISD & 0b11110000; // forca saidas
}

/**
 *  \brief Sinal inicial para varrer as teclas
 */
void KB_Reset(void) {
    KB_OUT = KB_OUT | KB_Coluna_Nao;
}

/**
 *  \brief executa a varredura do teclado matricial
 */
void KB_trata_teclado(void) {
    unsigned char coluna_mascarado;
    unsigned char linha_mascarado;
    unsigned char tecla_apertada = KB_Tecla_Nenhuma;
	static char KB_PassoDedo = 0;
	static char bounce; // leitura de 4 colunas vazias

    // 1o sempre acertar o momento da proxima varredura
    KB_TempoVarredura = 50;

    // remocao de sinais inconvenientes atraves de mascara e rotacao
    coluna_mascarado = KB_OUT & 0x0F;
	linha_mascarado = KB_IN;
	linha_mascarado = linha_mascarado >> 1;
    linha_mascarado = linha_mascarado & 0x0F;

	switch (linha_mascarado) {
        case KB_linha_0:
            if (KB_Coluna_0 == coluna_mascarado) tecla_apertada = KB_Tecla_A;
            if (KB_Coluna_1 == coluna_mascarado) tecla_apertada = KB_Tecla_B;
            if (KB_Coluna_2 == coluna_mascarado) tecla_apertada = KB_Tecla_C;
            if (KB_Coluna_3 == coluna_mascarado) tecla_apertada = KB_Tecla_D;
            break;
        case KB_linha_1:
            if (KB_Coluna_0 == coluna_mascarado) tecla_apertada = KB_Tecla_3;
            if (KB_Coluna_1 == coluna_mascarado) tecla_apertada = KB_Tecla_6;
            if (KB_Coluna_2 == coluna_mascarado) tecla_apertada = KB_Tecla_9;
            if (KB_Coluna_3 == coluna_mascarado) tecla_apertada = KB_Tecla_Velha;
            break;
        case KB_linha_2:
            if (KB_Coluna_0 == coluna_mascarado) tecla_apertada = KB_Tecla_2;
            if (KB_Coluna_1 == coluna_mascarado) tecla_apertada = KB_Tecla_5;
            if (KB_Coluna_2 == coluna_mascarado) tecla_apertada = KB_Tecla_8;
            if (KB_Coluna_3 == coluna_mascarado) tecla_apertada = KB_Tecla_0;
            break;
        case KB_linha_3:
            if (KB_Coluna_0 == coluna_mascarado) tecla_apertada = KB_Tecla_1;
            if (KB_Coluna_1 == coluna_mascarado) tecla_apertada = KB_Tecla_4;
            if (KB_Coluna_2 == coluna_mascarado) tecla_apertada = KB_Tecla_7;
            if (KB_Coluna_3 == coluna_mascarado) tecla_apertada = KB_Tecla_Estrela;
            break;
        case KB_linha_Nao:
            tecla_apertada = KB_Tecla_Nenhuma;
            break;
    }

	if (tecla_apertada == KB_Tecla_Nenhuma) {
		if (bounce<255) bounce++;
	} else {
		bounce = 0;
	}

    // Houve tecla valida?
	switch (KB_PassoDedo){
	case 0:
		if (tecla_apertada != KB_Tecla_Nenhuma){
			KB_UltimaLida = tecla_apertada;
			KB_Dado_Novo = TRUE;
			KB_PassoDedo = 1;
		}
		break;
	case 1:
		if (bounce > 5){
			KB_PassoDedo = 0;
		}
		break;
	}

    //proxima coluna
    switch (coluna_mascarado) {
        case KB_Coluna_0:
            KB_OUT = (KB_OUT | 0x0F) & (0xF0 | KB_Coluna_1);
            break;
        case KB_Coluna_1:
            KB_OUT = (KB_OUT | 0x0F) & (0xF0 | KB_Coluna_2);
            break;
        case KB_Coluna_2:
            KB_OUT = (KB_OUT | 0x0F) & (0xF0 | KB_Coluna_3);
            break;
        case KB_Coluna_3:
            KB_OUT = (KB_OUT | 0x0F) & (0xF0 | KB_Coluna_0);
            break;
        case KB_Coluna_Nao:
            KB_OUT = (KB_OUT | 0x0F) & (0xF0 | KB_Coluna_0);
            break;
    }
}

/**
 *  \brief retorna a última tecla acionada uma vez
 *  \return Código ASCII
 */
char KB_Tecla_Nova(void) {
    if (KB_Dado_Novo) {
        KB_Dado_Novo = FALSE;
        return KB_UltimaLida;
    } else {
        return KB_Tecla_Nenhuma;
    }
}
