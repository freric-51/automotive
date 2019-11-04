/**
 * \file   _lcd.c
 * \author Ricardo de Freitas 1023007
 * \brief comandos especificos para o LCD
 *
 *  \name    nFig3
 * @{
 * \section Fig3 JHD162A
 * \ref Figuras "Lista de Figuras"
 * \addindex Fig3
 * \image    latex lcd_timing.eps "JHD162A - pulso de execucao de um comando: RS = 0 - pulso de escrita de letra: RS = 1" width=14cm
 * @} 
 */

#include	"_fuses.h"
#include	"_lcd.h"
#include	<string.h>

/**
 * @{
 * Bits do LCD
 */
#define lcd_PIN_4 pin_d4
#define lcd_PIN_5 pin_d5
#define lcd_PIN_6 pin_d6
#define lcd_PIN_7 pin_d7

#define lcd_rs pin_e0
#define lcd_en pin_e1
/** @} */

/**
 * @{
 * comandos para o LCD
 */
#define __dado 1
#define __instrucao 0
/** @} */

/**
 * \fn	LCD_tris(void)
 *  \brief Configura Portas para entrada ou saida
 */
void LCD_tris(void) {
	//Hex 0x1F
	// Binary 0b010010
	// TRISD4 = 0;
	// TRISD5 = 0;
	// TRISD6 = 0;
	// TRISD7 = 0;

	// TRISE0 = 0;
	// TRISE1 = 0;

    TRISD = TRISD & 0b00001111; // forca saidas
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
    for (i = 0; i < 2; i++) {
        LCD_escreve_4bits(dado, i);
        LCD_pulso_comando(tipo);
    }
}

/**
 *  \brief Escreve no LCD usando ligação de 4 bits
 *  \param [in] dado dado a ser mostrado no display
 *  \param [in] pos Posição no display
 */
void LCD_escreve_4bits(unsigned char dado, char pos) {
    if (pos == 0) {
        if (0 != (dado & 0x80)) {
            output_high(lcd_PIN_7);
        } else {
            output_low(lcd_PIN_7);
        }
        if (0 != (dado & 0x40)) {
            output_high(lcd_PIN_6);
        } else {
            output_low(lcd_PIN_6);
        }
        if (0 != (dado & 0x20)) {
            output_high(lcd_PIN_5);
        } else {
            output_low(lcd_PIN_5);
        }
        if (0 != (dado & 0x10)) {
            output_high(lcd_PIN_4);
        } else {
            output_low(lcd_PIN_4);
        }
    } else {
        if (0 != (dado & 0x08)) {
            output_high(lcd_PIN_7);
        } else {
            output_low(lcd_PIN_7);
        }
        if (0 != (dado & 0x04)) {
            output_high(lcd_PIN_6);
        } else {
            output_low(lcd_PIN_6);
        }
        if (0 != (dado & 0x02)) {
            output_high(lcd_PIN_5);
        } else {
            output_low(lcd_PIN_5);
        }
        if (0 != (dado & 0x01)) {
            output_high(lcd_PIN_4);
        } else {
            output_low(lcd_PIN_4);
        }
    }
    delay_us(1);
}

/**
 *  \brief Aciona as linha RS e EN conforme o tipo do dado
 *  \param [in] tipo Dado ou instrução para o LCD?
 */
void LCD_pulso_comando(unsigned char tipo) {
    //1 - Dado, 0 - Instrução
    if (tipo == __dado)
        output_high(lcd_rs);

    if (tipo == __instrucao)
        output_low(lcd_rs);

    //Pulso em EN RE1
    output_low(lcd_en);
    delay_us(5);
    output_high(lcd_en);
    delay_us(10);
    output_low(lcd_en);
    delay_us(5);
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
void LCD_inicializacao() {

    //1-function set
    //2-display on off
    //3-entry mode set
    //4 display clear
    //comunicacao 8 vias, duas colunas, 7x5 = 0x38
    //comunicacao 4 vias, duas colunas, 7x5 = 0x28

    unsigned char info;

    delay_ms(500); //necessario esperar antes de qq coisa

    LCD_tris();

    output_low(lcd_rs);
    output_high(lcd_en);
    output_low(lcd_PIN_7);
    output_low(lcd_PIN_6);
    output_low(lcd_PIN_5);
    output_low(lcd_PIN_4);

    info = 0x33;
    LCD_escreve_word(info, __instrucao);
    delay_ms(4);

    info = 0x32;
    LCD_escreve_word(info, __instrucao);
    delay_ms(4);

    info = 0x28; //comunicacao 4 vias, duas colunas, 7x5 = 0x28
    LCD_escreve_word(info, __instrucao);
    delay_ms(2);

    info = 0x08; //desliga display
    LCD_escreve_word(info, __instrucao);
    delay_us(40);

    info = 0x0f; //Cursor com Alternância
    LCD_escreve_word(info, __instrucao);
    delay_us(40);

    LCD_posicao_inicio();

    info = 0x28; //comunicacao 4 vias, duas colunas, 7x5 = 0x28
    LCD_escreve_word(info, __instrucao);
    delay_ms(2);

    info = 0x0c; //ligar o display sem cursor
    LCD_escreve_word(info, __instrucao);
    delay_us(40);

    info = 0x06; //deslocamento auto para esquerda
    LCD_escreve_word(info, __instrucao);
    delay_us(40);

    delay_ms(500); //aguarda LCD estabilizar
    LCD_posicao_inicio();
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
