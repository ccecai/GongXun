/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    04-August-2014
  * @brief  
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "valpackage.h"
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/
/** 
  * @brief   XXX Init structure definition  
  */ 
typedef struct
{
	uint64_t 	rx_index;
	uint64_t 	read_index;
	uint8_t 	read_flag;
	uint16_t 	read_temp_index;
	uint8_t  	read_sum;
}VALP_RXStatusTypeDef;

typedef struct
{
	uint16_t 	data_index;
	uint8_t		send_sum;
}VALP_TXStatusTypeDef;;
/* Private define ------------------------------------------------------------*/ 
// 一般需要512字节以上，需要根据实际接收数据的速度和proc函数的频率考虑
#define VALPACK_BUFFER_SIZE 2048
#define PACK_HEAD 0xa5   
#define PACK_TAIL 0x5a
const uint32_t VALUEPACK_INDEX_RANGE = VALPACK_BUFFER_SIZE<<3; //除以字节的大小，得到字节长度

// bool变量实际至少占用一个char的空间，只有超过8个时才会增加空间
const uint16_t TXPACK_BYTE_SIZE = ((TX_BOOL_NUM+7)>>3) + TX_BYTE_NUM + (TX_SHORT_NUM<<1) + (TX_INT_NUM<<2) + (TX_FLOAT_NUM<<2);
const uint16_t RXPACK_BYTE_SIZE = ((RX_BOOL_NUM+7)>>3) + RX_BYTE_NUM + (RX_SHORT_NUM<<1) + (RX_INT_NUM<<2) + (RX_FLOAT_NUM<<2);
/*----------- 包结构 0xa5 bool byte short int float sum 0x5a ---------------*/
const uint16_t RXPACK_LENGTH = RXPACK_BYTE_SIZE+3; // 加上包头包尾与校验和
/* Global param ------------------------------------------------------------*/
//rx与tx的buf
uint8_t valpack_rxbuff[VALPACK_BUFFER_SIZE];
uint8_t valpack_txbuff[TXPACK_BYTE_SIZE + 3];
VALP_RXStatusTypeDef rx_status_t;
VALP_TXStatusTypeDef tx_status_t;
RX_PackTypeDef rx_datas_t;
TX_PackTypeDef tx_datas_t;
unsigned int err=0;
uint8_t bits[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

/* Private functions --------------------------------------------------------*/
void VALP_SendBuffer(unsigned char *p, unsigned short length)
{
	int i;
	for(i=0;i<length;i++)
	{
		USART_SendData(USART2, *p++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
	}
}
/* Functions ------------------------------------------------------------------*/

/**
  * @brief  初始化包状态
  * @param  None
  * @retval uint8_t 成功返回1
  */
void VALPACK_Init(void)
{
	rx_status_t.read_flag 			= 0;
	rx_status_t.read_index      = 0;
	rx_status_t.read_sum        = 0;
	rx_status_t.read_temp_index = 0;
	rx_status_t.rx_index        = 0;
	tx_status_t.data_index      = 0;
	tx_status_t.send_sum        = 0;
}

/**
  * @brief  读一次所有数据并解析
  * @param  None
  * @retval uint8_t 成功返回1
  */
uint8_t VALPACK_ReadAll(RX_PackTypeDef *rx_pack_ptr)
{
	//uint16_t 	temp_index 	= 0;//这个应该也是全局变量
	uint16_t 	data_index 	= 0; // 从存有数据的地方开始的索引
	uint16_t 	data_length = 0;
	uint16_t 	loop_index; // 用于循环
	uint32_t 	read_ptr;
	rx_status_t.read_flag = 0;// 0 0； 2 1no; 2 2no 2 3; 2 4ok；ok 
	//USART_SendData(USART2, 'o');
	while(rx_status_t.read_index < (rx_status_t.rx_index - ((RXPACK_LENGTH)*2)))
		rx_status_t.read_index += RXPACK_LENGTH; // 每一个length都是完整的一个包，不解析前面累计的
	// 需要有一个包长才开始解析
	while(rx_status_t.read_index <= (rx_status_t.rx_index - RXPACK_LENGTH))
	{
		//printf("%lld\n", rx_status_t.read_index);
		rx_status_t.read_temp_index = rx_status_t.read_index % VALPACK_BUFFER_SIZE; // 超出BUFFER, 中断会自动覆盖从头开始
		data_index = rx_status_t.read_temp_index+1;
		if(valpack_rxbuff[rx_status_t.read_temp_index] == PACK_HEAD) // 检测到包头
		{
			//USART_SendData(USART2, 'o');
			if(valpack_rxbuff[(rx_status_t.read_temp_index + RXPACK_BYTE_SIZE + 2) % VALPACK_BUFFER_SIZE] == PACK_TAIL) // 检测到包尾
			{
				// 计算校验和, 从1开始才是数据
				rx_status_t.read_sum=0;
				for(loop_index = 0; loop_index < RXPACK_BYTE_SIZE; loop_index++)
				{
					rx_status_t.read_temp_index++;
					if(rx_status_t.read_temp_index >= VALPACK_BUFFER_SIZE)
						rx_status_t.read_temp_index -= VALPACK_BUFFER_SIZE;
					rx_status_t.read_sum += valpack_rxbuff[rx_status_t.read_temp_index];
				}
				rx_status_t.read_temp_index++;
				if(rx_status_t.read_temp_index >= VALPACK_BUFFER_SIZE)
					rx_status_t.read_temp_index -= VALPACK_BUFFER_SIZE; //或者取余
				
				if(rx_status_t.read_sum == valpack_rxbuff[rx_status_t.read_temp_index]) // 校验和
				{
					// 提取数据包数据 一共有五步
					// 保证一个字节一个读，不会一个类型一个类型读，上位机发送bool类型满8个多一个字节
					// 处理都是以字节为单位，在结构体中自动组合成对应的类型
					// 1. bool
					#if RX_BOOL_NUM>0
					read_ptr = (uint32_t)rx_pack_ptr->bools;//指向bool起始地址
					data_length = (RX_BOOL_NUM + 7)>>3; 
					for(loop_index = 0; loop_index < data_length; loop_index++)
					{
						if(data_index >= VALPACK_BUFFER_SIZE)
							data_index -= VALPACK_BUFFER_SIZE;
						(*((unsigned char *)read_ptr))= valpack_rxbuff[data_index];
						data_index++;
						read_ptr++;
					}
					#endif
					
					// 2.byte
					#if RX_BYTE_NUM>0
					read_ptr = (uint32_t)(rx_pack_ptr->bytes);
					data_length = RX_BYTE_NUM;
					for(loop_index = 0; loop_index<data_length; loop_index++)
					{
						if(data_index >= VALPACK_BUFFER_SIZE)
							data_index -= VALPACK_BUFFER_SIZE;
						(*((unsigned char *)read_ptr)) = valpack_rxbuff[data_index];//直接指向，所以bool包的大小并不影响后面
						data_index++;
						read_ptr++;
					}
					#endif
					// 3.short
					#if RX_SHORT_NUM>0
					read_ptr = (uint32_t)(rx_pack_ptr->shorts);
					data_length = RX_SHORT_NUM<<1;
					for(loop_index=0; loop_index<data_length; loop_index++)
					{
						if(data_index >= VALPACK_BUFFER_SIZE)
							data_index -= VALPACK_BUFFER_SIZE;
						(*((unsigned char *)read_ptr))= valpack_rxbuff[data_index];
						data_index++;
						read_ptr++;
					}
					#endif
					// 4.int
					#if RX_INT_NUM>0
					read_ptr = (uint32_t)(&(rx_pack_ptr->integers[0]));
					data_length = RX_INT_NUM<<2;
					for(loop_index = 0; loop_index<data_length; loop_index++)
					{
						if(data_index >= VALPACK_BUFFER_SIZE)
							data_index -= VALPACK_BUFFER_SIZE;
						(*((unsigned char *)read_ptr))= valpack_rxbuff[data_index];
						data_index++;
						read_ptr++;
					}
					#endif
					// 5.float
					#if RX_FLOAT_NUM>0
					read_ptr = (uint32_t)(&(rx_pack_ptr->floats[0]));
					data_length = RX_FLOAT_NUM<<2;
					for(loop_index=0; loop_index<data_length; loop_index++)
					{
						if(data_index >= VALPACK_BUFFER_SIZE)
						data_index -= VALPACK_BUFFER_SIZE;
						(*((unsigned char *)read_ptr)) = valpack_rxbuff[data_index];
						data_index++;
						read_ptr++;
					}
					#endif
					err = data_index;
					rx_status_t.read_index += RXPACK_LENGTH;
					rx_status_t.read_flag = 1;
				}
				else
				{
					rx_status_t.read_index++;
					err++;
				}
			}
			else
			{
				rx_status_t.read_index++;
				err++;
			}		
		}
		else
		{
			rx_status_t.read_index++;
			err++;
		}
	}
	//USART_Printf(USART2, "%d\n", rx_status_t.read_index);
	return rx_status_t.read_flag;
}

/**
  * @brief  在循环中更新放到rx_datas_t，测试不能在定时器中断中使用
  * @param  None
  * @retval None
  */
void VALPACK_Loop(void)
{
		if (VALPACK_ReadAll(&rx_datas_t))
		{
			tx_datas_t.floats[0] = rx_datas_t.floats[0];
			tx_datas_t.floats[1] = rx_datas_t.floats[1];
			tx_datas_t.floats[2] = rx_datas_t.floats[2];
			tx_datas_t.floats[3] = rx_datas_t.floats[3];
			tx_datas_t.floats[4] = rx_datas_t.floats[4];
			VALPACK_SendAll(&tx_datas_t);
		}	
}

/**
  * @brief  外部函数获取包值
  * @param  None
  * @retval rx_datas_t
  */
RX_PackTypeDef VALPACK_GetDatas(void)
{
	return rx_datas_t;
}

/**
* @brief  一次性发送包值
  * @param  TX_PackTypeDef *tx_pack_ptr
  * @retval None
  */
void VALPACK_SendAll(TX_PackTypeDef *tx_pack_ptr)
{
	int i;
	uint8_t data_bit_index = 0;
	uint8_t data_index = 1;
	uint16_t loop_index;
	valpack_txbuff[0] = PACK_HEAD;
	tx_status_t.send_sum = 0;

  #if TX_BOOL_NUM > 0 
	for(loop_index = 0; loop_index < TX_BOOL_NUM; loop_index++)
	{
		if(tx_pack_ptr->bools[loop_index] ) // 此处实际直接读取了一个字节的内容，需要修改
		//发现bug，上位机并没有正确解析bool变量
		//if((tx_pack_ptr->bools[loop_index] >> loop_index) & 0x01) // 0011 >> 0 & 0x01=1 >>3 = 0
			valpack_txbuff[data_index] |= 0x01<<data_bit_index;
		else
			valpack_txbuff[data_index] &= ~(0x01<<data_bit_index);
		data_bit_index++;
		if(data_bit_index >= 8)
		{
			data_bit_index = 0;
			data_index++; // bit存满一个byte后再往后
		}
	}
	data_index++;
	#endif

	#if TX_BYTE_NUM > 0
	for(loop_index = 0; loop_index < TX_BYTE_NUM; loop_index++)
	{
		valpack_txbuff[data_index++] = tx_pack_ptr->bytes[loop_index];
	}
	#endif
	
	#if TX_SHORT_NUM > 0 
	for(loop_index = 0; loop_index < TX_SHORT_NUM; loop_index++)
	{
		valpack_txbuff[data_index++] = (tx_pack_ptr->shorts[loop_index]) 		&0xff;//先存低8位
		valpack_txbuff[data_index++] = (tx_pack_ptr->shorts[loop_index]>>8)	&0xff;	//存放高8位
	}
	#endif

	#if TX_INT_NUM > 0
	for(loop_index = 0; loop_index < TX_INT_NUM; loop_index++)
	{	
		valpack_txbuff[data_index++] = (tx_pack_ptr->integers[loop_index])		&0xff;
		valpack_txbuff[data_index++] = (tx_pack_ptr->integers[loop_index]>>8)	&0xff;
		valpack_txbuff[data_index++] = (tx_pack_ptr->integers[loop_index]>>16)&0xff;
		valpack_txbuff[data_index++] = (tx_pack_ptr->integers[loop_index]>>24)&0xff;
	}
	#endif
	
	#if TX_FLOAT_NUM > 0   
	for(loop_index = 0; loop_index < TX_FLOAT_NUM; loop_index++)
	{
		i = *(int *)(&(tx_pack_ptr->floats[loop_index]));//将float转化成int
		
		valpack_txbuff[data_index++] = 	i     &0xff;
		valpack_txbuff[data_index++] = 	(i>>8)&0xff;
		valpack_txbuff[data_index++] =	(i>>16)&0xff;
		valpack_txbuff[data_index++] = 	(i>>24)&0xff;
	}
	#endif
	
	for(loop_index=1; loop_index <= TXPACK_BYTE_SIZE; loop_index++)
		 tx_status_t.send_sum += valpack_txbuff[loop_index];
	valpack_txbuff[TXPACK_BYTE_SIZE+1] = tx_status_t.send_sum;
	valpack_txbuff[TXPACK_BYTE_SIZE+2] = PACK_TAIL;
	VALP_SendBuffer(valpack_txbuff, TXPACK_BYTE_SIZE+3);
}

/**
  * @brief  定时器2接收中断，更新包值
  * @param  None
  * @retval None
  */
static uint16_t vp_circle_rx_index = 0;
//void USART2_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE)) 
//	{
//		valpack_rxbuff[vp_circle_rx_index] = USART2->DR;
//		//USART_SendData(USART2, USART2->DR);
//		vp_circle_rx_index++;
//		if(vp_circle_rx_index >= VALPACK_BUFFER_SIZE)
//			vp_circle_rx_index = 0; // 溢出了就会从头开始占据空间
//		rx_status_t.rx_index++; // 中断给出rx的index
//	}
//	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//}

/**
  * @brief  测试包函数
  * @param  None
  * @retval None
  */
void VALPACK_Debug(void)
{
	
}
