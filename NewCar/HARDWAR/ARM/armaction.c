//#include "armaction.h"
//#include "usart.h"
//#include "delay.h"

////char start_unfold[] = "#1P1950#2P1470#3P1470#4P1470#5P1500T800\r\n";
////char start_fold[] = "#1P1230#2P2400#3P2500#4P2130#5P2100T800\r\n";

////char mid_go1[] = "#1P1950#2P1184#3P2474#4P1316#5P1710T800\r\n";
////char mid_go2[] = "#1P1935#2P500#3P2150#4P500#5P1710T800\r\n";
////char mid_get[] = "#1P1935#2P500#3P2150#4P500#5P2140T500\r\n";
////char mid_back[] = "#1P1950#2P1184#3P2474#4P1316#5P2140T800\r\n";

////char all_back[] = "#1P1230#2P2400#3P2500#4P1930#5P2140T1000\r\n";

////char left_go1[] = "#1P2211#2P1711#3P2421#4P1700#5P2100T800\r\n";
////char left_go2[] = "#1P2181#2P500#3P1962#4P819#5P1740T800\r\n";
////char left_get[] = "#1P2181#2P500#3P1962#4P819#5P2140T800\r\n";
////char left_back[] = "#1P2181#2P1474#3P2263#4P2026#5P2140T800\r\n";

////char right_go1[] = "#1P1658#2P1711#3P2421#4P1700#5P2100T800\r\n";
////char right_go2[] = "#1P1658#2P500#3P1962#4P819#5P1740T800\r\n";
////char right_get[] = "#1P1658#2P500#3P1962#4P819#5P2140T800\r\n";
////char right_back[] = "#1P1658#2P1474#3P2263#4P2026#5P2140T800\r\n";

////char home1_go1[] = "#1P1230#2P1368#3P1053#4P632#5P2140T1200\r\n";
////char home1_go2[] = "#1P1230#2P2430#3P1050#4P2053#5P2140T1000\r\n";
////char home1_put[] = "#1P1230#2P2430#3P1050#4P2053#5P1740T800\r\n";
////char home1_back1[] = "#1P1230#2P1500#3P789#4P1053#5P2140T800\r\n";
////char home1_back2[] = "#1P1230#2P2400#3P2500#4P2130#5P2100T1500\r\n";

////char home2_go1[] = "#1P1230#2P1816#3P2105#4P2263#5P2140T800\r\n";
////char home2_go2[] = "#1P1230#2P500#3P2020#4P639#5P2140T1500\r\n";
////char home2_put[] = "#1P1230#2P500#3P2020#4P639#5P1740T500\r\n";
////char home2_back1[] = "#1P1230#2P1263#3P2053#4P1895#5P2140T1000\r\n";
////char home2_back2[] = "#1P1230#2P2400#3P2500#4P2130#5P2100T800\r\n";

////char orderreturn[] = "#CC\r\n";

////void armgo(char*a)
////{
//////	while(strcmp(USART_RX_BUF,orderreturn) != 0)
//////	{
//////		printf("%s",a);
//////		LED0_OPEN;
//////		delay_ms(500);
//////	}
//////	for(int i = 0;i == USART_RX_STA;i++)
//////	{
//////		USART_RX_BUF[i] = 0;		
//////	}
//////	USART_RX_STA = 0;
//////	LED1_OPEN;
//////	LED0_CLOSE;
//////	LED1_CLOSE;
////	printf("%s",a);
////}
//void start(void)          //上电伸直后收缩
//{
////	printf("#1P1950#2P1470#3P1470#4P1470#5P1500T800\r\n");
////	delay_ms(1000);
//	printf("#1P1230#2P2400#3P2500#4P2130#5P2100T800\r\n");
//	delay_ms(1000);
//}
//void mid_home1(void)
//{
//	printf("#1P1950#2P1184#3P2474#4P1316#5P1710T800\r\n");
//	delay_ms(1000);
//	printf("#1P1935#2P500#3P2150#4P500#5P1710T800\r\n");
//	delay_ms(1000);
//	printf("#1P1935#2P500#3P2150#4P500#5P2140T500\r\n");
//	delay_ms(700);
//	printf("#1P1950#2P1184#3P2474#4P1316#5P2140T800\r\n");
//	delay_ms(1000);
//	delay_ms(2000);
//	printf("#1P1230#2P1368#3P1053#4P632#5P2140T1200\r\n");
//	delay_ms(1400);
//	printf("#1P1230#2P2430#3P958#4P2133#5P2140T1000\r\n");
//	delay_ms(1000);
//	printf("#1P1230#2P2430#3P958#4P2133#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P1230#2P1500#3P789#4P1053#5P2140T800\r\n");
//	delay_ms(1000);
//	Position_SetCarGoTo(-315,300,0, 0.7);	
//	delay_ms(2000);
//	printf("#1P1230#2P2400#3P2500#4P2130#5P2100T1500\r\n");
//	delay_ms(1700);
//	
//	
//}
//void mid_home2(void)
//{
//	printf("#1P1950#2P1184#3P2474#4P1316#5P1710T800\r\n");
//	delay_ms(1000);
//	printf("#1P1935#2P500#3P2150#4P500#5P1710T800\r\n");
//	delay_ms(1000);
//	printf("#1P1935#2P500#3P2150#4P500#5P2140T500\r\n");
//	delay_ms(700);
//	printf("#1P1950#2P1184#3P2474#4P1316#5P2140T800\r\n");
//	delay_ms(1000);
//	delay_ms(1000);
//	Position_SetCarGoTo(315,300,0, 0.7);	
//	delay_ms(2000);
//	

//	printf("#1P1230#2P1816#3P2105#4P2263#5P2140T800\r\n");
//	delay_ms(1000);
//	printf("#1P1230#2P500#3P2069#4P639#5P2140T1500\r\n");
//	delay_ms(1700);
//	printf("#1P1230#2P500#3P2020#4P639#5P1740T500\r\n");
//	delay_ms(700);
//	printf("#1P1230#2P1263#3P2053#4P1895#5P2140T1000\r\n");
//	delay_ms(1200);
//	Position_SetCarGoTo(-315,-300,0, 0.7);	
//	delay_ms(2000);
//	
//	printf("#1P1230#2P2400#3P2500#4P2130#5P2100T800\r\n");
//	delay_ms(1000);
//}
//void left_home1(void)
//{
//	//printf("#1P2211#2P1711#3P2421#4P1700#5P2100T800\r\n");
//	printf("#1P2211#2P1711#3P2421#4P1700#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P2181#2P500#3P1962#4P819#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P2181#2P500#3P1962#4P819#5P2140T800\r\n");
//	delay_ms(1000);
//	printf("#1P2181#2P1474#3P2263#4P2026#5P2140T800\r\n");
//	delay_ms(1000);
//	
//	Position_SetCarGoTo(315,-300,0, 0.7);	
//	delay_ms(2000);
//	printf("#1P1230#2P1368#3P1053#4P632#5P2140T1200\r\n");
//	delay_ms(1400);
//	printf("#1P1230#2P2430#3P958#4P2133#5P2140T1000\r\n");
//	delay_ms(1200);
//	printf("#1P1230#2P2430#3P958#4P2133#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P1230#2P1500#3P789#4P1053#5P2140T800\r\n");
//	delay_ms(1000);
//	Position_SetCarGoTo(-315,300,0, 0.7);	
//	delay_ms(2000);
//	
//	printf("#1P1230#2P2400#3P2500#4P2130#5P2100T1500\r\n");
//	delay_ms(1700);
//}
//void left_home2(void)
//{
//	//printf("#1P2211#2P1711#3P2421#4P1700#5P2100T800\r\n");
//	printf("#1P2211#2P1711#3P2421#4P1700#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P2181#2P500#3P1962#4P819#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P2181#2P500#3P1962#4P819#5P2140T800\r\n");
//	delay_ms(1000);
//	printf("#1P2181#2P1474#3P2263#4P2026#5P2140T800\r\n");
//	delay_ms(1000);
//	Position_SetCarGoTo(315,300,0, 0.7);	
//	delay_ms(2000);
//	
//	printf("#1P1230#2P1816#3P2105#4P2263#5P2140T800\r\n");
//	delay_ms(1000);
//	printf("#1P1230#2P500#3P2069#4P639#5P2140T1500\r\n");
//	delay_ms(1700);
//	printf("#1P1230#2P500#3P2020#4P639#5P1740T500\r\n");
//	delay_ms(700);
//	printf("#1P1230#2P1263#3P2053#4P1895#5P2140T1000\r\n");
//	delay_ms(1200);
//	Position_SetCarGoTo(-315,-300,0, 0.7);	
//	delay_ms(2000);
//	printf("#1P1230#2P2400#3P2500#4P2130#5P2100T800\r\n");
//	delay_ms(1000);
//}
//void right_home1(void)
//{
//	//printf("#1P1658#2P1711#3P2421#4P1700#5P2100T800\r\n");
//	printf("#1P1658#2P1711#3P2421#4P1700#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P1679#2P500#3P1962#4P819#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P1679#2P500#3P1962#4P819#5P2140T800\r\n");
//	delay_ms(1000);
//	printf("#1P1658#2P1474#3P2263#4P2026#5P2140T800\r\n");
//	delay_ms(1000);
//	Position_SetCarGoTo(315,-300,0, 0.7);	
//	delay_ms(2000);
//	
//	printf("#1P1230#2P1368#3P1053#4P632#5P2140T1200\r\n");
//	delay_ms(1400);
//	printf("#1P1230#2P2430#3P958#4P2133#5P2140T1000\r\n");
//	delay_ms(1200);
//	printf("#1P1230#2P2430#3P958#4P2133#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P1230#2P1500#3P789#4P1053#5P2140T800\r\n");
//	delay_ms(1000);
//	Position_SetCarGoTo(-315,300,0, 0.7);	
//	delay_ms(2000);
//	
//	printf("#1P1230#2P2400#3P2500#4P2130#5P2100T1500\r\n");
//	delay_ms(1700);
//}
//void right_home2(void)
//{
//	//printf("#1P1658#2P1711#3P2421#4P1700#5P2100T800\r\n");
//	printf("#1P1658#2P1711#3P2421#4P1700#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P1679#2P500#3P1962#4P819#5P1740T800\r\n");
//	delay_ms(1000);
//	printf("#1P1679#2P500#3P1962#4P819#5P2140T800\r\n");
//	delay_ms(1000);
//	printf("#1P1658#2P1474#3P2263#4P2026#5P2140T800\r\n");
//	delay_ms(1000);
//	delay_ms(1000);
//	Position_SetCarGoTo(315,300,0, 0.7);	
//	delay_ms(2000);
//	
//	printf("#1P1230#2P1816#3P2105#4P2263#5P2140T800\r\n");
//	delay_ms(1000);
//	printf("#1P1230#2P500#3P2069#4P639#5P2140T1500\r\n");
//	delay_ms(1700);
//	printf("#1P1230#2P500#3P2020#4P639#5P1740T500\r\n");
//	delay_ms(700);
//	printf("#1P1230#2P1263#3P2053#4P1895#5P2140T1000\r\n");
//	delay_ms(1200);
//	Position_SetCarGoTo(-315,-300,0, 0.7);	
//	delay_ms(2000);
//	
//	printf("#1P1230#2P2400#3P2500#4P2130#5P2100T800\r\n");
//	delay_ms(1000);
//}




