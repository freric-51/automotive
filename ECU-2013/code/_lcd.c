/**
 * \file   _lcd.c
 * \author Ricardo de Freitas 1023007
 * \brief comandos especificos para o LCD
 * \date \ref s7
 *
 *  \name    nFig4
 * @{
 * \section Figura_10_4 JHD162A
 * \ref Figuras "Lista de Figuras"
 * \addindex Figura_10_4
 * \image    latex lcd_timing.eps "JHD162A - pulso de execução de um comando: RS = 0 - pulso de escrita de letra: RS = 1" width=14cm
 * @} 
 */

#include 	"_fuses.h"
#include 	"_pinagem_ecu.h"
#include 	"_lcd.h"
#include	<string.h>

// comandos para o LCD
#define __dado 1	/**<  \brief Enviar dado */
#define __instrucao 0	/**<  \brief Enviar comando */

// ##### prototipos #####
//void LCD_tris(void);
void LCD_escreve_word(unsigned char dado, unsigned char tipo);
void LCD_escreve_4bits(unsigned char dado, char pos);
void LCD_pulso_comando(unsigned char tipo);
void LCD_posicao_inicio(void);
// ##### fim prototipos #####

/**
 * \fn	LCD_tris(void)
 * \brief Configura Portas para entrada ou saida
 */
void LCD_tris(void) {
	TRISD = TRISD & 0b00000000; // forca saidas
	TRISE = TRISE & 0b11111100; // forca saidas
}

/**
 *  \brief Escreve uma letra ou comando no display
 *
 *  \param [in] dado Caractere a ser enviado ao display
 *  \param [in] tipo Texto ou Comando ?
 */
void LCD_escreve_word(unsigned char dado, unsigned char tipo) {
    char i;
	output_d(dado);
	LCD_pulso_comando(tipo);
}

/**
 *  \brief Aciona as linha RS e EN conforme o tipo do dado
 *  \param [in] tipo Dado ou instrução para o LCD?
 */
void LCD_pulso_comando(unsigned char tipo) {
    //1 - Dado, 0 - Instrução
    if (tipo == __dado)
        output_high(LCD_RS);

    if (tipo == __instrucao)
        output_low(LCD_RS);
	
    //Pulso em EN RE1
    output_low(LCD_EN);
    delay_us(10);
    output_high(LCD_EN);
    delay_us(30);
    output_low(LCD_EN);
    delay_us(10);
}

/**
 *  \brief Movimenta o cursor para a posição inicial do LCD
 */
void LCD_posicao_inicio(void) {
    unsigned char info;
    info = 0x01; //limpar o display e posicionar o cursor linha 1 coluna 1
    LCD_escreve_word(info, __instrucao);
    delay_us(1800);
}

/**
 *  \brief Inicialização do display LCD
 */
void LCD_inicializacao(void) {

    unsigned char info;

    delay_ms(500); //necessário esperar antes de qq coisa

    LCD_tris();

	delay_ms(15);
	output_d(0x30);
	LCD_pulso_comando(__instrucao);
	delay_ms(4);
	LCD_pulso_comando(__instrucao);
	delay_us(100);
	LCD_pulso_comando(__instrucao);
	delay_us(40);
	output_d(0x38);
	LCD_pulso_comando(__instrucao);
	delay_us(40);
	output_d(0x01);
	LCD_pulso_comando(__instrucao);
	delay_us(1800);
	output_d(0x0C);
	LCD_pulso_comando(__instrucao);
	delay_us(40);
	output_d(0x06);
	delay_us(40);
}

/**
 *  \brief Escreve uma frase completa no display
 *  \param [in] posicao Posição a ser escrita no display
 *  \param [in] frase Texto
 */
void LCD_mensagem(unsigned char posicao, unsigned char *frase) {
    char i;

    LCD_escreve_word(posicao, __instrucao); //comecar da linhaXcoluna
    delay_us(40);

    for (i = 0; i < strlen(frase); i++) {
        if (0 == frase[i]) break;
        LCD_escreve_word(frase[i], __dado);
    }
}
