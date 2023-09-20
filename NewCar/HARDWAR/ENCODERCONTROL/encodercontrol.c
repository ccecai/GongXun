/**MADE BY ME
	******************************************************************************
	* @author   LIU_Standard
	* @version 	V1.0.0
	* @date    	2021-7-25
	* @brief  	����������
	* 
	******************************************************************************
	* @attention 
	* ������1 TIM1 PE9 PE11
	* ������2 TIM3 PA6 PA7
	* ������3 TIM2 PA15 PB3
	* ������4 TIM5 PA0 PA1
	* 
	******************************************************************************
	*/

/* Private Includes ----------------------------------------------------------*/
#include "encodercontrol.h"
#include "speedcontrol.h"
#include "positioncontrol.h"
#include "moter.h"
#include "trace.h"
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ELIMINATE_EVERYTIME 1
#define ACC_PULSE 0

/* Private functions --------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
/** @paramgroup Speed
 *  @brief  ����5ms��ʱ���л�õ�ǰ�ٶ�
 *
 */
int16_t Speed1 = 0;
int16_t Speed2 = 0;
int16_t Speed3 = 0;
int16_t Speed4 = 0; //ʹ�þֲ�ȫ�ֱ�������ͬһ���ļ��ĵ���
int16_t S_Speed1 = 0;
int16_t S_Speed2 = 0;

#if ELIMINATE_EVERYTIME // ÿ�ζ����������
extern uint8_t isTracing;
void ENCODER_UpdateLoop(void)
{
	static int i;
	Speed1 = (short)TIM2->CNT;
	Speed1 = -Speed1;
	TIM2->CNT = 0;
	
	Speed2 = (short)TIM3->CNT;
	Speed2 = Speed2;
	TIM3->CNT = 0;
	
	Speed3 = (short)TIM5->CNT;
	Speed3 = Speed3;
	TIM5->CNT = 0;
	
	Speed4 = (short)TIM4->CNT; //ʹ��short�ɶ�������
	Speed4 = -Speed4;
	
	TIM4->CNT = 0;
	
//	printf("speed:%d, %d, %d, %d\n", Speed1,Speed2, Speed3, Speed4);
	SPEEDC_RealSpeedUpdate(Speed1, Speed2, Speed3, Speed4, S_Speed1, S_Speed2); //important,�ٶȻ����ݵ����õ�ֵ
	//if(!isTracing)//ѭ��������λ�ã����ڵĴ��������������Ǻܺ�
	POSITONC_UpdateLoop(Speed1, Speed2, Speed3, Speed4, S_Speed1, S_Speed2);
}
#endif

/** @paramgroup overflow_times
 *  @brief ������¼���ʱ��
 *
 */
static int16_t overflow_times_1 = 0;
static int16_t overflow_times_2 = 0;
static int16_t overflow_times_3 = 0;
static int16_t overflow_times_4 = 0;

#if ACC_PULSE // ʹ���ۼ�����ķ��������ֶ����������
void ENCODER_UpdateLoop(void)
{
	static int64_t all_pulse_1, all_pulse_2, all_pulse_3, all_pulse_4;
	static int64_t last_all_pulse_1, last_all_pulse_2, last_all_pulse_3, last_all_pulse_4;
	static int i;

	int32_t temp1, temp, temp3, temp4;
	//ԭϵ�� 1.48 1.48 1.46�����Ƽ��ٱ���1:20��Ӧ����1.5
	temp1 = (int32_t)TIM1->CNT;
	all_pulse_1 = (short)(temp1 + overflow_times_1 * ENCODER_TIM_PERIOD);
	Speed1 = all_pulse_1 - last_all_pulse_1;

	temp = TIM3->CNT;
	all_pulse_2 = (short)(temp + overflow_times_2 * ENCODER_TIM_PERIOD);
	Speed2 = all_pulse_2 - last_all_pulse_2;

	temp3 = TIM4->CNT;
	all_pulse_3 = (short)(temp3 + overflow_times_3 * ENCODER_TIM_PERIOD);
	Speed3 = all_pulse_3 - last_all_pulse_3;

	temp4 = TIM8->CNT; //ʹ��short�ɶ�������
	all_pulse_4 = (short)(temp4 + overflow_times_4 * ENCODER_TIM_PERIOD);
	Speed4 = all_pulse_4 - last_all_pulse_4; //5ms����

	last_all_pulse_1 = all_pulse_1;
	last_all_pulse_2 = all_pulse_2;
	last_all_pulse_3 = all_pulse_3;
	last_all_pulse_4 = all_pulse_4;
	i++;
	if (i == 100)
	{
		printf("pulse:%d, %lld, %lld, %lld, %lld, %d, %d, %d, %d\n",
			   temp1, all_pulse_1, all_pulse_2, all_pulse_3, all_pulse_4, overflow_times_1, overflow_times_2, overflow_times_3, overflow_times_4);
		i = 0;
	}
	SPEEDC_RealSpeedUpdate(Speed1, Speed2, Speed3, Speed4); //important,�ٶȻ����ݵ����õ�ֵ
	POSITONC_UpdateLoop(all_pulse_1, all_pulse_2, all_pulse_3, all_pulse_4);
}
#endif

/**
  * @brief  �������ת�� ��λm/s
  * @param  None
  * @retval None
	* @record
	* 	��ѹ	5ms����	ת��(m/s)
	*		12.4	 49r		1.25~1.30	
  */
void Moter_Performance_Test(void)
{
	float temp1, temp2, temp3, temp4;
	MOTER_SetCurrent(2000, 2000, 2000, 2000);
	temp1 = (float)Speed1 / 13 / 30 / 4 / 5 * 1000 * 32.5f * 2 * 3.141415926f / 1000; // r/s * mm / 1000 = m/s
	temp2 = (float)Speed2 / 13 / 30 / 4 / 5 * 1000 * 32.5f * 2 * 3.141415926f / 1000; // r/s * mm / 1000 = m/s
	temp3 = (float)Speed3 / 13 / 30 / 4 / 5 * 1000 * 32.5f * 2 * 3.141415926f / 1000; // r/s * mm / 1000 = m/s
	temp4 = (float)Speed4 / 13 / 30 / 4 / 5 * 1000 * 32.5f * 2 * 3.141415926f / 1000; // r/s * mm / 1000 = m/s
	//printf("5MS_PULSE: %d, %d, %d, %d\n", Speed1, Speed2, Speed3, Speed4);
	printf("ACTUAL_SPEED: %f, %f, %f, %f\n", temp1, temp2, temp3, temp4);
}

/**
  * @brief  �߼���ʱ��1��8������ģʽ��ʼ��
  * @param  uint32_t psc, uint32_t arr
  * @retval None
  */
void TIM18_Encoder_Init(uint32_t psc, uint32_t arr)
{
	//TIM1 PE9 PE11    TIM8 PC6 PC7
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	TIM_SetCounter(TIM1, 0); //TIM1->CNT=0
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_SetCounter(TIM8, 0); //TIM1->CNT=0
	TIM_Cmd(TIM8, ENABLE);
}

/**
  * @brief  ������ʱ��2��3��4��ʼ��
  * @param  uint32_t psc, uint32_t arr
  * @retval None
  */
void TIM2345_Encoder_Init(uint32_t psc, uint32_t arr)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_15 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn | TIM3_IRQn | TIM5_IRQn | TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_SetCounter(TIM2, 0);
	TIM_SetCounter(TIM3, 0);
	TIM_SetCounter(TIM5, 0);
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief  ���б�������ʼ��
  * @param  None
  * @retval None
  */
void ENCODER_Init(void)
{
//	TIM18_Encoder_Init(0, ENCODER_TIM_PERIOD);
	TIM2345_Encoder_Init(0, ENCODER_TIM_PERIOD); //����Ƶ���Զ���װ��ֵΪ65535
}

/**
  * @brief  �����Ƕ�ʱ�����жϺ���
  * @param  None
  * @retval None
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		if ((TIM1->CR1 & 0x10) != 0) //�ж������ʱ�������ϻ������¼���
		{
			overflow_times_1--;
		}
		else
		{
			overflow_times_1++;
		}
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}
void TIM8_UP_TIM13_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)
	{
		if ((TIM8->CR1 & 0x10) != 0) //�ж������ʱ�������ϻ������¼���
		{
			overflow_times_1--;
		}
		else
		{
			overflow_times_1++;
		}
	}
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);

}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if ((TIM2->CR1 & 0x10) != 0) //�ж������ʱ�������ϻ������¼���
		{
			overflow_times_2--;
		}
		else
		{
			overflow_times_2++;
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		if ((TIM3->CR1 & 0x10) != 0) //�ж������ʱ�������ϻ������¼���
		{
			overflow_times_2--;
		}
		else
		{
			overflow_times_2++;
		}
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{
		if ((TIM5->CR1 & 0x10) != 0) //�ж������ʱ�������ϻ������¼���
		{
			overflow_times_4--;
		}
		else
		{
			overflow_times_4++;
		}
	}
	overflow_times_4++;
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}
