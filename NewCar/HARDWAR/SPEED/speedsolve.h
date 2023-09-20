/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    2021-7-27
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPEED_SOLVE_H
#define __SPEED_SOLVE_H

/* Includes ------------------------------------------------------------------*/
#include "speedcontrol.h"
/* defines --------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	float Speed_1;
	float Speed_2;
	float Speed_3;
	float Speed_4;
	float Speed_X;
	float Speed_Y;
	float Speed_Z;
	float Speed_AbsX;
	float Speed_AbsY;
	uint8_t UpData;
} SPEEDS_SpeedOutTypeDef;

typedef struct
{
	float PositionX_mm;
	float PositionY_mm;
	float PositionZ_D; //角度度
	float PositionZ_R; //角度弧度
	float Absolute_X_mm;
	float Absolute_Y_mm;
	float Last_X_mm;
	float Last_Y_mm;
} SPEEDS_PositionOutTypeDef;

typedef struct
{
	float pulse_1;
	float pulse_2;
	float pulse_3;
	float pulse_4;
} SPEEDS_PulseOutTypeDef;

/* Functions ------------------------------------------------------------------*/
SPEEDS_SpeedOutTypeDef SPEEDS_GetWheelSpeed(float Speed_x, float Speed_y, float Speed_z);
SPEEDS_PositionOutTypeDef SPEEDS_GetCarPosition(float Pulse_1, float Pulse_2, float Pulse_3, float Pulse_4);
SPEEDS_SpeedOutTypeDef SPEEDS_GetCarSpeed(void);
SPEEDS_PulseOutTypeDef PulseSolving4(float angle);
SPEEDS_PulseOutTypeDef GetErrPulse(void);
void SPEEDS_GetCurrentSpeedLoop(float s1, float s2, float s3, float s4);
void SpeedSolving4_Set(void);
void SpeedSolving4_SetWithStruct(SPEEDS_SpeedOutTypeDef *Target);
void SpeedSolving4_SetWithTime(float HowTime, SPEEDS_SpeedOutTypeDef *TargetPulse);
float SPEEDS_SpeedToPulse(float Speed);
void SPEEDS_RemoteSet(void);

#endif

/*************MADE BY LIU****************END OF FILE****/
