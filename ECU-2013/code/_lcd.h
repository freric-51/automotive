/**
 * \file _lcd.h
 * \brief prototipos para _lcd.c
 */

/**
 *  \name LCD162
 *  @{
 *   LCD 16x2
 */
#define LCD_colunas 16 	/**<  \brief Quantidade de colunas do display */
#define LCD_linhas 2 	/**<  \brief Quantidade de linhas do display */
#define LCD_L1   0x80 	/**<  \brief Endereco Base da primeira linha do display */
#define LCD_L2   0xC0 	/**<  \brief Endereco Base da segunda linha do display */
#define LCD_DataWidth	8 	/**<  \brief Tamanho do barramento de dados do display */
/** @} */

//prototipos das funcoes
void LCD_inicializacao(void);
void LCD_mensagem(unsigned char, unsigned char*);
void LCD_tris(void);
