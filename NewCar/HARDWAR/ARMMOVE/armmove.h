/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    
  * @brief   机械臂
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ARM_MOVE_H
#define __ARM_MOVE_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines --------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float arm_len1;
  float arm_len2;
  float arm_len3;
  float arm_len4;

} ARMM_ParamTypeDef;

typedef struct
{
  float pulese_begin;
  float pulese_end;
  float angle_begin;
  float angle_end;
} ARMM_ServoParamTypeDef;
/* Functions --------------------------------------------------------------------*/
void ARM_Init(void);
void Point1_Grab(int i);
void Point2_Put(int i);
void Point2_Grab(int i);
void Point3_Stack(int i);

void ARMM_Position1_GetTop(uint8_t * code_order, uint8_t * color_order);
void ARMM_Position1_GetBottom();
void ARMM_Position2_PutTop(int8_t color_order[6]);
void ARMM_Position2_PutBottom(uint8_t * color_order);
void ARMM_Position2_GetAgain(int8_t color_order[6]);
void ARMM_Position3_Stack(uint8_t * color_order);
void ARMM_Test(void);
void ARMM_DeInit(void);
void ARMM_ServoInit(void);
ARMM_ParamTypeDef ARMM_GetParam(ARMM_ParamTypeDef);
void ARMM_SetServoRange(ARMM_ServoParamTypeDef); //设置舵机工作范围脉冲对应的角度
void ARMM_SetServoAngle(void);                   //将解算得到的角度设置成脉冲值给舵机
void GrabUp_1(void);
void ARMM_RePosition2_PutTop(uint8_t color_order[6]);
void ARMM_RePosition2_GetAgain( uint8_t color_order[6]);
void Put_theLeft_Again(void);
void Put_theMid_Again(void);
void Put_theRight_Again(void);
void Put_theLeft_GetAgain(void);
void Put_theMid_GetAgain(void);
void Put_theRight_GetAgain(void);
void MoveFrom_CarLeft_GetAgain(void);
void MoveFrom_CarMid_GetAgain(void);
void MoveFrom_CarRight_GetAgain(void);
void Point2_Put_GetAgain(int i);
void Point2_Put_Stack(int i);
void Put_theLeft_Stack(void);
void Put_theMid_Stack(void);
void Put_theRight_Stack(void);
void MoveFrom_CarMid_Stack(void);
void MoveFrom_CarLeft_Stack(void);
void MoveFrom_CarRight_Stack(void);

#endif
/*************MADE BY LIU****************END OF FILE****/
