/**MADE BY ME
  ******************************************************************************
  * @author   LIU_Standard
  * @version V1.0.0
  * @date    2021-8-6
  * @brief   ��е��
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
//�Ƚ����ݣ���ִ���ĸ�����
//��color_order ��code_order ��ץȡ˳��
//����code_order Ϊ 1 3 2 / 2 3 1  color_order Ϊ 2 3 1 / 1 2 3
void wait_Cam_Judge();
void ARMM_Position1_GetTop(uint8_t * code_order, uint8_t * color_order)
{
	int i = 0, j;
	for (j = 0; j < 3; j++)
	{
		while(1)
		{//ץ��i��
			if(color_order[i] == code_order[j])
			{
				Point1_Grab(i);
				break;//ץ�����break
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
	delay_ms(2000);//�𲽽���е�����ڴ�ֱλ��
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
					printf("#3P1324#4P1853T1000\r\n");//����ȥ
					delay_ms(2000);
					
					printf("#5P1680T1000\r\n");//��ס���
					delay_ms(2000);
					
					printf("#3P1882T1000\r\n");//������
					delay_ms(2000);
					
					printf("#1P1324T1000\r\n");//ת������洢
					delay_ms(2000);
					
					printf("#3P1500#4P1500T1000\r\n");//����
					delay_ms(2000);
				
					printf("#5P1235T1000\r\n");//�ɿ����
					delay_ms(2000);
				
					printf("#3P1706#4P1618T1000\r\n");//̧����
					delay_ms(2000);
				
					printf("#1P2000T1000\r\n");//�ص���ֱλ��
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

//ץ
//ץ������ϵ�˳���ԭ���İڷ�λ��һ�£�������Ҫ����RGB��˳��ڷ�

void ARMM_Position2_PutTop(int8_t color_order[6])
{
	//����RGB�ڷţ�Ҫ֪��RGB�ڳ����ĸ�λ��
	//put color_order[i]
	int i = 0, j=0;
	wait_Cam_Judge();
	//j������ر�λ�ú�ɫ��1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
				Point2_Put(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
					
				//��ץȡ���� ץȡi�����i�ǳ�������װ���̶���
				
			
			
		}
		i = 0;
	
}


void ARMM_RePosition2_PutTop(uint8_t color_order[6])
{
	//����RGB�ڷţ�Ҫ֪��RGB�ڳ����ĸ�λ��
	//put color_order[i]
	int i = 0, j=0;
	//j������ر�λ�ú�ɫ��1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
				Point2_Put(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
					
				//��ץȡ���� ץȡi�����i�ǳ�������װ���̶���
				
			
			
		}
		i = 0;
	
}

void ARMM_Position2_PutBottom(uint8_t * color_order)
{
	//����RGB�ڷţ�Ҫ֪��RGB�ڳ����ĸ�λ��
	//put color_order[i]
	int i = 3, j;
	//j������ر�λ�ú�ɫ��1
	for (j = 1; j < 4; j++)
	{
		while(1)
		{//ץ��i��
			if(color_order[i] == j)
			{
				Point2_Put(i);
				if(j<3)
					RACE_RunWithDelay(0, 150, 0, 0.4);
				//��ץȡ���� ץȡi�����i�ǳ�������װ���̶���
				break;//ץ�����break
			}
			i++;
		}
		i = 3;
	}
}

void ARMM_Position3_Stack(uint8_t color_order[6])
{
	//����RGB�ڷţ�Ҫ֪��RGB�ڳ����ĸ�λ��
	//put color_order[i]
	int i = 0, j=0;
	//j������ر�λ�ú�ɫ��1
	for (i = 0; i < 3; i++)
	{
				j = color_order[i];

				RACE_RunWithDelay(0, (j-2)*150, 0, 0.4);
				delay_ms(500);
				Point2_Put_Stack(j);
				
				RACE_RunWithDelay(0, (2-j)*150, 0, 0.4);
				delay_ms(500);
					
				//��ץȡ���� ץȡi�����i�ǳ�������װ���̶���
				
		
		}
		i = 0;
	
}

//�����Ҫ���������ֵ���бȽ�
void ARMM_Position2_GetAgain(int8_t color_order[6])
{
	//����RGB�ڷţ�Ҫ֪��RGB�ڳ����ĸ�λ��
	//put color_order[i]
	int i = 0, j=0;
	//j������ر�λ�ú�ɫ��1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
				Point2_Put_GetAgain(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
				
				//��ץȡ���� ץȡi�����i�ǳ�������װ���̶���
				
			
			
		}
		i = 0;
}

void ARMM_RePosition2_GetAgain(uint8_t color_order[6])
{
	//����RGB�ڷţ�Ҫ֪��RGB�ڳ����ĸ�λ��
	//put color_order[i]
	int i = 0, j=0;
	//j������ر�λ�ú�ɫ��1
	for (i = 0; i < 3; i++)
	{
		j = color_order[i];

				RACE_RunWithDelay((j-2)*150, 0, 0, 0.4);
				delay_ms(500);
//				Point2_Put(j);
				
				RACE_RunWithDelay((2-j)*150, 0, 0, 0.4);
				delay_ms(500);
					
				//��ץȡ���� ץȡi�����i�ǳ�������װ���̶���
				
			
			
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
		printf("#1P1324T1000\r\n");//����е�۷ŵ�����
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е�۷���
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//������ס
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//����������
    delay_ms(2000);
    printf("#1P2500T1000\r\n");//�����ŵ������Ϸ�
    delay_ms(2000);
    printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//��צ���ɿ�
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//����е�۾�����
    delay_ms(2000);
//    printf("#1P1500T1000\r\n");
//    delay_ms(2000);
//    printf("#3P1500#4P1500T1000\r\n");
//    delay_ms(2000);
}

void MoveFrom_CarMid()
{
		printf("#1P1471T1000\r\n");//�ص��м����ȡ��
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//�ع��м�
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//���������ȡ
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//������
    delay_ms(2000);
    printf("#1P2500T1000\r\n");//�˶��������Ϸ�
    delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//צ���ɿ�
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//��צ�Ӿ�����
    delay_ms(2000);
}

void MoveFrom_CarRight()
{
		printf("#1P1618T1000\r\n");//����е���ƶ�������
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//��צ�Ӽ�����
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
    printf("#1P2500T1000\r\n");//����е�۷��ڵ����Ϸ�
    delay_ms(2000);
    printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//�ɿ�צ��
    delay_ms(2000);
    printf("#3P1794#4P1441T1000\r\n");//����е��ץ����
    delay_ms(2000);
}

void MoveFrom_CarLeft_GetAgain(void)
{
		printf("#1P2500T1000\r\n");//����е�۷��ڵ����Ϸ�
		delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//��צ�Ӽ�����
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P1324T1000\r\n");//����е�۷ŵ�����
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//�ɿ�צ��
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//����е��ץ����
    delay_ms(2000);
}

void MoveFrom_CarMid_GetAgain(void)
{
		printf("#1P2500T1000\r\n");//����е�۷��ڵ����Ϸ�
		delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//��צ�Ӽ�����
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//�ɿ�צ��
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//����е��ץ����
    delay_ms(2000);
}

void MoveFrom_CarRight_GetAgain(void)
{
		printf("#1P2500T1000\r\n");//����е�۷��ڵ����Ϸ�
		delay_ms(2000);
		printf("#1P2500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//��צ�Ӽ�����
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P1618T1000\r\n");//����е�۷ŵ����� 
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//�ɿ�צ��
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//����е��ץ����
    delay_ms(2000);
}

void MoveFrom_CarLeft_Stack(void)
{
		printf("#1P1324T1000\r\n");//����е�۷ŵ�����
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//��צ�Ӽ�����
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P500T1000\r\n");//����е�۷��ڵ����Ϸ�
		delay_ms(2000);
		printf("#1P500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//�ɿ�צ��
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
}

void MoveFrom_CarMid_Stack(void)
{
		printf("#1P1471T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//��צ�Ӽ�����
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P500T1000\r\n");//����е�۷��ڵ����Ϸ�
		delay_ms(2000);
		printf("#1P500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//�ɿ�צ��
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//����е�۷ŵ��м�
    delay_ms(2000);
}

void MoveFrom_CarRight_Stack(void)
{
		printf("#1P1618T1000\r\n");//����е�۷ŵ�����
    delay_ms(2000);
		printf("#3P1500#4P1500T1000\r\n");//����е���½�
    delay_ms(2000);
		printf("#5P1618T1000\r\n");//��צ�Ӽ�����
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P500T1000\r\n");//����е�۷��ڵ����Ϸ�
		delay_ms(2000);
		printf("#1P500#3P1118#4P2130#5P1676T1000\r\n");//��������
    delay_ms(2000);
		printf("#5P1250T1000\r\n");//�ɿ�צ��
    delay_ms(2000);
		printf("#3P1794#4P1441T1000\r\n");//̧���е��
    delay_ms(2000);
		printf("#1P1471T1000\r\n");//����е�۷ŵ��м�
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

/*ץ�ϲ㣬����1���ŵ����󣬵�2���ŵ��м䣬��3���ŵ�����*/
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
/*�ȴ��Ӿ���ƫ*/
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
/*ץ�²㣬����1���ŵ����󣬵�2���ŵ��м䣬��3���ŵ�����*/
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


void Put_theLeft(void) //����������Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarLeft();

//	MoveTo_Ground();
}

void Put_theMid(void) //�������м��Ǹ�������ȥ
{
//	wait_Cam_Judge();
	MoveFrom_CarMid();
//	MoveTo_Ground();
}

void Put_theRight(void) //�������ұ��Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarRight();

//	MoveTo_Ground();
}

void Put_theLeft_GetAgain(void) //����������Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarLeft_GetAgain();

//	MoveTo_Ground();
}

void Put_theMid_GetAgain(void) //�������м��Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarMid_GetAgain();

//	MoveTo_Ground();
}

void Put_theRight_GetAgain(void) //�������ұ��Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarRight_GetAgain();

//	MoveTo_Ground();
}

void Put_theLeft_Stack(void) //����������Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarLeft_Stack();

//	MoveTo_Ground();
}

void Put_theMid_Stack(void) //�������м��Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarMid_Stack();

//	MoveTo_Ground();
}

void Put_theRight_Stack(void) //�������ұ��Ǹ�������ȥ
{
	//	wait_Cam_Judge();
	MoveFrom_CarRight_Stack();

//	MoveTo_Ground();
}



void Grab_TotheLeft(void) //�����ϵĿ�����õ����ϵ����
{
    MoveFrom_Ground();
	MoveTo_CarLeft();
}

void Grab_TotheMid(void) //�����ϵĿ�����õ����ϵ��м�
{
    MoveFrom_Ground();
	MoveTo_CarMid();
}

void Grab_TotheRight(void) //�����ϵĿ�����õ����ϵ��ұ�
{
    MoveFrom_Ground();
	MoveTo_CarRight();
}

/*��⣬��Ҫ�����˶�*/
void Stack_theLeft(void)//����������Ǹ�������ȥ���
{
	MoveFrom_CarLeft();
	MoveTo_Stack();
}

void Stack_theMid(void)//�������м��Ǹ�������ȥ���
{
	MoveFrom_CarMid();
	MoveTo_Stack();
}

void Stack_theRight(void)//�������ұ��Ǹ�������ȥ���
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
