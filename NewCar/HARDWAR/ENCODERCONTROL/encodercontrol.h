/**MADE BY ME
  ****************************************************************************** 	
  * @author  LIU_Standard
  * @version V1.0.0
  * @date    2021-7-25
  * @brief   encodercontrol
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENCODER_CONTROL_H
#define __ENCODER_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines --------------------------------------------------------------------*/
#define ENCODER_TIM_PERIOD 65535U
/* Exported types ------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
void ENCODER_UpdateLoop(void);
void Moter_Performance_Test(void);
void TIM1_Encoder_Init(uint32_t psc, uint32_t arr);
void TIM235_Encoder_Init(uint32_t psc, uint32_t arr);
void ENCODER_Init(void);
#endif

/*************MADE BY LIU****************END OF FILE****/
