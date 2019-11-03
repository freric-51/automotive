/*
 * File:   main.c
 * Authors:
 * Ricardo de Freitas 1023007
 * Trabalho de Redes de comunicação automotivas
 * Created on 6 de Outubro de 2012
 * 0xEFF2 = PIC18F452
 * 0x1080 = PIC18F4520
 * 0xFFe0 = 
 * PIC16F877A
 */

//##########################################
// biblioteca
#include <.\fuses.c>
#include <.\lcd.c>
#include <.\spi_can.c>

//##########################################
#define led_luz_freio pin_e0
#define led_luz_re pin_e1
#define led_luz_pisca pin_e2

#define chaves input_d()
#define chave_0 pin_d0
#define chave_1 pin_d1
#define chave_2 pin_d2
#define chave_3 pin_d3

//##########################################
// tratativa de interrupcoes
//##########################################

#int_ssp

void trata_int_ssp(void) {
    SPI_FatoNovo = 1;
}

void tris_led(void) {
    /*
     * Hex 0x1F
     * Binary 0b010010
     */
    set_tris_e(0b11111000);
}

void tris_chaves(void) {
    set_tris_d(0b11111111);
}

void tris_analog(void) {
    set_tris_a(0b11111111);
}

void inicializacao_hardware() {
    disable_interrupts(GLOBAL);
    disable_interrupts(int_timer0);
    disable_interrupts(int_timer1);
    disable_interrupts(int_timer2);
#if defined(__PCH__)
    //18F4520
    disable_interrupts(int_timer3);
#endif

    setup_psp(PSP_DISABLED);
    //setup_spi(FALSE);

    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_1);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    setup_timer_2(T2_DISABLED, 0, 1);
#if defined(__PCH__)
    //18F4520
    setup_timer_3(T3_INTERNAL | T3_DIV_BY_1);
#endif

    tris_analog();
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(ALL_ANALOG);

    //set_tris_b(0xFF);
    //ext_int_edge(0,L_TO_H);

    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_32); // Define timer 0
    //set_timer0 (0);
}

char Teste_SPI(unsigned char can_end) {
    int dummy;
    unsigned char* texto_lcd[17];
    unsigned char can_dado, can_lido;
    char troca_led = 0, spi_error = 0;

    can_dado = 0x00;

    //mascara:
    dummy = sprintf(texto_lcd, "CAN:__>__<__\0");
    LCD_mensagem(0x80, texto_lcd);

    while (spi_error < 11) {
        //mostra ciclo de processamento
        if (troca_led == 0) {
            output_high(led_luz_pisca);
            troca_led = 1;
        } else {
            output_low(led_luz_pisca);
            troca_led = 0;
        }

        //can_dado 0 a FF;
        can_dado++;
        if (0 == can_dado)
            return 0; //foi ok ate o fim

        dummy = sprintf(texto_lcd, "%X>%X\0", can_end, can_dado);
        LCD_mensagem(0x84, texto_lcd);

        CAN_Write(can_end, can_dado);

        if (0 == spi_error)
            delay_ms(50); //efeito visual no LCD
        else
            delay_ms(1000); //efeito visual no LCD

        can_lido = CAN_Read(can_end);
        dummy = sprintf(texto_lcd, "<%X\0", can_lido);
        LCD_mensagem(0x89, texto_lcd);

        if (can_lido == can_dado) {
            output_high(led_luz_re);
            spi_error = 0;
        } else {
            output_low(led_luz_re);
            spi_error++; //repetir o mesmmo dado
            can_dado--;
        }
    }
    return 1; //erro
}

void Teste_LED(void) {
    unsigned char i;
    tris_led();
    for (i = 0; i < 10; i++) {
        output_low(led_luz_re);
        delay_ms(100);
        output_high(led_luz_re);
        delay_ms(100);
    }

    for (i = 0; i < 10; i++) {
        output_low(led_luz_pisca);
        delay_ms(100);
        output_high(led_luz_pisca);
        delay_ms(100);
    }

    for (i = 0; i < 10; i++) {
        output_low(led_luz_freio);
        delay_ms(100);
        output_high(led_luz_freio);
        delay_ms(100);
    }
}

void main(void) {
    int dummy;
    unsigned long can_end, cand_end_1, cand_end_2, cand_end_lido;
    unsigned char* texto_lcd[17];
    unsigned char can_end_Low, can_end_High, can_dado, can_lido, can_lido_1, can_lido_2;
    unsigned char CANINTF_BKP;

inicio:
    inicializacao_hardware();
    CAN_SPI_init();

    Teste_LED();

    //######### LCD #########
    //'E'scola 'C'asa
    LCD_inicializacao('E');
    delay_ms(10);
    LCD_posicao_inicio();
    dummy = sprintf(texto_lcd, "%s\0", "CAN 2o 2012.1");
    LCD_mensagem(0x80, texto_lcd);
    dummy = sprintf(texto_lcd, "%s\0", "Ricardo Freitas!");
    LCD_mensagem(0xC0, texto_lcd);
    delay_ms(3000);

    //######### SPI CAN #########
    // Habilita interrupção SPI
    enable_interrupts(int_ssp);
    clear_interrupt(int_ssp);
    // Habilita tratar interrupcoes
    enable_interrupts(global);

    CAN_reset();
    CAN_config();

    //limpar LCD
    LCD_posicao_inicio();

    if (0 != Teste_SPI(CANINTE))
        goto inicio;

    //fim dos testes
    output_high(led_luz_pisca);
    output_high(led_luz_re);
    output_high(led_luz_freio);
    delay_ms(2000);

    tris_chaves();
    can_end = 0x0220 | (chaves & 0x0F);

    LCD_posicao_inicio();

    dummy = sprintf(texto_lcd, "%s%LX\0", "Local:", can_end);
    LCD_mensagem(0x80, texto_lcd);

    dummy = sprintf(texto_lcd, "%s\0", "0000:00 0000:00");
    LCD_mensagem(0xC0, texto_lcd);

    cand_end_1 = 0xffff;
    cand_end_2 = 0xffff;

    CAN_Write(CANINTE, 0b00000011); //disable interrupts on error and enable RXB0 RXB1
    CAN_Write(CANINTF, 0x00); //prepara receber/enviar mensagem

    while (1) {
        delay_ms(200); //base de tempo de atualização e envio de dados

        //transmitir
        CAN_Write(TXB2CTRL, 0x03); //TXP = 11 Mensagem de alta prioridade TXREQ=0 ABORT

        //At a minimum, the TXBnSIDH, TXBnSIDL and TXBnDLC registers must be loaded
        //mover 11o bit para a posição 16a
        can_end_Low = (can_end << 5);
        can_end_High = (can_end << 5) >> 8;
        CAN_Write(TXB2SIDH, can_end_High); //ID da mensagem
        CAN_Write(TXB2SIDL, can_end_Low); //ID da mensagem
        CAN_Write(TXB2DLC, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)

        tris_analog();
        set_adc_channel(0);
        delay_us(10);
        can_dado = read_adc();
        dummy = sprintf(texto_lcd, "%s%X\0", "V=", can_dado);
        LCD_mensagem(0x8B, texto_lcd);
        CAN_Write(TXB2D0, can_dado); //Endereço onde a mesagem será guardada

        CAN_Write(TXB2CTRL, 0b00001011); //Instrução para enviar a mensagem
        CAN_Write(TXB2CTRL, 0b00000011); //abortar msg

        //RECEBER
        while (!(0x03 & CAN_Read(CANINTF))); //Espera algo novo

        CANINTF_BKP = CAN_Read(CANINTF);

        if (bit_test(CANINTF_BKP, 0)) {
            //RX0
            //Endereco do no (ID)
            cand_end_lido = 0;
            can_lido = CAN_Read(RXB0SIDH);
            cand_end_lido = can_lido;
            cand_end_lido = cand_end_lido << 8;
            can_lido = CAN_Read(RXB0SIDL);
            cand_end_lido = cand_end_lido + can_lido;
            cand_end_lido = cand_end_lido >> 5;

            if (0xffff == cand_end_1)
                cand_end_1 = cand_end_lido;
            if (cand_end_1 != cand_end_lido)
                cand_end_2 = cand_end_lido;

            //Dado da mensagem (sei que DLC =1)
            can_lido = CAN_Read(RXB0D0);

            if (cand_end_1 == cand_end_lido)
                can_lido_1 = can_lido;
            else
                can_lido_2 = can_lido;
        }

        if (bit_test(CANINTF_BKP, 1)) {
            //RX1
            //Endereco do no (ID)
            cand_end_lido = 0;
            can_lido = CAN_Read(RXB1SIDH);
            cand_end_lido = can_lido;
            cand_end_lido = cand_end_lido << 8;
            can_lido = CAN_Read(RXB1SIDL);
            cand_end_lido = cand_end_lido + can_lido;
            cand_end_lido = cand_end_lido >> 5;

            if (0xffff == cand_end_1)
                cand_end_1 = cand_end_lido;
            if (cand_end_1 != cand_end_lido)
                cand_end_2 = cand_end_lido;

            //Dado da mensagem (sei que DLC =1)
            can_lido = CAN_Read(RXB1D0);

            if (cand_end_1 == cand_end_lido)
                can_lido_1 = can_lido;
            else
                can_lido_2 = can_lido;
        }

        if (bit_test(CANINTF_BKP, 0))
            bit_clear(CANINTF_BKP, 0); //RX0

        if (bit_test(CANINTF_BKP, 1))
            bit_clear(CANINTF_BKP, 1); //RX1

        CAN_Write(CANINTF, CANINTF_BKP);

        dummy = sprintf(texto_lcd, "%LX:%X %LX:%X\0", cand_end_1, can_lido_1, cand_end_2, can_lido_2);
        LCD_mensagem(0xC0, texto_lcd);

    }
}