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
#ifndef __POSITION_CONTROL_H
#define __POSITION_CONTROL_H
/* Includes ------------------------------------------------------------------*/
#include "pidcontrol.h"
#include "speedsolve.h"
#include "sys.h"
/* defines --------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float Target_X;
  float Target_Y;
  float Target_Z;
  int64_t AllPulse_1;
  int64_t AllPulse_2;
  int64_t AllPulse_3;
  int64_t AllPulse_4;
  int64_t AllPulse_5;
  int64_t AllPulse_6;

  SPEEDS_PositionOutTypeDef All_Position; //所有的位置信息
  uint8_t Updata;                         //调试用
} POSITIONC_RecordTypeDef;
/* Functions --------------------------------------------------------------------*/

float Abs(float x);
void POSITIONC_PidDeInit(void);
void POSITONC_UpdateLoop(int64_t Wheel_1, int64_t Wheel_2, int64_t Wheel_3, int64_t Wheel_4, int64_t Wheel_5, int64_t Wheel_6);
uint32_t POSITONC_SetCarGoTo(float Position_x, float Position_y, float Angel_z, float POSITION_MAX_Velocity);
void POSITONC_CorrectLoop(SPEEDS_PulseOutTypeDef Err_Pulse);
void POSITIONC_ControlLoop(void);
void POSITIONC_DebugPID(void);
void POSITIONC_TestX(void);
void POSITIONC_TestY(void);
void POSITIONC_TestZ(void);

void POSITIONC_UpdateLoop(void);
void POSITIONC_CleanLoop(void);

POSITIONC_RecordTypeDef POSITONC_GetPositionRecord(void);
uint16_t GetSpeedTurn(void);

#endif
/*************MADE BY LIU****************END OF FILE****/
