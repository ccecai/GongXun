#include "sensor.h"
#include "usart.h"
#include "mpu9250.h"
#include "math.h"
#include "speedsolve.h"

//控制PB14高低电平模拟输入捕获
#define DISTANCE_LR 50.0000f //左右传感器距离
#define WIDTH_LINE 10.0000f //线宽

u8  TIM12_CAPTURE_STA=0;    				
u32	TIM12_CAPTURE_VAL;

typedef struct
{
	float Err_Z; //Z轴误差方向 逆时针(左偏)为正 顺时针(右偏)为负
}SENSOR_ErrTypeDef;//预留结构体 以防不时之需

typedef struct
{
	int8_t FirstOnLine;
	int8_t EnterLineNum;
	int8_t isOnLine; 
	
}SENSOR_RecordLines;


SENSOR_ErrTypeDef sensor_F;

void SENSOR_ControlLoop(void)
{
	SENSOR_Err_Calc(); //预留函数体 为其他方向传感器做准备
}

void SENSOR_Err_Calc(void)//在TIM13中断服务函数中调用
{
//	int8_t isFirst=1,Polarity_Z; //避免走过线后重复检测
//	float speed_y,wait_time;
//	
//	if(FL&&!FR)//先检测到线的传感器开启捕获 并标记误差方向 以及是否为首次检测
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
//	if(!FL&&FR)//后检测到线的传感器关闭捕获
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
//		wait_time=TIM12_CAPTURE_STA&0X3F*0XFFFF+TIM12_CAPTURE_VAL;//计算捕获时间
//		speed_y=SPEEDS_GetCarSpeed().Speed_AbsY;//获取当前速度
//		sensor_F.Err_Z=Polarity_Z*asin((1000*wait_time*speed_y-WIDTH_LINE)/DISTANCE_LR);//1000us*(m/s)/mm
//		//使用单轴速度模型是asin 全速度模型则是atan
//		//可以再传入当前z轴目标值来减小误差
//		TIM12_CAPTURE_STA=0;
//		isFirst=1;
//	}
}



float SENSOR_GetErr_Z(void)//在POSITIONC_ControlLoop里调用 不知道行不行
{
	float err=sensor_F.Err_Z;
	sensor_F.Err_Z=0;
	if(err>0.0174f&&err<0.7854f) //限幅 大于45°可用于数线
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


void TIM12_Cap_Init(uint16_t psc,uint32_t arr)//但愿TIM12能用
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

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(16位定时器,1us加1,溢出时间:0.06秒???)
void TIM12_IRQHandler(void)
{
 	if((TIM12_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM12_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM12_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM12_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM12_CAPTURE_VAL=0XFFFF;
				}
				else
					TIM12_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM12, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM12_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM12_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM12_CAPTURE_VAL=TIM_GetCapture1(TIM12);//获取当前的捕获值.
	 			TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}
			else 								//还未开始,第一次捕获上升沿
			{
				TIM12_CAPTURE_STA=0;			//清空
				TIM12_CAPTURE_VAL=0;
				TIM12_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM12,DISABLE ); 	//关闭定时器12
	 			TIM_SetCounter(TIM12,0);
	 			TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM12,ENABLE ); 	//使能定时器12
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM12, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}


void SENSOR_Test(void)
{
	while(1)
	{
		//白线是1 黑线是0
//		printf("s:%ld,%ld,%ld,%ld,   %ld,%ld,%ld,%ld,   %ld,%ld,%ld,%ld,    %ld,%ld,%ld,%ld\n", 
		delay_ms(40);
	}
}
