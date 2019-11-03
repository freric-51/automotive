/*
 * File:   lcd.c
 * Authors:
 * Ricardo de Freitas 1023007
 * comandos especificos para o LCD
 */

/*
 * Bits do LCD
 */
#define lcd_PIN_4 pin_d0
#define lcd_PIN_5 pin_d1
#define lcd_PIN_6 pin_d2
#define lcd_PIN_7 pin_d3

#define lcd_rs pin_b5
#define lcd_en pin_d4

// comandos para o PLC
#define __dado 1
#define __instrucao 0

/*
   LCD ==== ==== ==== PIC
   RS-----------------RB5
   W - GND
   EN-----------------RD4

   RB4----------------RD0
   ...
   RB7----------------RD3

   pulso de execucao de um comando:
   RS = 0
   EN   0____1_____0
        5us__10us__5us

   pulso de escrita de letra
   RS = 1
   EN   0____1_____0
        5us__10us__5us
 */

// ##### prototipos #####
void LCD_tris(void);
void LCD_escreve_word(unsigned char dado, unsigned char tipo);
void LCD_escreve_4bits(unsigned char dado, char pos);
void LCD_pulso_comando(unsigned char tipo);
void LCD_posicao_inicio(void);
void LCD_inicializacao(char modelo);
void LCD_mensagem(unsigned char posicao, unsigned char *frase);
// ##### fim prototipos #####

void LCD_tris(void) {
    /*
     * Hex 0x1F
     * Binary 0b010010
     */
    set_tris_D(0b11100000);
    set_tris_B(0b11011011);
}

void LCD_escreve_word(unsigned char dado, unsigned char tipo) {
    char i;
    for (i = 0; i < 2; i++) {
        LCD_escreve_4bits(dado, i);
        LCD_pulso_comando(tipo);
    }
}

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

void LCD_posicao_inicio(void) {
    unsigned char info;
    LCD_tris();

    info = 0x01; //limpar o display e posicionar o cursor linha 1 coluna 1
    LCD_escreve_word(info, __instrucao);
    delay_us(1800);
}

void LCD_inicializacao(char modelo) {
    //modelo 'E' = escola 'C' = casa

    //1-function set
    //2-display on off
    //3-entry mode set
    //4 display clear
    //comunicacao 8 vias, duas colunas, 7x5 = 0x38
    //comunicacao 4 vias, duas colunas, 7x5 = 0x28

    unsigned char info;

    delay_ms(15); //necessario esperar 15ms antes de qq coisa

    LCD_tris();
    output_low(lcd_rs);
    output_high(lcd_en);
    output_low(lcd_PIN_7);
    output_low(lcd_PIN_6);
    output_low(lcd_PIN_5);
    output_low(lcd_PIN_4);

    if ('E' == modelo) {
        info = 0x33;
        LCD_escreve_word(info, __instrucao);
        delay_ms(4);

        info = 0x32;
        LCD_escreve_word(info, __instrucao);
        delay_ms(4);
    }

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

    LCD_posicao_inicio();
}

void LCD_mensagem(unsigned char posicao, unsigned char *frase) {
    char i;
    LCD_tris();

    LCD_escreve_word(posicao, __instrucao); //comecar da linhaXcoluna
    delay_us(40);

    for (i = 0; i < strlen(frase); i++) {
        if (0 == frase[i])
            break;

        LCD_escreve_word(frase[i], __dado);
    }
}