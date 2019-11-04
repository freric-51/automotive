/**
 * \file main.c
 * \brief Este arquivo contem a lista de desenvolvedores,
 * rotinas de configuração e
 * a máquina de estado principal
 *
 * \mainpage FATEC Santo Andre 
 * <b><em><H1>Dr. Newton da Costa Brandão</H1></em></b> \n
 * <b>Curso de Graduacao em Tecnologia em Eletrônica Automotiva</b> \n
 * <b>Disciplina :</b> Unidades de Gerenciamento de Motores - 4o semestre \n
 * \anchor Edson <b>Professor :</b> Edson Caoru Kitani \n
 * <b>Software de Gerenciamento de Injeção Eletrônica Monoponto</b>
 * \n
 * \anchor Figura_1_1
 * \addindex Figura_1_1
 * \image 	latex topoH.eps "Montagem inicial conforme projeto anterior e montagem final com o condicionamento do MAP, chaveamento dos LEDs ao terra e dissipador no regulador de tensão."  width=\textwidth
 * \latexonly \newpage \endlatexonly
 * \section sc1 Integrantes do Grupo
 *
 * \subsection s1	1o semestre de 2010
 * \authors
 * Albino Sergio de Souza           R.A. :	\n
 * Bruno Martin de Alcantara Dias   R.A. :	\n
 * Marcelo Silva de Santana         R.A. :	\n
 *
 * \subsection s2	2o semestre de 2010
 * \authors
 * Daniel Alvarez Businaro          R.A. : 0910041	\n
 * Douglas Barbosa de Oliveira      R.A. : 0910112	\n
 * Fabricio Sforcin                 R.A. : 0910341	\n
 * Renan do Nascimento Felix        R.A. : 0910090	\n
 * Wellington Alves dos Santos      R.A. : 0910880	\n
 *
 * \subsection s3	1o semestre de 2011
 * \authors
 * Andre                            R.A. :			\n
 * Bruno Rodrigues                  R.A. : 0910945	\n
 * Rodrigo da Silva Cassimiro       R.A. : 0920071	\n
 * Ronaldo Siqueira da Gama         R.A. : 0920037	\n
 * Sergio Mitsue Kague              R.A. : 0920215	\n
 *
 * \subsection s4	2o semestre de 2011
 * \authors
 * Bruno Silva                      R.A. : 1012010	\n
 * Filipi Assis Lima                R.A. : 0920630	\n
 * Henrique Mendes                  R.A. : 1012038	\n
 * Lenilson Santana Lima            R.A. : 1012028	\n
 * Marcos Felipe                    R.A. : 1013003	\n
 *
 * \subsection s5	1o semestre de 2012
 * \authors
 * Anderson Cazzolato               R.A. : 1023005	\n
 * Carlos Augusto Mortagua          R.A. : 1013034	\n
 * Eduardo Aquino                   R.A. : 1023026	\n
 * Fabio Martinez de Moraes         R.A. : 1023014	\n
 * Roberto Juncker                  R.A. : 1112034	\n
 *
 * \subsection s6	2o semestre de 2012
 * \authors
 * Bruno Zani Sampaio               R.A. : 1112003	\n
 * Douglas Matias Martins           R.A. : 1112006	\n
 * Renato C. de Souza               R.A. : 1012020	\n
 * Vitor Silva Santos               R.A. : 1022028	\n
 * \note \ref s6 \n
 * 		A temperatura do ar de admissao não influencia o cálculo do tempo de injeção. \n
 *		TPS esta sem função derivativa (bomba de aceleração). \n
 * 		SPOUT nao funciona corretamente. \n
 * 		Definir uma matriz para o ‘Tempo_Injeta_Mais’. \n
 * 		Projetar circuito para monitoração da tensão da bateria. \n
 * 		Conectar o sinal do sensor MAP e anexar funcionalidade ao programa. \n
 * 		Corrigir a matriz referente aos valores de temperatura. \n
 * 		Aperfeiçoar a matriz de valores do TPS.
 *
 * \subsection s7	1o semestre de 2013
 * \authors
 * Alex Guedes                      R.A. : 1222009	\n
 * Pedro Augusto Domingos           R.A. : 1123009	\n
 * Ricardo de Freitas               R.A. : 1023007	\n
 * Rogerio Zavan                    R.A. : 1123018	\n
 * Vinicius Garcia Duarte           R.A. : 1122018	\n
 *
 * \warning \ref s7 \n
 *		Segundo nota do prof. \ref Edson : \n
 *		"Os softwares de controle do bico injetor encontram-se no sitio da disciplina.
 *		Eles devem ser analisados e implementados novamente."\n
 *		Portanto, as fontes do semestre anterior foram usadas como ponto de partida. \n
 *		A qualquer momento as fontes dos semestres anteriores estão a disposição com o prof. \ref Edson.\n
 *		Tendo em vista uma melhor compreensão e facilidade em trabalho em grupo, foi feita a separação em arquivos por função ou sinal.\n
 *		Foi configurado as entradas analógicas na sequência: AN0 (TPS), AN1(ACT), AN2(MAP) e AN3(ECT).\n
 *		A entrada RB1 que antes era usada em paralelo com RB0 para detetar as transições do PIP agora passa a comandar o display.
 *
 * \bug \ref s7 \n
 * 		Erros encontrados durante a apresentação final serão passados para o próximo semestre pelo prof. \ref Edson.
 *
 * \todo \ref s7 \n
 *		- Passagem de porções do programa para assembler. \n
 *		- Mudança do acesso das matrizes de indice (com um programa feito pelo CCS) para ponteiro. \n
 *		- O condicionamento dos sinais analógicos estão limitados à excursão do LM358 alimentado a 5Vcc. \n
 *		  Na prática a excursão de saída nos circuitos condicionadores analógicos ficou entre 0,6V e 3,5V. Ver figura ilustrativa ( \ref Figura_1_2 ).\n
 * 		  O adequado é substituí-los por Rail-to-Rail Operational Amplifiers. \n
 * 		  Exemplo: MCP6001, OP295, LTC6081, LT1677 entre outros. \n
 *		  Uma outra possibilidade é alimentar os operacionais com 7 volts.\n
 *		- Quanto ao condicionamento dos sinais digitais, o circuito com o comparador LM339 deve ser modificado para Schmitt trigger. \n
 *		  Os divisores de tensão devem ser revistos pois se a tensão da bateria for abaixo de 10V durante a partida os sinais digitais (exemplo o sensor Hall) se perdem. \n
 *		- Configurar AN4 para a leitura da tensão da bateria.
 *		- Acrescentar circuto de condicionamento para AN3 e AN4
 *		- Esta á a primeira vez que foi usado Doxygen para documentação. Melhores práticas para indexação, nomes de referências e diagramação são esperados.
 * \anchor Figura_1_2
 * \ref Figuras "Lista de Figuras"
 * \addindex Figura_1_2
 * \image 	latex ComumOpAmp.eps "Excursão do sinal de saída de um amplificador operacional comum" width=14cm
 *
 */

/**
 * \page	Figuras	Lista de tabelas, figuras e arquivos
 *
 * \section sc2_1 Tabelas
 * \anchor Tables
 * \addindex Tables
 * \latexonly
 *		\begingroup
 * 		\let \clearpage \relax
 * 		\let \cleardoublepage \relax
 * 		\listoftables
 * \endlatexonly
 *
 * \section	sc2_2	Relacao de esquemas, imagens, gráficos, figuras
 * \secreflist
 * 		\refitem Figura_1_1 "Evolução da montagem"
 * 		\refitem Figura_1_2 "Rail-to-Rail Operational Amplifiers"
 * 		\refitem Figura_10_1 "Conversor MAP FxV"
 * 		\refitem Figura_10_2 "Sinal de ignição"
 * 		\refitem Figura_10_4 "JHD162A - temporização de escrita"
 *		\refitem Figura_10_5 "Aceleração a 2200 RPM e motor a 90C"
 *		\refitem Figura_10_7 "Avanço de ignição a 6000 RPM e motor a 90C"
 *		\refitem Figura_10_9 "Partida a frio 10C e 1 bar"
 *		\refitem Figura_10_11 "Partida a quente a 60C"
 * \endsecreflist
 *
 * \section	Compiler	Como o programa foi compilado
 * \include	.\build.bat
 *
 * \section	ROM_RAM	Estatísticas de utilização de memória
 * \include	.\ger201301.STA
 *
  * \latexonly
 * 		\endgroup
 * \endlatexonly
 */

/*
 * Bibliotecas
 */
#include	"_fuses.h"
#include	"_lcd.h"
#include	"_PIP.h"
#include	"_ANA.h"
#include 	"_MemoryMap.h"
#include 	"_pinagem_ecu.h"
#include 	"_funcoes_gerais.h"
#include 	"_BaseTempo.h"
#include 	"_estrategia.h"
#include 	"_matriz.h"
#include	"_BotaoDisplay.h"

/*
 * prototipos
 */
void InitHW(void);
void InitSystem(void);
void Atualiza_Display(char);
void Reset_Prog(void);

/**
 * \fn main
 * \brief Funcao principal do programa
 * com aplicacao das funcoes auxiliares e de tratamento
 * gerando as interrupcoes para o gerenciamento em si
 */
void main(void) {
	static char RPMpointer_Anterior;
	//static char LCDpagina_loop = 0;
	static char LCDpagina_atualizacoes = 0;
    char texto[1 + LCD_colunas] ; // Caracteres do display
	union uBitByteNibble i; // usado em loops
	char dummy;

    InitHW(); // Congiguracoes iniciais do Pic
	InitSystem(); // Configuracoes de dispositivos externos

	/* limpa variaveis */
	MemoryMap.IGN_AVANCO = 0;
	MemoryMap.RPM_RAW = 0;
	MemoryMap.RPM_pointer = 15;
	MemoryMap.Estrategias = 0;

	/* Boas Vindas: */
	Atualiza_Display(Pagina_BoasVindas);
	delay_ms(2000);

	//Animacao dos LEDS
	LED_Start = 0;
	LED_Ign = 0;
	LED_Injetor = 0;
	LED_Bomba = 0;

	for (i.uByte=1; i.uByte<31; i.uByte=i.uByte<<1){
		LED_Start = i.bits.B0;
		LED_Ign = i.bits.B1;
		LED_Bomba = i.bits.B2;
		LED_Injetor = i.bits.B3;
		delay_ms(100);
	}

	// Pronto
	LED_Ign = 1;
	LED_Injetor = 1;
	LED_Bomba = 1;

	/* Aguarda K15 */
	LED_Start = 1;
	Atualiza_Display(Pagina_K15);

	while (!Ign){
		//Aguarda Sinal Linha 15
		LED_Start = !LED_Start;
		delay_ms(300);
	}
	LED_Start = 0;// sempre aceso

	// acerto inicial das variáveis para 700RPM
	MemoryMap.RPM_pointer = 7;
	Calcular_Estrategia();
	Calcular_Avanco_IGN();
	Calcular_Injecao();

	enable_interrupts(int_ext); // habilita interrupcao Externa (apos K15)
	enable_interrupts(int_ext1); // habilita botão LCD troca tela

	// Ligar bomba
	LED_Bomba = 0;
	Bomba = 1;

	//aceita interrupcoes
	enable_interrupts(GLOBAL);

	while (Ign) {
		if(!MemoryMap.Estrategias_CicloCalc){
			MemoryMap.Estrategias_CicloCalc = 200;
			Calcular_Estrategia();
		}

		// Bomba de aceleração
		if(!MemoryMap.TPS_CicloCalc){
			MemoryMap.TPS_CicloCalc = 50;
			Calcular_Aceleracao();
		}

		if (!MemoryMap.LCD_SHOW){
			MemoryMap.LCD_SHOW = 30;
			/* varredura de telas */
			if (LCDpagina_atualizacoes) LCDpagina_atualizacoes--;

			if (!LCDpagina_atualizacoes){
				LCDpagina_atualizacoes = 35;
				//substituido para escolha por botão no lugar de tempo
				//LCDpagina_loop++;
			}

			switch(MemoryMap.LCD_Pagina){
			case 1:
				Atualiza_Display(Pagina_Tela1);
				break;
			case 2:
				Atualiza_Display(Pagina_Tela2);
				break;
			default:
				MemoryMap.LCD_Pagina = 1;
			}
		}

		/* Calculos que dependem da variação da rotação */
		if ( RPMpointer_Anterior != MemoryMap.RPM_pointer){
			RPMpointer_Anterior = MemoryMap.RPM_pointer;
			Calcular_Avanco_IGN();
			Calcular_Injecao();
		}

		/* trick */
		#ASM
			NOP
			NOP
			NOP
		#ENDASM
	}

	/* Sinal K15 = desligado */
	Reset_Prog();

} /* Final da função main */

/**
 * \fn InitHW
 * \brief Funcao de inicializacao com as configuracoes do uControlador
 */
void InitHW(void) {
	/** set all ports as input */
    TRISA = 0XFF;
    TRISB = 0XFF;
    TRISC = 0XFF;
    TRISD = 0XFF;
    TRISE = 0XFF;

	/* desabilita interrupções */
	disable_interrupts(GLOBAL);
	/* desabilita interrupções individualmente */
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
	/* desabilita temporizadores */
	T1CON = 0x00;
	T2CON = 0x00;
	T3CON = 0x00;
	/* AD Converter */
    setup_adc(ADC_OFF);
    setup_adc_ports(NO_ANALOGS);
    setup_psp(PSP_DISABLED); /* Parallel slave port */
    setup_spi(FALSE); /* SPI Disabled */

    setup_comparator(NC_NC_NC_NC); /* Desliga comparadores internos */
    setup_vref(FALSE); /* Tensao de referencia  para o comparador */

	/* agora set o que necessário */
    setup_adc_ports(AN0_TO_AN3); /* ConfiG analogicas: AN0-TPS>>AN1-TEMP */
    setup_adc(ADC_CLOCK_DIV_32); /* Configura divisao do clock ADC */
	enable_interrupts(INT_AD);
    setup_psp(PSP_DISABLED); /* PSP nao utilizado */
    setup_spi(FALSE); /* SPI nao utilizado */

	/* Timer 0 = base de tempo fixo em 100us */
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2);
	set_timer0(TMR_0_Prescaler);
    enable_interrupts(int_timer0);

	/* Timer 1 Tempo de ignicao SPOUT em 100us */
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
	set_timer1(0); /* MAX > 43ms */
	enable_interrupts(int_timer1);

	/* Timer 2 Tempo de injecao
	 * (mode,period,postscale)
	 * mode = T2_DISABLED, T2_DIV_BY_1, T2_DIV_BY_4, T2_DIV_BY_16
	 * period = 0-255 that determines when the clock value is reset
	 * postscale = 1-16 that determines how many timer overflows before
	 * div_16:15 = 20us
	 * 1=20us 5=100us 200=4ms 255=5.1ms
	 */
    setup_timer_2(T2_DIV_BY_16, Padrao_Inj_OffSet, 15);
	//enable_interrupts(int_timer2); no PIP.c

	set_tris_a(0b10111111);
    set_tris_b(0b00000011); // Configuracao de I/O do PORTB
    set_tris_c(0b00000100); // Configuracao de I/O do PORTC
	LCD_tris();
    //set_tris_d(0b00000000); // Configuracao de I/O do PORTD LCD
    //set_tris_e(0b1100); // Configuracao de I/O do PORTE LCD

    output_a(0x00); // Atribui PORT A em nivel logico 0
    output_b(0x00); // Atribui PORT B em nivel logico 0
    output_c(0x00); // Atribui PORT C em nivel logico 0
    //output_d(0x00); // Atribui PORT D em nivel logico 0
    //output_e(0x00); // Atribui PORT E em nivel logico 0

    //set_adc_channel(0); // Seta RA0 como entrada analogica

    //ext_int_edge(0, H_TO_L); // Configura int0 ext c borda descida
	INTEDG0 = 0;
	//enable_interrupts(int_ext); // habilita interrupcoes Externas (apos K15)

    ext_int_edge(1, H_TO_L); // Configura botão LCD int1 ext com borda descida
	
	init_Pinagens();
}

/**
 * \fn InitSystem
 * \brief Inicializa circuitos externos ao microcontrolador
 */
void InitSystem(void) {
	LCD_inicializacao(); // Funcao que inicializa o Display
}

/**
 * \fn Reset_Prog
 * \brief Funcao de reset do programa
 */
void Reset_Prog(void) {
    disable_interrupts(global); // Desabilita interrupcoes Globais
    Reset_CPU();
}

/**
 *  \fn Atualiza_Display(char pagina)
 *  \brief Mostra no display ou uma mensagem estática ou a rotação de duas ou mais
 *  \param [in] pagina referência do que é mostrado no display
 *
 * \anchor Figura_10_5
 * \ref Figuras "Lista de Figuras"
 * \addindex Figura_10_5
 * \image 	latex display01.eps "Aceleração a 2200 RPM e motor a 90C" width=7cm
 * \image 	latex display02.eps "ACT ECT TPS MAP" width=7cm
 *
 * \anchor Figura_10_7
 * \ref Figuras "Lista de Figuras"
 * \addindex Figura_10_7
 * \image 	latex display03.eps "Avanço de ignição a 6000 RPM e motor a 90C" width=7cm
 * \image 	latex display04.eps "ACT ECT TPS MAP" width=7cm
 *
 * \anchor Figura_10_9
 * \ref Figuras "Lista de Figuras"
 * \addindex Figura_10_9
 * \image 	latex display05.eps "Partida a frio 10C e 1 bar" width=7cm
 * \image 	latex display06.eps "ACT ECT TPS MAP" width=7cm
 *
 * \anchor Figura_10_11
 * \ref Figuras "Lista de Figuras"
 * \addindex Figura_10_11
 * \image 	latex display07.eps "Partida a quente a 60C" width=7cm
 * \image 	latex display08.eps "ACT ECT TPS MAP" width=7cm
 *
 */
void Atualiza_Display(char pagina) {
    static char loop_LCD = 0;
	static char pagina_anterior = 0xFF;
	char dummy;
    char texto[1 + LCD_colunas] ; // Caracteres do display

	if(pagina_anterior != pagina){
		pagina_anterior = pagina;

		switch(pagina){
		case Pagina_BoasVindas:
			// Boas Vindas:
			strcpy(texto, LCD_BemVindo_L1);
			LCD_mensagem(LCD_L1, texto);
			strcpy(texto, LCD_BemVindo_L2);
			LCD_mensagem(LCD_L2, texto);
			break;
		case Pagina_K15:
			// Aguarda K15:
			strcpy(texto, LCD_EsperaK15_L1);
			LCD_mensagem(LCD_L1, texto);
			break;
		case Pagina_Tela1:
			// Mascara RPM, Ignição, Temperatura Motor, TPS:
			strcpy(texto, LCD_Mascara_L1);
			LCD_mensagem(LCD_L1, texto);
			strcpy(texto, LCD_Mascara_L2);
			LCD_mensagem(LCD_L2, texto);
			break;
		case Pagina_Tela2:
			// Valores analógicos ACT ECT TPS MAP:
			strcpy(texto, LCD_Mascara_L3);
			LCD_mensagem(LCD_L1, texto);
			strcpy(texto, LCD_Mascara_L4);
			LCD_mensagem(LCD_L2, texto);
			break;
		}
		// atualiza pagina apenas no próximo ciclo
		return;
	}

	if (pagina==Pagina_Tela1){
		switch (loop_LCD){
		case 0:
			dummy = sprintf(texto, "%04lu\0", MemoryMap.RPM);
			LCD_mensagem(LCD_L1 + 0, texto);
			loop_LCD = 1;
			break;
		case 1:
			strcpy(texto,EstrategiaTexto[MemoryMap.Estrategia_ID]);
			LCD_mensagem(LCD_L1 + 8, texto);
			loop_LCD = 2;
			break;
		case 2:
			LongFix_to_ASCII(MemoryMap.INJ, 1, texto);
			LCD_mensagem(LCD_L2 + 2, texto);
			loop_LCD = 3;
			break;
		case 3:
			LongFix_to_ASCII(MemoryMap.IGN_AVANCO, 1, texto);
			LCD_mensagem(LCD_L2 + 10, texto);
			loop_LCD = 0;
			break;
		default:
			loop_LCD = 0;
			break;
		}
	}

	if (pagina==Pagina_Tela2){
		switch (loop_LCD){
		case 0:
			// Air Charger Temperature Sensor
			dummy = sprintf(texto, "%03d", MemoryMap.ACT ); //MemoryMap.ACT
			LCD_mensagem(LCD_L1 + 4, texto);
			loop_LCD = 1;
			break;
		case 1:
			// Engine Coolant Temperature Sensor
			dummy = sprintf(texto, "%03d", MemoryMap.ECT);
			LCD_mensagem(LCD_L1 + 12, texto);
			loop_LCD = 2;
			break;
		case 2:
			// Throttle Position Sensor
			dummy = sprintf(texto, "%03d", MemoryMap.MAP_RAW);//MemoryMap.TPS
			LCD_mensagem(LCD_L2 + 3, texto);
			loop_LCD = 3;
			break;
		case 3:
			// Manifold Absolute Pressure Sensor
			LongFix_to_ASCII(MemoryMap.MAP, 2, texto);
			LCD_mensagem(LCD_L2 + 11, texto);
			loop_LCD = 0;
			break;
		default:
			loop_LCD = 0;
			break;
		}
	}
}
