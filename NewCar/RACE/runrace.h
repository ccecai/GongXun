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
#ifndef __RUN_RACE_H
#define __RUN_RACE_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* defines --------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Functions ----------------------------------------------------------------*/
void RACE_RunSome(void);
void RACE_RunWithDelay(float Position_x, float Position_y, float Angel_z, float max_velocity);
void RACE_RunPoint(void);
extern int8_t color_order[6];
#endif 

/*************MADE BY LIU****************END OF FILE****/

