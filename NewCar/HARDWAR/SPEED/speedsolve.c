/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version  V1.0.0
  * @date    2021-7-26
  * @brief  
  * 
  ******************************************************************************
  * @attention
  * 四轮速度解算，参考https://zhuanlan.zhihu.com/p/20282234
	* Speed_x表示 X 轴运动的速度，即左右方向，定义向右为正；
	*	Speed_y表示 Y 轴运动的速度，即前后方向，定义向前为正；
	*	Speed_z表示 yaw 轴自转的角速度，定义逆时针为正
	*   和为179.6 a95 b89
  ******************************************************************************
	* 优化：解算函数中不应该有设定函数，分割开来
  */

/* Private Includes ----------------------------------------------------------*/
#include "usart.h"
#include "speedsolve.h"
#include "positioncontrol.h"
#include "math.h"
#include "gyroscope.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup 
 *  @brief    速度转换所需参数
 */
#define SPEEDS_DATA_N2S 0.20420352f	 // 1000 * 60 / 5.0f / 1560.0f, 转速转化成真实速度
#define SPEEDS_DATA_D2R 0.01745329f	 //度转换为弧度 3.1415926f/180.0f
#define SPEEDS_DATA_W2N 9.549296586f //角速度转为转速要乘的比例，即N=W*60/2pi=9.549*W
#define SPEEDS_DATA_R2D 57.2957795f	 //D=O/3.1415926*180
#define SPEEDS_DATA_W2P	47.746483f	 //角速度到脉冲速度pulse/5ms (60000*5/(2pi*1000))

/** @defgroup SPEED_SOLVING_DATA Group
 *  @brief    速度结算所需要的参数 
 */
#define SPEED_SOLVING4_DATA_A 120.17f					 //小车横向尺寸的半宽，即旋转中心到左边轮子的距离。单位是mm
#define SPEED_SOLVING4_DATA_B 101.2f					 //小车纵向尺寸的半长，即旋转中心到前边轮子的距离。单位是mm
#define SPEED_SOLVING4_DATA_R_Y 38.935f				 	//麦轮半径(mm)，通过位移计算出的半径。这个半径适用于Y方向
#define SPEED_SOLVING4_DATA_R_X 37.10f					 //麦轮半径(mm)，通过位移计算出的半径。这个半径适用于X方向
#define SPEED_SOLVING4_DATA_R_Z 38.5f					 //麦轮半径(mm)，通过位移计算出的半径。这个半径适用于Z方向
#define SPEED_SOLVING4_DATA_R 38.5f				 //卖家给的麦轮半径。这个也是理论半径，只用这个值会产生误差
#define SPEED_SOLVING4_DATA_TAN_A 1.0f				 //tan(a)，及角度阿尔法的正切值
#define SPEED_SOLVING4_DATA_MOTOR_N 369.23f			 //电机最快转速
#define SPEED_SOLVING4_DATA_MOTOR_PWM 2000.0f		 //PWM最大值
#define SPEED_SOLVING4_DATA_MAX_PULSE 2000.0f			 //5ms电机最大转速最大值,可以直接计算
#define SPEED_SOLVING4_DATA_ENCODE_PULSE_MAX 60000.0f //(500*30*4.0f)	//轮子转一圈对应的编码器脉冲数,四倍频，但实际有两个轮子是20减速比
#define SPEED_SOLVING_RAMP_TIME 4.0f				 //经4次使斜坡达到最终值

/* Global param ------------------------------------------------------------*/
SPEEDS_SpeedOutTypeDef speed_out_t;
SPEEDS_PositionOutTypeDef position_out_t;
SPEEDS_PulseOutTypeDef pulse_out_t;
/* Private functions --------------------------------------------------------*/

/**
  * @brief  脉冲变化转化为角速度
  * @param  None
  * @retval None
  */
float SpeedS_PToW(float Pulse_B)
{
	static float temp = 2 * 3.1415926 / SPEED_SOLVING4_DATA_ENCODE_PULSE_MAX; //编码器脉冲转为角度要乘的比例，
	return (Pulse_B * temp);
}

/* Functions ------------------------------------------------------------------*/

/**
  * @brief  速度m/s转化为脉冲 5ms
  * @param  float Speed
  * @retval uint32_t pulse
  */
float SPEEDS_SpeedToPulse(float Speed)
{
	float n;
	float pulse;
	n = Speed * 1000/ (SPEED_SOLVING4_DATA_R * 2 * 3.1415926f);
	pulse = n * 5 / 1000 * SPEED_SOLVING4_DATA_ENCODE_PULSE_MAX;
	return pulse;
}

/**
  * @brief  速度5ms脉冲速度转化为m/s
  * @param  float Speed
  * @retval uint32_t pulse
  */
float SPEEDS_PulseToSpeed(float Pulse)
{
	float n;
	float speed;
	n = Pulse / SPEED_SOLVING4_DATA_ENCODE_PULSE_MAX * 1000 / 5;
	speed = n / 1000 * (SPEED_SOLVING4_DATA_R * 2 * 3.1415926f);
	
	return speed;
}

/**
  * @brief 	逆解算，xyz 得到 speed1234, 积分则是给位置姿态得脉冲数
  * @param  float Speed_x,float Speed_y,float Speed_z，传入m/s
  * @retval speed_out_t
  */
SPEEDS_SpeedOutTypeDef SPEEDS_GetWheelSpeed(float speed_x, float speed_y, float speed_z)
{
	static float SPEEDS_DATA1 = 1 / (SPEED_SOLVING4_DATA_R_X * SPEED_SOLVING4_DATA_TAN_A);
	static float SPEEDS_DATA2 = 1 / SPEED_SOLVING4_DATA_R_Y;	
	static float SPEEDS_DATA3 = (SPEED_SOLVING4_DATA_TAN_A * SPEED_SOLVING4_DATA_A + SPEED_SOLVING4_DATA_B)
								/ (SPEED_SOLVING4_DATA_R_Z * SPEED_SOLVING4_DATA_TAN_A);
	static float w1, w2, w3, w4;				//角速度
	float speed_x_temp, speed_y_temp;
	speed_x_temp = speed_x * 1000;		 //单位从m/s转为mm/s
	speed_y_temp = speed_y * 1000;		 //单位从m/s转为mm/s
	speed_z = speed_z * SPEEDS_DATA_D2R; //传入角度得到弧度
	//position_out_t.PositionZ_R = 0;
	// 世界坐标解算得到的为世界坐标对应的vx,vy
	float cos_radz = cos(position_out_t.PositionZ_R);
	float sin_radz = sin(position_out_t.PositionZ_R);
	
	// 让车走的是车坐标下的vx，vy 所以需要坐标逆变换
//	Speed_x = cos_radz * speed_x_temp + sin_radz * speed_y_temp;
//	Speed_y = -sin_radz * speed_x_temp + cos_radz * speed_y_temp;
	speed_x =speed_x_temp;
	speed_y =speed_y_temp;
	// mm/s */mm = rad/s 	均除以一个轮子半径 rad/s
	w1 = -speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 + speed_z * SPEEDS_DATA3;
	w2 =  speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 - speed_z * SPEEDS_DATA3;
	w3 = -speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 - speed_z * SPEEDS_DATA3;
	w4 =  speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 + speed_z * SPEEDS_DATA3;

	// 后面的常数只是把角速度转化脉冲速度, 转化后速度单位是pulse/5ms
	speed_out_t.Speed_1 = w1 * SPEEDS_DATA_W2P;		//角速度到脉冲数
	speed_out_t.Speed_2 = w2 * SPEEDS_DATA_W2P;
	speed_out_t.Speed_3 = w3 * SPEEDS_DATA_W2P;
	speed_out_t.Speed_4 = w4 * SPEEDS_DATA_W2P;		//(600000*5/(2pi*1000))
	
	speed_out_t.UpData = 1; //调试用
	return speed_out_t;
}


/**
  * @brief  正解算，通过脉冲数得位置,输出mm 度
  * @param  float Pulse_1,float Pulse_2,float Pulse_3,float Pulse_4
  * @retval position_out_t
  */
SPEEDS_PositionOutTypeDef SPEEDS_GetCarPosition(float Pulse_1, float Pulse_2, float Pulse_3, float Pulse_4)
{
	static float angel_1, angel_2, angel_3, angel_4;
	static float data_1 = 0.25f * SPEED_SOLVING4_DATA_R_X * SPEED_SOLVING4_DATA_TAN_A;																			   //正解算矩阵里面的第一行公算子，算了1/4
	static float data_2 = 0.25f * SPEED_SOLVING4_DATA_R_Y;																										   //正解算矩阵里面的第二行公算子
	static float data_3 = 0.25f * SPEED_SOLVING4_DATA_R_Z * SPEED_SOLVING4_DATA_TAN_A / (SPEED_SOLVING4_DATA_TAN_A * SPEED_SOLVING4_DATA_A + SPEED_SOLVING4_DATA_B); //正解算矩阵里面的第三行公算子
	double cos_radz, sin_radz;
	float derta_x, derta_y;
	angel_1 = SpeedS_PToW(Pulse_1);
	angel_2 = SpeedS_PToW(Pulse_2);
	angel_3 = SpeedS_PToW(Pulse_3);
	angel_4 = SpeedS_PToW(Pulse_4);
	position_out_t.PositionX_mm = (-angel_1 + angel_2 - angel_3 + angel_4) * data_1;
	position_out_t.PositionY_mm = (angel_1  + angel_2 + angel_3 + angel_4) * data_2;
	position_out_t.PositionZ_R =  (angel_1  - angel_2 - angel_3 + angel_4) * data_3;
	cos_radz = cosf(position_out_t.PositionZ_R);
	sin_radz = sinf(position_out_t.PositionZ_R);
//	derta_x = position_out_t.PositionX_mm - position_out_t.Last_X_mm;
//	derta_y = position_out_t.PositionY_mm - position_out_t.Last_Y_mm;//使用当前角度的位移变化转换为世界坐标的位移

//	position_out_t.Absolute_X_mm += cos_radz * derta_x - sin_radz * derta_y;
//	position_out_t.Absolute_Y_mm += sin_radz * derta_x + cos_radz * derta_y;
	
	position_out_t.Absolute_X_mm =	position_out_t.PositionX_mm;
	position_out_t.Absolute_Y_mm =  position_out_t.PositionY_mm;

	position_out_t.PositionZ_D = MPU_GetBiasToStartYaw();
//	position_out_t.PositionZ_D = position_out_t.PositionZ_R * SPEEDS_DATA_R2D;
//	position_out_t.PositionZ_D = 0;
	position_out_t.Last_X_mm = position_out_t.PositionX_mm;
	position_out_t.Last_Y_mm = position_out_t.PositionY_mm;
	return position_out_t;
}

/**
  * @brief  正解算，通过脉冲速度（车轮速）得车速,输出m/s 度/s
  * @param  float Pulse_1,float Pulse_2,float Pulse_3,float Pulse_4
  * @retval position_out_t
  */
SPEEDS_SpeedOutTypeDef SPEEDS_GetCarSpeed(void)
{
	double cos_radz, sin_radz;
	float v1, v2, v3, v4;
	SPEEDC_SpeedsTypeDef temp_speedt;
	temp_speedt = SPEEDC_GetCurrentSpeed();
	v1 = SPEEDS_PulseToSpeed(temp_speedt.ecd_speed1);
	v2 = SPEEDS_PulseToSpeed(temp_speedt.ecd_speed2);
	v3 = SPEEDS_PulseToSpeed(temp_speedt.ecd_speed3);
	v4 = SPEEDS_PulseToSpeed(temp_speedt.ecd_speed4);
	speed_out_t.Speed_X = 0.25f * (-v1 + v2 - v3 + v4);
	speed_out_t.Speed_Y = 0.25f * (	v1 + v2 + v3 + v4);
	speed_out_t.Speed_Z = 0.25f * (	v1 - v2 - v3 + v4) * 1000;
	speed_out_t.Speed_Z /= SPEED_SOLVING4_DATA_TAN_A * SPEED_SOLVING4_DATA_A + SPEED_SOLVING4_DATA_B; //w
	speed_out_t.Speed_Z *= 180 / 3.1415926f;
	cos_radz = cos(position_out_t.PositionZ_R);
	sin_radz = sin(position_out_t.PositionZ_R);
	speed_out_t.Speed_AbsX = cos_radz * speed_out_t.Speed_X - sin_radz * speed_out_t.Speed_Y;
	speed_out_t.Speed_AbsY = sin_radz * speed_out_t.Speed_X + cos_radz * speed_out_t.Speed_Y;
	return speed_out_t;
}

void SPEEDS_GetCurrentSpeedLoop(float s1, float s2, float s3, float s4)
{
	speed_out_t.Speed_1 = SPEEDS_PulseToSpeed(s1);
	speed_out_t.Speed_2 = SPEEDS_PulseToSpeed(s2);
	speed_out_t.Speed_3 = SPEEDS_PulseToSpeed(s3);
	speed_out_t.Speed_4 = SPEEDS_PulseToSpeed(s4);
}

/**
  * @brief  修正脉冲，未测试
  * @param  float angle
  * @retval None
  */
SPEEDS_PulseOutTypeDef PulseSolving4(float angle)
{
	static float Pulse_Solving4_Data_3 = (SPEED_SOLVING4_DATA_TAN_A * SPEED_SOLVING4_DATA_A + SPEED_SOLVING4_DATA_B) / (SPEED_SOLVING4_DATA_R_Z * SPEED_SOLVING4_DATA_TAN_A) * 1560; //angle * (a+b) / r * 1560 得到电机需要转的脉冲数

	static float Tras_value = 1 / 360.0; //度转换为弧度..这儿改了一个bug

	angle = angle * Tras_value;

	pulse_out_t.pulse_1 =  angle * Pulse_Solving4_Data_3; //均用的毫米
	pulse_out_t.pulse_2 = -angle * Pulse_Solving4_Data_3;
	pulse_out_t.pulse_3 = -angle * Pulse_Solving4_Data_3;
	pulse_out_t.pulse_4 =  angle * Pulse_Solving4_Data_3;
	return pulse_out_t;
}

SPEEDS_PulseOutTypeDef GetErrPulse(void)
{
	return pulse_out_t;
}

void SpeedSolving4_Set(void)
{
	SpeedSolving4_SetWithTime(SPEED_SOLVING_RAMP_TIME, &speed_out_t);
}

//设置加速度
void SpeedSolving4_SetWithTime(float HowTime, SPEEDS_SpeedOutTypeDef *TargetPulse)
{
	//将速度解算出的结果设置给速度环，需要考虑速度环里面的斜坡
	static float StepTemp1 = 0;
	static float StepTemp2 = 0;
	static float StepTemp3 = 0;
	static float StepTemp4 = 0;

	//解算得到的当前四个轮子目标速度 - 前一刻目标值 / 次数 得到步幅，即每次增加的量
	StepTemp1 = (TargetPulse->Speed_1 - SPEEDC_GetTarget(1)) / HowTime;
	StepTemp2 = (TargetPulse->Speed_2 - SPEEDC_GetTarget(2)) / HowTime;
	StepTemp3 = (TargetPulse->Speed_3 - SPEEDC_GetTarget(3)) / HowTime;
	StepTemp4 = (TargetPulse->Speed_4 - SPEEDC_GetTarget(4)) / HowTime;

	//在这儿设置斜坡的步幅和过多少次增加一个步骤
	SPEEDC_SetOneRamp(1, 0, Abs(StepTemp1));
	SPEEDC_SetOneRamp(2, 0, Abs(StepTemp2));
	SPEEDC_SetOneRamp(3, 0, Abs(StepTemp3));
	SPEEDC_SetOneRamp(4, 0, Abs(StepTemp4));

	SPEEDC_SetTarget(TargetPulse->Speed_1, TargetPulse->Speed_2,
					 TargetPulse->Speed_3, TargetPulse->Speed_4);
	TargetPulse->UpData = 0;
}

void SPEEDS_DataReport(void)
{
	float MyTemp1;

	MyTemp1 = SPEED_SOLVING4_DATA_MOTOR_N * 2 * 3.1415926f * SPEED_SOLVING4_DATA_R / 60000; //每分钟最快转过的mm数/60000ms=m/s
	//得最快速度为1.69m/s。假设3s内达到最快速度不会打滑，则加速度最大为0.565m/s2，
	printf("最快速度%fm/s\r\n", MyTemp1);
}
