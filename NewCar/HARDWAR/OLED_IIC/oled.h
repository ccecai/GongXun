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
#ifndef __OLED_H
#define __OLED_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* defines -------------------------------------------------------------------*/
#define high 1
#define low 0
#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
/* Exported types ------------------------------------------------------------*/
typedef enum
{
    Pen_Clear = 0x00,
    Pen_Write = 0x01,
    Pen_Inversion = 0x02,
}Pen_Typedef;

/* Functions -----------------------------------------------------------------*/

/*************MADE BY LIU****************END OF FILE****/
void OLED_Init(void);
void OLED_WrDat(unsigned char dat);// -- 向OLED屏写数据
void OLED_WrCmd(unsigned char cmd);// -- 向OLED屏写命令
void OLED_Set_Pos(unsigned char x, unsigned char y);// -- 设置显示坐标
//void OLED_Fill(unsigned char bmp_dat);// -- 全屏显示(显示BMP图片时才会用到此功能)
void OLED_CLS(void);// -- 复位/清屏
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 6x8点整，用于显示ASCII码的最小阵列，不太清晰
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char * ch);// -- 8x16点整，用于显示ASCII码，非常清晰
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N);// -- 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- 将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可
//void OLED_RefreshGram(unsigned char Gram[128][8]);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void OLED_RefreshGram(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t light);
void OLED_ShowBMP(uint8_t bmp_data[128][8]);
void OLED_ShowBMP2(uint8_t bmp_data[]);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void OLED_ShowString(uint8_t row, uint8_t col, uint8_t *chr, uint8_t size);
void OLED_LOGO(uint8_t LOGO_BMP[128][8]);
void OLED_DrawNum32(uint16_t x, uint16_t y, uint16_t num);
void OLED_Printf(uint8_t x, uint8_t y, const char *fmt,...);
#endif


