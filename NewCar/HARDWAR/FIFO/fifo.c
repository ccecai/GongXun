#include "fifo.h"
#include "stdio.h"
#include "stdlib.h"

/*
* fifo初始化函数
* 给定初始化大小,和datas指向的单元返回一个fifo状态结构体
*/
FIFO_State_t * FIFO_Init(FIFO_State_t * FIFO_StateStructure, uint32_t max_size, uint8_t * datas)
{
	//在外面定义时已经申请空间，重复申请定位到其它位置？？接受返回值的时候可以用这个
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
* 判断fifo是否满
* 少用一个空间以区分满和空的指针位置
* 返回1表示已经满了，0表示未满
*/
uint8_t FIFO_IsFull(FIFO_State_t * FIFO_StateStructure)
{
	uint32_t pwrite = FIFO_StateStructure->fifo_pwrite;
	uint32_t max = FIFO_StateStructure->fifo_max_size;
	uint32_t pread = FIFO_StateStructure->fifo_pread;
	return (uint8_t)(((pwrite + 1) % max) == pread);
}

/*
* 判断fifo是否空
* 少用一个空间以区分满和空的指针位置
* 返回1表示已经空了，0表示未空
*/
uint8_t FIFO_IsEmpty(FIFO_State_t * FIFO_StateStructure)
{
	uint32_t pwrite = FIFO_StateStructure->fifo_pwrite;
	uint32_t pread = FIFO_StateStructure->fifo_pread;
	return  (uint8_t)(pwrite == pread);
}

/*
* 向fifo中添加数据
* 添加成功返回1，满了返回0
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
* 从fifo中获取数据
* 返回0未获取，成功返回获取的数据
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
* 向fifo中添加数据集
* 添加成功返回实际添加的数据数，满了返回0
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
* 从fifo中获取数据集
* 获取的值直接返回给传入的地址
* 获取成功返回实际获取的数据数，空了返回0
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
* 完全清空FIFO
* 实际是让read和write都指向开头
*/
void FIFO_Flush(FIFO_State_t * FIFO_StateStructure)
{
	FIFO_StateStructure->fifo_pread = 0;
	FIFO_StateStructure->fifo_pwrite = 0;
	FIFO_StateStructure->fifo_used_size = 0;
}
