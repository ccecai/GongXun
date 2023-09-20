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
#ifndef __REMOTE_CONTROL_H
#define __REMOTE_CONTROL_H

/* Includes ------------------------------------------------------------------*/

/* defines --------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Functions --------------------------------------------------------------------*/
void REMOTEC_ControlLoop(void);
void REMOTEC_Init(void);
void REMOTEC_SetCarSpeed(float A_xy, float A_z, float V_x, float V_y, float V_z);
void REMOTEC_Debug(void);
void REMOTEC_SetWithControl(void);
void REMOTEC_UpdateRecInfo(void);
#endif

/*************MADE BY LIU****************END OF FILE****/
