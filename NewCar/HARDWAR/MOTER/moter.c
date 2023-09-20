/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    2021-7-25
  * @brief  moter control
  * 
  ******************************************************************************
  * @attention 电机控制参数
	* 控制方向1-4 PF12 PF14 PE7 PC0
	*	电机使能EN PC11
	*	电机PWM TIM2_CH1-4 PA5 PA1 PB10 PB11
	*	线数13ppr，4倍频减速比30：1，经过测试5ms,最大读47个脉冲
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
  * @brief 设定小车的运动方向 控制方向1-4 PF12 PF14 PE7 PC0
  * @param  int8_t Wheel_1,int8_t Wheel_2,int8_t Wheel_3,int8_t Wheel_4
  * @retval None
  */
//////////////////////////
//设置四个电机控制正反转的引脚中，驱动桥每个通道的IN1为高电平向前走还是IN2为高电平向前走
//输入形参：Wheel_1为0表示输出GND刹车，1表示设置IN1为高IN2为低，2表示设置IN2为高IN1为低，3表示IN1和IN2都为1，
//默认Wheel_1、2、3、4分别为和PE9/PE11/PE13/PE14相配合的
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
  * @brief  设置电机电流，最大电流为2000
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
	//第一个是电机3  第二个是电机-2 第三个是电机4  第四个是电机1
	
	MOTER_SetPolarity(Temp4, Temp2, Temp1, Temp3);
	MOTER_SetCompare(Wheel4, Wheel2, Wheel1, Wheel3);
}

/**
  * @brief 	设定电机占电流，转化后全为正数 
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
  * @brief  电机GPIO初始化
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
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9); //复用为TIM2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9); //复用为TIM2
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10); //复用为TIM2
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11); //复用为TIM2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
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
  * @brief  PWM 定时器9-ch12-pa2 pa3 定时器10-ch1-pf6 定时器11-ch1-pf7初始化 普通定时器168MHZ f = 84/(psc + 1)/(arr + 1)
  * @param  uint32_t psc, uint32_t arr
  * @retval None
  */
void MOTOR_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定时器基本设置结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;//定时器输出结构体
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);	
  
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//即输出类型设置，
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOE,GPIO_Pin_9 |GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14);
	//设置低电平，小车处于刹车状态 ,GPIO_Pin_9 |GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14
  
	GPIO_InitStructure.GPIO_Pin = \
	(GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//即输出类型设置，
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	//设置低电平，小车处于刹车状态 ,
	
	TIM_TimeBaseStructure.TIM_Prescaler=21-1;
//	//定时器1在APB2上，频率为168MHZ，定时器时钟==168MHZ/TIM_Prescaler+1，则在此配置下，计数周期为1/8000000秒，即0.125微秒
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=2000 - 1;   //自动重装载值，一次计数0.125微秒，则PWM周期为250us，频率为4KHZ，最小脉宽0.125微秒
//	//注意，驱动桥上说最小脉宽小于10微秒时驱动桥可能没输出，10us在此配置下为80个脉冲，可软件设置当占空比小于80时设置为0，也可以不用管
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //设置时钟分割，DIV1=0不延时。
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	//初始化TIM1 Channel 1/2/3/4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:PWM1(先输出有效电平，达到计数值后输出无效电平,），PWM2刚好相反
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高，即高电平有效,HIGH是高电平有效;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//在空闲时输出，解析见：https://blog.csdn.net/gtkknd/article/details/52188266	
	TIM_OCInitStructure.TIM_Pulse=0;//即初始化时的占空比，设置为0%，
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	//Motor_SetCompareNum(2000,2000,0,0);	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 ，即更新事件后才同步到影子寄存器，开始起作用
		
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1，就开始产生pwm波了。
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能TIM1的PWM输出，TIM1与TIM8有效,如果没有这行会问题,
}


