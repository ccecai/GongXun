/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    16-August-2014
  * @brief   ����ң�ؿ��� 
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "remotecontrol.h"
#include "speedsolve.h"
#include "usart.h"
#include "valpackage.h"
#include "pidcontrol.h"
#include "positioncontrol.h"
#include "math.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/
/** 
  * @brief   XXX Init structure definition  
  */ 
typedef struct
{
	float v_x;
	float v_y;
	float v_z;
	float a_xy;
	float a_z;
}REMOTEC_RecInfos;
/* Private define ------------------------------------------------------------*/
#define REMOTE_KP_X 40
#define REMOTE_KI_X 10
#define REMOTE_KD_X 0

#define REMOTE_KP_Y 1
#define REMOTE_KI_Y 0
#define REMOTE_KD_Y 0

#define REMOTE_KP_Z 40
#define REMOTE_KI_Z 10
#define REMOTE_KD_Z 0
/* Global param ------------------------------------------------------------*/
//POSITIONC_RecordTypeDef position_record_t;
PID_PramTypeDef speed_pidx_t;
PID_PramTypeDef speed_pidy_t;
PID_PramTypeDef speed_pidz_t;
REMOTEC_RecInfos received_infos;
SPEEDS_SpeedOutTypeDef remotes_out_t;
RX_PackTypeDef temp_data_t;
/* Private functions --------------------------------------------------------*/
/**
  * @brief  ������
  * @param  float A_xy, float A_z, float V_x, float V_y, float V_z
  * @retval None
  */
void REMOTEC_SetCarSpeed(float A_xy, float A_z, float V_x, float V_y, float V_z)
{
	received_infos.a_xy = A_xy;
	received_infos.a_z = A_z;
	received_infos.v_x = V_x;
	received_infos.v_y = V_y;
	received_infos.v_z = V_z;
	REMOTEC_SetWithControl();
}
/* Functions ------------------------------------------------------------------*/
/**
  * @brief  ң��ѭ������, 5*5msִ��һ��
  * @param  None
  * @retval None
  */
void REMOTEC_ControlLoop(void)
{
	static float tempx, tempy, tempz;
	/* 	
	��������
		1.ͨ����������Ϣ�������ǰλ�ã���Ϊ�������
		2.PID���������������ٶ�, Ŀ��ֵΪ��������ϵ�µ��ٶ�
		3.ͨ�������õ��ĸ����ӵ��ٶ�
		4.���õ��ת�� 25ms ����һ�ν��յ�ת��
	 */
	REMOTEC_UpdateRecInfo();
	REMOTEC_SetWithControl();
	
	POSITIONC_UpdateLoop();//���½ǶȺ�λ����Ϣ 25msһ��
	remotes_out_t = SPEEDS_GetCarSpeed(); //��λm/s
//	tempx = PID_GetOutput(&speed_pidx_t, remotes_out_t.Speed_AbsX);
//	tempy = PID_GetOutput(&speed_pidy_t, remotes_out_t.Speed_AbsY);
//	tempz = PID_GetOutput(&speed_pidz_t, remotes_out_t.Speed_Z); // ����ʱoutֵ��ͻ��Ϊ����������bug
	tempx = PID_UpdateTargetNow(&speed_pidx_t);
	tempy = PID_UpdateTargetNow(&speed_pidy_t);
	tempz = PID_UpdateTargetNow(&speed_pidz_t);

	SPEEDS_GetWheelSpeed(tempx, tempy, tempz);
	printf("t:%f, %f, %f, %f\n", tempy, remotes_out_t.Speed_AbsY, speed_pidy_t.ramp_target, speed_pidy_t.target_now);
	SpeedSolving4_Set();
	//ֱ������Ŀ��ֵ
	//SPEEDS_RemoteSet();
}

/**
  * @brief  ���½���ֵ��received_infos��
  * @param  None
  * @retval None
  */
void REMOTEC_UpdateRecInfo(void)
{
	temp_data_t = VALPACK_GetDatas();
	received_infos.a_xy = temp_data_t.floats[0];
	received_infos.a_z = temp_data_t.floats[1];
	received_infos.v_x = temp_data_t.floats[2];
	received_infos.v_y = temp_data_t.floats[3];
	received_infos.v_z = temp_data_t.floats[4];
}


/**
  * @brief  ��received_infos��ȡֵ���ٶȼ��ٶ���
  * @param  None
  * @retval None
  */
void REMOTEC_SetWithControl(void)
{
	static float a_x, a_y, a_z;
	float v_x, v_y, v_a, v_z;
	v_x = received_infos.v_x;
	v_y = received_infos.v_y;
	v_a = sqrtf(v_x * v_x + v_y * v_y);
	v_z = received_infos.v_z;
	if (v_a != 0)
	{
		a_x = Abs(received_infos.a_xy * v_x / v_a);
		a_y = Abs(received_infos.a_xy * v_y / v_a);
	}
	a_z = Abs(received_infos.a_z);

	SPEEDC_SetCarAcc(&speed_pidx_t, a_x);
	SPEEDC_SetCarAcc(&speed_pidy_t, a_y);
	SPEEDC_SetCarAcc(&speed_pidz_t, a_z);
	
	SPEEDC_SetCarVel(&speed_pidx_t, v_x);
	SPEEDC_SetCarVel(&speed_pidy_t, v_y);
	SPEEDC_SetCarVel(&speed_pidz_t, v_z);
}

/**
  * @brief  �����ٶȿ��ƹ��ܺ���
  * @param  None
  * @retval None
  */
void REMOTEC_Debug(void)
{
	while (1)
	{
		REMOTEC_SetCarSpeed(0.4, 30, 0, 0.7, 0);
		delay_ms(3000);                        
		REMOTEC_SetCarSpeed(0.4, 30, 0, 0, 0);
		delay_ms(4000);
//		REMOTEC_SetCarSpeed(1, 30, 0, -0.3, -90);
//	//	while(1);
//		delay_ms(2000); 
	}
                  
//	REMOTEC_SetCarSpeed(0.0003, 1, 0, 0.8, 50);
//	delay_ms(2000);                        
//	REMOTEC_SetCarSpeed(0.0003, 1, 0, 0.2, 50);
}

/**
  * @brief  ��ʼ��pid�ٶȿ����⻷
  * @param  None
  * @retval None
  */
void REMOTEC_Init(void)
{
	PID_DeInit(&speed_pidx_t);
	PID_DeInit(&speed_pidy_t);
	PID_DeInit(&speed_pidz_t);
	
	speed_pidx_t.Kp = REMOTE_KP_X;
	speed_pidx_t.Ki = REMOTE_KI_X;
	speed_pidx_t.Kd = REMOTE_KD_X;
	speed_pidx_t.PID_RunState = PID_RAMP;
	
	speed_pidy_t.Kp = REMOTE_KP_Y;
	speed_pidy_t.Ki = REMOTE_KI_Y;
	speed_pidy_t.Kd = REMOTE_KD_Y;
	speed_pidy_t.PID_RunState = PID_RAMP;
	
	speed_pidz_t.Kp = REMOTE_KP_Z;
	speed_pidz_t.Ki = REMOTE_KI_Z;
	speed_pidz_t.Kd = REMOTE_KD_Z;
	speed_pidz_t.PID_RunState = PID_RAMP;
	
	received_infos.a_xy = 0;
	received_infos.a_z 	= 0;
	received_infos.v_x 	= 0;
	received_infos.v_y 	= 0;
	received_infos.v_z 	= 0;
}
