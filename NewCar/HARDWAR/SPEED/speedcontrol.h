/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    2021-7-25
  * @brief   �ٶȻ�
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
#define SpeedC_PRECISION 0           //���ƾ��ȣ���Ŀ���ٶ���ʵ���ٶ�С�ڴ�ֵʱ����Ϊû����ʹPID���ȶ�
#define SpeedC_ERRALL_MAX 1000       //����ERR_ALL���ֵ������ERR_ALL���ֵ���󣬻�ʹPID��Ӧ�������ȶ�
#define SpeedC_POSITION_OUT_MAX 2000 //λ��ʽPID����޷�
#define SpeedC_POSITION_STEP_MAX 1   //����λ��ʽPID��󲽷�
#define SpeedC_INCREMENT_STEP_MAX 1  //����λ��ʽPID��󲽷�

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	int16_t ecd_speed1;
	int16_t ecd_speed2;
	int16_t ecd_speed3;
	int16_t ecd_speed4;  //���������������
	int16_t ecd_speed5;
	int16_t ecd_speed6;  //���������������
	int16_t speed_update_times; //�����ø��´�����¼
	float target_speed1;
	float target_speed2;
	float target_speed3;
	float target_speed4;
} SPEEDC_SpeedsTypeDef; //�ٶȻ��ṹ��

/* Functions --------------------------------------------------------------------*/
void ENCODER_UpdateLoop(void);
void SPEEDC_ControlLoop(void);
void SPEEDC_SetTarget(float Wheel_1, float Wheel_2, float Wheel_3, float Wheel_4);
float SPEEDC_GetTarget(uint8_t Wheel_x);
void SpeedC_SetPIDNumber(uint8_t Wheel_x, uint16_t KP, uint16_t KI, uint16_t KD);                //USMART��������ʹ��
void SPEEDC_RealSpeedUpdate(int16_t Wheel_1, int16_t Wheel_2, int16_t Wheel_3, int16_t Wheel_4, int16_t Wheel_5, int16_t Wheel_6);
void SPEEDC_SetFourRamp(float RampTime, float RampStep);                                         //ͬʱ�����ĸ�б�²���
void SPEEDC_SetOneRamp(uint8_t WhichWheel, float RampTime, float RampStep);                      //λ�û����棬��Ҫ����ÿһ�����Ӳ�ͬ��б��
void SPEEDC_ClearSpeedPID(void);
float SPEEDC_GetPulseSpeed(uint8_t Wheel_x);
void SPEEDC_DebugSpeedPID(void); //����PID����
void SPEEDC_SetCarAcc(PID_PramTypeDef * WhichPID, float Acc);
void SPEEDC_SetCarVel(PID_PramTypeDef * WhichPID, float v);

SPEEDC_SpeedsTypeDef SPEEDC_GetCurrentSpeed(void);
void SPEEDC_SetCarSpeed(float vx, float vy, float vz);
void SPEEDC_PidInit(void);
#endif

/*************MADE BY LIU****************END OF FILE****/
