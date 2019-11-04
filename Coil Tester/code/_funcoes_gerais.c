/**
 * \file _funcoes_gerais.c
 * \brief comandos de manipulacao de textos e numeros
 */

#include 	"_fuses.h"

/**
 * \fn 			char Int_to_ASCII(unsigned int val)
 * \brief      	Converte um inteiro em ASCII.
 * \param [in]  val valor a ser convertido.
 * \return		valor convertido.
 */
char* Int_to_ASCII(unsigned int val) {
	char str[6];
    str[0] = (val / 10000) + 0x30;
    str[1] = ((val % 10000) / 1000) + 0x30;
    str[2] = ((val % 1000) / 100) + 0x30;
    str[3] = ((val % 100) / 10) + 0x30;
    str[4] = (val % 10) + 0x30;
    str[5] = 0;
	return str;
}

/**
 * \fn 			int ASCII_TO_INT(char val)
 * \brief 		só para 0~9
 * \param [in]  val valor a ser convertido.
 * \return		valor convertido.
 */
int ASCII_TO_INT(char val) {
    return (val-48);
}

/**
 * \fn 				LongFix_to_ASCII(unsigned long int val, int pos, char *str)
 * \brief      		Converte um inteiro com ponto decimal em ASCII. 999 > 99.9 ou 9.99 ou .999
 * \param [in]  	val valor a ser convertido.
 * \param [in]  	pos ponto decimal.
 * \param [in,out]  str transfere dado
 */

void LongFix_to_ASCII(unsigned long int val, int pos, char *str ) {
	int i,p;

    p = 3 - pos;
    i=0;

    if (p==i){
        str[i] = '.';
        i++;
        }
    str[i] = ((val % 1000) / 100) + 0x30;
    i++;

    if (p==i){
        str[i] = '.';
        i++;
        }
    str[i] = ((val % 100) / 10) + 0x30;
    i++;

    if (p==i){
        str[i] = '.';
        i++;
        }
    str[i] = (val % 10) + 0x30;
    i++;

    str[i] = 0;
}
