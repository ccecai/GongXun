/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    2021-7-25
  * @brief   速度环
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPEED_CONTROL_H
#define __SPEED_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "pidcontrol.h"
/* defines --------------------------------------------------------------------*/
#define SpeedC_PRECISION 0           //控制精度，当目标速度与实际速度小于此值时，认为没有误差，使PID更稳定
#define SpeedC_ERRALL_MAX 1000       //控制ERR_ALL最大值，否则ERR_ALL最大值过大，会使PID反应慢，不稳定
#define SpeedC_POSITION_OUT_MAX 2000 //位置式PID输出限幅
#define SpeedC_POSITION_STEP_MAX 1   //限制位置式PID最大步幅
#define SpeedC_INCREMENT_STEP_MAX 1  //限制位置式PID最大步幅

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	int16_t ecd_speed1;
	int16_t ecd_speed2;
	int16_t ecd_speed3;
	int16_t ecd_speed4;  //编码器测得脉冲数
	int16_t ecd_speed5;
	int16_t ecd_speed6;  //编码器测得脉冲数
	int16_t speed_update_times; //测试用更新次数记录
	float target_speed1;
	float target_speed2;
	float target_speed3;
	float target_speed4;
} SPEEDC_SpeedsTypeDef; //速度环结构体

/* Functions --------------------------------------------------------------------*/
void ENCODER_UpdateLoop(void);
void SPEEDC_ControlLoop(void);
void SPEEDC_SetTarget(float Wheel_1, float Wheel_2, float Wheel_3, float Wheel_4);
float SPEEDC_GetTarget(uint8_t Wheel_x);
void SpeedC_SetPIDNumber(uint8_t Wheel_x, uint16_t KP, uint16_t KI, uint16_t KD);                //USMART参数调试使用
void SPEEDC_RealSpeedUpdate(int16_t Wheel_1, int16_t Wheel_2, int16_t Wheel_3, int16_t Wheel_4, int16_t Wheel_5, int16_t Wheel_6);
void SPEEDC_SetFourRamp(float RampTime, float RampStep);                                         //同时设置四个斜坡参数
void SPEEDC_SetOneRamp(uint8_t WhichWheel, float RampTime, float RampStep);                      //位置环里面，需要设置每一个轮子不同的斜坡
void SPEEDC_ClearSpeedPID(void);
float SPEEDC_GetPulseSpeed(uint8_t Wheel_x);
void SPEEDC_DebugSpeedPID(void); //调试PID参数
void SPEEDC_SetCarAcc(PID_PramTypeDef * WhichPID, float Acc);
void SPEEDC_SetCarVel(PID_PramTypeDef * WhichPID, float v);

SPEEDC_SpeedsTypeDef SPEEDC_GetCurrentSpeed(void);
void SPEEDC_SetCarSpeed(float vx, float vy, float vz);
void SPEEDC_PidInit(void);
#endif

/*************MADE BY LIU****************END OF FILE****/
