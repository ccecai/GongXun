#include "led.h"
#include "delay.h"
/***********************************************************************
//PG10	������չ�壬LED1
//PG11	������չ�壬LED2
//PG12	
//PG13	LED0���͵�ƽʱ����
//PG14	LED1���͵�ƽʱ����,USART6_TX��Ĭ�ϲ�Ҫʹ��LED1
***********************************************************************/


void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
  GPIO_ResetBits(GPIOG,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13);
  delay_ms(50);
  GPIO_SetBits(GPIOG,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13);
	
}

void LED_Test(void)
{
	while(1)
	{	
		LOCAL_LED0 = LED_ON;
		delay_ms(500);
		LOCAL_LED0 = LED_OFF;
		delay_ms(500);
		
		EXPAND_LED1 = LED_ON;
		delay_ms(500);
		EXPAND_LED1 = LED_OFF;
		delay_ms(500);
		
		EXPAND_LED2 = LED_ON;
		delay_ms(500);
		EXPAND_LED2 = LED_OFF;
		delay_ms(500);
	}	
}

void LED_Run(void)
{
	static uint8_t LED_State = 0;
	if(LED_State == 0)
	{
		EXPAND_LED1 = LED_ON;
		EXPAND_LED2 = LED_OFF;
		LOCAL_LED0 = LED_OFF;
		++ LED_State;
	}
	else if(LED_State == 1)
	{
		EXPAND_LED2 = LED_ON;
		EXPAND_LED1 = LED_OFF;
		LOCAL_LED0 = LED_OFF;
		++ LED_State;
	}
	else if(LED_State == 2)
	{
		EXPAND_LED2 = LED_OFF;
		EXPAND_LED1 = LED_OFF;
		LOCAL_LED0 = LED_ON;
		LED_State = 0;
	}
}


void LED_AllOpen(void)
{
		EXPAND_LED2 = LED_ON;
		EXPAND_LED1 = LED_ON;
		LOCAL_LED0 = LED_ON;
	
}

void LED_AllClose(void)
{
		EXPAND_LED2 = LED_ON;
		EXPAND_LED1 = LED_ON;
		LOCAL_LED0 = LED_ON;
	
}





