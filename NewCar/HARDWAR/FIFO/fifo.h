#ifndef __FIFO_H
#define __FIFO_H
#include "sys.h"

typedef struct
{
	uint8_t *fifo_datas;	 // fifo����
	uint32_t fifo_max_size;	 // fifo�Ĵ�С
	uint32_t fifo_used_size; // fifo�Ѿ�ʹ���˵Ŀռ�
	uint32_t fifo_pread;	 // fifo��ָ��,ʵ���Ͼ���ͷ
	uint32_t fifo_pwrite;	 // fifoдָ��,ʵ���Ͼ���ĩβ
} FIFO_State_t;

FIFO_State_t *FIFO_Init(FIFO_State_t *FIFO_StateStructure, uint32_t max_size, uint8_t *datas);
uint8_t FIFO_IsFull(FIFO_State_t *FIFO_StateStructure);
uint8_t FIFO_IsEmpty(FIFO_State_t *FIFO_StateStructure);
uint8_t FIFO_AddData(FIFO_State_t *FIFO_StateStructure, uint8_t data);
uint8_t FIFO_GetData(FIFO_State_t *FIFO_StateStructure);
uint32_t FIFO_AddDatas(FIFO_State_t *FIFO_StateStructure, uint8_t *datas_add, uint32_t data_len);
uint32_t FIFO_GetDatas(FIFO_State_t *FIFO_StateStructure, uint8_t *datas_get, uint32_t data_len);
void FIFO_Flush(FIFO_State_t *FIFO_StateStructure);

#endif
