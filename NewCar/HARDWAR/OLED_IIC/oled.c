/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    04-August-2014
  * @brief  
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "oled.h"
#include "code.h"
#include "delay.h"
#include "myiic.h"
#include "oledfont.h"
#include <stdio.h>
#include <stdarg.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Global param ------------------------------------------------------------*/
uint8_t OLED_Gram[128][8]; //任何操作都是更新他
/* Private functions --------------------------------------------------------*/
/**
 * OLED flash Addr:
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
**/
/**
  * @brief  OLED_写数据
  * @param  None
  * @retval None
  */
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	IIC_Send_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

/**
  * @brief  OLED_写命令
  * @param  None
  * @retval None
  */
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	IIC_Send_Byte(0x78);            //Slave address,SA0=0
	if(IIC_Wait_Ack())
	{
		//while(1);					//明显有问题
	}
	IIC_Send_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	IIC_Send_Byte(IIC_Command); 
	IIC_Wait_Ack();
	IIC_Stop();
}
/**
  * @brief  设置点位置
  * @param  None
  * @retval None
  */
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	x += 2;
	OLED_WrCmd(0xb0+y);			//确定页
	OLED_WrCmd((x&0xf0)|0x00); //设置低四位地址
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
} 


/* Functions -----------------------------------------------------------------*/

/**
  * @brief  调用一次刷新屏幕
  * @param  None
  * @retval None
  */
void OLED_RefreshGram(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_Set_Pos(0, i);
		for (n = 0; n < 128; n++)
		{
			OLED_WrDat(OLED_Gram[n][i]);
		}
	}
}

/**
  * @brief  画一个点,从左上角为(0,0)
  * @param  uint8_t x, uint8_t y, uint8_t light（是否点亮）
  * @retval None
  */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t light)
{
	uint8_t which_row, which_page;
	
	if (x>127 || y > 63) return;//表示超出范围了
	
	which_page = 7 - y / 8;  //这样可以从左上角开始定位
	//which_page = y / 8; 
	
	which_row = 1 << (7 - (y % 8)); // 0000 0001 << (7 - 0)
	// 使用或等于不会清除原来的点
	if (light)OLED_Gram[x][which_page] |= which_row; //操作位
	else OLED_Gram[x][which_page] &= ~which_row;
}

/**
 * @brief   画线
 * @param   x1, y1: the start point of line
 * @param   x2, y2: the end of line
 * @param  
 * @retval  None
 */
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    uint8_t col = 0, row = 0;
    uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
    float k = 0.0f, b = 0.0f;

    if (y1 == y2)
    {
        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x1);

        for (col = x_st; col <= x_ed; col++)
        {
            OLED_DrawPoint(col, y1, 1);
        }
    }
    else if (x1 == x2)
    {
        (y1 <= y2) ? (y_st = y1):(y_st = y2);
        (y1 <= y2) ? (y_ed = y2):(y_ed = y1);

        for (row = y_st; row <= y_ed; row++)
        {
            OLED_DrawPoint(x1, row, 1);
        }
    }
    else
    {
        k = ((float)(y2 - y1)) / (x2 - x1);
        b = (float)y1 - k * x1;

        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x2);

        for (col = x_st; col <= x_ed; col++)
        {
            OLED_DrawPoint(col, (uint8_t)(col * k + b), 1);
        }
    }
}

/**
  * @brief  显示一个字符
  * @param  x,y 字符, 字符大小12 16 24，正反显示模式
  * @retval None
  */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
	uint8_t c_size = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //获得一个字体字符占用的字节数
	uint8_t which_char;
	uint8_t y0 = y;
	chr = chr - 32;
	for (uint8_t i = 0; i <c_size; i++)
	{
		if (size == 12)which_char = asc2_1206[chr][i];
		else if (size == 16)which_char = asc2_1608[chr][i];
		else if (size == 24)which_char = asc2_2412[chr][i];
		//else if (size == 255)which_char = sz32[1][i];//6432  8*32=126
		else return;
		for (uint8_t j = 0; j < 8; j++)
		{
			if(which_char & 0x80) OLED_DrawPoint(x, y, mode);
			else  OLED_DrawPoint(x, y, !mode);//判断最高位是否需要画点
			which_char <<= 1;
			y++;
			if((y - y0) == size) //进行下一列
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

/**
 * @brief   显示一个字符串
 * @param   row: row of character string begin
 * @param   col: column of character string begin
 * @param   chr: the pointer to character string
 * @retval  None
 */
void OLED_ShowString(uint8_t row, uint8_t col, uint8_t *chr, uint8_t size)
{
    uint8_t n =0;

    while (chr[n] != '\0')
    {
        OLED_ShowChar(row, col, chr[n], size, 1);
        row+=size/2;
		n++;
    }
}

/**
  * @brief  清屏
  * @param  None
  * @retval None
  */
void OLED_CLS(void)
{
	uint8_t y,x;
	for(y=0;y<8;y++)
	{
		for(x=0;x < X_WIDTH;x++)
			OLED_Gram[x][y] = 0;
	}
}

void OLED_LOGO(uint8_t LOGO_BMP[128][8])
{
    OLED_CLS();
    uint8_t temp_char = 0;
    uint8_t x = 0, y = 0;
    uint8_t i = 0;
    for(; y < 64; y += 8)
    {
        for(x = 0; x < 128; x++)
        {
            temp_char = LOGO_BMP[x][y/8];
            for(i = 0; i < 8; i++)
            {
                if(temp_char & 0x80) OLED_DrawPoint(x, y + i,1);
                else OLED_DrawPoint(x,y + i,0);
                temp_char <<= 1;
            }
        }
    }
    OLED_RefreshGram();
}

void OLED_ShowBMP(uint8_t bmp_data[128][8])
{
	OLED_LOGO(bmp_data);
}

void OLED_DrawNum32(uint16_t x, uint16_t y, uint16_t num)
{
	uint8_t i,j,k,c;

    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c = *(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	OLED_DrawPoint(x+j*8+k,y+i,1);
			}
		}
	}
}

void OLED_Printf(uint8_t x, uint8_t y, const char *fmt,...)
{
    uint8_t LCD_BUF[128] = {0};
    uint8_t remain_size = 0;
    va_list ap;

    va_start(ap, fmt);
    vsprintf((char *)LCD_BUF, fmt, ap);
    va_end(ap);
    remain_size = 128 - y;
    LCD_BUF[remain_size] = '\0';
	
    OLED_ShowString(x, y, LCD_BUF, 16);
}
/**
  * @brief  OLED初始化
  * @param  None
  * @retval None
  */
void OLED_Init(void)
{
	IIC_Init();
	delay_ms(500);//初始化之前的延时很重要！
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc0);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock   
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_CLS(); //初始清屏
	OLED_Set_Pos(0,0);
}
