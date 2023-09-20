#include "main.h"

// 主函数在此初始化
void MainInit(void);

int main()
{
	MainInit();
	
//	LED_Test();
//	MPU_PrintMPUYaw();
//	SPEEDC_DebugSpeedPID();
//	POSITIONC_TestZ();
//	RACE_RunPoint();
//	TRACE_ToPoint1();
	RACE_RunSome();
//	TRACE_Test();
//	TRACE_Adjust();
	
	while(1){
//		USART1_PrintfTest();
//		USART1_DmaLoop();
		delay_ms(100);
		
	}
}

void MainInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	LED_Init();
	KEY_Init();
//	while(1);
	COM_PCInit(9600);
	SPEEDC_PidInit();
	POSITIONC_PidDeInit();
	USART1_InitWithDma(115200);
	MOTOR_PWM_Init();
	HWT101_Init();
	TIM6_TimingInit(84 - 1, 5000 - 1); // 5ms 84M/84/5000 = 0.2khz, 5ms
	
//	KEY1_Delay();
	SENSOR_Init();
	
	ENCODER_Init();
	delay_ms(1000);
}
