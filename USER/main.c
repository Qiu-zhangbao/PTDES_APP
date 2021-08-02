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
#include "global_config.h"
#include "main_page.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
#include "lab6.h"
#include "usart.h"

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
//	0x20001000	0xC000
//	0x2000D000	0x3000


//Ĭ������
//	0x8000000	0x80000
//	0x20000000	0x10000

u32 main_init_time_us=0;


extern u32 sensor1_in_time_us;

extern  u32 sensor1_out_time_us;


u32 us_oldd=0;

int main(void)
{	
	SystemInit();//��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	systime.init();
	
	uart_init(115200);
	LED_Init();
	LCD_Init();//Һ������ʼ�� 
	TP_Init();
	KEY_Init();
	EE_SX670_INIT();
//	TIM2_Int_Init(999,71);
//	TIM3_PWM_Init(20000-1,7199); 
	Init_event_queue();
	Apc_InitFunCtrlSM();
	Control_Init();
//	TIM4_Int_Init(99,7199);
	
	main_init_time_us=systime.get_time_us();
	printf("\r\nSYSTEM INIT TIME:%d.%d\r\n",main_init_time_us/1000,main_init_time_us%1000);

	#if	( USER_MODE == OFFICIAL_MODE )	//��ʽģʽ
		SCB->VTOR = SRAM_BASE | 0x1000;	//�ж�������ƫ��
	#endif
//	TIM_SetCompare2(TIM3,10000);
	while(1)
	{
//		LED0=1;
		main_control();

		KEY_Scan(0);			
		if(sx670_enable)
		{
			if(time_us_reset_flag==1)
			{
				time_us_old=systime.get_time_us();
				time_us_reset_flag=0;
			}
			time_us=systime.get_time_us()-time_us_old;
			if(time_us>4294967295)
			{
				time_us=4294967295;
			}
		}

		if(page_state_now == lab1  )
		{
			lab1_show_data();
			
		}
		else if(page_state_now == lab2  )
		{	
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			
			LCD_Show_Num_Float(376-2*8,22,time_us,8,3,16);

		}
		else if(page_state_now == lab3  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			
			LCD_Show_Num_Float(376,22,time_us,8,3,16);
			

		}
		else if(page_state_now == lab4  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			
			LCD_ShowNum(376,22,time_us/100,7,16);
			LCD_ShowChar(376+8*7,22,POINT_COLOR,BACK_COLOR,'.',16,0);
			LCD_ShowNum_Cover(376+8*8,22,time_us%100,2,16);
		}
		else if(page_state_now == lab5  )
		{
			uint8_t x=190,y=150;
			POINT_COLOR=MY_DARKBLUE;
			BACK_COLOR=WHITE;
		
			LCD_ShowNum_32(x+24,y-40,lab5_times_num,7,32);
			
			LCD_ShowNum_32(x,y,time_us/1000000,3,32);//s
			LCD_ShowChar_32(x+3*16,y,POINT_COLOR,BACK_COLOR,11);
			LCD_ShowNum_32(x+4*16,y,(time_us/1000)%1000,3,32);//ms
			LCD_ShowChar_32(x+7*16,y,POINT_COLOR,BACK_COLOR,11);
			LCD_ShowNum_32(x+8*16,y,time_us%1000,3,32);//us
		
		}		
		else if(page_state_now == lab6  )
		{
			static uint8_t x=60;
		
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			lab6_parm.time_ms=time_us;
			

			lab6_parm.period_num=lab6_parm.cnt/lab6_parm.period_uint;
			
			lab6_parm.period=lab6_parm.time_ms/lab6_parm.period_num;
			lab6_parm.frequency=1000*1000*1000/lab6_parm.period;
			
			LCD_Show_Num_Float(x+64,80,lab6_parm.time_ms,6,3,16);
			
			LCD_Show_Num_Float(x+64,80+50,lab6_parm.period,6,3,16);
			LCD_Show_Num_Float(x+64,80+50+50,lab6_parm.frequency,6,3,16);
		

			LCD_ShowNum(x+180+55,80,lab6_parm.period_uint,6,16);
			LCD_ShowNum(x+180+55,80+50,lab6_parm.cnt,6,16);	

			LCD_ShowNum(x+180+55+24,80+50+50,lab6_parm.period_num,6,16);
		}
		POINT_COLOR=MY_DARKBLUE;
		LCD_DrawLine(0,0,480,0);
		LCD_DrawLine(0,1,480,1);		
		POINT_COLOR=WHITE;		
		
//		LED0=0;
//		systime.delay_ms(1);
		
	}				  
	
 }

