/**
 *  \file _MemoryMap.h
 *  \brief Memoria usada para transferencia de informacao entre os modulos do programa
 *  \date \ref s7
 */

/**
 * \union uBitByteNibble
 * \brief Acesso a Byte/Nibble/Bits de uma posição de memória
 *
 *	Sintaxe de uso da UNION		\n
 *	- definicao de variaveis	\n
 *  	-# union uBitByteNibble IOA, IOB;	\n
 *  - Leitura e Escrita:		\n
 *  	-# IOA.uByte = PORTA;	\n
 *  	-# x = IOA.bits.b0;		\n
 *  	-# PORTA = IOA.uByte;	\n
 */
union uBitByteNibble {
    unsigned int uByte; /**< agrupamento da estrutura Byte/Nibble/Bits */

    struct {
        unsigned int LSB : 4;
        unsigned int MSB : 4;
    } Nibble; /**< agrupamento de 4 bits */

    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;
    } bits; /**< agrupamento de 8 bits */
};

/**
 *  \name Modos
 *  @{
 *  \details - Modos de Operacao \n
 * -# Partida do motor para 300 RPM. \n
 * -# Aquecimento \n
 * -# Marcha lenta \n
 * -# Cargas parciais \n
 * -# Plena carga \n
 * -# Aceleração rápida \n
 * -# Desaceleração \n
 *  \author Fabrício José Pacheco Pujatti
 */
#define MOp_PartidaFrio 			MemoryMap.Estrategias.bits.b0
#define MOp_PartidaQuente 			MemoryMap.Estrategias.bits.b1
#define MOp_Aquecimento 			MemoryMap.Estrategias.bits.b2
#define MOp_MarchaLenta 			MemoryMap.Estrategias.bits.b3
#define MOp_CargaParcial		 	MemoryMap.Estrategias.bits.b4
#define MOp_PlenaCarga			 	MemoryMap.Estrategias.bits.b5
#define MOp_AceleracaoRapida		MemoryMap.Estrategias.bits.b6
#define MOp_Desaceleracao 			MemoryMap.Estrategias.bits.b7
#define MOp 						MemoryMap.Estrategias.uByte

 int EstrategiaTexto [8][9]={
	"Pt Frio ",	"PtQuente",	"Aquecimt",	"Mr Lenta",
	"CParcial",	"Plena Cg",	"Acelera ",	"Desacel "
};
/** @} */

/**
 *  \struct strMemoryMap
 *  \brief Estrutura do Mapa de Memoria
 */
 struct strMemoryMap{

	/**
	 *  \name TPS
	 *  @{
	 *  Throttle Position Sensor
	 */
					int TPS;
					int TPS_RAW;
		unsigned 	int	TPS_pointer;
					int	TPS_Port;
					int	TPS_Anterior;
					int	TPS_Aceleracao;
		unsigned 	int	TPS_CicloCalc;
	/** @} */

	/**
	 *  \name RPM
	 *  @{
	 *  Rotational Speed Sensor
	 */
	 	unsigned long int 	RPM;
		unsigned long int	RPM_RAW;
		unsigned int		RPM_pointer;
	/** @} */

	/**
	 *  \name MAP
	 *  @{
	 *  Manifold Absolute Pressure Sensor
	 */
		long int	 	MAP;
			int 		MAP_RAW;
		unsigned int	MAP_pointer;
			int			MAP_Port;
	/** @} */

	/**
	 *  \name ACT
	 *  @{
	 *  Air Charger Temperature Sensor
	 */
				int		ACT;
				int 	ACT_RAW;
		unsigned int	ACT_pointer;
				int		ACT_Port;
	/** @} */

	/**
	 *  \name ECT
	 *  @{
	 *  Engine Coolant Temperature Sensor
	 */
				int 	ECT;
				int 	ECT_RAW;
		unsigned int	ECT_pointer;
				int		ECT_Port;
	/** @} */

	/**
	 *  \name PIP
	 *  @{
	 *  Profile Ignition Pickup Sensor
	 */
		unsigned long int	PIP_T_TOT;
		unsigned long int	PIP_TH;
		unsigned long int	PIP_TL;
		unsigned long int	IGN_AVANCO;
		unsigned long int	IGN_T1_set;
		unsigned	  int1 	PIP_SimNao;
		unsigned	  int1	PIP_SimNaoComandar;
	/** @} */

	/**
	 *  \name INJ
	 *  @{
	 *  Engine Tempo Injetor
	 */
		unsigned long int	INJ_T2_set;
		unsigned long int	INJ;
	/** @} */

	/**
	 *  \name LCD
	 *  @{
	 *  LCD update
	 */
		int	LCD_SHOW;
		int LCD_Pagina;
	/** @} */

	/**
	 *  \name ANA
	 *  @{
	 *  Analogics update
	 */
		unsigned int	ANA_UpdateCicle;
	/** @} */

	/**
	 *  \name Lista
	 *  @{
	 *  Lista de todas as estrategias de funcionamento
	 */
					union 	uBitByteNibble		Estrategias;
					int 	Estrategia_ID;
		unsigned 	int		Estrategias_CicloCalc;
	/** @} */
 } ;

struct strMemoryMap	MemoryMap; /**<  \brief Variável com as informações comums */

/**
 *  \name Const
 *  @{
 *  Constantes de funcionamento dos componentes
 */
	/**
	 *  \var Padrao_Inj_OffSet
	 *  1=20us 100=2ms 255[max]=5.1ms
	 */
	 int Padrao_Inj_OffSet = 160;
/** @} */
