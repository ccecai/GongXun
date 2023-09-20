/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    10-Oct-2021
  * @brief   KEY1-KEY4   PG2-PG5
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "key.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Global param ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/

static uint8_t KeyPressed = UNPRESSED;


void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //默认上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

uint8_t IsKeyPressed(void)
{
	return KeyPressed;	
}

void ClearKeyPressed(void)
{
	KeyPressed = UNPRESSED;
}

void KEY1_Delay(void)
{
	while(1)
	{
		if(KEY1 == PRESS)
		{
			delay_ms(10);
			if(KEY1== PRESS)//10ms后还在按着，说明不是抖动
				break;
		}
		delay_ms(1);
	}
	while(1)
	{
		if(KEY1 == UNPRESS)
		{
			delay_ms(10);
			if(KEY1 == UNPRESS)//10ms后还在松着，说明不是抖动
				break;
		}
		delay_ms(1);
	}	
}

void KEY2_Delay(void)
{
	while(1)
	{
		if(KEY2 == PRESS)
		{
			delay_ms(10);
			if(KEY2== PRESS)//10ms后还在按着，说明不是抖动
				break;
		}
		delay_ms(1);
	}
	while(1)
	{
		if(KEY2 == UNPRESS)
		{
			delay_ms(10);
			if(KEY1 == UNPRESS)//10ms后还在松着，说明不是抖动
				break;
		}
		delay_ms(1);
	}	
}
