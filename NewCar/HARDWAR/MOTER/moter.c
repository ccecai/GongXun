/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    2021-7-25
  * @brief  moter control
  * 
  ******************************************************************************
  * @attention ������Ʋ���
	* ���Ʒ���1-4 PF12 PF14 PE7 PC0
	*	���ʹ��EN PC11
	*	���PWM TIM2_CH1-4 PA5 PA1 PB10 PB11
	*	����13ppr��4��Ƶ���ٱ�30��1����������5ms,����47������
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "moter.h"
#include "usart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Global param --------------------------------------------------------------*/
MotorCurrent_t MotorCurrent;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief �趨С�����˶����� ���Ʒ���1-4 PF12 PF14 PE7 PC0
  * @param  int8_t Wheel_1,int8_t Wheel_2,int8_t Wheel_3,int8_t Wheel_4
  * @retval None
  */
//////////////////////////
//�����ĸ������������ת�������У�������ÿ��ͨ����IN1Ϊ�ߵ�ƽ��ǰ�߻���IN2Ϊ�ߵ�ƽ��ǰ��
//�����βΣ�Wheel_1Ϊ0��ʾ���GNDɲ����1��ʾ����IN1Ϊ��IN2Ϊ�ͣ�2��ʾ����IN2Ϊ��IN1Ϊ�ͣ�3��ʾIN1��IN2��Ϊ1��
//Ĭ��Wheel_1��2��3��4�ֱ�Ϊ��PE9/PE11/PE13/PE14����ϵ�
void MOTER_SetPolarity(int8_t Wheel_1,int8_t Wheel_2,int8_t Wheel_3,int8_t Wheel_4)
{
	switch(Wheel_1)
	{
		case 0:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_8);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_9);//IN2
			break;
		}
		case 1:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_8);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_9);//IN2
			break;
		}
		case 2:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_8);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_9);//IN2
			break;
		}
		case 3:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_8);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_9);//IN2
			break;
		}
	}
	
	switch(Wheel_2)
	{
		case 0:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_10);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);//IN2
			break;
		}
		case 1:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_10);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);//IN2
			break;
		}
		case 2:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_10);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_11);//IN2
			break;
		}
		case 3:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_10);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_11);//IN2
			break;
		}
	}

	switch(Wheel_3)
	{
		case 0:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_12);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_13);//IN2
			break;
		}
		case 1:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_12);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_13);//IN2
			break;
		}
		case 2:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_12);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_13);//IN2
			break;
		}
		case 3:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_12);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_13);//IN2
			break;
		}
	}

	switch(Wheel_4)
	{
		case 0:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_14);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_15);//IN2
			break;
		}
		case 1:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_14);//IN1
			GPIO_ResetBits(GPIOF,GPIO_Pin_15);//IN2
			break;
		}
		case 2:
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_14);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_15);//IN2
			break;
		}
		case 3:
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_14);//IN1
			GPIO_SetBits(GPIOF,GPIO_Pin_15);//IN2
			break;
		}
	}	
}

/* Functions ------------------------------------------------------------------*/
/**
  * @brief  ���õ��������������Ϊ2000
  * @param  int16_t Wheel1, int16_t Wheel2,int16_t Wheel3,int16_t Wheel4
  * @retval None
  */
void MOTER_SetCurrent(int16_t Wheel1, int16_t Wheel2, int16_t Wheel3, int16_t Wheel4)
{
	int8_t Temp1 = 1, Temp2 = 1, Temp3 = 1, Temp4 = 1;

//	MotorCurrent.Wheel1 = Wheel1;
//	MotorCurrent.Wheel2 = Wheel2;
//	MotorCurrent.Wheel3 = Wheel3;
//	MotorCurrent.Wheel4 = Wheel4;
	Wheel2 = -Wheel2;
//	Wheel1 = -Wheel1;
	Wheel4 = -Wheel4;
	if( Wheel1 < 0) 
	{
		Wheel1 = -Wheel1;
		Temp1 = 1;
	}
	else if(Wheel1 > 0)
	{
		Wheel1 = Wheel1;
		Temp1 = 2;	  
	}
	else 
	{
		Wheel1 = 0;
		Temp1 = 0;	  
	}

	if( Wheel2 < 0) 
	{
		Wheel2 = -Wheel2;
		Temp2 = 1;
	}
	else if(Wheel2 > 0)
	{
		Wheel2 = Wheel2;
		Temp2 = 2;	  
	}
	else 
	{
		Wheel2 = 0;
		Temp2 = 0;	  
	}
	
	if( Wheel3 < 0) 
	{
		Wheel3 = -Wheel3;
		Temp3 = 2;
	}
	else if(Wheel3 > 0)
	{
		Wheel3 = Wheel3;
		Temp3 = 1;	  
	}
	else 
	{
		Wheel3 = 0;
		Temp3 = 0;	  
	}
	
	if( Wheel4 < 0) 
	{
		Wheel4 = -Wheel4;
		Temp4 = 2;
	}
	else if(Wheel4 > 0)
	{
		Wheel4 = Wheel4;
		Temp4 = 1;	  
	}
	else 
	{
		Wheel4 = 0;
		Temp4 = 0;	  
	}
	//��һ���ǵ��3  �ڶ����ǵ��-2 �������ǵ��4  ���ĸ��ǵ��1
	
	MOTER_SetPolarity(Temp4, Temp2, Temp1, Temp3);
	MOTER_SetCompare(Wheel4, Wheel2, Wheel1, Wheel3);
}

/**
  * @brief 	�趨���ռ������ת����ȫΪ���� 
  * @param  int16_t w_ch1,int16_t w_ch2,int16_t w_ch3, int16_t w_ch4
  * @retval None
  */
void MOTER_SetCompare(int16_t w_ch1, int16_t w_ch2, int16_t w_ch3, int16_t w_ch4)
{
	if (w_ch1 >= 2000)
		w_ch1 = 2000;
	if (w_ch2 >= 2000)
		w_ch2 = 2000;
	if (w_ch3 >= 2000)
		w_ch3 = 2000;
	if (w_ch4 >= 2000)
		w_ch4 = 2000;

	if (w_ch1 < 0)
		w_ch1 = 0;
	if (w_ch2 < 0)
		w_ch2 = 0;
	if (w_ch3 < 0)
		w_ch3 = 0;
	if (w_ch4 < 0)
		w_ch4 = 0;

	TIM_SetCompare1(TIM1, w_ch1);
	TIM_SetCompare2(TIM1, w_ch2);
	TIM_SetCompare3(TIM1, w_ch3);
	TIM_SetCompare4(TIM1, w_ch4);
}

/**
  * @brief  ���GPIO��ʼ��
  * @param  None
  * @retval None
  */
void MOTOR_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9); //����ΪTIM2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9); //����ΪTIM2
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10); //����ΪTIM2
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11); //����ΪTIM2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_11 | GPIO_Pin_0);
	GPIO_ResetBits(GPIOE, GPIO_Pin_7);
	GPIO_ResetBits(GPIOF, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
}

/**
  * @brief  PWM ��ʱ��9-ch12-pa2 pa3 ��ʱ��10-ch1-pf6 ��ʱ��11-ch1-pf7��ʼ�� ��ͨ��ʱ��168MHZ f = 84/(psc + 1)/(arr + 1)
  * @param  uint32_t psc, uint32_t arr
  * @retval None
  */
void MOTOR_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//��ʱ���������ýṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;//��ʱ������ṹ��
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);	
  
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������������ã�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���� 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOE,GPIO_Pin_9 |GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14);
	//���õ͵�ƽ��С������ɲ��״̬ ,GPIO_Pin_9 |GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14
  
	GPIO_InitStructure.GPIO_Pin = \
	(GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������������ã�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���� 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	//���õ͵�ƽ��С������ɲ��״̬ ,
	
	TIM_TimeBaseStructure.TIM_Prescaler=21-1;
//	//��ʱ��1��APB2�ϣ�Ƶ��Ϊ168MHZ����ʱ��ʱ��==168MHZ/TIM_Prescaler+1�����ڴ������£���������Ϊ1/8000000�룬��0.125΢��
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=2000 - 1;   //�Զ���װ��ֵ��һ�μ���0.125΢�룬��PWM����Ϊ250us��Ƶ��Ϊ4KHZ����С����0.125΢��
//	//ע�⣬��������˵��С����С��10΢��ʱ�����ſ���û�����10us�ڴ�������Ϊ80�����壬��������õ�ռ�ձ�С��80ʱ����Ϊ0��Ҳ���Բ��ù�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //����ʱ�ӷָDIV1=0����ʱ��
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
	
	//��ʼ��TIM1 Channel 1/2/3/4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:PWM1(�������Ч��ƽ���ﵽ����ֵ�������Ч��ƽ,����PWM2�պ��෴
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ըߣ����ߵ�ƽ��Ч,HIGH�Ǹߵ�ƽ��Ч;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//�ڿ���ʱ�������������https://blog.csdn.net/gtkknd/article/details/52188266	
	TIM_OCInitStructure.TIM_Pulse=0;//����ʼ��ʱ��ռ�ձȣ�����Ϊ0%��
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	//Motor_SetCompareNum(2000,2000,0,0);	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� ���������¼����ͬ����Ӱ�ӼĴ�������ʼ������
		
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1���Ϳ�ʼ����pwm���ˡ�
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��TIM1��PWM�����TIM1��TIM8��Ч,���û�����л�����,
}


