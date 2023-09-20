/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    2021-7-26
  * @brief   位置环实现，很关键的文件
  * 
  ******************************************************************************
  * @attention
  * 位置环输出为速度，作为速度环的输入，中间经过速度解算，将3元位置参数
  * 变成4元轮子的速度
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "positioncontrol.h"
#include "math.h"
#include "delay.h"
#include "usart.h"
#include "sensor.h"
#include "speedsolve.h"
#include "sensor.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define STABLE_TIME 500					   //到达目标稳定时间，
#define POSITION_ACC 1.2f				   //XY位置环加速度
#define POSITION_ANGEL_ACC 360.0f //360.0f		   //角度环加速度
#define POSITION_MAX_ANGEL_Velocity 360.0f  //720.0f //最大角速度

/** @defgroup PISITION_KX_ Group
 *  @brief    位置环PID参数
 */
#define POSITION_KI_X 0.000f
#define POSITION_KP_X 0.000f
#define POSITION_KD_X 0.00000f

#define POSITION_KI_Y 0.000f
#define POSITION_KP_Y 0.000f
#define POSITION_KD_Y 0.00000f

#define POSITION_KI_Z 0.000f
#define POSITION_KP_Z 4.2000f
#define POSITION_KD_Z 0.0000f
/* Global param ------------------------------------------------------------*/
/** @paramgroup position_pid_t
 *  @brief    	位置环PID参数
 */
PID_PramTypeDef position_pidx_t;
PID_PramTypeDef position_pidy_t;
PID_PramTypeDef position_pidz_t;
POSITIONC_RecordTypeDef position_record_t;
/* Private functions --------------------------------------------------------*/
/**
  * @brief  浮点数绝对值函数
  * @param  None
  * @retval None
  */
float Abs(float x)
{
	float res;
	if (x >= 0)
		res = x;
	else
		res = -x;
	return res;
}
/* Functions ------------------------------------------------------------------*/
/**
  * @brief  清空位置环
  * @param  None
  * @retval None
  */
void POSITIONC_CleanLoop(void)
{
	//重置PID
	PID_DeInit(&position_pidx_t);
	PID_DeInit(&position_pidy_t);
	
	//清空累计脉冲值
	position_record_t.AllPulse_1 = 0;
	position_record_t.AllPulse_2 = 0;
	position_record_t.AllPulse_3 = 0;
	position_record_t.AllPulse_4 = 0;
	
	//清空目标值
	position_pidx_t.target_now = 0;
	position_pidx_t.target_now = 0;
}

/**
  * @brief  初始化位置环PID参数
  * @param  None
  * @retval None
  */
void POSITIONC_PidDeInit(void)
{
	PID_DeInit(&position_pidx_t);
	PID_DeInit(&position_pidy_t);
	PID_DeInit(&position_pidz_t);

	position_pidx_t.Kp = POSITION_KP_X;
	position_pidx_t.Ki = POSITION_KI_X;
	position_pidx_t.Kd = POSITION_KD_X;
	position_pidx_t.pid_precision=1;

	position_pidy_t.Kp = POSITION_KP_Y;
	position_pidy_t.Ki = POSITION_KI_Y;
	position_pidy_t.Kd = POSITION_KD_Y;
	position_pidy_t.pid_precision=1;

	position_pidz_t.Kp = POSITION_KP_Z;
	position_pidz_t.Ki = POSITION_KI_Z;
	position_pidz_t.Kd = POSITION_KD_Z;
	position_pidz_t.pid_precision=0;
}

/**
  * @brief  更新记录位置的参数（为每个轮子的脉冲）
  * @param  int64_t Wheel_1,int64_t Wheel_2,int64_t Wheel_3,int64_t Wheel_4
  * @retval None
  */
void POSITONC_UpdateLoop(int64_t Wheel_1, int64_t Wheel_2, int64_t Wheel_3, int64_t Wheel_4, int64_t Wheel_5, int64_t Wheel_6)
{
	position_record_t.AllPulse_1 += Wheel_1;
	position_record_t.AllPulse_2 += Wheel_2;
	position_record_t.AllPulse_3 += Wheel_3;
	position_record_t.AllPulse_4 += Wheel_4;
	position_record_t.AllPulse_5 += Wheel_5;
	position_record_t.AllPulse_6 += Wheel_6;
//	printf("pulse: %lld, %lld, %lld, %lld, %lld, %lld\n", position_record_t.AllPulse_1, position_record_t.AllPulse_2, position_record_t.AllPulse_3, 
//						position_record_t.AllPulse_4, position_record_t.AllPulse_5, position_record_t.AllPulse_6);
	position_record_t.Updata = 1;
}
POSITIONC_RecordTypeDef POSITONC_GetPositionRecord(void)
{
	return position_record_t;
}
/**
  * @brief  脉冲值修正函数
  * @param  SPEEDS_PulseOutTypeDef Err_Pulse
  * @retval None
  */
void POSITONC_CorrectLoop(SPEEDS_PulseOutTypeDef Err_Pulse)
{
	position_record_t.AllPulse_1 += Err_Pulse.pulse_1;
	position_record_t.AllPulse_2 += Err_Pulse.pulse_2;
	position_record_t.AllPulse_3 += Err_Pulse.pulse_3;
	position_record_t.AllPulse_4 += Err_Pulse.pulse_4;
}

/**
  * @brief  设置目标距离mm，角度度，最大速度m/s
  * @param  float Position_x,float Position_y,float Angel_z, float POSITION_MAX_Velocity 
  * @retval 时间
  */
uint32_t POSITONC_SetCarGoTo(float Position_x, float Position_y, float Angel_z, float max_velocity)
{
	float temp;																		 //中间变量
	float acc_z;																	 //z方向需要改变的加速度大小
	float acc_time, angle_acc_time;													 //加速时间
	float POSITION_ACC_MM = POSITION_ACC / 1000;									 //加速度常数 mm/s2
	float velocity_changein25ms_x, velocity_changein25ms_y, velocity_changein25ms_z; //每个5ms需要改变的量
	float position_all = sqrtf(Position_x * Position_x + Position_y * Position_y);	 //总位移mm
	float velocity = 0, velocity_x = 0, velocity_y = 0, velocity_z = 0;
	float move_totaltime_ms, rotate_time_ms; //

	//按照直接减速后减速推导，判断是否需要匀速阶段
	temp = 4 * position_all / POSITION_ACC_MM;
	move_totaltime_ms = sqrtf(temp);	   //移动总时间 ms
	acc_time = (move_totaltime_ms) / 2.0f; //匀加速时间 ms
	velocity = acc_time * POSITION_ACC_MM; //最大速度 m/s

	//距离足够大，有匀速运动阶段
	if (velocity > max_velocity)
	{
		move_totaltime_ms = (position_all / max_velocity + max_velocity / POSITION_ACC_MM); //移动总时间 ms
		acc_time = max_velocity / POSITION_ACC_MM;											//匀加速时间 ms
		velocity = max_velocity;
	}

	if (position_all != 0)
	{
		//算出X，Y方向最大速度的绝对值，单位是m/s
		velocity_x = velocity * Abs(Position_x) / position_all;
		velocity_y = velocity * Abs(Position_y) / position_all;

		//得出25ms应该改变的m/s量，保证一起到达最大速度
		velocity_changein25ms_x = velocity_x / acc_time * 25;
		velocity_changein25ms_y = velocity_y / acc_time * 25;
	}
	else
	{
		velocity_changein25ms_x = POSITION_ACC_MM * 25; //设置25ms的速度该变量
		velocity_changein25ms_y = POSITION_ACC_MM * 25;
	}

	//设置微分限幅参数
	position_pidx_t.pid_outstep_max = velocity_changein25ms_x;
	position_pidy_t.pid_outstep_max = velocity_changein25ms_y;

	if (velocity_x != 0)
	{
		position_pidx_t.pid_out_max = velocity_x; //通过输出限幅保证最大加速度
		//使开始减速那个点的err*p刚好等于输出限幅v_x
		position_pidx_t.Kp = 2.0f / acc_time * 0.94f;
	}
	else
	{
		position_pidx_t.pid_out_max = max_velocity;
		position_pidx_t.Kp = POSITION_KP_X;
	}

	if (velocity_y != 0)
	{
		position_pidy_t.pid_out_max = velocity_y;
		position_pidy_t.Kp = 2.0f / acc_time * 0.94f;
	}
	else
	{
		position_pidy_t.pid_out_max = max_velocity;
		position_pidy_t.Kp = POSITION_KP_Y;
	}
	position_pidx_t.target_now += Position_x; //累计目标值
	position_pidy_t.target_now += Position_y;

	//下面是z方向的计算
	acc_z = POSITION_ANGEL_ACC;
	temp = 4 * Abs(Angel_z) / acc_z;
	rotate_time_ms = sqrtf(temp) * 1000.0f;		   //计算总时间 ms
	angle_acc_time = (rotate_time_ms) / 2.0f;	   //得出匀加速时间 ms
	velocity_z = angle_acc_time * acc_z / 1000.0f; //单位是 度/s
	//产生差异的原因，传入的位置是国标，角度不是
	if (velocity_z > POSITION_MAX_ANGEL_Velocity)
	{
		rotate_time_ms = (Abs(Angel_z) / POSITION_MAX_ANGEL_Velocity + POSITION_MAX_ANGEL_Velocity / acc_z) * 1000;
		angle_acc_time = POSITION_MAX_ANGEL_Velocity / acc_z * 1000;
		velocity_z = POSITION_MAX_ANGEL_Velocity;
	}
	if (rotate_time_ms < move_totaltime_ms && rotate_time_ms > 0)
	{
		// z轴时间短，进行加长
		rotate_time_ms = move_totaltime_ms;
		angle_acc_time = acc_time;
		velocity_z = Abs(Angel_z / (rotate_time_ms - angle_acc_time) * 1000);
		acc_z = velocity_z / angle_acc_time * 1000;
	}
	velocity_changein25ms_z = 25 * acc_z / 1000; //得出25ms应该改变的量
	position_pidz_t.pid_outstep_max = velocity_changein25ms_z;
	if (velocity_z != 0)
	{
		position_pidz_t.pid_out_max = velocity_z;
		position_pidz_t.Kp = 2.0f / angle_acc_time * 1000 * 0.89f;
	}
	else
	{
		position_pidz_t.pid_out_max = POSITION_MAX_ANGEL_Velocity;
		position_pidz_t.Kp = POSITION_KP_Z;
	}

	position_pidz_t.target_now += Angel_z;

	return move_totaltime_ms;
//	printf("总位移x, y: %.2f,%.2f\r\n", Position_x, Position_y);
//	printf("总时间t_ms: %.2f\r\n", move_totaltime_ms);
//	printf("匀加速时间，最大速度: %.2f,%.2f\r\n", acc_time, velocity);
//	printf("最大速度X/Y: %.3f,%.3f\r\n", velocity_x, velocity_y);
//	printf("X/Y 计算出的KP: %.6f,%.6f\r\n", position_pidx_t.Kp, position_pidy_t.Kp);
//	printf("X/Y 微分限幅（加速度）: %.6f,%.6f\r\n", position_pidx_t.pid_outstep_max, position_pidy_t.pid_outstep_max);
//	printf("总角度: %.2f\r\n", Angel_z);
//	printf("z加速时间/z旋转时间: %.5f,%.5f\r\n", angle_acc_time, rotate_time_ms);
//	printf("最大角速度: %.6f\r\n", velocity_z);
//	printf("Z 微分限幅（加速度）: %.6f\r\n", position_pidz_t.pid_outstep_max);
//	printf("Z 计算出的KP%.6f\r\n", position_pidz_t.Kp);
	//while(1);
}

uint16_t Test_Count = 0;
/**
  * @brief  位置环循环函数, 5*5ms执行一次
  * @param  None
  * @retval None
  */
extern float adj_vx, adj_vy;
extern uint8_t isTracing;
void POSITIONC_ControlLoop(void)
{
	float tempx, tempy, tempz;
	/* 	
	整体流程
		1.通过编码器信息解算出当前位置，作为PID当前值
		2.PID解算出三个方向的速度
		3.通过逆解算得到四个轮子的速度
		4.设置电机转速
	 */
	POSITIONC_UpdateLoop();
	
	if(isTracing)
	{
		tempx = adj_vx;
		tempy = adj_vy;
		tempz = PID_GetOutput(&position_pidz_t, position_record_t.All_Position.PositionZ_D);
		SPEEDS_GetWheelSpeed(tempx, tempy, tempz);
		SpeedSolving4_Set();
		Test_Count += 1;
	}
	else
	{
		tempx = PID_GetOutput(&position_pidx_t, position_record_t.All_Position.Absolute_X_mm);
		tempy = PID_GetOutput(&position_pidy_t, position_record_t.All_Position.Absolute_Y_mm);
		tempz = PID_GetOutput(&position_pidz_t, position_record_t.All_Position.PositionZ_D);
		SPEEDS_GetWheelSpeed(tempx, tempy, tempz);
		SpeedSolving4_Set();
		Test_Count += 1;
	}
}

/**
  * @brief  只更新位置和角度，而不解算
  * @param  None
  * @retval None
  */
void POSITIONC_UpdateLoop(void)
{
		position_record_t.All_Position = SPEEDS_GetCarPosition(position_record_t.AllPulse_1, position_record_t.AllPulse_2,
														   position_record_t.AllPulse_3, position_record_t.AllPulse_4);
}


void POSITIONC_TestZ(void)
{
	int16_t Temp_ms = 0;

	POSITONC_SetCarGoTo(0, 0, 360, 0.1);
	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
			printf("PID_Y:%.2f,%.2f,%.2f\r\n", position_pidy_t.target_now, position_pidy_t.input_now,
				   position_pidy_t.out_now);

//			printf("PID_Y:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d\r\n", position_pidz_t.target_now, position_pidz_t.input_now,
//				   position_pidz_t.out_now,
//				   SPEEDC_GetTarget(1), SPEEDC_GetTarget(2), SPEEDC_GetTarget(3), SPEEDC_GetTarget(4), Temp_ms);

//			printf("PID_Y:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidz_t.target_now, position_pidz_t.input_now,
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4));
			if (Temp_ms > 6000)
				break;
		}
	}

	Temp_ms = 0;

	POSITONC_SetCarGoTo(0, 0, -360, 0.1);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
			printf("PID_Y:%.2f,%.2f,%.2f\r\n", position_pidy_t.target_now, position_pidy_t.input_now,
				   position_pidy_t.out_now); //

//			printf("PID_Y:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d\r\n", position_pidz_t.target_now, position_pidz_t.input_now,
//				   position_pidz_t.out_now,
//				   SPEEDC_GetTarget(1), SPEEDC_GetTarget(2), SPEEDC_GetTarget(3), SPEEDC_GetTarget(4), Temp_ms); //

//			printf("PID_Y:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidz_t.target_now, position_pidz_t.input_now,
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4)); //
			if (Temp_ms > 6000)
				break;
		}
	}
//	PCout(11) = 0;

	while (1)
		;
}
void POSITIONC_TestY(void)
{
	uint16_t Temp_ms = 0;

	POSITONC_SetCarGoTo(0, 2400, 0, 0.8);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_record_t.All_Position.Absolute_X_mm, position_record_t.All_Position.Absolute_Y_mm, position_record_t.All_Position.PositionZ_D,
				   position_pidy_t.out_now * 38.2f,	
				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
			if (Temp_ms > 6000)
				break;
		}
	}

	Temp_ms = 0;
	POSITONC_SetCarGoTo(-1200, 0, 0.0, 0.8);//最大1.8  1.6不行 斜着最大1

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_record_t.All_Position.Absolute_X_mm, position_record_t.All_Position.Absolute_Y_mm, position_record_t.All_Position.PositionZ_D,
				   position_pidy_t.out_now * 38.2f,	
				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
			
			
			if (Temp_ms > 6000)
				break;
		}
	}
	
	Temp_ms = 0;
	POSITONC_SetCarGoTo(0, 0, 180, 1);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_record_t.All_Position.Absolute_X_mm, position_record_t.All_Position.Absolute_Y_mm, position_record_t.All_Position.PositionZ_D,
//				   position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
			
			
			if (Temp_ms > 5000)
				break;
		}
	}
	
		
	Temp_ms = 0;
	POSITONC_SetCarGoTo(0, 2400, 0, 1.0);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_record_t.All_Position.Absolute_X_mm, position_record_t.All_Position.Absolute_Y_mm, position_record_t.All_Position.PositionZ_D,
//				   position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
			
			
			if (Temp_ms > 8000)
				break;
		}
	}
	
	Temp_ms = 0;
	POSITONC_SetCarGoTo(-1200, 0, 0, 0.8);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_record_t.All_Position.Absolute_X_mm, position_record_t.All_Position.Absolute_Y_mm, position_record_t.All_Position.PositionZ_D,
//				   position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
			
			
			if (Temp_ms > 6000)
				break;
		}
	}
	
		Temp_ms = 0;
	POSITONC_SetCarGoTo(0, 0, -180, 0.8);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_record_t.All_Position.Absolute_X_mm, position_record_t.All_Position.Absolute_Y_mm, position_record_t.All_Position.PositionZ_D,
//				   position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
//			printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f\r\n", position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
			
			
			if (Temp_ms > 6000)
				break;
		}
	}
	while (1);
//		printf("PID_Y:%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", position_record_t.All_Position.Absolute_X_mm, position_record_t.All_Position.Absolute_Y_mm, position_record_t.All_Position.PositionZ_D,
//				   position_pidy_t.out_now * 38.2f,	
//				   SPEEDC_GetPulseSpeed(1), SPEEDC_GetPulseSpeed(2), SPEEDC_GetPulseSpeed(3), SPEEDC_GetPulseSpeed(4), SPEEDC_GetPulseSpeed(5), SPEEDC_GetPulseSpeed(6)); //
}

void POSITIONC_TestX(void)
{
	int16_t Temp_ms = 0;

	POSITONC_SetCarGoTo(1200, 0, 0, 1.2);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
			//			printf("PID_X:%.2f,%.2f,%.2f\r\n",PositionRunX.PID_Position.target_now,PositionRunX.PID_Position.input_now,
			//												PositionRunX.PID_Position.out_now);//

			//			printf("PID_X:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d\r\n",PositionRunX.PID_Position.target_now,PositionRunX.PID_Position.input_now,
			//												PositionRunX.PID_Position.out_now,
			//												SPEEDC_GetTarget(1),SPEEDC_GetTarget(2),SPEEDC_GetTarget(3),SPEEDC_GetTarget(4),Temp_ms);//

			//			printf("PID_Y:%.2f,%.2f,%.2f,%lld,%lld,%lld,%lld\r\n",PositionRunX.PID_Position.target_now,PositionRunX.PID_Position.input_now,
			//												PositionRunX.PID_Position.out_now*1000.0f,
			//												position_record_t.AllPulse_1,position_record_t.AllPulse_2,
			//												position_record_t.AllPulse_3,position_record_t.AllPulse_4);
			//??????????????????????????????????????????????????

			//			printf("PID_Y:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",PositionRunX.PID_Position.target_now,PositionRunX.PID_Position.input_now,
			//												SPEEDC_GetPulseSpeed(1),SPEEDC_GetPulseSpeed(2),SPEEDC_GetPulseSpeed(3),SPEEDC_GetPulseSpeed(4));//
			if (Temp_ms > 6000)
				break;
		}
	}

	Temp_ms = 0;
	POSITONC_SetCarGoTo(-1200, 0, 0, 1.2);

	while (1)
	{
		if (Test_Count >= 1)
		{
			Test_Count = 0;
			Temp_ms += 25;
			//			printf("PID_Y:%.2f,%.2f,%.2f\r\n",PositionRunX.PID_Position.target_now,PositionRunX.PID_Position.input_now,
			//												PositionRunX.PID_Position.out_now);//

			//			printf("PID_Y:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",PositionRunX.PID_Position.target_now,PositionRunX.PID_Position.input_now,
			//												PositionRunX.PID_Position.out_now,
			//												SPEEDC_GetTarget(1),SPEEDC_GetTarget(2),SPEEDC_GetTarget(3),SPEEDC_GetTarget(4));//

			//			printf("PID_Y:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",PositionRunX.PID_Position.target_now,PositionRunX.PID_Position.input_now,
			//												SPEEDC_GetPulseSpeed(1),SPEEDC_GetPulseSpeed(2),SPEEDC_GetPulseSpeed(3),SPEEDC_GetPulseSpeed(4));//
			if (Temp_ms > 6000)
				break;
		}
	}

	while (1)
		;
}


/**
  * @brief  位置环测试函数 
  * @param  None
  * @retval None
  */
void POSITIONC_DebugPID(void)
{
	//POSITIONC_TestX();
	//POSITIONC_TestZ();
	//POSITIONC_TestY();
	//Position_Test_RunSome();
}
