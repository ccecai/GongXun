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
#ifndef __TRACE_H
#define __TRACE_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines --------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct 
{
	//������1 ������0
	int8_t ConditionR[7];//ʵʱ�洢����״̬���ҷ�
	uint8_t ConditionByte_R;//ʵʱ�洢����״̬��
	int8_t Con_RightOnBlack_R;//���ұ��ĸ����ں�����
	int8_t Con_LeftOnBlack_R;//������ĸ����ں�����
	int8_t Con_BlackNum_R;
	
	int8_t ConditionF[7];//ʵʱ�洢����״̬��ǰ��
	uint8_t ConditionByte_F;//ʵʱ�洢����״̬��ǰ��
	int8_t Con_RightOnBlack_F;//���ұ��ĸ����ں�����
	int8_t Con_LeftOnBlack_F;//������ĸ����ں�����
	int8_t Con_BlackNum_F;//��ɫ�Ƶ���Ŀ��0��ȫ�ף����ڵ���TCK_ALL_BLACK_NUMΪȫ��
	
	int8_t ConditionL[7];//ʵʱ�洢����״̬����
	uint8_t ConditionByte_L;//ʵʱ�洢����״̬��
	int8_t Con_RightOnBlack_L;//���ұ��ĸ����ں�����
	int8_t Con_LeftOnBlack_L;//������ĸ����ں�����
	int8_t Con_BlackNum_L;
	
	int8_t ConditionB[7];//ʵʱ�洢����״̬����
	uint8_t ConditionByte_B;//ʵʱ�洢����״̬��
	int8_t Con_RightOnBlack_B;//���ұ��ĸ����ں�����
	int8_t Con_LeftOnBlack_B;//������ĸ����ں�����
	int8_t Con_BlackNum_B;
	
	float LastPosition_X;
	float LastPosition_Y;
	float NowPosition_X;
	float NowPosition_Y;
	int64_t UpdatePositionTime_X;
	int64_t UpdatePositionTime_Y;
}TRACE_Status_t; 		//ѭ������״̬

typedef struct
{
	int8_t FirstOnBlack;//�����ߺ��Ϊ1����ʱ�������µ�ǰ���꣬��ʾ��Ҫ��֤����֤ͨ���������Ҫ���½�����Y+�������Ϊ2�������Ҫ���½�����Y-�������Ϊ-2����ʾ��Ҫ���µ����꣬���µ�������Ϊ0
	int8_t EnterBlackNum;//���˼��κ���
	int8_t IsMainBias;//�Ƿ�����Ҫ������λ�Ǳ������ĵ�����1��ʾ�ǣ�0��ʾ����
	float  FirstPosition_Y;//������ʱ����
	float  LastCredibleBias_Y;
	float  CredibleBias_Y;//����ƫ��
	
	float  NowCredible_Y;//��������
	int8_t NowCredibleState;//3����Ҫ��֤������+��-3��Ҫ��֤������-��4�ǽ����߲�֪��ʲô��5�ǳ����߲�֪��ʲô��6����֤������+,-6����֤������-
	float  LastPosition_Y;//�ϴν����߻��߳�����ʱ����
	int64_t LastTime_ms;//�ϴν����߻��߳�����ʱ��ʱ��
	float  MayLines_Y;//��������
	
	int8_t EnterOrOut;//�����߻��߳����ߣ�1���ں�������
}TRACE_RecordLine_t;//��¼ÿ������������XY������


/* Functions -----------------------------------------------------------------*/
void TRACE_UpdatLoop(void);
void TRACE_Test(void);
uint8_t TRACE_ToPoint1(void);
uint8_t TRACE_ToPoint2(void);
void TRACE_Adjust(void);
#endif 

/*************MADE BY LIU****************END OF FILE****/
