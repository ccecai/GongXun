/*

 PA9,USART_TX,,,,,,,
 PA10,USART_RX,,,,,,,
 �Ѿ�ʹ��DMA2��������7��ͨ��4��

*/


#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "delay.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USE_USART1_PRINTF 1
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void Usart1PrintInit(void);
void UseDmaSendChars(uint8_t *source, uint32_t HowMany);
void Usart1DmaLoop(void);
	



#endif


