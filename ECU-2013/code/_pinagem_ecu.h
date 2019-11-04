/**
 * \file _pinagem_ecu.h
 * \brief Arquivo de Configuracao e prototipo de _pinagem_ecu.c
 *
 * \latexonly
 * 	\begin{table}
 *		\caption{Pinagem do PIC 18F4550 e uso no projeto} % title of Table
 * 		\begin{tabular}{cllll}
 *		\cline{1-5}
 * 		pino & Funções & Utilizado & ligação & comentário \\
 * 		1 & MCLR/VPP/RE3 & MCLR &  & Circuito de reset \\
 * 		2 & RA0/AN0 & AN0 &  & Sensor TPS \\
 * 		3 & RA1/AN1 & AN1 &  & Sensot ACT \\
 * 		4 & RA2/AN2/VREF-/CVREF & AN2 &  & Sensor MAP após conversor FxV \\
 * 		5 & RA3/AN3/VREF+ & AN3 &  & Sensor ECT \\
 * 		6 & RA4/T0CKI/C1OUT/RCV & RA4 &  & IGN – Linha 15 \\
 * 		7 & RA5/AN4/SS/HLVDIN/C2OUT & AN4 & VBat & Tensão da Bateria \\
 * 		8 & RE0/AN5/CK1SPP & RE0 & LCD CMD & LCD RS (4) \\
 * 		9 & RE1/AN6/CK2SPP & RE1 & LCD CMD & LCD E (6) \\
 * 		10 & RE2/AN7/OESPP &  &  &  \\
 * 		11 & VDD & VDD & +5V & Alimentação \\
 * 		12 & VSS & VSS & 0V & Alimentação \\
 * 		13 & OSC1/CLKI & OSC1 & XTAL & Cristal 20 Mhz \\
 * 		14 & OSC2/CLKO/RA6 & OSC2 & XTAL & Cristal 20 Mhz \\
 * 		15 & RC0/T1OSO/T13CKI & RC0 & LED & Indicador Ignicao \\
 * 		16 & RC1/T1OSI/CCP2/UOE & RC1 & LED & Indicador de inicio de programa \\
 * 		17 & RC2/CCP1/P1A &  &  &  \\
 * 		18 & VUSB &  &  &  \\
 * 		19 & RD0/SPP0 & RD0 & LCD Data & LCD D0 (7) \\
 * 		20 & RD1/SPP1 & RD1 & LCD Data & LCD D1 (8) \\
 * 		21 & RD2/SPP2 & RD2 & LCD Data & LCD D2 (9) \\
 * 		22 & RD3/SPP3 & RD3 & LCD Data & LCD D3 (10) \\
 * 		23 & RC4/D-/VM &  &  &  \\
 * 		24 & RC5/D+/VP &  &  &  \\
 * 		25 & RC6/TX/CK & RC6 & LED & Indicador Injecao \\
 * 		26 & RC7/RX/DT/SDO & RC7 & LED & Indicador Bomba \\
 * 		27 & RD4/SPP4 & RD4 & LCD Data & LCD D4 (11) \\
 * 		28 & RD5/SPP5/P1B & RD5 & LCD Data & LCD D5 (12) \\
 * 		29 & RD6/SPP6/P1C & RD6 & LCD Data & LCD D6 (13) \\
 * 		30 & RD7/SPP7/P1D & RD7 & LCD Data & LCD D7 (14) \\
 * 		31 & VSS & VSS & 0V & Alimentação \\
 * 		32 & VDD & VDD & +5V & Alimentação \\
 * 		33 & RB0/AN12/INT0/FLT0/SDI/SDA & INT0 & HALL & Rotação Motor \\
 * 		34 & RB1/AN10/INT1/SCK/SCL & INT1 & LCD & Troca Tela no Display \\
 * 		35 & RB2/AN8/INT2/VMO & RB2 &  & saida para bomba \\
 * 		36 & RB3/AN9/CCP2(1)/VPO & RB3 &  & saida para valvula injetora \\
 * 		37 & RB4/AN11/KBI0/CSSPP & RB4 & Spout & saida para o modulo de ignicao \\
 * 		38 & RB5/KBI1/PGM &  &  &  \\
 * 		39 & RB6/KBI2/PGC & PGC &  & Programação In-Circuit \\
 * 		40 & RB7/KBI3/PGD & PGD &  & Programação In-Circuit \\
 *		\cline{1-5}
 * 		\end{tabular}
 *		\label{table:18F4550pin} % is used to refer this table in the text
 * 	\end{table}
 *
 *  \ref tables "Lista de Tabelas"
 * \endlatexonly
 *
 */

/**
 * \defgroup gPin Funcao dos pinos do PIC
 * @{ */

 /**
 * \name MainStart
 * @{
 */
#define LED_Start		LATC1	/**< \brief Indicador de inicio de programa */
/** @} */

/**
 * \name Ignicao
 * @{
 */
#define SPout           LATB4	/**< \brief saida para o modulo de ignicao */
#define LED_Ign         LATC0	/**< \brief Indicador Ignicao */
#define Ign             RA4		/**< \brief Linha 15 - K15 */
/** @} */

/**
 * \name Injecao
 * @{
 */
#define LED_Injetor     LATC6	/**< \brief Indicador Injecao */
#define Injetor_out     LATB3	/**< \brief saida para valvula injetora */
#define LED_Bomba       LATC7	/**< \brief Indicador Bomba */
#define Bomba           LATb2	/**< \brief saida para bomba */
/** @} */

/**
 * \name LCD_D
 * @{
 * LCD Barramento de dados
 */
#define LCD_PORTA_DADOS output_D
#define LCD_PIN_0 pin_d0
#define LCD_PIN_1 pin_d1
#define LCD_PIN_2 pin_d2
#define LCD_PIN_3 pin_d3
#define LCD_PIN_4 pin_d4
#define LCD_PIN_5 pin_d5
#define LCD_PIN_6 pin_d6
#define LCD_PIN_7 pin_d7
/** @} */

/**
 * \name LCD_C
 * @{
 * LCD Linhas de comando
 */
#define LCD_RS pin_E0
#define LCD_EN pin_E1
#define LCD_Botao RB1
/** @} */

/**
 * \name TPS
 * @{
 */
#define TPSpin     		pin_a0 /**< \brief pinagem usada no TPS */
/** @} */

/**
 * \name MAP
 * @{
 */
#define MAPpin     		pin_a2 /**< \brief pinagem usada no MAP */
/** @} */

/**
 * \name Portas
 * @{
 */
void init_Pinagens(void);
/** @} */

/**
 * @}
 * fim do grupo gPin
 */