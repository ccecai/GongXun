#include "fifo.h"
#include "stdio.h"
#include "stdlib.h"

/*
* fifo��ʼ������
* ������ʼ����С,��datasָ��ĵ�Ԫ����һ��fifo״̬�ṹ��
*/
FIFO_State_t * FIFO_Init(FIFO_State_t * FIFO_StateStructure, uint32_t max_size, uint8_t * datas)
{
	//�����涨��ʱ�Ѿ�����ռ䣬�ظ����붨λ������λ�ã������ܷ���ֵ��ʱ����������
	//FIFO_StateStructure = (FIFO_State_t *)malloc(sizeof(FIFO_State_t));
	//FIFO_StateStructure->fifo_datas = (uint8_t *)malloc(max_size * sizeof(uint8_t));

	FIFO_StateStructure->fifo_datas = datas;
	FIFO_StateStructure->fifo_max_size = max_size;
	FIFO_StateStructure->fifo_pread = 0;
	FIFO_StateStructure->fifo_pwrite = 0;
	FIFO_StateStructure->fifo_used_size = 0;
	FIFO_Flush(FIFO_StateStructure);
	return FIFO_StateStructure;
}

/*
* �ж�fifo�Ƿ���
* ����һ���ռ����������Ϳյ�ָ��λ��
* ����1��ʾ�Ѿ����ˣ�0��ʾδ��
*/
uint8_t FIFO_IsFull(FIFO_State_t * FIFO_StateStructure)
{
	uint32_t pwrite = FIFO_StateStructure->fifo_pwrite;
	uint32_t max = FIFO_StateStructure->fifo_max_size;
	uint32_t pread = FIFO_StateStructure->fifo_pread;
	return (uint8_t)(((pwrite + 1) % max) == pread);
}

/*
* �ж�fifo�Ƿ��
* ����һ���ռ����������Ϳյ�ָ��λ��
* ����1��ʾ�Ѿ����ˣ�0��ʾδ��
*/
uint8_t FIFO_IsEmpty(FIFO_State_t * FIFO_StateStructure)
{
	uint32_t pwrite = FIFO_StateStructure->fifo_pwrite;
	uint32_t pread = FIFO_StateStructure->fifo_pread;
	return  (uint8_t)(pwrite == pread);
}

/*
* ��fifo���������
* ��ӳɹ�����1�����˷���0
*/
uint8_t FIFO_AddData(FIFO_State_t * FIFO_StateStructure, uint8_t data)
{
	if (FIFO_IsFull(FIFO_StateStructure))
	{
		return 0;
	}
	else
	{
		FIFO_StateStructure->fifo_pwrite++;
		FIFO_StateStructure->fifo_pwrite %= FIFO_StateStructure->fifo_max_size;
		FIFO_StateStructure->fifo_datas[FIFO_StateStructure->fifo_pwrite] = data;
		FIFO_StateStructure->fifo_used_size++;
		return 1;
	}
}

/*
* ��fifo�л�ȡ����
* ����0δ��ȡ���ɹ����ػ�ȡ������
*/
uint8_t FIFO_GetData(FIFO_State_t * FIFO_StateStructure)
{
	if (FIFO_IsEmpty(FIFO_StateStructure))
	{
		return 0;
	}
	else
	{
		FIFO_StateStructure->fifo_pread++;
		FIFO_StateStructure->fifo_pread %= FIFO_StateStructure->fifo_max_size;
		FIFO_StateStructure->fifo_used_size--;
		return FIFO_StateStructure->fifo_datas[FIFO_StateStructure->fifo_pread];
	}
}

/*
* ��fifo��������ݼ�
* ��ӳɹ�����ʵ����ӵ������������˷���0
*/
uint32_t FIFO_AddDatas(FIFO_State_t * FIFO_StateStructure, uint8_t * datas_add, uint32_t data_len)
{
	uint32_t i;
	uint32_t get_len = 0;
	if (FIFO_IsFull(FIFO_StateStructure))
	{
		return 0;
	}
	else
	{
		for (i = 0; i < data_len && (!FIFO_IsFull(FIFO_StateStructure)); i++)
		{
			FIFO_AddData(FIFO_StateStructure, datas_add[i]);
			get_len++;
		}	
		return get_len;
	}
}

/*
* ��fifo�л�ȡ���ݼ�
* ��ȡ��ֱֵ�ӷ��ظ�����ĵ�ַ
* ��ȡ�ɹ�����ʵ�ʻ�ȡ�������������˷���0
*/
uint32_t FIFO_GetDatas(FIFO_State_t * FIFO_StateStructure, uint8_t * datas_get, uint32_t data_len)
{
	uint8_t i;
	uint32_t get_len = 0;
	if (FIFO_IsEmpty(FIFO_StateStructure))
	{
		return 0;
	}
	else
	{
		for (i = 0; i < data_len && (!FIFO_IsEmpty(FIFO_StateStructure)); i++)
		{
			datas_get[i] = FIFO_GetData(FIFO_StateStructure);
			// datas_get[i] = 'a';
			get_len++;
		}
		return get_len;
	}
}

/*
* ��ȫ���FIFO
* ʵ������read��write��ָ��ͷ
*/
void FIFO_Flush(FIFO_State_t * FIFO_StateStructure)
{
	FIFO_StateStructure->fifo_pread = 0;
	FIFO_StateStructure->fifo_pwrite = 0;
	FIFO_StateStructure->fifo_used_size = 0;
}
