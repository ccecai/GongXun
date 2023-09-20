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
 *  @brief    四个轮子的位置式PID参数
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
  * @brief  速度环PID初始化
  * @param  None
  * @retval None
  */
void SPEEDC_PidInit(void)
{
	PID_DeInit(&speed_pid1_t);
	PID_DeInit(&speed_pid2_t);
	PID_DeInit(&speed_pid3_t);
	PID_DeInit(&speed_pid4_t);
	//注意KI
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
  * @brief 	此函数在5ms定时器中反复更新各个电机的设定值
  * @param  None
  * @retval None
  */
void SPEEDC_ControlLoop(void)
{
	//速度环一直处于RAMP模式中, 传入的输入值是脉冲速度
	PID_GetOutput(&speed_pid1_t, current_speeds_t.ecd_speed1);
	PID_GetOutput(&speed_pid2_t, current_speeds_t.ecd_speed2);
	PID_GetOutput(&speed_pid3_t, current_speeds_t.ecd_speed3);
	PID_GetOutput(&speed_pid4_t, current_speeds_t.ecd_speed4);
	//即占空比可以直接和输入比较，并作为电流的输入
//	MOTER_SetCurrent(2000, 2000, 2000, 2000);
	MOTER_SetCurrent(speed_pid1_t.out_now, speed_pid2_t.out_now, speed_pid3_t.out_now, speed_pid4_t.out_now);
}

/**
  * @brief  更新每个轮子的速度值，单位n脉冲/5ms
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
	++current_speeds_t.speed_update_times; // 用于调试,每更新一次速度变化加一
}

/**
  * @brief 	获取当前的速度信息
  * @param  None
  * @retval None
  */
SPEEDC_SpeedsTypeDef SPEEDC_GetCurrentSpeed(void)
{
	return current_speeds_t;
}

/**
  * @brief 	设定速度环的目标值，由位置环给出
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
  * @brief  获取当前的速度目标值，用于调试
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
  * @brief 	设置四个轮子的斜坡次数与步幅
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
  * @brief  设置单个轮子的斜坡次数与步幅
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
  * @brief  设置小车总加速度以及角加速度，实际是设置pid_outstep_max，以25ms算
  * @param  PID_PramTypeDef * WhichPID, float Acc 单位m/s2 or 度/s2
  * @retval None
  */
void SPEEDC_SetCarAcc(PID_PramTypeDef * WhichPID, float Acc)
{
	WhichPID->ramp_target_time = 0;
	WhichPID->ramp_target_step = (Acc * 25 / 1000);//转化成25ms的速度增量
}

void SPEEDC_SetCarVel(PID_PramTypeDef * WhichPID, float v)
{
	WhichPID->PID_RunState = PID_RAMP;
	WhichPID->ramp_target = v;
}
/**
  * @brief  清除速度环pid参数
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
  * @brief 	获得轮子的真实速度 单位脉冲/5ms
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
  * @brief 	速度环测试函数，很重要
  * @param  None
  * @retval None
  */
void SPEEDC_DebugSpeedPID(void)
{
	int16_t total_time = 0;
	SPEEDC_SetTarget(600, -600, -600, -600);
	while (1)
	{
		if (current_speeds_t.speed_update_times >= 1) //1次打印一次
		{
			total_time += 5;
			current_speeds_t.speed_update_times = 0;
			//输出4个轮子的目标值、当前速度
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
			//输出4个轮子的目标值、当前速度
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
			//输出4个轮子的目标值、当前速度
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
