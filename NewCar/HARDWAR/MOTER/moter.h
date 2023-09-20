/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    2021-7-25
  * @brief  有关电机的参数在此 
  ******************************************************************************
  * @attention
  * 电机购买 https://item.taobao.com/item.htm?id=45347924687
	* 驱动器购买 https://item.taobao.com/item.htm?id=551092190133
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTER_H
#define __MOTER_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines --------------------------------------------------------------------*/
#define MOTER_EN PCout(11)
/* Exported types ------------------------------------------------------------*/
/** 
  * @brief 电机电流值 structure definition  
  */
typedef struct
{
  int16_t Wheel1;
  int16_t Wheel2;
  int16_t Wheel3;
  int16_t Wheel4;
} MotorCurrent_t;

/* Functions --------------------------------------------------------------------*/
void MOTER_SetCurrent(int16_t Wheel1, int16_t Wheel2, int16_t Wheel3, int16_t Wheel4);
void MOTER_SetPolarity(int8_t Wheel_1, int8_t Wheel_2, int8_t Wheel_3, int8_t Wheel_4);
void MOTER_SetCompare(int16_t w_ch1, int16_t w_ch2, int16_t w_ch3, int16_t w_ch4);
void MOTOR_GPIO_Config(void);
void TIM2_PWM_Init(uint32_t psc, uint32_t arr); //psc预分频--arr范围
void TIM18_Encoder_Init(uint32_t psc, uint32_t arr);
void TIM34_Encoder_Init(uint32_t psc, uint32_t arr);
void MOTOR_PWM_Init(void);
#endif

/*************MADE BY LIU****************END OF FILE****/
