/**
 * \file main.c
 * \brief Este arquivo contem a lista de desenvolvedores,
 * rotinas de configuração e
 * a máquina de estado principal
 * \date	1o semestre 2013
 *
 * \mainpage FATEC Santo Andre
 * <b>Curso de Graduacao em Tecnologia em Eletrônica Automotiva</b> \n
 * <b>Disciplina :</b> Carga e Partida - 4o semestre \n
 * <b>Professor :</b> Edson Caoru Kitani \n
 * <b>Testador de Bobinas até 2A</b>
 * \n
 * \anchor FigProjeto
 * \addindex FigProjeto
 * \image    latex cp_geral.eps "Montagem Final" height=13cm
 * \n
 * \section sc1 Integrantes do Grupo
 *
 * \subsection s1	1o semestre de 2013
 * \authors
 * Alex Guedes                      R.A. : 1222009	\n
 * Pedro Augusto Domingos           R.A. : 1123009	\n
 * Ricardo de Freitas               R.A. : 1023007	\n
 * Rogerio Zavan                    R.A. : 1123018	\n
 * Vinicius Garcia Duarte           R.A. : 1122018	\n
 *
 */

 /**
 * \page    Figuras      Lista de Figuras
 *  Relacao de Esquemas, Imagens, Gráficos, Figuras
 * \secreflist
 *       \refitem FigProjeto "Montagem Final"
 *       \refitem Fig1 "Teclado"
 *       \refitem Fig2 "Display"
 *       \refitem Fig3 "JHD162A - temporização de escrita"
 * \endsecreflist
 */

  /**
 * \page 	ROM_RAM		Estatísticas de utilização de memória
 * \include	.\cp.STA
 */
 
// Bibliotecas
#include	"_fuses.h"
#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	"_funcoes_gerais.h"
#include	"_analogico.h"
#include	"_lcd.h"
#include	"_teclado.h"
#include	"_delay.h"
#include	"_matriz.h"

/**
 *  @{
 *  Define posição no display
 */
#define POS_TH 2
#define POS_TL 2
#define POS_AD 11
/** @} */

//  Constantes
unsigned long int TH_Lim_SUP = 500; /**< Limite superior em us */
unsigned long int TH_Lim_INF = 30; /**< Limite inferior em us */
unsigned long int TL_Lim_SUP = 1000; /**< Limite superior em ms */
unsigned long int TL_Lim_INF = 100; /**< Limite inferior em ms */

// prototipos
void InitHW(void);
void InitSystem(void);
char ValidarEntrada( unsigned long int, unsigned long int, unsigned long int);

// Variaveis
unsigned char LCD_Mascara_L1 [1 + LCD_colunas] = "TH ##.us I=#.##A"; /**< Mascara no Display */
unsigned char LCD_Mascara_L2 [1 + LCD_colunas] = "TL ##.ms        "; /**< Mascara no Display */
unsigned char LCD_BemVindo_L1[1 + LCD_colunas] = " FATEC 1o 2013  "; /**< Mensagem no Display */
unsigned char LCD_BemVindo_L2[1 + LCD_colunas] = " Carga e Partida"; /**< Mensagem no Display */

/**
 * \fn main
 * \brief Funcao principal do programa
 * com aplicacao das funcoes auxiliares e de tratamento
 * gerando as interrupcoes para o gerenciamento em si
 * \n
 *  \name    nFig2
 * @{
 * \section Fig2 Display
 * \ref Figuras "Lista de Figuras"
 * \addindex Fig2
 * \image    latex display.eps "Display" width=14cm
 * @} */

void main(void) {
	static unsigned long int Tempo_montar;
	static unsigned char AD_ValorZ1;
	unsigned char AD_ValorAtual;
    unsigned char tecla = 0;

    unsigned char msg[1 + LCD_colunas];
    char dummy;
	unsigned long int dummyUL;

	unsigned char POS_TH_L = LCD_L1;
	unsigned char POS_TL_L = LCD_L2;

	char PassoTeclado = 0;

    InitHW();
    InitSystem();

    //Boas Vindas:
    strcpy(msg, LCD_BemVindo_L1);
    LCD_mensagem(LCD_L1, msg);
    strcpy(msg, LCD_BemVindo_L2);
    LCD_mensagem(LCD_L2, msg);
	delay_ms(3000);
	// Mascara:
    strcpy(msg, LCD_Mascara_L1);
    LCD_mensagem(LCD_L1, msg);
    strcpy(msg, LCD_Mascara_L2);
    LCD_mensagem(LCD_L2, msg);

	// Limites para TH e TL
	if (!ValidarEntrada(TH, TH_Lim_SUP, TH_Lim_INF))
		TH = TH_Lim_INF;
	if (!ValidarEntrada(TL, TL_Lim_SUP, TL_Lim_INF))
		TL = TL_Lim_INF;

	// Atualizar TH e TL
	dummy = sprintf(msg, "%04lu\0", TH);
	LCD_mensagem(POS_TH + POS_TH_L, msg);

	dummy = sprintf(msg, "%04lu\0", TL);
	LCD_mensagem(POS_TL + POS_TL_L, msg);
	delay_ms(100);

	//Partir a analógica
	AD_ValorAtual = 100;
	AD_ValorZ1 = 1;
	enable_interrupts(GLOBAL);
	AD_Start(0);

    while (TRUE) {
		// Pulso
		AD_ValorAtual = AD_Leitura();
		if (AD_ValorZ1 != AD_ValorAtual){
			AD_ValorZ1 = AD_ValorAtual;
			dummyUL = Matriz[AD_ValorAtual];
			LongFix_to_ASCII(dummyUL, 2, msg);
			LCD_mensagem(POS_AD + LCD_L1, msg);
		}

		// Tratamento de teclado
		// Atualizacao de TH TL
        if (!KB_TempoVarredura) KB_trata_teclado();
        tecla = KB_Tecla_Nova();
        if (tecla != KB_Tecla_Nenhuma) {
			//para acompanhamento
            dummy = sprintf(msg, "%c\0", tecla);
            LCD_mensagem(15 + LCD_L2, msg);

			switch (PassoTeclado){
			case 0:
				if ('A' == tecla){
					PassoTeclado = 1;
					msg = "____";
					LCD_mensagem(POS_TH + POS_TH_L, msg);
				}
				if ('B' == tecla) {
					PassoTeclado = 101;
					msg = "____";
					LCD_mensagem(POS_TL + POS_TL_L, msg);
				}
				break;

			// TEMPO EM ALTO
			case 1:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 2;
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar =  dummyUL;
					dummy = sprintf(msg, "___%01lu\0", Tempo_montar);
					LCD_mensagem(POS_TH + POS_TH_L, msg);
				}
				break;
			case 2:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 3;
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar = 10ul * Tempo_montar + dummyUL;
					dummy = sprintf(msg, "__%02lu\0", Tempo_montar);
					LCD_mensagem(POS_TH + POS_TH_L, msg);
				}
				break;
			case 3:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 4;
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar = 10ul * Tempo_montar + dummyUL;
					dummy = sprintf(msg, "_%03lu\0", Tempo_montar);
					LCD_mensagem(POS_TH + POS_TH_L, msg);
				}
				break;
			case 4:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 0; //retornando por ser a UNIDADE
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar = 10ul * Tempo_montar + dummyUL;
					// Atualizar
					if (ValidarEntrada(Tempo_montar, TH_Lim_SUP, TH_Lim_INF))
						TH = Tempo_montar;
					dummy = sprintf(msg, "%04lu\0", TH);
					LCD_mensagem(POS_TH + POS_TH_L, msg);
					PassoTeclado = 0;
				}
				break;

			// TEMPO EM Baixo
			case 101:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 102;
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar =  dummyUL;
					dummy = sprintf(msg, "___%01lu\0", Tempo_montar);
					LCD_mensagem(POS_TL + POS_TL_L, msg);
				}
				break;
			case 102:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 103;
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar = 10ul * Tempo_montar + dummyUL;
					dummy = sprintf(msg, "__%02lu\0", Tempo_montar);
					LCD_mensagem(POS_TL + POS_TL_L, msg);
				}
				break;
			case 103:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 104;
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar = 10ul * Tempo_montar + dummyUL;
					dummy = sprintf(msg, "_%03lu\0", Tempo_montar);
					LCD_mensagem(POS_TL + POS_TL_L, msg);
				}
				break;
			case 104:
				if ((tecla >= '0') && (tecla <= '9')){
					PassoTeclado = 0; //retornando por ser a UNIDADE
					dummyUL = ASCII_TO_INT (tecla);
					Tempo_montar = 10ul * Tempo_montar + dummyUL;
					// Atualizar
					if (ValidarEntrada(Tempo_montar, TL_Lim_SUP, TL_Lim_INF))
						TL = Tempo_montar;
					dummy = sprintf(msg, "%04lu\0", TL);
					LCD_mensagem(POS_TL + POS_TL_L, msg);
					PassoTeclado = 0;
				}
				break;
			}
        }
    }
}

/**
 * \fn InitHW
 * \brief Funcao de inicializacao com as configuracoes do uControlador
 */
void InitHW(void) {
	// set all ports as input
    TRISA = 0XFF;
    TRISB = 0XFF;
    TRISC = 0XFF;
    TRISD = 0XFF;
    TRISE = 0XFF;

	// desabilita interrupções
	disable_interrupts(GLOBAL);
	// desabilita interrupções individualmente
    disable_interrupts(int_timer0);
    disable_interrupts(int_timer1);
    disable_interrupts(int_timer2);
    disable_interrupts(int_timer3);
	disable_interrupts(int_ext);
	disable_interrupts(int_ext1);
	disable_interrupts(int_ext2);
    disable_interrupts(INT_AD);
    disable_interrupts(INT_CCP1);
    disable_interrupts(INT_CCP2);
    disable_interrupts(INT_COMP);
    // disable_interrupts(INT_PSP);
    disable_interrupts(INT_RB);
    disable_interrupts(INT_RDA);
    disable_interrupts(INT_RTCC);
    disable_interrupts(INT_SSP);
    disable_interrupts(INT_TBE);
	// desabilita temporizadores
	T1CON = 0x00;
	T2CON = 0x00;
	T3CON = 0x00;
	// AD Converter
    setup_adc(ADC_OFF);
    setup_adc_ports(NO_ANALOGS);
    setup_psp(PSP_DISABLED); // Parallel slave port
    setup_spi(FALSE); // SPI Disabled

    setup_comparator(NC_NC_NC_NC); // Desliga comparadores internos
    setup_vref(FALSE); // Tensao de referencia  para o comparador

    // Quero 8us que * 8 bits = 2048us
    // 16MHz/4 = 4MHz -> 1/4Mhz = 250ns
    // 8us / 250ns = 32
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_32 | RTCC_8_BIT);
    set_timer0(TMR_Prescaler_1ms);
    enable_interrupts(int_timer0);

	// Perigo T1 toma controle da porta C /div 1 2 4 8
	// timer 1 : 16Mhz / 4 = 250ns
	// 250ns * 4 = 1us
	// a cada 10us
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
	set_timer1(65500);
	enable_interrupts(int_timer1);

    //setup_timer_3(T3_DISABLED);
	// Perigo T3 toma controle da porta C | T3_DIV_BY_4

    AD_TRIS();
    setup_adc(ADC_CLOCK_DIV_8);
    setup_adc_ports(AN0);
	enable_interrupts(int_AD);

    KB_tris();
    KB_Reset();
	PULSO_tris();
}

/**
 * \fn InitSystem
 * \brief Inicializa circuitos externos ao microcontrolador
 */
void InitSystem(void) {
    LCD_inicializacao(); // Funcao que inicializa o Display
}

/**
 *  \brief Verifica se um valor está dentro de limites
 *  \param [in] A Valor a ser comparado
 *  \param [in] MAX Limite superior
 *  \param [in] MIN Limite inferior
 *  \return TRUE / FALSE
 */
char ValidarEntrada(unsigned long int A, unsigned long int MAX, unsigned long int MIN){
	if (A > MAX) return false;
	if (A < MIN) return false;
	return true;
}
