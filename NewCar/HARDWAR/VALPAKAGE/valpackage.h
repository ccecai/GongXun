/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    
  * @brief   用于发送或者接收数据包
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VAL_PACKAGE_H
#define __VAL_PACKAGE_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines --------------------------------------------------------------------*/

/** @defgroup TX_Group
 *  @brief    
 */
#define TX_BOOL_NUM  0  
#define TX_BYTE_NUM  0 
#define TX_SHORT_NUM 0  
#define TX_INT_NUM   0  
#define TX_FLOAT_NUM 5 

/** @defgroup RX_Group
 *  @brief    
 */
#define RX_BOOL_NUM  0
#define RX_BYTE_NUM  1
#define RX_SHORT_NUM 0
#define RX_INT_NUM   0
#define RX_FLOAT_NUM 5
/* Exported types ------------------------------------------------------------*/

typedef struct   
{	
	//			tx_buf.bools[0] = rx_buf.bools[0] & 0x01;
	//		  tx_buf.bools[1] = rx_buf.bools[0] & 0x02;
	#if TX_BOOL_NUM > 0
	unsigned char bools[TX_BOOL_NUM]; // 需要上位机优化
	#endif

	#if TX_BYTE_NUM > 0
  char bytes[TX_BYTE_NUM];
  #endif
	
	#if TX_SHORT_NUM > 0
	short shorts[TX_SHORT_NUM];	
	#endif
	
	#if TX_INT_NUM > 0
	int  integers[TX_INT_NUM];
	#endif
	
	#if TX_FLOAT_NUM > 0
	float floats[TX_FLOAT_NUM];
	#endif
	char space; // 无意义，只为了不让结构体为空，结构体为空会报错。
}TX_PackTypeDef;

typedef struct 
{	
	#if RX_BOOL_NUM > 0
	unsigned char bools[(RX_BOOL_NUM+7)>>3];
	#endif
	
	#if RX_BYTE_NUM > 0
  char bytes[RX_BYTE_NUM];
  #endif
	
	#if RX_SHORT_NUM > 0
	short shorts[RX_SHORT_NUM];	
	#endif
	
	#if RX_INT_NUM > 0
	int  integers[RX_INT_NUM];
	#endif
	
	#if RX_FLOAT_NUM > 0
	float floats[RX_FLOAT_NUM];
	#endif
	char space; // 无意义，只为了不让结构体为空，结构体为空会报错。
}RX_PackTypeDef;
/* Functions --------------------------------------------------------------------*/
uint8_t VALPACK_ReadAll(RX_PackTypeDef *rx_pack_ptr);
void VALPACK_SendAll(TX_PackTypeDef *tx_pack_ptr);
void VALPACK_Init(void);
RX_PackTypeDef VALPACK_GetDatas(void);
void VALPACK_Loop(void);
#endif 

/*************MADE BY LIU****************END OF FILE****/



