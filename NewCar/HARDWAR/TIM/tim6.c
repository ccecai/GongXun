/**MADE BY ME
  ******************************************************************************
  * @author   	LIU_Standard
  * @version  	V1.0.0
  * @date    	2021-7-27
  * @brief  	以5ms得周期得循环函数在此
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Private Includes ----------------------------------------------------------*/
#include "tim6.h"
#include "SpeedControl.h"
#include "encodercontrol.h"
#include "positioncontrol.h"
#include "moter.h"
#include "usart.h"
#include "mpu9250.h"
#include "remotecontrol.h"
#include "valpackage.h"
#include "oled.h"
#include "comother.h"
#include "trace.h"
//#include "bmp.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Global param ------------------------------------------------------------*/
static int64_t TIM6_Times = 0;
/* Private functions --------------------------------------------------------*/
/**
  * @brief 	定时器中断函数，在此做循环
  * @param  None
  * @retval None
  */
extern SPEEDC_SpeedsTypeDef current_speeds_t;
extern uint8_t isTracing;
extern uint8_t isStopping;
extern uint8_t isVertical;
extern uint8_t isReversed;
void TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		USART1_DmaLoop();	  // 更新FIFO，只要5ms不超过2k就可以
		ENCODER_UpdateLoop(); // 读编码器，更新速度和位置信息
		TRACE_UpdatLoop();    //更新传感器的值

		SPEEDC_ControlLoop(); //速度环s
		if (TIM6_Times % 5 == 0)
		{
			if(isStopping)
				MPU_UserSetStartYaw(0);//一直设置开始值，防止偏
			if(isVertical)
				MPU_UserSetStartYaw(-90);
			if(isReversed)
				MPU_UserSetStartYaw(-180);
//			if(!isTracing) //循迹屏蔽位置环
			POSITIONC_ControlLoop();
//			TRACE_UpdatLoop();
		}
		if (TIM6_Times % 20 == 1)
		{
			//OLED_RefreshGram();
			//OLED_DrawBMP(0, 0, 127, 127, BMP1);
		}
//		MOTER_SetCurrent(500, 500, 500, 500);
		TIM6_Times++;
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

/* Functions ------------------------------------------------------------------*/
/**
  * @brief  定时器初始化，周期f=84MHz/(1+psc)(1+arr)
  * @param  None
  * @retval None
  */
void TIM6_TimingInit(uint32_t psc, uint32_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_DeInit(TIM6);
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
}
