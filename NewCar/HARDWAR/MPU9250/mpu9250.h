/**********************************************************************
* 陀螺仪型号为：JY901，陀螺仪网站为www.wit-motion.com
* 支持波特率：2400，4800，9600，19200，38400，57600，115200，230400，460800，921600
* 优先配置为460800，一个起始位，8个数据位，1个停止位，一个字节共10位，传输速度为每秒46080字节，一个字节占用时间为21.7微秒，
* 陀螺仪返回数据一帧共11个字节，以0x55开头，第二个字节为输出类型标志，角度输出为0x53，接着是8个字节的数据，最后一个字节的SUM和校验
* 一帧需要时间为22*11 = 242微秒，时间较长，故不能一直在中断里等待接收数据，要配置为DMA模式。
************************************************************************/
#ifndef __GYROSCOPE_H__
#define __GYROSCOPE_H__


#include "sys.h"
#include <stm32f4xx_usart.h>
#include <stm32f4xx_rcc.h>
#include "delay.h"
#include "fifo.h"


void TestFIFO(void);
void Mpu9250_Init(void);
void MPU_PrintMPUYaw(void);
void MPU_TestMPURate(void);
float ReadMpuYaw(void);
uint8_t MPU_IsUpdated(void);
float MPU_GetStartYaw(void);

void MPU_SetStartYaw(void);
void MPU_UserSetStartYaw(float Yaw);//强制设置初始角度

#endif


#ifndef __DATA_FIFO_H__
#define __DATA_FIFO_H__
#define __USE_LOCAL_DATA_FIFO_H__

#include "stdlib.h"
#include "stdio.h"
//定义FIFO结构


#define ASSERT(x) do {while(!(x));} while(0)

 //! FIFO Memory Model (Single Byte Mode)
typedef struct
{
    uint8_t   *start_addr;                   //Start Address
    uint8_t   *end_addr;                     //End Address
    uint32_t  free;                         //The capacity of FIFO
    uint32_t  buf_size;                     //Buffer size
    uint32_t  used;                         //The number of elements in FIFO
    uint8_t   read_index;                   //Read Index Pointer
    uint8_t   write_index;                  //Write Index Pointer
} fifo_s_t;


fifo_s_t* fifo_s_create(uint32_t unit_cnt);
void     fifo_s_destory(fifo_s_t* pfifo);
int32_t fifo_s_put(fifo_s_t* pfifo, uint8_t element);
int32_t fifo_s_puts(fifo_s_t *pfifo, uint8_t *psource, uint32_t number);

uint8_t  fifo_s_get(fifo_s_t* pfifo);
uint16_t fifo_s_gets(fifo_s_t* pfifo, uint8_t* source, uint8_t len);

uint8_t  fifo_s_pre_read(fifo_s_t* pfifo, uint8_t offset);
uint8_t  fifo_is_empty(fifo_s_t* pfifo);
uint8_t  fifo_is_full(fifo_s_t* pfifo);
uint32_t fifo_used_count(fifo_s_t* pfifo);
uint32_t fifo_free_count(fifo_s_t* pfifo);
uint8_t  fifo_flush(fifo_s_t* pfifo);
int32_t fifo_s_init(fifo_s_t* pfifo, void* base_addr, uint32_t unit_cnt);
/*user-defined*/
void clear_half_fifo(fifo_s_t* pfifo);


#endif







