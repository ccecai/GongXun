#ifndef _SENSOR_H
#define _SENSOR_H
#include "sys.h"
#define ONTHELINE OnTheLine()

#define LED0 PDout(10)
#define LED1 PDout(11)

/*
PE0  PF0 PF11 PD0
PE1  PF1 PF12 PD1
PE2  PF2 PF13 PD2
PE3  PF3 PF14 PD3
*/
// 正向靠右
#define TRACE_R1 PCin(0)
#define TRACE_R2 PCin(1)
#define TRACE_R3 PCin(2)
#define TRACE_R4 PCin(3)
#define TRACE_R5 PCin(4)
#define TRACE_R6 PCin(5)
#define TRACE_R7 PCin(13)

// 正向靠前
#define TRACE_F1 PGin(0)
#define TRACE_F2 PGin(1)
#define TRACE_F3 PGin(2)
#define TRACE_F4 PGin(3)
#define TRACE_F5 PGin(4)
#define TRACE_F6 PGin(5)
#define TRACE_F7 PGin(6)

// 正向靠左
#define TRACE_L1 PDin(3)
#define TRACE_L2 PDin(4)
#define TRACE_L3 PDin(7)
#define TRACE_L4 PDin(10)
#define TRACE_L5 PDin(11)
#define TRACE_L6 PDin(14)
#define TRACE_L7 PDin(15)

// 正向靠后
#define TRACE_B1 PEin(0)
#define TRACE_B2 PEin(1)
#define TRACE_B3 PEin(2)
#define TRACE_B4 PEin(3)
#define TRACE_B5 PEin(4)
#define TRACE_B6 PEin(7)
#define TRACE_B7 PEin(8)

void SENSOR_ControlLoop(void);
void SENSOR_Err_Calc(void);
float SENSOR_GetErr_Z(void);
char OnTheLine(void);//留着做数线？
void SENSOR_Init(void);
void SENSOR_Test(void);
void TIM12_Cap_Init(uint16_t psc,uint32_t arr);

void sensor_test(void);
#endif
