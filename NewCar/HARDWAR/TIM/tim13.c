#include "tim13.h"
#include "sensor.h"


static int64_t TIM13_Times = 0;

void TIM13_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		SENSOR_ControlLoop();
		TIM13_Times++;
	}
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}


void TIM13_TimingInit(uint32_t psc, uint32_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	TIM_DeInit(TIM13);
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM13, TIM_FLAG_Update);
	TIM_ITConfig(TIM13, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM13, ENABLE);
}
