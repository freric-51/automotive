/**
 * \file	_matriz.h
 * \brief	Mapas de comandos, conversoes AD e mensagens
 *
 * \date \ref s7
 * \todo	Eficiência Volumétrica = f(RPM, TPS)\n
 * 			Injetor = f(Vbat)\n
 * 			Na partida: Injetor = f(ECT)
 * \pre As matrizes são nomeadas M_(onde é usado)_(origem da informação).
 * \pre Matrizes dependentes da rotação têm tamanho preferencial 60.
 * \pre	Contudo RPM16 indica um índice de 0 a 15 sendo obtido pela divisão por 4 do índice RPM.
 * \pre Matrizes dependentes de sinais analógicos têm tamanho 16.
 */

#include "_LCD.h"

/**
 * \name LCD_M
 * @{ 
 * Mascaras usadas no display
 */
#define Pagina_BoasVindas	0
 char LCD_BemVindo_L1 [1 + LCD_colunas]	 = "FATEC 1o 2013   ";
 char LCD_BemVindo_L2 [1 + LCD_colunas]	 = "Gerenciamento I ";
#define Pagina_K15	1
 char LCD_EsperaK15_L1[1 + LCD_colunas]	 = "Aguarda Ignicao ";
#define Pagina_Tela1	2
 char LCD_Mascara_L1  [1 + LCD_colunas]	 = "____RPM         ";
 char LCD_Mascara_L2  [1 + LCD_colunas]	 = "I=__,_msA=__,_ms";
#define Pagina_Tela2	3
 char LCD_Mascara_L3  [1 + LCD_colunas]	 = "ACT___ cECT___ c";
 char LCD_Mascara_L4  [1 + LCD_colunas]	 = "TPS___% MAP___ b";
/** @} */

/**
 *  \var M_MAP_ANA[16]
 *  Matriz que relaciona Conversor Frequência Tensão para o sensor MAP (\ref Figura_10_1)\n
 *  conforme : \n
 *  - 160 Hz = x.xxV = 0.00 bar \n
 *  - 135 Hz = 3.70V = 0.79 bar \n
 *  - 122 Hz = 3.52V = 0.54 bar \n
 *  - 77 Hz  = 0.75V = 1.00 bar \n
 *  - bar = 1.1 - AD * 0.46/143 \n
 */
byte  M_MAP_ANA [16] =
{
	43,49,54,59,64,69,74,79,
	85,90,95,100,101,102,103,104
};

/**
 *  \var M_TPS_ANA
 *   Matriz de linearização do sinal TPS
 */
byte  M_TPS_ANA [16] =
{
	0,	6,	13,	20,	26,	33,	40,	46,
	53,	60,	66,	73,	80,	86,	93,	100
};

/**
 *  \var M_ECT_ANA[16]
 *   Temperatura do motor (água de arrefecimento) em graus Celsius
 */
byte  M_ECT_ANA [16] =
{
	-30,	-20,	-10,	0,		10,		20,		30,		40,
	50,		60,		70,		80,		90,		100,	110,	120
};

/**
 *  \var M_TPS_ACELERACAO_TPS_ECT[16][16]
 *  TPS_ACELERACAO depende da variação de TPS[16] e de ECT[16] \n
 *  M_TPS_ACELERACAO_TPS_ECT = f(TPS,ECT) \n
 *  onde 1=20us, 100=2ms, 255[max]=5.1ms
 */
byte  M_TPS_ACELERACAO_TPS_ECT [16][16] =
{
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{37, 31, 26, 21, 16, 12, 8, 7, 7, 6, 5, 4, 3, 3, 2, 2},
	{41, 35, 30, 25, 21, 17, 14, 11, 8, 7, 6, 5, 4, 3, 2, 2},
	{43, 37, 31, 26, 22, 18, 15, 12, 9, 8, 7, 6, 5, 4, 3, 2},
	{46, 40, 34, 28, 24, 20, 17, 13, 10, 9, 8, 7, 6, 5, 4, 3},
	{50, 44, 38, 32, 28, 24, 21, 17, 14, 12, 11, 10, 9, 8, 6, 5},
	{55, 52, 49, 46, 40, 34, 29, 25, 22, 20, 18, 17, 15, 14, 11, 8},
	{60, 57, 55, 53, 47, 42, 37, 33, 30, 28, 26, 25, 23, 22, 18, 14},
	{69, 68, 67, 67, 60, 54, 48, 46, 45, 42, 39, 37, 34, 32, 26, 21},
	{100, 99, 98, 98, 92, 86, 80, 70, 60, 58, 56, 54, 52, 51, 45, 39},
	{108, 107, 106, 106, 101, 96, 91, 83, 75, 73, 71, 70, 68, 67, 62, 57},
	{116, 116, 115, 115, 111, 107, 103, 96, 90, 88, 87, 86, 84, 84, 80, 76},
	{125, 124, 124, 124, 121, 118, 115, 110, 105, 104, 103, 102, 101, 100, 97, 94},
	{133, 133, 132, 132, 130, 128, 126, 123, 120, 119, 118, 118, 117, 117, 115, 113},
	{141, 141, 141, 141, 140, 139, 138, 136, 135, 134, 134, 134, 133, 133, 132, 131},
	{150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}
};

/**
 * \var M_IgnAvanco_RPM16_TPS[16][16]
 * Mapa de avanco baseado na rotação e na carga: \n
 * - indice: 0 [100 RPM] a 15 [6000 RPM], neste caso, o indice RPM16 = [RPM_pointer]/4 \n
 * - indice: 0 [1.05 bar] a 15 [0.28 bar] \n
 * - valor: 1 = 100us, 10 = 1ms
 */
byte  M_IgnAvanco_RPM16_TPS[16][16] =
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
	{14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14},
	{14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
	{14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14},
	{13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
	{12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
	{11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9}
};
