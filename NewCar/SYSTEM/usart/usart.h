/*

 PA9,USART_TX,,,,,,,
 PA10,USART_RX,,,,,,,
 已经使用DMA2的数据流7，通道4。

*/


#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "delay.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define USE_USART1_PRINTF 1
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

void Usart1PrintInit(void);
void UseDmaSendChars(uint8_t *source, uint32_t HowMany);
void Usart1DmaLoop(void);
	



#endif


