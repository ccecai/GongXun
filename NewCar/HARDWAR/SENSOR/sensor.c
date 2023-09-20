#include "sensor.h"
#include "usart.h"
#include "mpu9250.h"
#include "math.h"
#include "speedsolve.h"

//����PB14�ߵ͵�ƽģ�����벶��
#define DISTANCE_LR 50.0000f //���Ҵ���������
#define WIDTH_LINE 10.0000f //�߿�

u8  TIM12_CAPTURE_STA=0;    				
u32	TIM12_CAPTURE_VAL;

typedef struct
{
	float Err_Z; //Z������ ��ʱ��(��ƫ)Ϊ�� ˳ʱ��(��ƫ)Ϊ��
}SENSOR_ErrTypeDef;//Ԥ���ṹ�� �Է���ʱ֮��

typedef struct
{
	int8_t FirstOnLine;
	int8_t EnterLineNum;
	int8_t isOnLine; 
	
}SENSOR_RecordLines;


SENSOR_ErrTypeDef sensor_F;

void SENSOR_ControlLoop(void)
{
	SENSOR_Err_Calc(); //Ԥ�������� Ϊ�������򴫸�����׼��
}

void SENSOR_Err_Calc(void)//��TIM13�жϷ������е���
{
//	int8_t isFirst=1,Polarity_Z; //�����߹��ߺ��ظ����
//	float speed_y,wait_time;
//	
//	if(FL&&!FR)//�ȼ�⵽�ߵĴ������������� ��������� �Լ��Ƿ�Ϊ�״μ��
//	{
//		GPIO_SetBits(GPIOB,GPIO_Pin_14);
//		if(isFirst)
//		{
//			Polarity_Z=-1;
//			isFirst=0;
//		}
//		else
//			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
//	}
//	if(!FL&&FR)//���⵽�ߵĴ������رղ���
//	{
//		GPIO_SetBits(GPIOB,GPIO_Pin_14);
//		if(isFirst)
//		{
//			Polarity_Z=1;
//			isFirst=0;
//		}
//		else
//			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
//	}
//	if(TIM12_CAPTURE_STA&0x80)
//	{
//		wait_time=TIM12_CAPTURE_STA&0X3F*0XFFFF+TIM12_CAPTURE_VAL;//���㲶��ʱ��
//		speed_y=SPEEDS_GetCarSpeed().Speed_AbsY;//��ȡ��ǰ�ٶ�
//		sensor_F.Err_Z=Polarity_Z*asin((1000*wait_time*speed_y-WIDTH_LINE)/DISTANCE_LR);//1000us*(m/s)/mm
//		//ʹ�õ����ٶ�ģ����asin ȫ�ٶ�ģ������atan
//		//�����ٴ��뵱ǰz��Ŀ��ֵ����С���
//		TIM12_CAPTURE_STA=0;
//		isFirst=1;
//	}
}



float SENSOR_GetErr_Z(void)//��POSITIONC_ControlLoop����� ��֪���в���
{
	float err=sensor_F.Err_Z;
	sensor_F.Err_Z=0;
	if(err>0.0174f&&err<0.7854f) //�޷� ����45�����������
		return err;
	else
		return 0;
}        
/*
PE0  PF0 PF11 PD0
PE1  PF1 PF12 PD1
PE2  PF2 PF13 PD2
PE3  PF3 PF14 PD3

*/

void SENSOR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG,ENABLE); 
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


void TIM12_Cap_Init(uint16_t psc,uint32_t arr)//��ԸTIM12����
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM12_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12);
    
	TIM_DeInit(TIM12);
	TIM_TimeBaseStructure.TIM_Period=arr;	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);
	
	TIM12_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM12_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM12_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM12_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM12_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM12, &TIM12_ICInitStructure);
	
	TIM_ITConfig(TIM12,TIM_IT_Update|TIM_IT_CC1,ENABLE);

	TIM_Cmd(TIM12,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(16λ��ʱ��,1us��1,���ʱ��:0.06��???)
void TIM12_IRQHandler(void)
{
 	if((TIM12_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)//���
		{	     
			if(TIM12_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM12_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM12_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM12_CAPTURE_VAL=0XFFFF;
				}
				else
					TIM12_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM12, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM12_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM12_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			  TIM12_CAPTURE_VAL=TIM_GetCapture1(TIM12);//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else 								//��δ��ʼ,��һ�β���������
			{
				TIM12_CAPTURE_STA=0;			//���
				TIM12_CAPTURE_VAL=0;
				TIM12_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM12,DISABLE ); 	//�رն�ʱ��12
	 			TIM_SetCounter(TIM12,0);
	 			TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM12,ENABLE ); 	//ʹ�ܶ�ʱ��12
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM12, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}


void SENSOR_Test(void)
{
	while(1)
	{
		//������1 ������0
//		printf("s:%ld,%ld,%ld,%ld,   %ld,%ld,%ld,%ld,   %ld,%ld,%ld,%ld,    %ld,%ld,%ld,%ld\n", 
		delay_ms(40);
	}
}
