/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    2021/10/18
  * @brief   使用串口1与机械臂通信，有printf重映射很方便
  *			 串口2与陀螺仪通信，只需要接收陀螺仪数据
  * 		 串口3与树莓派通信，需要收发
  ******************************************************************************
  * @attention 数据协议
  * 		串口1
  *			串口2 接收，看手册或者中断函数
  *			串口3 0X55(帧头) 0X3? 0X
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COM_OTHER_H
#define __COM_OTHER_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines --------------------------------------------------------------------*/
#define SACN_CODE_MODE		0X33
#define COLOR_DECT_MODE		0X30
#define CIRCLE_DECT_MODE	3
#define NONE_MODE			0x35
#define FRAME_HEADER '1'
#define START_FLAG 'a'
#define MOVE_YL_FLAG 'b'
#define MOVE_CJ_FLAG 'c'
#define MOVE_ZC_FLAG 'd'
#define REMOVE_YL_FLAG 'e'
#define REMOVE_CJ_FLAG 'f'
#define REMOVE_ZC_FLAG 'g'

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint64_t 	rx_index;
	uint64_t 	read_index;
	uint8_t 	read_flag;
	uint16_t 	read_temp_index;
	uint8_t  	read_sum;
}COM_RXStatusTypeDef;

typedef struct
{
	uint16_t 	data_index;
	uint8_t		send_sum;
}COM_TXStatusTypeDef;


/* Functions --------------------------------------------------------------------*/

void COM_PackInit(void);
void COM_PCInit(uint32_t bound);
void COM_Debug(void);
void COM_ChooseMode(uint8_t mode);
void COM_SendAll(uint8_t *tx_data_ptr);
uint8_t * COM_GetRXData(void);
uint8_t COM_GetColorOrder(uint8_t * order);
uint8_t COM_GetCodeOrder(uint8_t * codeorder);
void usart3_send(u8 data);
extern int16_t pos_error[3];
extern uint8_t rx_data[7];
extern uint8_t start_flag,Jiaqu_start_flag,start_flag_pos,stop_read_flag,pos_judge_flag;
extern int8_t com_rxbuff[16];
extern int Pos_x,Pos_y;
#endif 

/*************MADE BY LIU****************END OF FILE****/
