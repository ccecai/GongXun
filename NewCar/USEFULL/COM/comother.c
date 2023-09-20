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
#include "comother.h"
#include "runrace.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"
extern void RACE_Position0(void);
extern void RACE_Position1(void);
extern void RACE_Position2(void);
extern void RACE_Position3(void);

extern void RACE_RePosition1(void);
extern void RACE_RePosition2(void);
extern void RACE_RePosition3(void);

/* Private typedef -----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/ 

#define PACK_HEADER 0X55
#define CODE_MODE   0X33
#define COLOR_MODE	0X30
//#define NONE_MODE	0X34
uint8_t res_i = 0;
#define FRAME_HEADER '1' // 存放帧头 代表开始接受数据
#define START_FLAG 'a' //开始运动到二维码 标志位
#define MOVE_YL_FLAG 'b'//第一次去往原料区标志位
#define MOVE_CJ_FLAG 'c'//第一次去粗加工区标志位
#define MOVE_ZC_FLAG 'd'//第一次去暂存区标志位
#define REMOVE_YL_FLAG 'e'//第二次去原料区标志位
#define REMOVE_CJ_FLAG 'f'//第二次去粗加工区标志位
#define REMOVE_ZC_FLAG 'g'//第二次去暂存区标志位

const uint16_t RXALL_LENGTH = 9;
#define RXDATA_LENGTH 7
#define TXPACK_LENGTH 9
#define COM_BUFFER_SIZE 450
/* Global param -------------------------------------------------------------*/
int8_t com_rxbuff[16];
static uint8_t com_txbuff[TXPACK_LENGTH];
uint8_t recevice[7] = {0};
uint8_t usart_start_flag = 0;
uint8_t x_flag = 0,y_flag = 0;
COM_RXStatusTypeDef rx_status;
COM_TXStatusTypeDef tx_status;
//“1”为红色，“2”为绿色，“3”为蓝色
//然后机器人移动到原料区按任务码规定的顺序依次将原料区的上层物料搬运到机器人上
//选择模式需要发送的命令
//原料和粗加工区的搬运顺序一致
uint8_t color_send_data[7] = {0X30,0X00, 0X00,0X00,0X00,0X00,0X00};
uint8_t code_send_data[7] = {0X33,0X00, 0X00,0X00,0X00,0X00,0X00};

uint8_t pos_x[4] = {0},pos_y[4] = {0};
uint8_t x,y;
int Pos_x,Pos_y;
uint8_t Jiaqu[100] = {0};
uint8_t rx_data[7] = {0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00};
uint8_t c[6] = {0X00, 0X00, 0X00, 0X00, 0X00, 0X00};
uint8_t rx_data_position[6] = {0X00, 0X00, 0X00, 0X00, 0X00, 0X00};
int16_t pos_error[3]={0};
//uint8_t move_order[6] = {};//物料搬运的顺序
//uint8_t color_order[6] = {};//物料的摆放顺序
//uint8_t 
/* Private functions --------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

uint8_t PC_RecBuff[15] = {0};
uint8_t send_data[7] = {0X30, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00};//默认为无效数据
uint8_t start_flag = 0;

void COM_ChooseMode(uint8_t mode)
{
	send_data[0] = mode;
	COM_SendAll(send_data);
}
uint8_t COM_GetCodeOrder(uint8_t * codeorder)
{
	OLED_CLS();
	if(rx_data[0] == CODE_MODE && rx_data[3] != 0)//判断开始返回数值
	{
		
		for (int i = 0; i < 6; i++)
		{
			codeorder[i] = rx_data[i+1];
			OLED_Printf(i*12, 10, "%d", codeorder[i]);
		}
		
		return 1; //接收成功
	}
	COM_SendAll(rx_data);
	OLED_RefreshGram();
	delay_ms(100);
	return 0;

}
uint8_t COM_GetColorOrder(uint8_t * order)
{
	if(rx_data[0] == COLOR_MODE && rx_data[3] != 0  && rx_data[2] != 0)//判断开始返回数值
	{
		for (int i = 0; i < 6; i++)
			order[i] = rx_data[i+1];
		
		return 1;
	}
	delay_ms(1);
}

//使用串口3发送数据，只会发一次不会占用很多时间
void COM_SendBuffer(uint8_t *p, uint16_t length)
{
	int i;
	for(i=0;i<length;i++)
	{
		USART_SendData(USART3, *p++);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
	}
}

//串口3 TX PD8     RXPD9
void COM_PCInit(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	COM_PackInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;					
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						
	USART_InitStructure.USART_Parity = USART_Parity_No;							
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				
	USART_Init(USART3, &USART_InitStructure);

	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

void COM_PackInit(void)
{
	rx_status.read_flag 	  = 0;
	rx_status.read_index      = 0;
	rx_status.read_sum        = 0;
	rx_status.read_temp_index = 0;
	rx_status.rx_index        = 0;
	tx_status.data_index      = 0;
	tx_status.send_sum        = 0;
}

uint32_t err_com_data = 0;
uint8_t COM_ReadAll(uint8_t *rx_data_ptr)
{
	uint16_t 	data_index 	= 0; // 从存有数据的地方开始的索引
	uint16_t 	data_length = 0;
	uint16_t 	loop_index; // 用于循环
	uint32_t 	read_ptr;

	rx_status.read_flag = 0; 

	while((int64_t)rx_status.read_index < (int64_t)(rx_status.rx_index - (RXALL_LENGTH*2)))
	{
		rx_status.read_index += RXALL_LENGTH; // 每一个length都是完整的一个包，不解析前面累计的
	}

	// 需要有一个包长才开始解析
	while(rx_status.read_index <= (rx_status.rx_index - RXALL_LENGTH))
	{
		rx_status.read_temp_index = rx_status.read_index % COM_BUFFER_SIZE; // 超出BUFFER, 中断会自动覆盖从头开始
		data_index = rx_status.read_temp_index + 1;
		
//		OLED_Printf(8, 20, "%d, %d", rx_status.rx_index, rx_status.read_index);
//		OLED_RefreshGram();
		
		delay_ms(20); //这个bug没找到
		if(com_rxbuff[rx_status.read_temp_index] == PACK_HEADER) // 检测到包头
		{
			//头是对的至少
			rx_status.read_sum=0;
			for(loop_index = 0; loop_index < RXALL_LENGTH - 1; loop_index++)
			{
				if(rx_status.read_temp_index >= COM_BUFFER_SIZE)
					rx_status.read_temp_index -= COM_BUFFER_SIZE;
				rx_status.read_sum += com_rxbuff[rx_status.read_temp_index];
				rx_status.read_temp_index++;//得从0位开始加，注意校验位和和位不一样！！
			}
			
			//读完从0到8位 第九位为和
			//rx_status.read_temp_index++;
			if(rx_status.read_temp_index >= COM_BUFFER_SIZE)
				rx_status.read_temp_index -= COM_BUFFER_SIZE; //或者取余
			
			if(rx_status.read_sum == com_rxbuff[rx_status.read_temp_index]) // 校验和
			{
				read_ptr = (uint32_t)(rx_data_ptr);
				data_length = RXDATA_LENGTH;
				for(loop_index = 0; loop_index < data_length; loop_index++)
				{
					if(data_index >= COM_BUFFER_SIZE)
						data_index -= COM_BUFFER_SIZE;
					(*((uint8_t *)read_ptr)) = com_rxbuff[data_index];
					data_index++;
					read_ptr++;
				}
				err_com_data = data_index;
				rx_status.read_index += RXALL_LENGTH; //读了一个包但是忘记更新了
				rx_status.read_flag = 1;
			}
			
			else
			{
				rx_status.read_index++;
				err_com_data++;
			}
		}
		else
		{
			rx_status.read_index++;
			err_com_data++;
		}
	}
	return rx_status.read_flag;
}


void COM_SendAll(uint8_t *tx_data_ptr)
{
	int i;
	uint8_t data_bit_index = 0;
	uint8_t data_index = 1;
	uint16_t loop_index;
	com_txbuff[0] = PACK_HEADER;
	tx_status.send_sum = 0;
	
	for(loop_index = 0; loop_index < TXPACK_LENGTH - 1; loop_index++)
	{
		com_txbuff[data_index++] = tx_data_ptr[loop_index];
	}
	
	for(loop_index = 0; loop_index <= TXPACK_LENGTH - 1; loop_index++)
		 tx_status.send_sum += com_txbuff[loop_index];
	
	com_txbuff[TXPACK_LENGTH - 1] = tx_status.send_sum;
	COM_SendBuffer(com_txbuff, TXPACK_LENGTH);
}
void pos_message_solve(void)
{
	pos_error[0] = (int16_t)((rx_data_position[0]<<8)|rx_data_position[1]);
	pos_error[1] = (int16_t)((rx_data_position[2]<<8)|rx_data_position[3]);
	pos_error[2] = (int16_t)((rx_data_position[4]<<8)|rx_data_position[5]);
	
}
/**
  * @brief  定时器2接收中断，更新包值
  * @param  None
  * @retval None
  */
static uint16_t circle_rx_index = 0;
uint8_t USART3_RecNum = 0,Jisuan_flag = 0,Jiaqu_start_flag = 0,jiaqu_i,stop_read_flag=0,pos_judge_flag=0;
uint8_t USART3_RecNum_2 = 0,USART3_RecNum_3 = 0,start_flag_pos = 0;
uint16_t sum =0;
void USART3_IRQHandler(void)
{
	uint8_t Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3); 	
		if(start_flag == 1)
		{
			com_rxbuff[res_i++] = Res;
			if(res_i == 7) 
			{
				stop_read_flag = 1;
				start_flag = 0;res_i = 0;
			}
		}
		if(start_flag_pos == 1)
		{
			
			if(x_flag == 1)
			{
				if(Res == ',')
				{
					Jisuan_flag = 1;
				}
				else pos_x[x++] = Res;
				if(Jisuan_flag == 1)
				{
					if(pos_x[0] == '-')
					{
						if(x == 4) Pos_x = (pos_x[1]-48)*100+(pos_x[2]-48)*10+pos_x[3]-48;
						else Pos_x = (pos_x[1]-48)*10+(pos_x[2]-48)*1;
						for(uint8_t i = 0;i<5;i++)
						{
							pos_x[i] = 0;
						}
						Pos_x = -Pos_x;
						x_flag = 0;
						x = 0;
					}
					else if(pos_x[0] == '+')
					{
						if(x == 4) Pos_x = (pos_x[1]-48)*100+(pos_x[2]-48)*10+pos_x[3]-48;
						else Pos_x = (pos_x[1]-48)*10+(pos_x[2]-48)*1;
						for(uint8_t i = 0;i<5;i++)
						{
							pos_x[i] = 0;
						}
						x_flag = 0;
						x = 0;
					}
					Jisuan_flag = 0;
				}
			}
			if(y_flag == 1)
			{
				if(Res == ')')
				{
					Jisuan_flag = 1;
				}
				else pos_y[y++] = Res;
				if(Jisuan_flag == 1)
				{
					if(pos_y[0] == '-')
					{
						if(y == 4) Pos_y = (pos_y[1]-48)*100+(pos_y[2]-48)*10+(pos_y[3]-48);
						else Pos_y = (pos_y[1]-48)*10+(pos_y[2]-48)*1;
						for(uint8_t i = 0;i<5;i++)
						{
							pos_y[i] = 0;
						}
						Pos_y = -Pos_y;
						y_flag = 0;
						y = 0;
					}
					else if(pos_y[0] == '+')
					{
						if(y == 4) Pos_y = (pos_y[1]-48)*100+(pos_y[2]-48)*10+pos_y[3]-48;
						else Pos_y = (pos_y[1]-48)*10+(pos_y[2]-48)*1;
						for(uint8_t i = 0;i<5;i++)
						{
							pos_y[i] = 0;
						}
						y_flag = 0;
						y = 0;
					}
					Jisuan_flag = 0;
					pos_judge_flag = 1;
					start_flag_pos = 0;
				}
				
			}
			if(Res == '(')
			{
				x_flag = 1;
			}
			if(Res == ',')
			{
				y_flag = 1;
			}
		}
		if(Jiaqu_start_flag == 1)
		{
				Jiaqu[jiaqu_i++] = Res;
				Jiaqu_start_flag = 0;
		}
		if(Res == 0x77)
		{
			start_flag = 1;
		}
		if(Res == 0xEC)
		{
			Jiaqu_start_flag = 1;
		}
		if(Res == 0xFC)
		{
			pos_judge_flag = 0;
			start_flag_pos = 1;
		}	
	}
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
}

void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}


uint8_t * COM_GetRXData(void)
{
	return rx_data;
}


void COM_Debug(void)
{
	while(1)
	{
		//COM_PackInit();
		
//		COM_SendAll(send_data);
//		//
//		delay_ms(3000);
		send_data[0] = 0X33;
		COM_SendAll(send_data);
		delay_ms(500);
//		while(1)
//		{
//			COM_ReadAll(rx_data);//有问题
//////			send_data[0] = 0X30;
//			//COM_SendAll(rx_data);
//			printf("a:%d,%d,%d,%d,%d,%d,%d\n", rx_data[0], rx_data[1], rx_data[2], rx_data[3], rx_data[4], rx_data[5], rx_data[6]);
//			delay_ms(300);
//		}
		//delay_ms(3000);
//		while(1)
//		{
//			
//			delay_ms(300);
//		}
	}
}
