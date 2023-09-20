#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define USART_REC_LEN 200 //�����������ֽ��� 200
#define EN_USART1_RX 1    //ʹ�ܣ�1��/��ֹ��0������1����
#define NORMAL_USART 0
#define FIFO_DMA_USART 1

extern u8 USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA;               //����״̬���

void USART1_DmaSendData(uint32_t data_lenght);
void USART1_DmaConfig(uint32_t memory_addr, uint16_t buffsize);
void USART1_Init(u32 bound);
void USART1_DmaLoop(void);
void USART1_PrintfTest(void);
void USART1_InitWithDma(uint32_t bound);
void USART2_Init(u32 bound);
#endif
