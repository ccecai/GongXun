#include "sys.h"
#include "usart.h"
#include "fifo.h"
#include "moter.h"
#include "comother.h"
#define BUF_MAX_SIZE 4096
extern uint8_t Jiaqu[100];
extern uint8_t x_flag,y_flag,pos_x[4],pos_y[4];
typedef struct
{
	uint8_t dma_send_buf[BUF_MAX_SIZE];
	uint32_t dma_send_size;
	uint8_t dma_data_adding;
	uint8_t dma_data_getting;
} USART1_DmaStateTypeDef;
static USART1_DmaStateTypeDef USART1_DmaState_t = {{0}, 0, 0, 0};
static uint8_t fifo_datas[BUF_MAX_SIZE];
static FIFO_State_t USART1_PrintFifo;

#pragma import(__use_no_semihosting)
struct __FILE
{
	int handle;
};

FILE __stdout;

void _ttywrch(int ch)
{
	ch = ch;
}
void _sys_exit(int x)
{
	x = x;
}

#if NORMAL_USART // ʹ������ԭ�ӵ�printf
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		;
	USART1->DR = (u8)ch;
	return ch;
}
#endif
void USART1_PrintfTest(void)
{
//	printf("%d,%d,%d,%d,%d,%d,%d,%d\n",com_rxbuff[0],com_rxbuff[1],com_rxbuff[2],com_rxbuff[3],com_rxbuff[4],com_rxbuff[5],com_rxbuff[6],com_rxbuff[7]);
printf("%d,%d,%d,%d,%d,%d,%d,%d\r\n",Pos_x,Pos_y,com_rxbuff[0],com_rxbuff[1],com_rxbuff[2],com_rxbuff[3],Jiaqu[0],Jiaqu[1]);
//	printf("%d,%d,%d,%d,%d,%d\r\n",rx_data[0],rx_data[1],rx_data[2],rx_data[3],rx_data[4],rx_data[5]);
}

#if FIFO_DMA_USART // ʹ��DMA+FIFO
int fputc(int ch, FILE *f)
{
	USART1_DmaState_t.dma_data_adding = 1;
	if (FIFO_IsFull(&USART1_PrintFifo)) // �ж�FIFO�Ƿ�����
	{
		FIFO_Flush(&USART1_PrintFifo);
	}
	else
	{
		FIFO_AddData(&USART1_PrintFifo, (uint8_t)ch); // ʹprintf�е�����ȫ���ŵ�fifo����ȥ
		USART1_DmaState_t.dma_data_adding = 0;
	}

	return ch;
}
#endif

/*
* dmaѭ������
* �ڶ�ʱ���з���ѭ������DMA�е�����
*/
void USART1_DmaLoop(void)
{
	if (USART1_DmaState_t.dma_data_adding == 0 && USART1_DmaState_t.dma_data_getting == 0)
	{
		if (!FIFO_IsEmpty(&USART1_PrintFifo))
		{
			USART1_DmaState_t.dma_data_getting = 1;
			USART1_DmaState_t.dma_send_size = FIFO_GetDatas(&USART1_PrintFifo, USART1_DmaState_t.dma_send_buf, USART1_PrintFifo.fifo_used_size);
			USART1_DmaSendData(USART1_DmaState_t.dma_send_size);
			USART1_DmaState_t.dma_data_getting = 0;
		}
	}
}

void USART1_DmaConfig(uint32_t memory_addr, uint16_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_DeInit(DMA2_Stream7);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)
	{
	}
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)memory_addr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = buffsize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream7, DISABLE);
}

void USART1_DmaSendData(uint32_t data_lenght)
{
	DMA_Cmd(DMA2_Stream7, DISABLE);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)
		;
	DMA_SetCurrDataCounter(DMA2_Stream7, data_lenght);
	DMA_Cmd(DMA2_Stream7, ENABLE);
}

#if EN_USART1_RX 
u8 USART_RX_BUF[USART_REC_LEN];
u16 USART_RX_STA = 0; 
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
#if EN_USART1_RX
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
#endif
}

void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

void DMA2_Stream7_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{
		if (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7))
		{
			DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
			USART1_DmaState_t.dma_data_adding = 0;
		}
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
	}
}

void USART1_InitWithDma(uint32_t bound)
{
	static int n = 0;
	if (n == 0)
	{
		USART1_Init(bound);
		FIFO_Init(&USART1_PrintFifo, BUF_MAX_SIZE, fifo_datas);
		n = 1;
	}
	USART1_DmaConfig((uint32_t)USART1_DmaState_t.dma_send_buf, BUF_MAX_SIZE);
}

void USART1_IRQHandler(void)
{
	u8 Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART1); //(USART1->DR);
		if (Res == 's')
		{
			MOTER_EN = !MOTER_EN;
		}
		//printf("oook\n");
//		USART_SendData(USART1, Res);
		//LED1 = !LED1;
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
#endif
