#ifndef __LED_H
#define __LED_H
#include "sys.h"

/***********************************************************************
//PG10	底盘扩展板，LED1，低电平时点亮
//PG11	底盘扩展板，LED2，低电平时点亮
//PG12	
//PG13	LED0，低电平时点亮
//PG14	LED1，低电平时点亮,USART6_TX，默认不要使用LED1
***********************************************************************/	


//LED端口定义
#define LOCAL_LED0 PGout(13)
#define EXPAND_LED1 PGout(10)
#define EXPAND_LED2 PGout(11)

#define LED_ON 0
#define LED_OFF 1
	 
void LED_Init(void);//初始化
void LED_Test(void);
void LED_Run(void);
void LED_AllOpen(void);
void LED_AllClose(void);
#endif
