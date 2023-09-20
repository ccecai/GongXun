/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TRACE_H
#define __TRACE_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines --------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct 
{
	//白线是1 黑线是0
	int8_t ConditionR[7];//实时存储灯条状态，右方
	uint8_t ConditionByte_R;//实时存储灯条状态，
	int8_t Con_RightOnBlack_R;//最右边哪个灯在黑线上
	int8_t Con_LeftOnBlack_R;//最左边哪个灯在黑线上
	int8_t Con_BlackNum_R;
	
	int8_t ConditionF[7];//实时存储灯条状态，前方
	uint8_t ConditionByte_F;//实时存储灯条状态，前方
	int8_t Con_RightOnBlack_F;//最右边哪个灯在黑线上
	int8_t Con_LeftOnBlack_F;//最左边哪个灯在黑线上
	int8_t Con_BlackNum_F;//黑色灯的数目，0是全白，大于等于TCK_ALL_BLACK_NUM为全黑
	
	int8_t ConditionL[7];//实时存储灯条状态，左方
	uint8_t ConditionByte_L;//实时存储灯条状态，
	int8_t Con_RightOnBlack_L;//最右边哪个灯在黑线上
	int8_t Con_LeftOnBlack_L;//最左边哪个灯在黑线上
	int8_t Con_BlackNum_L;
	
	int8_t ConditionB[7];//实时存储灯条状态，后方
	uint8_t ConditionByte_B;//实时存储灯条状态，
	int8_t Con_RightOnBlack_B;//最右边哪个灯在黑线上
	int8_t Con_LeftOnBlack_B;//最左边哪个灯在黑线上
	int8_t Con_BlackNum_B;
	
	float LastPosition_X;
	float LastPosition_Y;
	float NowPosition_X;
	float NowPosition_Y;
	int64_t UpdatePositionTime_X;
	int64_t UpdatePositionTime_Y;
}TRACE_Status_t; 		//循迹灯条状态

typedef struct
{
	int8_t FirstOnBlack;//进黑线后变为1，此时立即记下当前坐标，表示需要验证。验证通过后，如果需要更新进黑线Y+数据则变为2，如果需要更新进黑线Y-数据则变为-2，表示需要更新到坐标，更新到坐标后变为0
	int8_t EnterBlackNum;//进了几次黑线
	int8_t IsMainBias;//是否是主要修正定位仪编码器的灯条。1表示是，0表示不是
	float  FirstPosition_Y;//进黑线时坐标
	float  LastCredibleBias_Y;
	float  CredibleBias_Y;//可信偏差
	
	float  NowCredible_Y;//可信线数
	int8_t NowCredibleState;//3是需要验证进黑线+，-3需要验证进黑线-，4是进黑线不知道什么，5是出黑线不知道什么，6是验证出黑线+,-6是验证出黑线-
	float  LastPosition_Y;//上次进黑线或者出黑线时坐标
	int64_t LastTime_ms;//上次进黑线或者出黑线时的时间
	float  MayLines_Y;//可能线数
	
	int8_t EnterOrOut;//进黑线或者出黑线，1是在黑线里面
}TRACE_RecordLine_t;//记录每个灯条数过的XY方向线


/* Functions -----------------------------------------------------------------*/
void TRACE_UpdatLoop(void);
void TRACE_Test(void);
uint8_t TRACE_ToPoint1(void);
uint8_t TRACE_ToPoint2(void);
void TRACE_Adjust(void);
#endif 

/*************MADE BY LIU****************END OF FILE****/
