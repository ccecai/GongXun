/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version  V1.0.0
  * @date    2021-7-26
  * @brief  
  * 
  ******************************************************************************
  * @attention
  * �����ٶȽ��㣬�ο�https://zhuanlan.zhihu.com/p/20282234
	* Speed_x��ʾ X ���˶����ٶȣ������ҷ��򣬶�������Ϊ����
	*	Speed_y��ʾ Y ���˶����ٶȣ���ǰ���򣬶�����ǰΪ����
	*	Speed_z��ʾ yaw ����ת�Ľ��ٶȣ�������ʱ��Ϊ��
	*   ��Ϊ179.6 a95 b89
  ******************************************************************************
	* �Ż������㺯���в�Ӧ�����趨�������ָ��
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
 *  @brief    �ٶ�ת���������
 */
#define SPEEDS_DATA_N2S 0.20420352f	 // 1000 * 60 / 5.0f / 1560.0f, ת��ת������ʵ�ٶ�
#define SPEEDS_DATA_D2R 0.01745329f	 //��ת��Ϊ���� 3.1415926f/180.0f
#define SPEEDS_DATA_W2N 9.549296586f //���ٶ�תΪת��Ҫ�˵ı�������N=W*60/2pi=9.549*W
#define SPEEDS_DATA_R2D 57.2957795f	 //D=O/3.1415926*180
#define SPEEDS_DATA_W2P	47.746483f	 //���ٶȵ������ٶ�pulse/5ms (60000*5/(2pi*1000))

/** @defgroup SPEED_SOLVING_DATA Group
 *  @brief    �ٶȽ�������Ҫ�Ĳ��� 
 */
#define SPEED_SOLVING4_DATA_A 120.17f					 //С������ߴ�İ������ת���ĵ�������ӵľ��롣��λ��mm
#define SPEED_SOLVING4_DATA_B 101.2f					 //С������ߴ�İ볤������ת���ĵ�ǰ�����ӵľ��롣��λ��mm
#define SPEED_SOLVING4_DATA_R_Y 38.935f				 	//���ְ뾶(mm)��ͨ��λ�Ƽ�����İ뾶������뾶������Y����
#define SPEED_SOLVING4_DATA_R_X 37.10f					 //���ְ뾶(mm)��ͨ��λ�Ƽ�����İ뾶������뾶������X����
#define SPEED_SOLVING4_DATA_R_Z 38.5f					 //���ְ뾶(mm)��ͨ��λ�Ƽ�����İ뾶������뾶������Z����
#define SPEED_SOLVING4_DATA_R 38.5f				 //���Ҹ������ְ뾶�����Ҳ�����۰뾶��ֻ�����ֵ��������
#define SPEED_SOLVING4_DATA_TAN_A 1.0f				 //tan(a)�����ǶȰ�����������ֵ
#define SPEED_SOLVING4_DATA_MOTOR_N 369.23f			 //������ת��
#define SPEED_SOLVING4_DATA_MOTOR_PWM 2000.0f		 //PWM���ֵ
#define SPEED_SOLVING4_DATA_MAX_PULSE 2000.0f			 //5ms������ת�����ֵ,����ֱ�Ӽ���
#define SPEED_SOLVING4_DATA_ENCODE_PULSE_MAX 60000.0f //(500*30*4.0f)	//����תһȦ��Ӧ�ı�����������,�ı�Ƶ����ʵ��������������20���ٱ�
#define SPEED_SOLVING_RAMP_TIME 4.0f				 //��4��ʹб�´ﵽ����ֵ

/* Global param ------------------------------------------------------------*/
SPEEDS_SpeedOutTypeDef speed_out_t;
SPEEDS_PositionOutTypeDef position_out_t;
SPEEDS_PulseOutTypeDef pulse_out_t;
/* Private functions --------------------------------------------------------*/

/**
  * @brief  ����仯ת��Ϊ���ٶ�
  * @param  None
  * @retval None
  */
float SpeedS_PToW(float Pulse_B)
{
	static float temp = 2 * 3.1415926 / SPEED_SOLVING4_DATA_ENCODE_PULSE_MAX; //����������תΪ�Ƕ�Ҫ�˵ı�����
	return (Pulse_B * temp);
}

/* Functions ------------------------------------------------------------------*/

/**
  * @brief  �ٶ�m/sת��Ϊ���� 5ms
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
  * @brief  �ٶ�5ms�����ٶ�ת��Ϊm/s
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
  * @brief 	����㣬xyz �õ� speed1234, �������Ǹ�λ����̬��������
  * @param  float Speed_x,float Speed_y,float Speed_z������m/s
  * @retval speed_out_t
  */
SPEEDS_SpeedOutTypeDef SPEEDS_GetWheelSpeed(float speed_x, float speed_y, float speed_z)
{
	static float SPEEDS_DATA1 = 1 / (SPEED_SOLVING4_DATA_R_X * SPEED_SOLVING4_DATA_TAN_A);
	static float SPEEDS_DATA2 = 1 / SPEED_SOLVING4_DATA_R_Y;	
	static float SPEEDS_DATA3 = (SPEED_SOLVING4_DATA_TAN_A * SPEED_SOLVING4_DATA_A + SPEED_SOLVING4_DATA_B)
								/ (SPEED_SOLVING4_DATA_R_Z * SPEED_SOLVING4_DATA_TAN_A);
	static float w1, w2, w3, w4;				//���ٶ�
	float speed_x_temp, speed_y_temp;
	speed_x_temp = speed_x * 1000;		 //��λ��m/sתΪmm/s
	speed_y_temp = speed_y * 1000;		 //��λ��m/sתΪmm/s
	speed_z = speed_z * SPEEDS_DATA_D2R; //����Ƕȵõ�����
	//position_out_t.PositionZ_R = 0;
	// �����������õ���Ϊ���������Ӧ��vx,vy
	float cos_radz = cos(position_out_t.PositionZ_R);
	float sin_radz = sin(position_out_t.PositionZ_R);
	
	// �ó��ߵ��ǳ������µ�vx��vy ������Ҫ������任
//	Speed_x = cos_radz * speed_x_temp + sin_radz * speed_y_temp;
//	Speed_y = -sin_radz * speed_x_temp + cos_radz * speed_y_temp;
	speed_x =speed_x_temp;
	speed_y =speed_y_temp;
	// mm/s */mm = rad/s 	������һ�����Ӱ뾶 rad/s
	w1 = -speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 + speed_z * SPEEDS_DATA3;
	w2 =  speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 - speed_z * SPEEDS_DATA3;
	w3 = -speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 - speed_z * SPEEDS_DATA3;
	w4 =  speed_x * SPEEDS_DATA1 + speed_y * SPEEDS_DATA2 + speed_z * SPEEDS_DATA3;

	// ����ĳ���ֻ�ǰѽ��ٶ�ת�������ٶ�, ת�����ٶȵ�λ��pulse/5ms
	speed_out_t.Speed_1 = w1 * SPEEDS_DATA_W2P;		//���ٶȵ�������
	speed_out_t.Speed_2 = w2 * SPEEDS_DATA_W2P;
	speed_out_t.Speed_3 = w3 * SPEEDS_DATA_W2P;
	speed_out_t.Speed_4 = w4 * SPEEDS_DATA_W2P;		//(600000*5/(2pi*1000))
	
	speed_out_t.UpData = 1; //������
	return speed_out_t;
}


/**
  * @brief  �����㣬ͨ����������λ��,���mm ��
  * @param  float Pulse_1,float Pulse_2,float Pulse_3,float Pulse_4
  * @retval position_out_t
  */
SPEEDS_PositionOutTypeDef SPEEDS_GetCarPosition(float Pulse_1, float Pulse_2, float Pulse_3, float Pulse_4)
{
	static float angel_1, angel_2, angel_3, angel_4;
	static float data_1 = 0.25f * SPEED_SOLVING4_DATA_R_X * SPEED_SOLVING4_DATA_TAN_A;																			   //�������������ĵ�һ�й����ӣ�����1/4
	static float data_2 = 0.25f * SPEED_SOLVING4_DATA_R_Y;																										   //�������������ĵڶ��й�����
	static float data_3 = 0.25f * SPEED_SOLVING4_DATA_R_Z * SPEED_SOLVING4_DATA_TAN_A / (SPEED_SOLVING4_DATA_TAN_A * SPEED_SOLVING4_DATA_A + SPEED_SOLVING4_DATA_B); //�������������ĵ����й�����
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
//	derta_y = position_out_t.PositionY_mm - position_out_t.Last_Y_mm;//ʹ�õ�ǰ�Ƕȵ�λ�Ʊ仯ת��Ϊ���������λ��

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
  * @brief  �����㣬ͨ�������ٶȣ������٣��ó���,���m/s ��/s
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
  * @brief  �������壬δ����
  * @param  float angle
  * @retval None
  */
SPEEDS_PulseOutTypeDef PulseSolving4(float angle)
{
	static float Pulse_Solving4_Data_3 = (SPEED_SOLVING4_DATA_TAN_A * SPEED_SOLVING4_DATA_A + SPEED_SOLVING4_DATA_B) / (SPEED_SOLVING4_DATA_R_Z * SPEED_SOLVING4_DATA_TAN_A) * 1560; //angle * (a+b) / r * 1560 �õ������Ҫת��������

	static float Tras_value = 1 / 360.0; //��ת��Ϊ����..�������һ��bug

	angle = angle * Tras_value;

	pulse_out_t.pulse_1 =  angle * Pulse_Solving4_Data_3; //���õĺ���
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

//���ü��ٶ�
void SpeedSolving4_SetWithTime(float HowTime, SPEEDS_SpeedOutTypeDef *TargetPulse)
{
	//���ٶȽ�����Ľ�����ø��ٶȻ�����Ҫ�����ٶȻ������б��
	static float StepTemp1 = 0;
	static float StepTemp2 = 0;
	static float StepTemp3 = 0;
	static float StepTemp4 = 0;

	//����õ��ĵ�ǰ�ĸ�����Ŀ���ٶ� - ǰһ��Ŀ��ֵ / ���� �õ���������ÿ�����ӵ���
	StepTemp1 = (TargetPulse->Speed_1 - SPEEDC_GetTarget(1)) / HowTime;
	StepTemp2 = (TargetPulse->Speed_2 - SPEEDC_GetTarget(2)) / HowTime;
	StepTemp3 = (TargetPulse->Speed_3 - SPEEDC_GetTarget(3)) / HowTime;
	StepTemp4 = (TargetPulse->Speed_4 - SPEEDC_GetTarget(4)) / HowTime;

	//���������б�µĲ����͹����ٴ�����һ������
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

	MyTemp1 = SPEED_SOLVING4_DATA_MOTOR_N * 2 * 3.1415926f * SPEED_SOLVING4_DATA_R / 60000; //ÿ�������ת����mm��/60000ms=m/s
	//������ٶ�Ϊ1.69m/s������3s�ڴﵽ����ٶȲ���򻬣�����ٶ����Ϊ0.565m/s2��
	printf("����ٶ�%fm/s\r\n", MyTemp1);
}
