/************************************************************
* 陀螺仪数据记录：
* 陀螺仪返回数据为11个字节，开头是0x55结尾进行和校验，则可以设置RX_BUFF为11个字节。每接收到11个字节并且校验通过后，
* 然后执行解包函数，把数据解包到相应存储取
* 默认配置接线为配置到启明欣欣板stm32f407的USART2，PA2（USART2_TX)、PA3（USART2_RX)口，
* USART2_RX用的是DMA1，数据流5，通道4
* WCC的陀螺仪采集代码
************************************************************/

#include "gyroscope.h"
#include "usart.h"
#include "stm32f4xx_dma.h"
//#include "pid.h"
//#include "GUI.h"

#define GyroscopeBuffLength 100
static uint8_t GyroscopeBuff[GyroscopeBuffLength] = {0};
static int8_t MpuData[22] = {0};    //每个帧中包含八个数据位，第一位为0x55，起始位，第二位0x53，表示角度帧，最后一位校验和
static uint8_t DMA_HadOpen = 0;    //DMA没打开时为0，准备打开时为1，打开后为2
static int8_t MpuCharNum = 0;  //记录接收到了几位数据
static int8_t MpuYawWhichNew = 1; //记录哪一个是最新的数值,0是没最新的数值
static float MpuYaw = 0.0;
static float MpuYaw2 = 0.0;
static float MpuYaw_Constant_1 = 0.0;
static float MpuYaw_Constant_2 = 0.0;//连续化处理后的角度
static int32_t Mpu_TurnNum = 0;//顺时针转的圈数
static uint16_t MPU_UpdataRata = 0;//测试时打印陀螺仪更新速率
static uint8_t MPU_YawUpdated = 0;//当YAW数据更新后置位1，由外界置0
static uint8_t MPU_ErrState = 0;//0是没更新的状态，1是可以收到陀螺仪数据的状态，说明接线没问题，2是可以正确收到陀螺仪数据，
//说明陀螺仪不用进行配置

fifo_s_t Fifo_GyroscopeBuff;
fifo_s_t* PFifo_GyroscopeBuff = &Fifo_GyroscopeBuff;


void MPU_SendChar_N(uint16_t HowMany,uint8_t *Array)
{
	uint16_t Temp = 0;
	for( ; Temp < HowMany ; ++ Temp)
	{
		while((USART2->SR & 0X40)==0); //循环发送,直到发送完毕   
		USART2->DR = (u8)Array[Temp];		
	}	
}
//

//检测陀螺仪是否断开连接，如果断开连接，会重新配置一下陀螺仪
void MPU_CheckConnected(void)
{
	
}

void MPU_UnLock(void)
{//解锁
	uint8_t Array_Unlock[] = {0XFF,0XAA,0X69,0X88,0XB5};
	MPU_SendChar_N(5, Array_Unlock);
}
//
void MPU_Set_Z_0(void)
{//Z轴归零
	uint8_t Array_Unlock[] = {0XFF,0XAA,0X76,0X00,0X00};
	MPU_SendChar_N(5,Array_Unlock);
}
//
void MPU_Set_ReturnRate(uint8_t HowRate)
{//设置回传速率
//0x01：0.1Hz
//0x02：0.5Hz
//0x03：1Hz
//0x04：2Hz
//0x05：5Hz
//0x06：10Hz（默认）
//0x07：20Hz
//0x08：50Hz
//0x09：100Hz
//0x0a：125Hz
//0x0b：250Hz
//0x0c：500Hz
	uint8_t Array_Unlock[] = {0XFF,0XAA,0X03,0X00,0X00};
	Array_Unlock[3] = HowRate;	
	
	MPU_SendChar_N(5, Array_Unlock);
}
//
//初始化配置陀螺仪
void MPU_InitialState(void)
{
	
	
}
void TestFIFO(void)
{
	uint8_t Temp = 0;
	uint8_t Temp1[11] = "1234567890";
	uint8_t Temp2[11] = "ABCDEFGHIJ";
	uint8_t Temp3[11];
	uint8_t Temp4 = 0;
	
	fifo_s_init(PFifo_GyroscopeBuff,GyroscopeBuff,GyroscopeBuffLength);
	

	++ Temp;
	fifo_s_puts(PFifo_GyroscopeBuff,Temp1,10);
	fifo_s_puts(PFifo_GyroscopeBuff,Temp2,10);
	delay_ms(100);
	printf("FIFO had:%d,%d\n\r",fifo_used_count(PFifo_GyroscopeBuff),fifo_free_count(PFifo_GyroscopeBuff) );
	
	Temp4 = fifo_s_gets(PFifo_GyroscopeBuff,Temp3,10);		
	Temp3[Temp4] = '\0';
	printf("%s\n\r",Temp3);
	Temp4 = fifo_s_gets(PFifo_GyroscopeBuff,Temp3,10);		
	Temp3[Temp4] = '\0';
	printf("%s\n\r",Temp3);
	
	while(1)
		delay_ms(100);
		

}

float ReadMpuYaw(void)
{
  MPU_YawUpdated = 0;
  	
  if(MpuYawWhichNew == 1)
    return MpuYaw_Constant_1;
  
  if(MpuYawWhichNew == 2)
    return MpuYaw_Constant_2;
  
//	return MpuYaw;
  return 360.0f;
}

uint8_t MPU_IsUpdated(void)
{
	if(MPU_YawUpdated != 0)
		return 1;
	else if(MPU_YawUpdated == 0)
		return 0;
	
	return 0;
}

void MPU_TestMPURate(void)
{
	while(1)
	{
		printf("F:%d\n\r",MPU_UpdataRata);
		MPU_UpdataRata = 0;
		delay_ms(1000);
	}
}

void MPU_PrintMPUYaw(void)
{
	//SetPIDTargetSpeed(-300,300,300,0);
	while(1)
	{
//		if(MPU_YawUpdated == 1)
//		{
			printf("Y:%.2f,%.2f\n\r",MPU_GetStartYaw(), MPU_GetBiasToStartYaw());
			delay_ms(100);
//		}
	}
}

static float MPU_StartYaw = 0;
//设置起始时的角度，采样50次，冒泡排序后取中间30位角度的平均值
void MPU_SetStartYaw(void)
{
	float PaiXu[50] = {0};
	uint8_t Count = 0;
	uint8_t Temp1 = 0;
	uint8_t Temp2 = 0;
	
	while(1)
	{
		//printf("Y:%.5f\n\r",ReadMpuYaw());
		if(MPU_YawUpdated == 1)
		{
			PaiXu[Count] = ReadMpuYaw();
			++ Count;			
		}
		if(Count >= 50)
			break;
	}
	
	//冒泡排序
	for(Temp1 = 0; Temp1 < 50; ++ Temp1)
	{
		for(Temp2 = Temp1 + 1;Temp2 < 50 - Temp1;++ Temp2)
		{
			if(PaiXu[Temp1] < PaiXu[Temp2])
			{
				PaiXu[Temp1] = PaiXu[Temp2];
			}
		}		
	}
	
	PaiXu[0] = 0.0;
	
	//求中间30个（10-39）的和
	for(Temp1 = 10; Temp1 < 40; ++ Temp1)
	{
		PaiXu[0] +=  PaiXu[Temp1];
	}
	//求平均值
	MPU_StartYaw =  PaiXu[0] / 30.0f;
}

float MPU_GetStartYaw(void)
{
	return MPU_StartYaw;
}

float MPU_GetBiasToStartYaw(void)
{
	
	return -(ReadMpuYaw() - MPU_StartYaw);
}

void MPU_UserSetStartYaw(float Yaw)
{
	MPU_StartYaw = ReadMpuYaw() - Yaw;
}



/***********************************************************************
* 配置USART2的DMA
* 在空闲中断里面清除DMA中断，然后就不会触发DMA中断了
***********************************************************************/
void SetMpuToDMA(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);		//使能DMA1时钟

	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);//开启串口DMA
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Stream5);
	
	while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE)
	{
		//等待DMA可配置 
	}
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);//DMA外设地址，对DR进行读操作相当于接收，对DR进行写操作相当于发送
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)GyroscopeBuff;//DMA存储器地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储模式
	DMA_InitStructure.DMA_BufferSize = GyroscopeBuffLength;//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储区增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度：8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度：8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//使用循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//最高优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//不使用FIFO
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//设置FIFO阈值无效
	DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;//设置DMA存储器突发模式为正常模式
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
		
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA1_Stream5, ENABLE);
}
//
void HWT101_Init(void)
{//陀螺仪的tx接单片机的RX，接串口的RX，陀螺仪的RX接单片机的TX，接串口的TX
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint32_t DMA_PreStart = 0;
	
	DMA_HadOpen = 0;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 230400;//460800; 230400//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;//| USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure);	
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	delay_ms(100);
	
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启相关空闲中断
	
	SetMpuToDMA();

	while(MPU_ErrState != 2)
	{
		++ DMA_PreStart;
		if(DMA_PreStart >= 1000)
		{//等了1000ms还没正确解析到陀螺仪数据
			DMA_PreStart = 0;
			if(MPU_ErrState == 0)
			{
				printf("MPU LINE ERR陀螺仪接线错误%d\n\r", MPU_ErrState);
				while(1);
			}
			if(MPU_ErrState == 1)
			{//陀螺仪波特率配置错误，需要重新配置陀螺仪
				printf("MPU LINE ERR陀螺仪配置错误%d\n\r", MPU_ErrState);
				while(1);
			}			
			
		}
		delay_ms(1);//等待DMA准备打开，即陀螺仪配置成功
	}
	
	
	delay_ms(500);
	////uncomment
	MPU_SetStartYaw();
	
//	MPU_PrintMPUYaw();
	//MPU_TestMPURate();
}

void DMA1_Stream5_IRQHandler(void)
{
	if(MPU_ErrState == 0)
		MPU_ErrState = 1;//陀螺仪可以收到数据，证明接线没错
	
    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
	//if(DMA1->HISR & 0X0000C000)
	{//查询数据流4传输是否完成
        DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
    }
}


void USART2_IRQHandler(void)
{
	static uint8_t rec = 0;	
	static uint32_t HowManyDataRec = 0;//因为这个变量每次进中断都要用，所以声明为static，不需要每次进来都创建变量，节省一个机器周期

	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
	{
		if(MPU_ErrState == 0)
				MPU_ErrState = 1;//陀螺仪可以收到数据，证明接线没错
		
		//printf("ERR 440\n");
		
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	
	if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE) == SET)
	{
		if(MPU_ErrState == 0)
				MPU_ErrState = 1;//陀螺仪可以收到数据，证明接线没错
		
		//printf("IDLE 440\n");
		DMA_Cmd(DMA1_Stream5, DISABLE); //关闭DMA,防止处理其间有数据
		rec = USART2->SR;
		rec = USART2->DR;//清楚空闲中断，必须先读SR，再读DR
		
		USART_ClearFlag(USART2,USART_FLAG_IDLE);
		
		HowManyDataRec = GyroscopeBuffLength - DMA_GetCurrDataCounter(DMA1_Stream5);//得到当前DMA收到了多少字节
		DMA_SetCurrDataCounter(DMA1_Stream5, GyroscopeBuffLength);
		
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);//不进DMA中断
		
//		printf("a: %d\n", HowManyDataRec);
		if(HowManyDataRec == 22)
		{
//			if(GyroscopeBuff[0] == 0x55 && GyroscopeBuff[1] == 0x52)
//			{//角速度     
//				GyroscopeBuff[0] = GyroscopeBuff[9] + GyroscopeBuff[8] + GyroscopeBuff[7] + GyroscopeBuff[6] + \
//				GyroscopeBuff[5] + GyroscopeBuff[4] + GyroscopeBuff[3] + GyroscopeBuff[2] + GyroscopeBuff[1] + GyroscopeBuff[0];
//			  
//				if( GyroscopeBuff[10] == GyroscopeBuff[0])
//				{
//					++ MPU_UpdataRata;
//					MPU_YawUpdated = 1;
//					GyroscopeBuff[1] = 0x00;
//					if(MpuYawWhichNew == 1)
//					{
//						MpuYaw2 = (int16_t)( GyroscopeBuff[7]<<8 | GyroscopeBuff[8]) /32768.0f *180.0f;					
//						MpuYawWhichNew = 2;					
//					}
//					else if(MpuYawWhichNew == 2)
//					{
//						MpuYaw = (int16_t)( GyroscopeBuff[7]<<8 | GyroscopeBuff[8]) /32768.0 *180.0;
//						MpuYawWhichNew = 1;
//					}
//				}			
//			}
			if(GyroscopeBuff[11] == 0x55 && GyroscopeBuff[12] == 0x53)
			{//角度
				GyroscopeBuff[11] = GyroscopeBuff[20] + GyroscopeBuff[19] + GyroscopeBuff[18] + GyroscopeBuff[17] + \
				GyroscopeBuff[16] + GyroscopeBuff[15] + GyroscopeBuff[14] + GyroscopeBuff[13] + GyroscopeBuff[12] + GyroscopeBuff[11];
			  
				if( GyroscopeBuff[21] == GyroscopeBuff[11])
				{
					MPU_ErrState = 2;
					++ MPU_UpdataRata;
					MPU_YawUpdated = 1;
					GyroscopeBuff[1] = 0x00;
					if(MpuYawWhichNew == 1)
					{
						MpuYaw2 = (int16_t)( GyroscopeBuff[18]<<8 | GyroscopeBuff[17]) /32768.0 *180.0;
						MpuYawWhichNew = 2;
						if(MpuYaw2 - MpuYaw > 170.0f)
						{//连续化处理
							Mpu_TurnNum -= 1.0f;
						}
						else if(MpuYaw2 - MpuYaw < -170.0f)
						{
							Mpu_TurnNum += 1.0f;
						}
						//printf("num:%d, %f, %f\n", Mpu_TurnNum, MpuYaw2, MpuYaw);
						MpuYaw_Constant_2 = MpuYaw2 + 360.0 * Mpu_TurnNum;	
						
						//MpuYaw_Constant_2 = MpuYaw2;						
					}
					
					else if(MpuYawWhichNew == 2)
					{
						MpuYaw = (int16_t)( GyroscopeBuff[18]<<8 | GyroscopeBuff[17]) /32768.0 *180.0;
						MpuYawWhichNew = 1;
						if(MpuYaw - MpuYaw2 > 170.0f)
						{//连续化处理
							Mpu_TurnNum -= 1;
						}
						else if(MpuYaw - MpuYaw2 < -170.0f)
						{
							Mpu_TurnNum += 1;
						}	
						//printf("num:%d, %f, %f\n", Mpu_TurnNum, MpuYaw2, MpuYaw);
						MpuYaw_Constant_1 = MpuYaw + 360.0 * Mpu_TurnNum;	
						//MpuYaw_Constant_1 = MpuYaw;
					}
				}
				
			}
		}
		DMA_Cmd(DMA1_Stream5, ENABLE); //关闭DMA,防止处理其间有数据	 
	  }  
}



#ifdef __USE_LOCAL_DATA_FIFO_H__

int32_t fifo_s_init(fifo_s_t* pfifo, void* base_addr, uint32_t unit_cnt)
{
    //! Check input parameters.
    ASSERT(NULL != pfifo);
    ASSERT(NULL != base_addr);
    ASSERT(0 != unit_cnt);

    //! Initialize FIFO Control Block.
    pfifo->start_addr = (uint8_t*)base_addr;
    pfifo->end_addr = (uint8_t*)base_addr + unit_cnt - 1;
    pfifo->buf_size = unit_cnt;
    pfifo->free = unit_cnt;
    pfifo->used = 0;
    pfifo->read_index = 0;
    pfifo->write_index = 0;

    return 0;

}
/*
* 创建一个fifo，形参是fifo的深度，fifo宽度是固定的8位
* 返回值是一个创建好的fifo结构体地址
*/

fifo_s_t* fifo_s_create(uint32_t unit_cnt)
{
    fifo_s_t *pfifo = NULL;
    uint8_t  *base_addr = NULL;

    //! Check input parameters.
    ASSERT(0 != unit_cnt);

    //! Allocate Memory for pointer of new FIFO Control Block.
    pfifo = (fifo_s_t*)malloc(sizeof(fifo_s_t));
    if (NULL == pfifo)
    {
        //! Allocate Failure, exit now.
        return (NULL);
    }

    //! Allocate memory for FIFO.
    base_addr = malloc(unit_cnt);
    if (NULL == base_addr)
    {
		free(pfifo);
        //! Allocate Failure, exit now.
        return (NULL);
    }

    fifo_s_init(pfifo, base_addr, unit_cnt);

    return (pfifo);
}

/*
* 释放一块fifo的空间
*/
void fifo_s_destory(fifo_s_t* pfifo)
{
    //! Check input parameters.
    ASSERT(NULL != pfifo);
    ASSERT(NULL != pfifo->start_addr);

    //! free FIFO memory
    free(pfifo->start_addr);
    //! free FIFO Control Block memory.
    free(pfifo);

    return;
}

/*
* 向fifo里面放1个字节数据
* 返回值-1表示FIFO满了
*/
int32_t fifo_s_put(fifo_s_t* pfifo, uint8_t element)
{
    //! Check input parameters.
    ASSERT(NULL != pfifo);

    if (0 >= pfifo->free)
    {
        //! Error, FIFO is full!
        return -1;
    }

    pfifo->start_addr[pfifo->write_index++] = element;
    pfifo->write_index %= pfifo->buf_size;
    pfifo->free--;
    pfifo->used++;

    return 0;
}

/*
* 向fifo里面放number个字节数据
* 返回值表示实际放进去了多少个
*/
int32_t fifo_s_puts(fifo_s_t *pfifo, uint8_t *psource, uint32_t number)
{
    int puts_num = 0;
	uint32_t i=0;

    //! Check input parameters.
    ASSERT(NULL != pfifo);

    if (psource == NULL)
        return -1;

    for (i = 0; (i < number) && (pfifo->free > 0); i++)
    {
        pfifo->start_addr[pfifo->write_index++] = psource[i];
        pfifo->write_index %= pfifo->buf_size;
        pfifo->free--;
        pfifo->used++;
        puts_num++;
    }
	
    return puts_num;
}

/*
* 从fifo里面读1个字节数据
* 返回值表示读到的数据
*/
uint8_t fifo_s_get(fifo_s_t* pfifo)
{
    uint8_t   retval = 0;

    //! Check input parameters.
    ASSERT(NULL != pfifo);

    retval = pfifo->start_addr[pfifo->read_index++];
    pfifo->read_index %= pfifo->buf_size;
    pfifo->free++;
    pfifo->used--;

    return retval;
}

/*
* 从fifo里面读len个字节数据
* 返回值表示实际读了多少个
*/
uint16_t fifo_s_gets(fifo_s_t* pfifo, uint8_t* source, uint8_t len)
{
    uint8_t   retval = 0;
	int i=0;

    //! Check input parameters.
    ASSERT(NULL != pfifo);

    for (i = 0; (i < len) && (pfifo->used > 0); i++)
    {
        source[i] = pfifo->start_addr[pfifo->read_index++];
        pfifo->read_index %= pfifo->buf_size;
        pfifo->free++;
        pfifo->used--;
        retval++;
    }

    return retval;
}

/******************************************************************************************
//
//! \brief  Pre-Read an element from FIFO(in single mode).
//!
//! \param  [in]  pfifo is the pointer of valid FIFO.
//! \param  [in]  offset is the offset from current pointer.
//!
//! \retval the data element of FIFO.
//
******************************************************************************************/
uint8_t fifo_s_pre_read(fifo_s_t* pfifo, uint8_t offset)
{
    uint32_t index;

    //! Check input parameters.
    ASSERT(NULL != pfifo);

    if (offset > pfifo->used)
    {
        return 0x00;
    }
    else
    {
        index = ((pfifo->read_index + offset) % pfifo->buf_size);
        // Move Read Pointer to right position   
        return pfifo->start_addr[index];
    }
}


/******************************************************************************************
//!
//! \retval - None-zero(true) if empty.
//!         - Zero(false) if not empty.
//
******************************************************************************************/
uint8_t fifo_is_empty(fifo_s_t* pfifo)
{
    //! Check input parameter.
    ASSERT(NULL != pfifo);

    return (0 == pfifo->used);
}

/*****************************************************************************************
//!
//! \retval - None-zero(true) if full.
//!         - Zero(false) if not full.
//
*****************************************************************************************/
uint8_t fifo_is_full(fifo_s_t* pfifo)
{
    //! Check input parameter.
    ASSERT(NULL != pfifo);

    return (0 == pfifo->free);
}

/******************************************************************************************
//!
//! \retval The number of elements in FIFO.
//
******************************************************************************************/
uint32_t fifo_used_count(fifo_s_t* pfifo)
{
    //! Check input parameter.
    ASSERT(NULL != pfifo);

    return (pfifo->used);
}

/******************************************************************************************
//!
//! \retval The number of elements in FIFO.
//
******************************************************************************************/
uint32_t fifo_free_count(fifo_s_t* pfifo)
{
    //! Check input parameter.
    ASSERT(NULL != pfifo);

    return (pfifo->free);
}


/******************************************************************************************
//
//! \brief  Flush the content of FIFO.清空
//!
//! \param  [in] pfifo is the pointer of valid FIFO.
//!
//! \retval 0 if success, -1 if failure.
//
******************************************************************************************/
uint8_t fifo_flush(fifo_s_t* pfifo)
{
    //! Check input parameters.
    ASSERT(NULL != pfifo);

    //! Initialize FIFO Control Block.
    pfifo->free = pfifo->buf_size;
    pfifo->used = 0;
    pfifo->read_index = 0;
    pfifo->write_index = 0;

    return 0;
}

void clear_half_fifo(fifo_s_t* pfifo){
    uint8_t num;
	//! Check input parameters.
    ASSERT(NULL != pfifo);
    
    num = pfifo->buf_size /2;
    if(num >= pfifo->used)return;
    pfifo->read_index += num;
    pfifo->read_index %= pfifo->buf_size;
    pfifo->used -= num;
    pfifo->free += num;
}
#endif


