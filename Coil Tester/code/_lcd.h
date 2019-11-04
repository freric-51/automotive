/**
 *  \file _lcd.h
 *  \brief protótipos para _lcd.c
 */
 
/** 
 *  @{
 *   LCD 16x2
 */
#define LCD_colunas 16
#define LCD_linhas 2

#define LCD_L1	0x80
#define LCD_L2	0xC0
/** @} */

// ##### prototipos #####
void LCD_tris(void);
void LCD_escreve_word(unsigned char dado, unsigned char tipo);
void LCD_escreve_4bits(unsigned char dado, char pos);
void LCD_pulso_comando(unsigned char tipo);
void LCD_posicao_inicio(void);
void LCD_inicializacao();
void LCD_mensagem(unsigned char, unsigned char*);
// ##### fim prototipos #####
