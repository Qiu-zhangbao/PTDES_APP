#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include "led.h"
#include "sx670.h"
#include "timer.h"
#include "key.h"
#include "gui.h"


/****************************************************************************************************
//=======================================液晶屏数据线接线==========================================//
//DB0       接PD14 
//DB1       接PD15 
//DB2       接PD0 
//DB3       接PD1 
//DB4~DB12  依次接PE7~PE15
//DB13      接PD8 
//DB14      接PD9
//DB15      接PD10  
//=======================================液晶屏控制线接线==========================================//
//LCD_CS	接PG12	//片选信号
//LCD_RS	接PG0	//寄存器/数据选择信号
//LCD_WR	接PD5	//写信号
//LCD_RD	接PD4	//读信号
//LCD_RST	接PC5	//复位信号
//LCD_LED	接PB0	//背光控制信号(高电平点亮)
//=========================================触摸屏触接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
//MO(MISO)	接PF8	//SPI总线输出
//MI(MOSI)	接PF9	//SPI总线输入
//PEN		接PF10	//触摸屏中断信号
//TCS		接PB2	//触摸IC片选
//CLK		接PB1	//SPI总线时钟
**************************************************************************************************/	


//IAP配置
//IROM1：0x20001000	0xC000
//IRAM1：0x2000D000	0x3000
//SCB->VTOR = SRAM_BASE | 0x1000;	这句话要放中断初始化之后

//默认配置
//IROM1：0x8000000	0x80000
//IRAM1：0x20000000	0x10000


vu8 key=0;	
uint32_t time_us=0;
SX670_t sx670_parm;

int main(void)
{	
	
	
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	delay_init();	     //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	LCD_Init();	   //液晶屏初始化 
	LED_Init();
	EE_SX670_INIT();
	TIM3_Int_Init(71,9);
	
	SCB->VTOR = SRAM_BASE | 0x1000;	//中断向量表偏移
	
	TP_Init();
	KEY_Init();
	
	DrawTestPage("光电计时数字实验系统");
	Show_Str(100,16+50,WHITE,BLACK,"传感器1:",16,1);
	Show_Str(100,16+50+50,WHITE,BLACK,"传感器2:",16,1);
	Show_Str(100,16+50+50+50,WHITE,BLACK,"传感器3:",16,1);
	Show_Str(100,16+50+50+50+50,WHITE,BLACK,"传感器4:",16,1);
	
	Show_Str(280,16+50,WHITE,BLACK,"us",16,1);
	Show_Str(280,16+50+50,WHITE,BLACK,"us",16,1);
	Show_Str(280,16+50+50+50,WHITE,BLACK,"us",16,1);
	Show_Str(280,16+50+50+50+50,WHITE,BLACK,"us",16,1);
	
	Show_Str(420,16+50+50+50+50+50,WHITE,RED,"清零",16,1);
	
	
	POINT_COLOR=RED;
	
	
	
	time_us=0;
	
	while(1)
	{	

		key = KEY_Scan(0);	
		tp_dev.scan(0); 

		LCD_ShowNum_Cover(0,0,time_us*10+(time_us/3)%10,12,16);

		LCD_ShowNum(180,16+50,sx670_parm.sensor1_us,12,16);
		LCD_ShowNum(180,16+50+50,sx670_parm.sensor2_us,12,16);
		LCD_ShowNum(180,16+50+50+50,sx670_parm.sensor3_us,12,16);
		LCD_ShowNum(180,16+50+50+50+50,sx670_parm.sensor4_us,12,16);

		if(key==KEY0_PRES)
		{
			sx670_parm.sensor1_us=0;
			sx670_parm.sensor2_us=0;
			sx670_parm.sensor3_us=0;
			sx670_parm.sensor4_us=0;
			
			LED1=!LED1;
			TIM_Cmd(TIM3, DISABLE);
			time_us=0;
		}
		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
			{	
				if(tp_dev.x>420&&tp_dev.x<460&&tp_dev.y>(16+50+50+50+50+50)&&tp_dev.y<(16+50+50+50+50+50+40))
				{
					sx670_parm.sensor1_us=0;
					sx670_parm.sensor2_us=0;
					sx670_parm.sensor3_us=0;
					sx670_parm.sensor4_us=0;
					TIM_Cmd(TIM3, DISABLE);
					time_us=0;
					LED1=!LED1;				
				}
			}
		}
		
	}				  
	
 }

