#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"
#define PRESS 0
#define UNPRESS	1

#define PRESSED 1	//按下过
#define UNPRESSED 0
#define PRESSING 3//正在按着

#define KEY1	PFin(6)
#define KEY2	PFin(7)

void KEY_Init(void);
void KEY1_Delay(void);
void KEY2_Delay(void);
void KeyStatusUpdate(void);
void ClearKeyPressed(void);
uint8_t IsKeyPressed(void);







#endif















