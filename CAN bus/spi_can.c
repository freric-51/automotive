/*
 * File:   spi_can.c
 * Authors:
 * Ricardo de Freitas 1023007
 * configura SPI e CAN
 */

//#include <spi.h>

#define CAN_CS pin_b2
#define CAN_SO pin_c4
#define CAN_SI pin_c5
#define CAN_SCK pin_c3

#if defined __PCH__
#byte SSPSTAT = 0xFC7
#byte SSPBUF =  0xFC9
#elif defined __PCM__
#byte SSPSTAT = 0x94
#byte SSPBUF =  0x13
#endif

//############################
//CAN Controller Register Map
//############################
#define RXF0SIDH 0x00
#define RXF3SIDH 0x10
#define RXM0SIDH 0x20
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70

#define RXF0SIDL 0x01
#define RXF3SIDL 0x11
#define RXM0SIDL 0x21
#define TXB0SIDH 0x31
#define TXB1SIDH 0x41
#define TXB2SIDH 0x51
#define RXB0SIDH 0x61
#define RXB1SIDH 0x71

#define RXF0EID8 0x02
#define RXF3EID8 0x12
#define RXM0EID8 0x22
#define TXB0SIDL 0x32
#define TXB1SIDL 0x42
#define TXB2SIDL 0x52
#define RXB0SIDL 0x62
#define RXB1SIDL 0x72

#define RXF0EID0 0x03
#define RXF3EID0 0x13
#define RXM0EID0 0x23
#define TXB0EID8 0x33
#define TXB1EID8 0x43
#define TXB2EID8 0x53
#define RXB0EID8 0x63
#define RXB1EID8 0x72

#define RXF1SIDH 0x04
#define RXF4SIDH 0x14
#define RXM1SIDH 0x24
#define TXB0EID0 0x34
#define TXB1EID0 0x44
#define TXB2EID0 0x54
#define RXB0EID0 0x64
#define RXB1EID0 0x74

#define RXF1SIDL 0x05
#define RXF4SIDL 0x15
#define RXM1SIDL 0x25
#define TXB0DLC 0x35
#define TXB1DLC 0x45
#define TXB2DLC 0x55
#define RXB0DLC 0x65
#define RXB1DLC 0x75

#define RXF1EID8 0x06
#define RXF4EID8 0x16
#define RXM1EID8 0x26
#define TXB0D0 0x36
#define TXB1D0 0x46
#define TXB2D0 0x56
#define RXB0D0 0x66
#define RXB1D0 0x76

#define RXF1EID0 0x07
#define RXF4EID0 0x17
#define RXM1EID0 0x27
#define TXB0D1 0x37
#define TXB1D1 0x47
#define TXB2D1 0x57
#define RXB0D1 0x67
#define RXB1D1 0x77

#define RXF2SIDH 0x08
#define RXF5SIDH 0x18
#define CNF3 0x28
#define TXB0D2 0x38
#define TXB1D2 0x48
#define TXB2D2 0x58
#define RXB0D2 0x68
#define RXB1D2 0x78

#define RXF2SIDL 0x09
#define RXF5SIDL 0x19
#define CNF2 0x29
#define TXB0D3 0x39
#define TXB1D3 0x49
#define TXB2D3 0x59
#define RXB0D3 0x69
#define RXB1D3 0x79

#define RXF2EID8 0x0A
#define RXF5EID8 0x1A
#define CNF1 0x2A
#define TXB0D4 0x3A
#define TXB1D4 0x4A
#define TXB2D4 0x5A
#define RXB0D4 0x6A
#define RXB1D4 0x7A

#define RXF2EID0 0x0B
#define RXF5EID0 0x1B
#define CANINTE 0x2B
#define TXB0D5 0x3B
#define TXB1D5 0x4B
#define TXB2D5 0x5B
#define RXB0D5 0x6B
#define RXB1D5 0x7B

#define BFPCTRL 0x0C
#define TEC 0x1C
#define CANINTF 0x2C
#define TXB0D6 0x3C
#define TXB1D6 0x4C
#define TXB2D6 0x5C
#define RXB0D6 0x6C
#define RXB1D6 0x7C

#define TXRTSCTRL 0x0D
#define REC 0x1D
#define EFLG 0x2D
#define TXB0D7 0x3D
#define TXB1D7 0x4D
#define TXB2D7 0x5D
#define RXB0D7 0x6D
#define RXB1D7 0x7D

#define CANSTAT 0x0E

#define CANCTRL 0x0F
//############################
//CAN Controller Register Map
//############################

char SPI_FatoNovo;

void CAN_Tris(void) {
    /*
     * Hex 0x1F
     * Binary 0b010010
     */
    set_tris_B(0b11011011);
    set_tris_C(0b10010111);
}

void CAN_CS_Low(void) {
    //---___
    output_low(CAN_CS);
    delay_us(10);
}

void CAN_CS_High(void) {
    //___---
    output_high(CAN_CS);
    delay_us(10);
}

void CAN_SPI_init(void) {
    CAN_Tris();
    //levantar CS
    CAN_CS_High();

    // Configura a comunicação SPI como Master,
    // com uma atuação na borda de subida e com uma divisão de 16 ou 64 no clock
    setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_XMIT_L_TO_H | SPI_CLK_DIV_64);

    // SPI
    // SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IN
    // Constants used in SETUP are:
    // SPI_MASTER       0x20
    // SPI_SLAVE        0x24
    // SPI_L_TO_H       0
    // SPI_H_TO_L       0x10
    // SPI_CLK_DIV_4    0
    // SPI_CLK_DIV_16   1
    // SPI_CLK_DIV_64   2
    // SPI_CLK_T2       3
    // SPI_SS_DISABLED  1
    // SPI_SAMPLE_AT_END 0x8000
    // SPI_XMIT_L_TO_H  0x4000
}

void CAN_Write(unsigned char can_end, unsigned char can_dado) {
    CAN_Tris();
    CAN_CS_Low();
    //##############
    SPI_FatoNovo = 0;
    spi_write(0b00000010); //SSPBUF = 0b00000010; //instrução de escrita
    while (!SPI_FatoNovo);
    //##############
    //SSPBUF = can_end;
    SPI_FatoNovo = 0;
    spi_write(can_end);
    while (!SPI_FatoNovo);
    //##############
    //SSPBUF = can_dado;
    SPI_FatoNovo = 0;
    spi_write(can_dado);
    while (!SPI_FatoNovo);
    //##############
    CAN_CS_High();
}

void CAN_reset(void) {
    CAN_Tris();
    CAN_CS_Low();
    //##############
    SPI_FatoNovo = 0;
    spi_write(0b11000000); //SSPBUF = 0b11000000;
    while (!SPI_FatoNovo);
    //##############
    CAN_CS_High();
}

unsigned char CAN_Read(unsigned char can_end) {
    unsigned char dado;
    CAN_Tris();
    CAN_CS_Low();
    //##############
    SPI_FatoNovo = 0;
    spi_write(0b00000011); //SSPBUF = 0b00000011; //instrução de leitura
    while (!SPI_FatoNovo);
    //##############
    //SSPBUF = can_end;
    SPI_FatoNovo = 0;
    spi_write(can_end);
    while (!SPI_FatoNovo);
    //##############
    //dado = SSPBUF;
    dado = spi_read(0);
    CAN_CS_High();
    //##############
    return dado;
}

void CAN_config(void) {
    //Configuração do modo de operação
    CAN_Write(CANCTRL, 0b10000000); // CANCTRL, coloca em modo de configuração
    delay_ms(100);

    // Configuração dos segmentos de propagação, fase 1, fase 2 e SJW
    //baud rate 125khz, osc 20Mhz

    //CNF1 SJW<7:6> 2(10) BRP<5:0> 8-1=7(000111)
    CAN_Write(CNF1, 0b10000111);

    //CNF2 F2<7> 1 SAM<6> 0 segF1<5:3> 4-1=3 segProp<2:0> 1-1=0
    CAN_Write(CNF2, 0b10011000);

    //CNF3 SOF<7> 0 WAKFIL<6> 0 <000> segF2<2:0> 4-1=3
    CAN_Write(CNF3, 0b00000011); // CNF3, filtro desabilitado, clock out habilitado, ps2 6 tqs

    // Configuração das máscaras e filtros, não usado
    CAN_Write(RXF0SIDH, 0x00); //filtros
    CAN_Write(RXF0SIDL, 0x00);
    CAN_Write(RXM0SIDH, 0x00); //máscaras
    CAN_Write(RXM0SIDL, 0x00);

    CAN_Write(RXB0CTRL, 0b01100100); //mascaras e filtros desligados <6:7>=11, com roll over <2>=1
    CAN_Write(RXB1CTRL, 0b01100000); //mascaras e filtros desligados <6:7>=11

    // Interrupções
    CAN_Write(CANINTE, 0x10); // interrupções
    CAN_Write(CANINTF, 0x00); // Zera as interrupções e libera a mensagem para ser enviada

    // Modo de funcionamento normal
    CAN_Write(CANCTRL, 0x00); //CANCTRL, coloca em modo de trabalho
}