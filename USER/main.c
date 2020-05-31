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
#include "control.h"
#include "event_queue.h"
#include "main_page.h"


/****************************************************************************************************
//=======================================Һ���������߽���==========================================//
//DB0       ��PD14 
//DB1       ��PD15 
//DB2       ��PD0 
//DB3       ��PD1 
//DB4~DB12  ���ν�PE7~PE15
//DB13      ��PD8 
//DB14      ��PD9
//DB15      ��PD10  
//=======================================Һ���������߽���==========================================//
//LCD_CS	��PG12	//Ƭѡ�ź�
//LCD_RS	��PG0	//�Ĵ���/����ѡ���ź�
//LCD_WR	��PD5	//д�ź�
//LCD_RD	��PD4	//���ź�
//LCD_RST	��PC5	//��λ�ź�
//LCD_LED	��PB0	//��������ź�(�ߵ�ƽ����)
//=========================================������������=========================================//
//��ʹ�ô�������ģ�鱾������������ɲ�����
//MO(MISO)	��PF8	//SPI�������
//MI(MOSI)	��PF9	//SPI��������
//PEN		��PF10	//�������ж��ź�
//TCS		��PB2	//����ICƬѡ
//CLK		��PB1	//SPI����ʱ��
**************************************************************************************************/	


//IAP����
//IROM1��0x20001000	0xC000
//IRAM1��0x2000D000	0x3000
//SCB->VTOR = SRAM_BASE | 0x1000;	��仰Ҫ���жϳ�ʼ��֮��

//Ĭ������
//IROM1��0x8000000	0x80000
//IRAM1��0x20000000	0x10000






int main(void)
{	
	u16 led0pwmval=0;
	u8 dir=1;	
	SystemInit();//��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	delay_init();//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LCD_Init();//Һ������ʼ�� 
	LED_Init();
	EE_SX670_INIT();
	TIM2_Int_Init(9,71);
	TIM3_PWM_Init(899,0);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz
	TIM4_Int_Init(99,7199);
	//SCB->VTOR = SRAM_BASE | 0x1000;	//�ж�������ƫ��
	TP_Init();
	KEY_Init();
	Init_event_queue();
	Apc_InitFunCtrlSM();
	Control_Init();
	time_us=0;
	while(1)
	{	
		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;

 		if(led0pwmval>200)dir=0;
		if(led0pwmval==0)dir=1;										 
		TIM_SetCompare2(TIM3,led0pwmval);	
		
		
//		if(PEN)LED1=1;
//		else LED1=0;
		
		if(page_state_now == lab1  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum_Cover(180+30,16+50+30,sx670_parm.sensor1_us,12,16);
			LCD_ShowNum_Cover(180+30,16+50+50+30,sx670_parm.sensor2_us,12,16);
			LCD_ShowNum_Cover(180+30,16+50+50+50+30,sx670_parm.sensor3_us,12,16);
			LCD_ShowNum_Cover(180+30,16+50+50+50+50+30,sx670_parm.sensor4_us,12,16);
			LCD_ShowNum(376,40,time_us*10+(time_us/3)%10,9,16);
		}
		else if(page_state_now == lab2  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,40,time_us*10+(time_us/3)%10,9,16);

			
			
		}
		else if(page_state_now == lab3  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,40,time_us*10+(time_us/3)%10,9,16);
		
		
		}
		else if(page_state_now == lab4  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,40,time_us*10+(time_us/3)%10,9,16);
		
		
		}
		else if(page_state_now == lab5  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,40,time_us*10+(time_us/3)%10,9,16);
		
		
		}		
		else if(page_state_now == lab6  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,40,time_us*10+(time_us/3)%10,9,16);
		
		
		}			
		
		
//		key = KEY_Scan(0);	
//		tp_dev.scan(0); 

//		LCD_ShowNum_Cover(0,0,time_us*10+(time_us/3)%10,12,16);

//		LCD_ShowNum(180+30,16+50+30,sx670_parm.sensor1_us,12,16);
//		LCD_ShowNum(180+30,16+50+50+30,sx670_parm.sensor2_us,12,16);
//		LCD_ShowNum(180+30,16+50+50+50+30,sx670_parm.sensor3_us,12,16);
//		LCD_ShowNum(180+30,16+50+50+50+50+30,sx670_parm.sensor4_us,12,16);

//		if(key==KEY0_PRES)
//		{
//			sx670_parm.sensor1_us=0;
//			sx670_parm.sensor2_us=0;
//			sx670_parm.sensor3_us=0;
//			sx670_parm.sensor4_us=0;
//			
//			LED1=!LED1;
//			TIM_Cmd(TIM2, DISABLE);
//			time_us=0;
//		}
//		 
//		if(tp_dev.sta&TP_PRES_DOWN)			//������������
//		{	
//			if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
//			{	
//				if(tp_dev.x>420&&tp_dev.x<460&&tp_dev.y>(16+50+50+50+50+50)&&tp_dev.y<(16+50+50+50+50+50+40))
//				{
//					sx670_parm.sensor1_us=0;
//					sx670_parm.sensor2_us=0;
//					sx670_parm.sensor3_us=0;
//					sx670_parm.sensor4_us=0;
//					TIM_Cmd(TIM2, DISABLE);
//					time_us=0;
//					LED1=!LED1;				
//				}
//			}
//		}
		
	}				  
	
 }

