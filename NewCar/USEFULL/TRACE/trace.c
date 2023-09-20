/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    04-August-2014
  * @brief  
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "trace.h"
#include "sensor.h"
#include "speedcontrol.h"
#include "delay.h"
#include "usart.h"
#include "positioncontrol.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ----------------------------------------------------------*/ 
#define TRACE_BLACK 0
#define TRACE_WHITE 1
/* Global param ------------------------------------------------------------*/
uint8_t isTracing = 0;
float adj_vx = 0, adj_vy = 0;
TRACE_RecordLine_t RecordLine_R;
TRACE_RecordLine_t RecordLine_F;
TRACE_RecordLine_t RecordLine_L;
TRACE_RecordLine_t RecordLine_B;

TRACE_Status_t trace_status;
/* Private functions --------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
//到点1，现在想的是，扫描完二维码后数值移动前4排传感器，在水平移动右边的

uint8_t TRACE_ToPoint1(void)
{
	uint8_t F_OnLine = 0, R_OnLine = 0;
	isTracing = 1;
	while (!F_OnLine)
	{
		SPEEDC_SetFourRamp(0,2);
		adj_vx =    0;
		adj_vy = 0.18;
		if(trace_status.Con_BlackNum_F >= 5)
		{
			F_OnLine = 1;
		}
		delay_ms(1);
	}
	adj_vx = 0;
	adj_vy = 0;
	delay_ms(100);
	while (!R_OnLine)
	{
		SPEEDC_SetFourRamp(0,2);
		adj_vx = 0.15;
		adj_vy =    0;
		if(trace_status.Con_BlackNum_R >= 5)
		{
			R_OnLine = 1;
		}
		delay_ms(1);
	}
	adj_vx = 0;
	adj_vy = 0;
	isTracing = 0;
	POSITIONC_CleanLoop();
	return 1;
}

uint8_t TRACE_ToPoint2(void)
{
	uint8_t F_OnLine = 0, L_OnLine = 0;
	isTracing = 1;
	while (!F_OnLine)
	{
		SPEEDC_SetFourRamp(0,2);
		adj_vx =    0;
		adj_vy = 0.18;
		if(trace_status.Con_BlackNum_F >= 5)
		{
			F_OnLine = 1;
		}
		delay_ms(1);
	}
	adj_vx = 0;
	adj_vy = 0;
	delay_ms(100);
	while (!L_OnLine)
	{
		SPEEDC_SetFourRamp(0,2);
		adj_vx =-0.15;
		adj_vy =    0;
		if(trace_status.Con_BlackNum_R >= 5)
		{
			L_OnLine = 1;
		}
		delay_ms(1);
	}
	adj_vx = 0;
	adj_vy = 0;
	isTracing = 0;
	POSITIONC_CleanLoop();
	return 1;
}


void Trace_FindBlackNum(int8_t* Array,int8_t* Result)
{
	int8_t i = 0;
	int8_t BlackNum = 0;
	for( ; i < 7 ; ++ i)
	{
		if(Array[i] == TRACE_BLACK)
		{
			BlackNum++;
		}
	}
	*Result = BlackNum;
}

/* Functions ------------------------------------------------------------------*/
//到点1，现在想的是，扫描完二维码后数值移动前4排传感器，在水平移动右边的
//

//车身前方循迹灯条的更新
void TRACE_UpdatF(void)
{
	trace_status.ConditionF[0] = TRACE_F1;
	trace_status.ConditionF[1] = TRACE_F2;
	trace_status.ConditionF[2] = TRACE_F3;
	trace_status.ConditionF[3] = TRACE_F4;
	trace_status.ConditionF[4] = TRACE_F5;
	trace_status.ConditionF[5] = TRACE_F6;
	trace_status.ConditionF[6] = TRACE_F7;
	Trace_FindBlackNum(trace_status.ConditionF, &trace_status.Con_BlackNum_F);
	trace_status.Con_BlackNum_F = 7 - trace_status.Con_BlackNum_F;//反逻辑
}

//车身左方循迹灯条的更新
void TRACE_UpdatL(void)
{
	trace_status.ConditionL[0] = TRACE_L1;
	trace_status.ConditionL[1] = TRACE_L2;
	trace_status.ConditionL[2] = TRACE_L3;
	trace_status.ConditionL[3] = TRACE_L4;
	trace_status.ConditionL[4] = TRACE_L5;
	trace_status.ConditionL[5] = TRACE_L6;
	trace_status.ConditionL[6] = TRACE_L7;
	Trace_FindBlackNum(trace_status.ConditionL, &trace_status.Con_BlackNum_L);
}

void TRACE_UpdatR(void)
{
	trace_status.ConditionR[0] = TRACE_R1;
	trace_status.ConditionR[1] = TRACE_R2;
	trace_status.ConditionR[2] = TRACE_R3;
	trace_status.ConditionR[3] = TRACE_R4;
	trace_status.ConditionR[4] = TRACE_R5;
	trace_status.ConditionR[5] = TRACE_R6;
	trace_status.ConditionR[6] = TRACE_R7;
	Trace_FindBlackNum(trace_status.ConditionR, &trace_status.Con_BlackNum_R);
}


void TRACE_UpdatB(void)
{
	trace_status.ConditionB[0] = TRACE_B1;
	trace_status.ConditionB[1] = TRACE_B2;
	trace_status.ConditionB[2] = TRACE_B3;
	trace_status.ConditionB[3] = TRACE_B4;
	trace_status.ConditionB[4] = TRACE_B5;
	trace_status.ConditionB[5] = TRACE_B6;
	trace_status.ConditionB[6] = TRACE_B7;
	Trace_FindBlackNum(trace_status.ConditionB, &trace_status.Con_BlackNum_B);
}

void TRACE_UpdatLoop(void)
{
	TRACE_UpdatF();
	TRACE_UpdatL();
	TRACE_UpdatR();
	TRACE_UpdatB();
}

/**
  * @brief  调整，屏蔽位置环使用速度环进行调整，向左上角靠
  * @param  None
  * @retval None
  */
void TRACE_Adjust(void)
{
	isTracing = 1;
	
	while(1)
	{
		SPEEDC_SetCarSpeed(0.5, 0, 0);
		//break;
	}//当满足条件，找到黑线退出
	isTracing = 0;
}

void TRACE_Test(void)
{
	while(1)
	{
		//OK
		printf("n:%d, %d,%d\n", trace_status.Con_BlackNum_F, trace_status.Con_BlackNum_L, trace_status.Con_BlackNum_R);
//		SPEEDC_SetTarget(0, 0, 0, 0);
//		while(trace_status.Con_BlackNum_F < 3)
//		{
//			printf("n:%d\n", trace_status.Con_BlackNum_F);
////			SPEEDC_SetTarget(5, 5, 5, 5);
//			delay_ms(10);
//		}
//		SPEEDC_SetTarget(0, 0, 0, 0);
		delay_ms(100);
	}
}
