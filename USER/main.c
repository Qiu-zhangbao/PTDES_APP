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

#include "main_page.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
#include "lab6.h"


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
	TIM2_Int_Init(999,71);
	TIM3_PWM_Init(999,71);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz
	TIM4_Int_Init(99,7199);
	TP_Init();
	KEY_Init();
	Init_event_queue();
	Apc_InitFunCtrlSM();
	Control_Init();
	time_us=0;
	
	#if	( USER_MODE == OFFICIAL_MODE )	//��ʽģʽ
		SCB->VTOR = SRAM_BASE | 0x1000;	//�ж�������ƫ��
	#endif
	
	while(1)
	{	
		if(dir)led0pwmval++;
		else led0pwmval--;

 		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;										 
		TIM_SetCompare2(TIM3,led0pwmval);	
		
		KEY_Scan(0);	
		
		if(PEN)LED1=1;
		else LED1=0;
		
		if(page_state_now == lab1  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(180+30,16+50+30,sx670_parm.sensor1_us,12,16);
			LCD_ShowNum(180+30,16+50+50+30,sx670_parm.sensor2_us,12,16);
			LCD_ShowNum(180+30,16+50+50+50+30,sx670_parm.sensor3_us,12,16);
			LCD_ShowNum(180+30,16+50+50+50+50+30,sx670_parm.sensor4_us,12,16);
			LCD_ShowNum(376,40,time_us,9,16);
		}
		else if(page_state_now == lab2  )
		{	
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,22,time_us,9,16);
		}
		else if(page_state_now == lab3  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,22,time_us,9,16);

		}
		else if(page_state_now == lab4  )
		{
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(376,40,time_us,9,16);
		}
		else if(page_state_now == lab5  )
		{
			uint8_t x=190,y=150;
			POINT_COLOR=MY_DARKBLUE;
			BACK_COLOR=WHITE;
		
			LCD_ShowNum_32(x,y-40,lab5_times_num,7,32);
			
			LCD_ShowNum_32(x,y,time_us/1000,3,32);
			LCD_ShowChar_32(x+3*16,y,POINT_COLOR,BACK_COLOR,11);
			LCD_ShowNum_32(x+4*16,y,time_us%1000,3,32);
		
		}		
		else if(page_state_now == lab6  )
		{
			static uint8_t x=80;
		
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			lab6_parm.time_ms=time_us;
			
			if(lab6_parm.cnt==0)
				lab6_parm.period_num=lab6_parm.cnt;
				else
				lab6_parm.period_num=(lab6_parm.cnt-1)/lab6_parm.period_uint;
			lab6_parm.period=lab6_parm.time_ms/lab6_parm.period_num;
			lab6_parm.frequency=100000/lab6_parm.period;
		
			LCD_ShowNum(x+64,80,lab6_parm.period_num,6,16);
			LCD_ShowNum(x+64,80+50,lab6_parm.time_ms,6,16);
			LCD_ShowNum(x+64,80+50+50,lab6_parm.period,6,16);
			
			LCD_ShowNum(x+64,80+50+50+50,lab6_parm.frequency/100,3,16);
			LCD_ShowChar(x+64+3*8,80+50+50+50,POINT_COLOR,BACK_COLOR,'.',16,0);
			LCD_ShowNum_Cover(x+64+4*8,80+50+50+50,lab6_parm.frequency%100,2,16);
				
			LCD_ShowNum(x+180+55,80,lab6_parm.period_uint,6,16);
			LCD_ShowNum(x+180+55,80+50,lab6_parm.cnt,6,16);	
				
				
		}
		POINT_COLOR=MY_DARKBLUE;
		LCD_DrawLine(0,0,480,0);
		LCD_DrawLine(0,1,480,1);		
		POINT_COLOR=WHITE;		
		
	}				  
	
 }

