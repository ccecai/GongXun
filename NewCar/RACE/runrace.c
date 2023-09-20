/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    2021/10/18
  * @brief   ʹ�ô���1���е��ͨ�ţ���printf��ӳ��ܷ���
  *			 ����2��������ͨ�ţ�ֻ��Ҫ��������������
  * 		 ����3����ݮ��ͨ�ţ���Ҫ�շ�
  ******************************************************************************
  * @attention ����Э��
  * 		����1
  *			����2 ���գ����ֲ�����жϺ���
  *			����3  ���� 0X55(֡ͷ) {0X3?(ģʽ) ����λX6} ���
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "runrace.h"
#include "positioncontrol.h"
#include "delay.h"
#include "comother.h"
#include "trace.h"
#include "usart.h"
#include "armmove.h"
#include "oled.h"
#include "comother.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/ 
enum ClawNumCMD
{
	POINT1_GET_TOP,	
};
#define POINT1_GET_TOP 		0
#define POINT1_GET_LOW 	 	1
#define POINT1_PUT_ONCAR 	2

#define POINT2_PUT_GROUND	6
#define POINT2_PUT_ONCAR 	7

#define POINT3_PUT_GROUND	11
#define POINT3_PUT_ONCAR 	12
#define POINT3_PUT_MADUO 	13 //���
//����ָ��
#define ARM_SCAN_CODE		16
#define ARM_INITIAL			17

#define TRACE_OFFSET       -45 //ѭ��ƫ����

/* Global param --------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Functions ------------------------------------------------------------------*/
void RACE_Position0(void);

void RACE_Position1(void);
void RACE_Position2(void);
void RACE_Position3(void);

void RACE_RePosition1(void);
void RACE_RePosition2(void);
void RACE_RePosition3(void);

void RACE_RunPoint(void);

static uint16_t Time_ms_X = 0, Time_ms_Y = 0, Time_ms_Z = 0;

/* ��������
	�ƶ�() ����ά��ɨ������ ɨ���ά��
	�ƶ�() ��ԭ������ 		���ն�ά��˳������ϲ����ϵ�����
	�ƶ�() ���ּӹ�����		���յر���ɫ(�̶�)�ڷ���飬Ӧ����ʱ�ǵó����ĸ�λ����ɶ��ɫ
	�ƶ�() ����				���ն�ά��˳����˴ּӹ�������鵽����
	�ƶ�() �����Ʒ����     ���յر���ɫ(�̶�)�ڷ����

	�ƶ�() ��ԭ������ 		���ն�ά��˳������²����ϵ�����
	�ƶ�() ���ּӹ�����		���յر���ɫ(�̶�)�ڷ���飬Ӧ����ʱ�ǵó����ĸ�λ����ɶ��ɫ
    �ƶ�() ����				���ն�ά��˳����˴ּӹ�������鵽����
    �ƶ�() �����Ʒ����     ���յر���ɫ(�̶�)�ڷ����

ע�� ÿһ��ץȡ��ڷŵ�ʱ��Ҫ���ر���߽��ж�λ
*/
int8_t color_order[6] = {1,2,3,1,2,3};
uint8_t isStopping = 0;
uint8_t isVertical = 0;
uint8_t isReversed = 0;

//typedef struct
//{
//	float x_last;
//	float y_last;
//	float z_last;
//	float x_now;
//	float y_now;
//	float z_now;
//	float x_out;
//	float y_out;
//	float z_out;
//}RACE_AbsPosTypeDef;

//RACE_AbsPosTypeDef abspos;

//void RACE_AbsToOut(float x, float y, float z)
//{
//	abspos.x_last = abspos.x_now;
//	abspos.y_last = abspos.y_now;	
//	abspos.z_last = abspos.z_now;
//	
//	abspos.x_now = x;
//	abspos.y_now = y;
//	abspos.z_now = z;
//	
//	abspos.z_out = abspos.x_now - abspos.z_last;
//	abspos.x_out = 0;
//	abspos.y_out = 0;
//}

void RACE_RunSome(void)
{
//	RACE_RunWithOffset(-750 + 150, 150, 0, 1.1);
//	while(1);
	
	RACE_Position0(); //��ά����

	//�״ΰ���
//	while(1);
	RACE_Position1(); //ԭ����
	
	
	RACE_Position2(); //�ӹ���
	RACE_Position3(); //���Ʒ��
	
	//�ٴΰ���
	RACE_RePosition1(); //ԭ����
	RACE_RePosition2(); //�ӹ���
	RACE_RePosition3(); //���Ʒ��

	while(1);
/********************************************************/

}

/**
  * @brief  ʹ����ʱ�ķ�ʽ�ж��Ƿ񵽴�Ŀ��ֵ
  * @param  float Position_x, float Position_y, float Angel_z, float max_velocity
  * @retval None
  */
void RACE_RunLessDelay(float Position_x, float Position_y, float Angel_z, float max_velocity)
{
	uint32_t delay_time;
	delay_time = POSITONC_SetCarGoTo(Position_x, Position_y, Angel_z, max_velocity);
	if(delay_time<200)
		delay_time = 210;
	delay_ms(delay_time - 200);
}

void RACE_RunWithDelay(float Position_x, float Position_y, float Angel_z, float max_velocity)
{
	uint32_t delay_time;
	delay_time = POSITONC_SetCarGoTo(Position_x, Position_y, Angel_z, max_velocity);
	delay_ms(delay_time + 200);
}

void RACE_RunMoreDelay(float Position_x, float Position_y, float Angel_z, float max_velocity)
{
	uint32_t delay_time;
	delay_time = POSITONC_SetCarGoTo(Position_x, Position_y, Angel_z, max_velocity);
	delay_ms(delay_time + 500);
}


/**
  * @brief  ʹ����ʱ�ķ�ʽ�ж��Ƿ񵽴�Ŀ��ֵ
  * @param  float Position_x, float Position_y, float Angel_z, float max_velocity
  * @retval None
  */
void RACE_RunWithOffset(float Position_x, float Position_y, float Angel_z, float max_velocity)
{
	RACE_RunLessDelay(Position_x + TRACE_OFFSET, Position_y + TRACE_OFFSET, Angel_z, max_velocity);
	delay_ms(100);
}

/**
  * @brief  �����ά������ʶ���ά��
  * @param  None
  * @retval None
  */
void RACE_Position0(void)
{

//	RACE_SendCMDToArm(ARM_INITIAL);//�ȳ�ʼ����λ���
//	RACE_SendCMDToArm(ARM_SCAN_CODE);//ɨ���ά��
	RACE_RunWithDelay(170, -490, 0, 0.6);
	delay_ms(1000);
	
//	while(1)//���յ��Ӿ�������ǰ��������
//	{
//		uint8_t break_flag = 0;
//		usart3_send (0x55);

////		printf("%d,%d,%d,%d,%d,%d\r\n",com_rxbuff[0],com_rxbuff[1],com_rxbuff[2],com_rxbuff[3],com_rxbuff[4],com_rxbuff[5]);
//		if(stop_read_flag == 1 && break_flag == 0)
//		{
//			color_order[0] = com_rxbuff[0]-48;
//			color_order[1] = com_rxbuff[1]-48;
//			color_order[2] = com_rxbuff[2]-48;
//			color_order[3] = com_rxbuff[3]-48;
//			color_order[4] = com_rxbuff[4]-48;
//			color_order[5] = com_rxbuff[5]-48;
//			break_flag =1;
//			printf("g:%d,%d,%d,%d,%d,%d\r\n",color_order[0],color_order[1],color_order[2],color_order[3],color_order[4],color_order[5]);
//		}
//		if(break_flag==1)break;
////		printf("g:%d,%d,%d,%d,%d,%d\r\n",color_order[0],color_order[1],color_order[2],color_order[3],color_order[4],color_order[5]);

//	  
//	}
//	while(1);
//	COM_ChooseMode(SACN_CODE_MODE);//��ݮ��ɨ���ά��ģʽ
//	COM_ChooseMode(SACN_CODE_MODE);
	
//	while(!COM_GetCodeOrder(code_order));
//	printf("code_order:%d,%d,%d,%d,%d, %d\n", code_order[0],code_order[1],code_order[2],code_order[3],code_order[4],code_order[5]);
	//�������õ�1��Ҫ���Ҳ����ȫ�ף��󷽵������м�
//	OLED_Printf(5, 10, "code %d,%d,%d,%d,%d,%d", code_order[0],code_order[1],code_order[2],code_order[3],code_order[4],code_order[5]);
//	OLED_RefreshGram();
}

/**
  * @brief  ��һ�ε���ԭ��������ɫʶ��ץȡ
  * @param  None
  * @retval None
  */
void RACE_Position1(void)
{
	
	//��һ��ǰ��ԭ����
	RACE_RunWithDelay(0, -860, 0, 0.6);
	delay_ms(1000);
	printf("#1P2000#3P1706#4P1618#5P1235T1000\r\n");
	delay_ms(2000);//�𲽽���е�����ڴ�ֱλ��
//	printf("#3P1290#4P1853T1000\r\n");
//					delay_ms(2000);
//					
//					printf("#5P1618T1000\r\n");
//					delay_ms(2000);
//					
//					printf("#3P1882T1000\r\n");
//					delay_ms(2000);
//					
//					printf("#1P1471T1000\r\n");
//					delay_ms(2000);
//					
//					printf("#3P1500#4P1500T1000\r\n");
//					delay_ms(2000);
//				
//					printf("#5P1235T1000\r\n");
//					delay_ms(2000);
//				
//					printf("#3P1706T1000\r\n");
//					delay_ms(2000);
//				
//					printf("#1P2000T1000\r\n");
//				  delay_ms(2000);
	ARMM_Position1_GetBottom();

//	TRACE_ToPoint1();

//	COM_ChooseMode(COLOR_DECT_MODE);
//	COM_ChooseMode(COLOR_DECT_MODE);
	
	//��ɫʶ��͵�һ��ץȡ
//	while(!COM_GetColorOrder(color_order));
//	OLED_Printf(5, 40, "color %d,%d,%d,%d,%d,%d", color_order[0], color_order[1],color_order[2],color_order[3],color_order[4],color_order[5]);
//	OLED_RefreshGram();
//	printf("color_order:%d,%d,%d,%d,%d, %d\n", color_order[0],color_order[1],color_order[2],color_order[3],color_order[4],color_order[5]);
	
//	RACE_RunWithDelay(420, 255, 0, 1.1);
//	isStopping = 1;
//	ARMM_Position1_GetTop(code_order, color_order);
//	isStopping = 0;
//	COM_ChooseMode(NONE_MODE);
}

/**
  * @brief  ��һ�ε���ӹ��������á�ץȡ
  * @param  None
  * @retval None
  */
void RACE_Position2(void)
{
	//��һ��ǰ���ӹ���


	//��һ���ڼӹ������ú�ץȡ
	RACE_RunWithDelay(0, -490, 0, 0.6);
	delay_ms(500);
	RACE_RunWithDelay(900, 0, 0, 0.6);
	delay_ms(500);
	
//	printf("#1P1471T1000\r\n");
//    delay_ms(2000);
//	printf("#3P1500#4P1500T1000\r\n");
//    delay_ms(2000);
//   printf("#5P1618T1000\r\n");
//    delay_ms(2000);
//    printf("#3P1794#4P1441T1000\r\n");
//    delay_ms(2000);
//    printf("#1P2500T1000\r\n");
//    delay_ms(2000);
//    printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");
//    delay_ms(2000);
//		printf("#5P1250T1000\r\n");
//    delay_ms(2000);
//    printf("#3P1706T1000\r\n");
//    delay_ms(2000);
	ARMM_Position2_PutTop(color_order);
	ARMM_Position2_GetAgain(color_order);
	
	
}

/**
  * @brief  ��һ�ε����Ʒ��������
  * @param  None
  * @retval None
  */
void RACE_Position3(void)
{
	//��һ��ǰ�����Ʒ��
	
	//���Ʒ����һ�����
	RACE_RunWithDelay(630, 0, 0, 0.6);
	delay_ms(1000);
	RACE_RunWithDelay(0, 825, 0, 0.6);
	delay_ms(1000);
	
//	printf("#1P1471#3P1471#4P1441#5P1382T1000\r\n");
//    delay_ms(2000);
//	printf("#5P1765T1000\r\n");
//    delay_ms(2000);
//   printf("#3P1824T1000\r\n");
//    delay_ms(2000);
//    printf("#1P1000T1000\r\n");
//    delay_ms(2000);
//    printf("#1P971#3P1529#4P2235T1000\r\n");
//    delay_ms(2000);
//    printf("#5P1250T1000\r\n");
//    delay_ms(2000);
//		printf("#1P1000T1000\r\n");
//    delay_ms(2000);
//    printf("#3P1706T1000\r\n");
//    delay_ms(2000);
//		printf("#1P1471#3P1471#4P1441#5P1382T1000\r\n");
//    delay_ms(2000);
//	RACE_RunWithOffset(300, 0, 0, 0.6);
//	delay_ms(500);
	ARMM_Position3_Stack(color_order);
	while(1);
	delay_ms(200);
	
//	ARMM_Position3_Stack(color_order);
	
//	isReversed = 1;
//	ARMM_Position2_PutTop(color_order);	
//	isReversed = 0;
	
}

/**
  * @brief  �ڶ��ε���ԭ������ץȡ
  * @param  None
  * @retval None
  */
void RACE_RePosition1(void)
{
	//�ڶ���ǰ��ԭ����
	RACE_RunWithDelay(0, -835, 0, 0.6);
	delay_ms(1000);
	RACE_RunWithDelay(-1680, 0, 0, 0.6);
	delay_ms(1000);
	RACE_RunWithDelay(0, 510, 0, 0.6);
	delay_ms(1000);
	
//	ARMM_Position1_GetBottom();
	
}

/**
  * @brief  �ڶ��ε���ӹ��������á�ץȡ
  * @param  None
  * @retval None
  */
void RACE_RePosition2(void)
{
	//��һ���ڼӹ������ú�ץȡ
	RACE_RunWithDelay(0, -515, 0, 0.6);
	delay_ms(500);
	RACE_RunWithDelay(910, 0, 0, 0.6);
	delay_ms(500);
	ARMM_RePosition2_PutTop(color_order);
	ARMM_RePosition2_GetAgain(color_order);
	
}

/**
  * @brief  �ڶ��ε����Ʒ�������á��������
  * @param  None
  * @retval None
  */
void RACE_RePosition3(void)
{
	//�ڶ���ǰ�����Ʒ��
	RACE_RunWithDelay(760, 0, 0, 0.6);
	delay_ms(1000);
	RACE_RunWithDelay(30, 830, 0, 0.6);
	delay_ms(1000);
	ARMM_Position3_Stack(color_order);
	RACE_RunWithDelay(50, 1080, 0, 0.6);
	delay_ms(1000);
	RACE_RunWithDelay(-1820, 100, 0, 0.6);
	delay_ms(1000);
	while(1);
}

void RACE_RunPoint(void)
{
	RACE_RunWithDelay(    900, 0,  0, 1.2);
	delay_ms(1500);
	RACE_RunWithDelay(    -900,0, 0, 1.2);
	delay_ms(1500);
	
//	RACE_RunWithDelay(    0, 1500,  0, 1.2);
//	RACE_RunWithDelay(    0,    0, 90, 0.6);
//	delay_ms(1500);
//	
//	RACE_RunWithDelay(    0, 2100,  0, 1.2);
//	RACE_RunWithDelay(    0,    0, 90, 0.6);
//	delay_ms(1500);
//	
//	RACE_RunWithDelay(    0, 1500,  0, 1.2);
//	RACE_RunWithDelay(    0,    0, 90, 0.6);
//	delay_ms(1500);
}
