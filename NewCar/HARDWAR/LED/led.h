#ifndef __LED_H
#define __LED_H
#include "sys.h"

/***********************************************************************
//PG10	������չ�壬LED1���͵�ƽʱ����
//PG11	������չ�壬LED2���͵�ƽʱ����
//PG12	
//PG13	LED0���͵�ƽʱ����
//PG14	LED1���͵�ƽʱ����,USART6_TX��Ĭ�ϲ�Ҫʹ��LED1
***********************************************************************/	


//LED�˿ڶ���
#define LOCAL_LED0 PGout(13)
#define EXPAND_LED1 PGout(10)
#define EXPAND_LED2 PGout(11)

#define LED_ON 0
#define LED_OFF 1
	 
void LED_Init(void);//��ʼ��
void LED_Test(void);
void LED_Run(void);
void LED_AllOpen(void);
void LED_AllClose(void);
#endif
