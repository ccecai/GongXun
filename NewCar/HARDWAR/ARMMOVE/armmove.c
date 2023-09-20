/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    2021-8-6
  * @brief   机械臂
  * 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private Includes ----------------------------------------------------------*/
#include "armmove.h"
#include "usart.h"
#include "delay.h"
#include "positioncontrol.h"
#include "runrace.h"
#include "comother.h"
#include "runrace.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Global param ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
//比较数据，看执行哪个命令
//找color_order 在code_order 的抓取顺序
//比如code_order 为 1 3 2 / 2 3 1  color_order 为 2 3 1 / 1 2 3
void wait_Cam_Judge();
void ARMM_Position1_GetTop(uint8_t * code_order, uint8_t * color_order)
{
	int i = 0, j;
	for (j = 0; j < 3; j++)
	{
		while(1)
		{//抓第i个
			if(color_order[i] == code_order[j])
			{
				Point1_Grab(i);
				break;//抓完过后break
			}
			i++;
		}
		i = 0;
	}
}
//OK
void ARMM_Position1_GetBottom()
{
	printf("#1P2000#3P1706#4P1618#5P1235T1000\r\n");
	delay_ms(2000);//起步将机械臂置于垂直位置
	for(uint8_t catch_times=0;catch_times<3;catch_times++)
	{
	while(1)
	{
		usart3_send(0x58);
		if(Jiaqu_start_flag	== 1)
			{
			
			switch (color_order[catch_times])
			{
				case 1:
				{
					printf("#3P1324#4P1853T1000\r\n");//放下去
					delay_ms(2000);
					
					printf("#5P1680T1000\r\n");//夹住物块
					delay_ms(2000);
					
					printf("#3P1882T1000\r\n");//举起来
					delay_ms(2000);
					
					printf("#1P1324T1000\r\n");//转到车身存储
					delay_ms(2000);
					
					printf("#3P1500#4P1500T1000\r\n");//放置
					delay_ms(2000);
				
					printf("#5P1235T1000\r\n");//松开物块
					delay_ms(2000);
				
					printf("#3P1706#4P1618T1000\r\n");//抬起来
					delay_ms(2000);
				
					printf("#1P2000T1000\r\n");//回到垂直位置
				  delay_ms(2000);
					break;
				}
				case 2:
				{
					printf("#3P1324#4P1853T1000\r\n");
					delay_ms(2000);
					
					printf("#5P1618T1000\r\n");
					delay_ms(2000);
					
					printf("#3P1882T1000\r\n");
					delay_ms(2000);
					
					printf("#1P1471T1000\r\n");
					delay_ms(2000);
					
					printf("#3P1500#4P1500T1000\r\n");
					delay_ms(2000);
				
					printf("#5P1235T1000\r\n");
					delay_ms(2000);
				
					printf("#3P1706#4P1618T1000\r\n");
					delay_ms(2000);
				
					printf("#1P2000T1000\r\n");
				  delay_ms(2000);
					break;
				}
				case 3:
				{
					printf("#3P1324#4P1853T1000\r\n");
					delay_ms(2000);
					
					printf("#5P1618T1000\r\n");
					delay_ms(2000);
					
					printf("#3P1882T1000\r\n");
					delay_ms(2000);
					
					printf("#1P1618T1000\r\n");
					delay_ms(2000);
					
					printf("#3P1500#4P1500T1000\r\n");
					delay_ms(2000);
				
					printf("#5P1235T1000\r\n");
					delay_ms(2000);
				
					printf("#3P1706#4P1618T1000\r\n");
					delay_ms(2000);
				
					printf("#1P2000T1000\r\n");
				  delay_ms(2000);
					break;
				}
				default:
				{
					printf("#1P1471T1000\r\n");
					delay_ms(2000);
					
					printf("#3P1500#4P1500T1000\r\n");
					delay_ms(2000);
					
					break;
				}
			}
			Jiaqu_start_flag = 0;
			break;
			}
	}
	}
  
}

//抓
//抓完过后背上的顺序和原本的摆放位置一致，后面需要按照RGB的顺序摆放

void ARMM_Position2_PutTop(int8_t color_order[6])
{
	//按照RGB摆放，要知道RGB在车上哪个位置
	//put color_order[i]
	int i = 0, j=0;
	wait_Cam_Judge();
	//j是下面地标位置红色是1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
				Point2_Put(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
					
				//填抓取动作 抓取i，这个i是车子上面装填快固定的
				
			
			
		}
		i = 0;
	
}


void ARMM_RePosition2_PutTop(uint8_t color_order[6])
{
	//按照RGB摆放，要知道RGB在车上哪个位置
	//put color_order[i]
	int i = 0, j=0;
	//j是下面地标位置红色是1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
				Point2_Put(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
					
				//填抓取动作 抓取i，这个i是车子上面装填快固定的
				
			
			
		}
		i = 0;
	
}

void ARMM_Position2_PutBottom(uint8_t * color_order)
{
	//按照RGB摆放，要知道RGB在车上哪个位置
	//put color_order[i]
	int i = 3, j;
	//j是下面地标位置红色是1
	for (j = 1; j < 4; j++)
	{
		while(1)
		{//抓第i个
			if(color_order[i] == j)
			{
				Point2_Put(i);
				if(j<3)
					RACE_RunWithDelay(0, 150, 0, 0.4);
				//填抓取动作 抓取i，这个i是车子上面装填快固定的
				break;//抓完过后break
			}
			i++;
		}
		i = 3;
	}
}

void ARMM_Position3_Stack(uint8_t color_order[6])
{
	//按照RGB摆放，要知道RGB在车上哪个位置
	//put color_order[i]
	int i = 0, j=0;
	//j是下面地标位置红色是1
	for (i = 0; i < 3; i++)
	{
				j = color_order[i];

				RACE_RunWithDelay(0, (j-2)*150, 0, 0.4);
				delay_ms(500);
				Point2_Put_Stack(j);
				
				RACE_RunWithDelay(0, (2-j)*150, 0, 0.4);
				delay_ms(500);
					
				//填抓取动作 抓取i，这个i是车子上面装填快固定的
				
		
		}
		i = 0;
	
}

//获得需要两个数组的值进行比较
void ARMM_Position2_GetAgain(int8_t color_order[6])
{
	//按照RGB摆放，要知道RGB在车上哪个位置
	//put color_order[i]
	int i = 0, j=0;
	//j是下面地标位置红色是1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
				Point2_Put_GetAgain(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
				
				//填抓取动作 抓取i，这个i是车子上面装填快固定的
				
			
			
		}
		i = 0;
}

void ARMM_RePosition2_GetAgain(uint8_t color_order[6])
{
	//按照RGB摆放，要知道RGB在车上哪个位置
	//put color_order[i]
	int i = 0, j=0;
	//j是下面地标位置红色是1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
//				Point2_Put(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
					
				//填抓取动作 抓取i，这个i是车子上面装填快固定的
				
			
			
		}
		i = 0;
}


void ARMM_Test(void)
{
	while(1)
	{
//		printf("#1P1106#2P1404#3P1290#4P1350T800\r\n");
//		delay_ms(1000);
//		printf("#1P1106#2P1404#3P1390#4P1350T200\r\n");
//		delay_ms(1000);
		GrabUp_1();
	}

}

void MoveFrom_CarLeft()
{
		printf("#1P1324T1000\r\n");//将机械臂放到左面
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂放下
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//把物块夹住
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//将物块举起来
    delay_ms(2000);
    printf("#1P2500T1000\r\n");//将物块放到地区上方
    delay_ms(2000);
    printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//将物块放下
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//把爪子松开
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//将机械臂举起来
    delay_ms(2000);
//    printf("#1P1500T1000\r\n");
//    delay_ms(2000);
//    printf("#3P1500#4P1500T1000\r\n");
//    delay_ms(2000);
}

void MoveFrom_CarMid()
{
		printf("#1P1471T1000\r\n");//回到中间进行取物
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//回归中间
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//进行物体夹取
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//举起来
    delay_ms(2000);
    printf("#1P2500T1000\r\n");//运动到地区上方
    delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//将物块放下
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//爪子松开
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//将爪子举起来
    delay_ms(2000);
}

void MoveFrom_CarRight()
{
		printf("#1P1618T1000\r\n");//将机械臂移动到右面
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//将爪子加起来
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
    printf("#1P2500T1000\r\n");//将机械臂放在地区上方
    delay_ms(2000);
    printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//放下物体
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//松开爪子
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//将机械臂抓起来
    delay_ms(2000);
}

void MoveFrom_CarLeft_GetAgain(void)
{
		printf("#1P2500T1000\r\n");//将机械臂放在地区上方
		delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//放下物体
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//将爪子加起来
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P1324T1000\r\n");//将机械臂放到左面
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//松开爪子
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//将机械臂抓起来
    delay_ms(2000);
}

void MoveFrom_CarMid_GetAgain(void)
{
		printf("#1P2500T1000\r\n");//将机械臂放在地区上方
		delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//放下物体
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//将爪子加起来
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//松开爪子
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//将机械臂抓起来
    delay_ms(2000);
}

void MoveFrom_CarRight_GetAgain(void)
{
		printf("#1P2500T1000\r\n");//将机械臂放在地区上方
		delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//放下物体
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//将爪子加起来
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P1618T1000\r\n");//将机械臂放到右面 
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//松开爪子
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//将机械臂抓起来
    delay_ms(2000);
}

void MoveFrom_CarLeft_Stack(void)
{
		printf("#1P1324T1000\r\n");//将机械臂放到左面
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//将爪子加起来
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P500T1000\r\n");//将机械臂放在地区上方
		delay_ms(2000);
		printf("#1P500#3P1118#4P2130#5P1676T1000\r\n");//放下物体
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//松开爪子
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
}

void MoveFrom_CarMid_Stack(void)
{
		printf("#1P1471T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//将爪子加起来
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P500T1000\r\n");//将机械臂放在地区上方
		delay_ms(2000);
		printf("#1P500#3P1118#4P2130#5P1676T1000\r\n");//放下物体
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//松开爪子
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
}

void MoveFrom_CarRight_Stack(void)
{
		printf("#1P1618T1000\r\n");//将机械臂放到右面
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//将机械臂下降
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//将爪子加起来
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P500T1000\r\n");//将机械臂放在地区上方
		delay_ms(2000);
		printf("#1P500#3P1118#4P2130#5P1676T1000\r\n");//放下物体
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//松开爪子
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//抬起机械臂
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//将机械臂放到中间
    delay_ms(2000);
}



void MoveFrom_Ground()
{
    printf("#1P877#2P1741#3P1581#4P1550T500\r\n");
    delay_ms(520);
    printf("#1P877#2P1941#3P1376#4P1550T500\r\n");
    delay_ms(520);
    printf("#1P874#2P2156#3P1299#4P1550T500\r\n");
    delay_ms(520);
    printf("#1P874#2P2156#3P1299#4P1800T500\r\n");
    delay_ms(520);
    printf("#1P874#2P1731#3P1724#4P1800T500\r\n");
    delay_ms(520);
}

void MoveFrom_Down()
{
	printf("#1P877#2P1741#3P1581#4P1550T500\r\n");
	delay_ms(520);
	POSITONC_SetCarGoTo( 250, 0, 0, 0.6);
	printf("#1P876#2P2041#3P1291#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P876#2P2041#3P1291#4P1800T500\r\n");
	delay_ms(710);
	POSITONC_SetCarGoTo(-250, 0, 0, 0.6);
	delay_ms(310);
	printf("#1P876#2P1741#3P1551#4P1800T500\r\n");
	delay_ms(520);
}

void MoveTo_CarLeft()
{
	printf("#1P1369#2P1451#3P1785#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P1369#2P1451#3P1532#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P1369#2P1451#3P1532#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P1369#2P1451#3P1785#4P1550T500\r\n");
	delay_ms(520);
}

void MoveTo_CarMid()
{
	printf("#1P1553#2P1541#3P1956#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P1553#2P1541#3P1546#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P1553#2P1541#3P1546#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P1553#2P1541#3P1956#4P1550T500\r\n");
	delay_ms(520);
}

void MoveTo_CarRight()
{
	printf("#1P1720#2P1443#3P1903#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P1720#2P1443#3P1507#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P1720#2P1443#3P1507#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P1720#2P1443#3P1903#4P1550T500\r\n");
	delay_ms(520);
}

void MoveTo_Ground()
{
		printf("#5P1618T1000\r\n");
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");
    delay_ms(2000);
    printf("#1P2500T1000\r\n");
    delay_ms(2000);
    printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");
    delay_ms(2000);
		printf("#5P1350T1000\r\n");
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");
    delay_ms(2000);
//    printf("#1P1500T1000\r\n");
//    delay_ms(2000);
//    printf("#3P1500#4P1500T1000\r\n");
//    delay_ms(2000);
}

void MoveTo_Stack()
{
    printf("#1P877#2P1741#3P1581#4P1800T500\r\n");
    delay_ms(520);
    printf("#1P877#2P1906#3P1381#4P1800T500\r\n");
    delay_ms(520);
    printf("#1P877#2P1906#3P1381#4P1550T500\r\n");
    delay_ms(520);
    printf("#1P877#2P1741#3P1581#4P1550T500\r\n");
    delay_ms(520);	
}

/*抓上层，按第1个放到最左，第2个放到中间，第3个放到最右*/
void GrabUp_1(void)
{
	printf("#1P1113#2P1644#3P1781#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P1113#2P1798#3P1594#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P1113#2P1798#3P1594#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P1113#2P1644#3P1781#4P1800T500\r\n");
	delay_ms(520);
	
	MoveTo_CarLeft();
}
void GrabUp_2(void)
{
	printf("#1P899#2P1520#3P1716#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P877#2P1629#3P1440#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P877#2P1629#3P1440#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P899#2P1520#3P1716#4P1800T500\r\n");
	delay_ms(520);
	
	MoveTo_CarMid();
}
void GrabUp_3(void)
{
	printf("#1P650#2P1644#3P1781#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P635#2P1798#3P1555#4P1550T500\r\n");
	delay_ms(520);
	printf("#1P635#2P1798#3P1555#4P1800T500\r\n");
	delay_ms(520);
	printf("#1P650#2P1644#3P1781#4P1800T500\r\n");
	delay_ms(520);
	
	MoveTo_CarRight();
}
/*等待视觉纠偏*/
void wait_Cam_Judge()
{
	printf("00\r\n");
	while(1)
	{
		usart3_send(0x60);
		if(start_flag_pos == 1&& pos_judge_flag == 1)
		{
			printf("%d,%d\r\n",Pos_x,Pos_y);
//			RACE_RunWithDelay( Pos_x, Pos_y, 0,0.5);
			delay_ms(500);
			Pos_x = 0;Pos_y = 0;
			start_flag_pos = 0;
			break;
		}
	}
}
/*抓下层，按第1个放到最左，第2个放到中间，第3个放到最右*/
void GrabDown_1(void)
{
	MoveFrom_Down();
	MoveTo_CarLeft();
}
void GrabDown_2(void)
{
	MoveFrom_Down();
	MoveTo_CarMid();
}
void GrabDown_3(void)
{
	MoveFrom_Down();
	MoveTo_CarRight();
}


void Put_theLeft(void) //将车上左边那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarLeft();

//	MoveTo_Ground();
}

void Put_theMid(void) //将车上中间那个放置下去
{
//	wait_Cam_Judge();
	MoveFrom_CarMid();
//	MoveTo_Ground();
}

void Put_theRight(void) //将车上右边那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarRight();

//	MoveTo_Ground();
}

void Put_theLeft_GetAgain(void) //将车上左边那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarLeft_GetAgain();

//	MoveTo_Ground();
}

void Put_theMid_GetAgain(void) //将车上中间那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarMid_GetAgain();

//	MoveTo_Ground();
}

void Put_theRight_GetAgain(void) //将车上右边那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarRight_GetAgain();

//	MoveTo_Ground();
}

void Put_theLeft_Stack(void) //将车上左边那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarLeft_Stack();

//	MoveTo_Ground();
}

void Put_theMid_Stack(void) //将车上中间那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarMid_Stack();

//	MoveTo_Ground();
}

void Put_theRight_Stack(void) //将车上右边那个放置下去
{
	//	wait_Cam_Judge();
	MoveFrom_CarRight_Stack();

//	MoveTo_Ground();
}



void Grab_TotheLeft(void) //将地上的块儿放置到车上的左边
{
    MoveFrom_Ground();
	MoveTo_CarLeft();
}

void Grab_TotheMid(void) //将地上的块儿放置到车上的中间
{
    MoveFrom_Ground();
	MoveTo_CarMid();
}

void Grab_TotheRight(void) //将地上的块儿放置到车上的右边
{
    MoveFrom_Ground();
	MoveTo_CarRight();
}

/*码垛，需要地盘运动*/
void Stack_theLeft(void)//将车上左边那个放置下去码垛
{
	MoveFrom_CarLeft();
	MoveTo_Stack();
}

void Stack_theMid(void)//将车上中间那个放置下去码垛
{
	MoveFrom_CarMid();
	MoveTo_Stack();
}

void Stack_theRight(void)//将车上右边那个放置下去码垛
{
	MoveFrom_CarRight();
	MoveTo_Stack();
}

void ARM_Init(void)
{
	printf("#1P899#2P1520#3P1716#4P1550T500\r\n");
	delay_ms(520);
	//1P 900
		
	//4P 1550-1800
}


void Point1_Grab(int i)
{
	switch(i)
	{
		case 0: GrabUp_1();break;
		case 1: GrabUp_2();break;
		case 2: GrabUp_3();break;
		case 3: GrabDown_1();break;
		case 4: GrabDown_2();break;
		case 5: GrabDown_3();break;
		
		default:break;
	}
}

void Point2_Put_GetAgain(int i)
{
	switch(i)
	{
		case 0: Put_theLeft_GetAgain();break;
		case 1: Put_theMid_GetAgain();break;
		case 2: Put_theRight_GetAgain();break;
		case 3: Put_theLeft_GetAgain();break;
		case 4: Put_theMid_GetAgain();break;
		case 5: Put_theRight_GetAgain();break;
		
		default:break;
	}
}

void Point2_Put_Stack(int i)
{
	switch(i)
	{
		case 0: Put_theLeft_Stack();break;
		case 1: Put_theMid_Stack();break;
		case 2: Put_theRight_Stack();break;
		case 3: Put_theLeft_Stack();break;
		case 4: Put_theMid_Stack();break;
		case 5: Put_theRight_Stack();break;
		
		default:break;
	}
}


void Point2_Put(int i)
{
	switch(i)
	{
		case 0: Put_theLeft();break;
		case 1: Put_theMid();break;
		case 2: Put_theRight();break;
		case 3: Put_theLeft();break;
		case 4: Put_theMid();break;
		case 5: Put_theRight();break;
		
		default:break;
	}
}

void Point2_Grab(int i)
{
	switch(i)
	{
		case 1: Grab_TotheLeft();break;
		case 2: Grab_TotheMid();break;
		case 3: Grab_TotheRight();break;
		
		default:break;
	}	
}

void Point3_Stack(int i)
{
	switch(i)
	{
		case 3: Stack_theLeft();break;
		case 4: Stack_theMid();break;
		case 5: Stack_theRight();break;
		
		default:break;
	}
}
