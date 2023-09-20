/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version  V1.0.0
  * @date     2021-7-25
  * @brief  	
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "pidcontrol.h"
#include "speedcontrol.h"
#include "speedsolve.h"
#include "moter.h"
#include "usart.h"
#include "moter.h"
#include "delay.h"

#include "positioncontrol.h"

/* Private define ------------------------------------------------------------*/
/** @defgroup PID_KP_POSITION GROUP
 *  @brief    �ĸ����ӵ�λ��ʽPID����
 *
 */
 // 47    2000  40
#define SPEED_KP_1 16.6000f
#define SPEED_KI_1 1.0000f
#define SPEED_KD_1 0.00000f

#define SPEED_KP_2 16.6000f
#define SPEED_KI_2 1.00000f
#define SPEED_KD_2 0.00000f

#define SPEED_KP_3 16.6000f
#define SPEED_KI_3 1.0000f
#define SPEED_KD_3 0.00000f

#define SPEED_KP_4 16.6000f
#define SPEED_KI_4 1.0000f
#define SPEED_KD_4 0.00000f


/* Private typedef -----------------------------------------------------------*/
/* Global param ------------------------------------------------------------*/
SPEEDC_SpeedsTypeDef current_speeds_t;

PID_PramTypeDef speed_pid1_t;
PID_PramTypeDef speed_pid2_t;
PID_PramTypeDef speed_pid3_t;
PID_PramTypeDef speed_pid4_t;

/* Private functions --------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/

void SPEEDC_SetCarSpeed(float vx, float vy, float vz)
{
	SPEEDS_GetWheelSpeed(vx, vy, vz);
	SpeedSolving4_Set();
}

	
	
/**
  * @brief  �ٶȻ�PID��ʼ��
  * @param  None
  * @retval None
  */
void SPEEDC_PidInit(void)
{
	PID_DeInit(&speed_pid1_t);
	PID_DeInit(&speed_pid2_t);
	PID_DeInit(&speed_pid3_t);
	PID_DeInit(&speed_pid4_t);
	//ע��KI
	speed_pid1_t.Kp = SPEED_KP_1;
	speed_pid1_t.Ki = SPEED_KI_1;
	speed_pid1_t.Kd = SPEED_KD_1;
	speed_pid1_t.ramp_target_step = 10;
	speed_pid1_t.pid_err_max = 20000;
	speed_pid1_t.pid_outstep_max = 1000;

	speed_pid2_t.Kp = SPEED_KP_2;
	speed_pid2_t.Ki = SPEED_KI_2;
	speed_pid2_t.Kd = SPEED_KD_2;
	speed_pid2_t.ramp_target_step = 10;
	speed_pid2_t.pid_err_max = 20000;
	speed_pid2_t.pid_outstep_max = 1000;
	
	speed_pid3_t.Kp = SPEED_KP_3;
	speed_pid3_t.Ki = SPEED_KI_3;
	speed_pid3_t.Kd = SPEED_KD_3;
	speed_pid3_t.ramp_target_step = 10;
	speed_pid3_t.pid_err_max = 20000;
	speed_pid3_t.pid_outstep_max = 1000;

	speed_pid4_t.Kp = SPEED_KP_4;
	speed_pid4_t.Ki = SPEED_KI_4;
	speed_pid4_t.Kd = SPEED_KD_4;
	speed_pid4_t.ramp_target_step = 10;
	speed_pid4_t.pid_err_max = 20000;
	speed_pid4_t.pid_outstep_max = 1000;
}

/**
  * @brief 	�˺�����5ms��ʱ���з������¸���������趨ֵ
  * @param  None
  * @retval None
  */
void SPEEDC_ControlLoop(void)
{
	//�ٶȻ�һֱ����RAMPģʽ��, ���������ֵ�������ٶ�
	PID_GetOutput(&speed_pid1_t, current_speeds_t.ecd_speed1);
	PID_GetOutput(&speed_pid2_t, current_speeds_t.ecd_speed2);
	PID_GetOutput(&speed_pid3_t, current_speeds_t.ecd_speed3);
	PID_GetOutput(&speed_pid4_t, current_speeds_t.ecd_speed4);
	//��ռ�ձȿ���ֱ�Ӻ�����Ƚϣ�����Ϊ����������
//	MOTER_SetCurrent(2000, 2000, 2000, 2000);
	MOTER_SetCurrent(speed_pid1_t.out_now, speed_pid2_t.out_now, speed_pid3_t.out_now, speed_pid4_t.out_now);
}

/**
  * @brief  ����ÿ�����ӵ��ٶ�ֵ����λn����/5ms
  * @param  None
  * @retval None
  */
void SPEEDC_RealSpeedUpdate(int16_t Wheel_1, int16_t Wheel_2, int16_t Wheel_3, int16_t Wheel_4, int16_t Wheel_5, int16_t Wheel_6)
{
	current_speeds_t.ecd_speed1 = Wheel_1;
	current_speeds_t.ecd_speed2 = Wheel_2;
	current_speeds_t.ecd_speed3 = Wheel_3;
	current_speeds_t.ecd_speed4 = Wheel_4;
	current_speeds_t.ecd_speed6 = Wheel_5;
	current_speeds_t.ecd_speed6 = Wheel_6;
	++current_speeds_t.speed_update_times; // ���ڵ���,ÿ����һ���ٶȱ仯��һ
}

/**
  * @brief 	��ȡ��ǰ���ٶ���Ϣ
  * @param  None
  * @retval None
  */
SPEEDC_SpeedsTypeDef SPEEDC_GetCurrentSpeed(void)
{
	return current_speeds_t;
}

/**
  * @brief 	�趨�ٶȻ���Ŀ��ֵ����λ�û�����
  * @param  float Wheel_1, float Wheel_2, float Wheel_3, float Wheel_4
  * @retval None
  */
void SPEEDC_SetTarget(float wheelSpeed1, float wheelSpeed2, float wheelSpeed3, float wheelSpeed4)
{
	PID_SetTargetWithRamp(&speed_pid1_t, wheelSpeed1);
	PID_SetTargetWithRamp(&speed_pid2_t, wheelSpeed2);
	PID_SetTargetWithRamp(&speed_pid3_t, wheelSpeed3);
	PID_SetTargetWithRamp(&speed_pid4_t, wheelSpeed4);
}

/**
  * @brief  ��ȡ��ǰ���ٶ�Ŀ��ֵ�����ڵ���
  * @param  uint8_t Wheel_x
  * @retval None
  */
float SPEEDC_GetTarget(uint8_t Wheel_x)
{
	switch (Wheel_x)
	{
	case 1:
		return speed_pid1_t.target_now;
	case 2:
		return speed_pid2_t.target_now;
	case 3:
		return speed_pid3_t.target_now;
	case 4:
		return speed_pid4_t.target_now;
	}
	return 0;
}

/**
  * @brief 	�����ĸ����ӵ�б�´����벽��
  * @param  float RampTime, float RampStep
  * @retval None
  */
void SPEEDC_SetFourRamp(float RampTime, float RampStep)
{
	speed_pid1_t.ramp_target_time = RampTime;
	speed_pid1_t.ramp_target_step = RampStep;

	speed_pid2_t.ramp_target_time = RampTime;
	speed_pid2_t.ramp_target_step = RampStep;

	speed_pid3_t.ramp_target_time = RampTime;
	speed_pid3_t.ramp_target_step = RampStep;

	speed_pid4_t.ramp_target_time = RampTime;
	speed_pid4_t.ramp_target_step = RampStep;
}

/**
  * @brief  ���õ������ӵ�б�´����벽��
  * @param  uint8_t WhichWheel, float RampTime, float RampStep
  * @retval None
  */
void SPEEDC_SetOneRamp(uint8_t WhichWheel, float RampTime, float RampStep)
{
	switch (WhichWheel)
	{
	case 1:
	{
		speed_pid1_t.ramp_target_time = RampTime;
		speed_pid1_t.ramp_target_step = RampStep;
		break;
	}
	case 2:
	{
		speed_pid2_t.ramp_target_time = RampTime;
		speed_pid2_t.ramp_target_step = RampStep;
		break;
	}
	case 3:
	{
		speed_pid3_t.ramp_target_time = RampTime;
		speed_pid3_t.ramp_target_step = RampStep;
		break;
	}
	case 4:
	{
		speed_pid4_t.ramp_target_time = RampTime;
		speed_pid4_t.ramp_target_step = RampStep;
		break;
	}
	}
}

/**
  * @brief  ����С���ܼ��ٶ��Լ��Ǽ��ٶȣ�ʵ��������pid_outstep_max����25ms��
  * @param  PID_PramTypeDef * WhichPID, float Acc ��λm/s2 or ��/s2
  * @retval None
  */
void SPEEDC_SetCarAcc(PID_PramTypeDef * WhichPID, float Acc)
{
	WhichPID->ramp_target_time = 0;
	WhichPID->ramp_target_step = (Acc * 25 / 1000);//ת����25ms���ٶ�����
}

void SPEEDC_SetCarVel(PID_PramTypeDef * WhichPID, float v)
{
	WhichPID->PID_RunState = PID_RAMP;
	WhichPID->ramp_target = v;
}
/**
  * @brief  ����ٶȻ�pid����
  * @param  None
  * @retval None
  */
void SPEEDC_ClearSpeedPID(void)
{
	PID_Clear(&speed_pid1_t);
	PID_Clear(&speed_pid2_t);
	PID_Clear(&speed_pid3_t);
	PID_Clear(&speed_pid4_t);
}

/**
  * @brief 	������ӵ���ʵ�ٶ� ��λ����/5ms
  * @param  uint8_t Wheel_x
  * @retval None
  */
float SPEEDC_GetPulseSpeed(uint8_t Wheel_x)
{
	switch (Wheel_x)
	{
	case 1:
		return current_speeds_t.ecd_speed1;
	case 2:
		return current_speeds_t.ecd_speed2;
	case 3:
		return current_speeds_t.ecd_speed3;
	case 4:
		return current_speeds_t.ecd_speed4;
	case 5:
		return current_speeds_t.ecd_speed5;
	case 6:
		return current_speeds_t.ecd_speed6;
	}
	return 0;
}

/**
  * @brief 	�ٶȻ����Ժ���������Ҫ
  * @param  None
  * @retval None
  */
void SPEEDC_DebugSpeedPID(void)
{
	int16_t total_time = 0;
	SPEEDC_SetTarget(600, -600, -600, -600);
	while (1)
	{
		if (current_speeds_t.speed_update_times >= 1) //1�δ�ӡһ��
		{
			total_time += 5;
			current_speeds_t.speed_update_times = 0;
			//���4�����ӵ�Ŀ��ֵ����ǰ�ٶ�
//			printf("v1,v2,v3,target:%d,%d,%d,%d,%.2f\n\r", current_speeds_t.real_pulse_speed1, current_speeds_t.real_pulse_speed2, current_speeds_t.real_pulse_speed3,
//				   current_speeds_t.real_pulse_speed4, speed_pid4_t.target_now);
			printf("one:%d,%f,%f,%f\n\r", current_speeds_t.ecd_speed3, speed_pid3_t.out_now, speed_pid3_t.input_now,
				   speed_pid3_t.target_now);
			if (total_time >= 5000)
			{
				break;
			}
		}
	}
	total_time = 0;
	SPEEDC_SetTarget(0, 0, 0, 0);
	while (1)
	{
		if (current_speeds_t.speed_update_times >= 1)
		{
			total_time += 5;
			current_speeds_t.speed_update_times = 0;
			//���4�����ӵ�Ŀ��ֵ����ǰ�ٶ�
//			printf("v1,v2,v3,target:%d,%d,%d,%d,%.2f\n\r", current_speeds_t.real_pulse_speed1, current_speeds_t.real_pulse_speed2, current_speeds_t.real_pulse_speed3,
//				   current_speeds_t.real_pulse_speed4, speed_pid4_t.target_now);
			printf("one:%d,%f,%f,%f\n\r", current_speeds_t.ecd_speed3, speed_pid3_t.out_now, speed_pid3_t.input_now,
				   speed_pid3_t.target_now);
			if (total_time >= 6000)
			{
				break;
			}
		}
	}

	while (1)
	{
		if (current_speeds_t.speed_update_times >= 1)
		{
			total_time += 5;
			current_speeds_t.speed_update_times = 0;
			//���4�����ӵ�Ŀ��ֵ����ǰ�ٶ�
//			printf("v1,v2,v3,target:%d,%d,%d,%d,%.2f\n\r", current_speeds_t.real_pulse_speed1, current_speeds_t.real_pulse_speed2, current_speeds_t.real_pulse_speed3,
//				   current_speeds_t.real_pulse_speed4, speed_pid4_t.target_now);
			printf("one:%d,%f,%f,%f\n\r", current_speeds_t.ecd_speed3, speed_pid3_t.out_now, speed_pid3_t.input_now,
				   speed_pid3_t.target_now);
			if (total_time >= 6000)
			{
//				break;
			}
		}
	}
}
