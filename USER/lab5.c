#include "lab5.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"
#include "sx670.h"
#include "control.h"
#include "led.h"
#include "touch.h" 
#include "timer.h"
#include "lab1.h"
#include "delay.h"
static uint16_t color1=WHITE,color2=BLACK;
static uint8_t  xiankuangjiange=5;
static uint8_t mode=0;
//static uint32_t times_us_old1=0;
//static uint32_t times_us_old2=0;
//static uint32_t times_us_old3=0;
//static uint32_t times_us_old4=0;


static uint8_t KEY_MANUAL[]=" EXIT | NONE | PAUSE  | START | BACK ";
static uint8_t  key_direct=1;
uint32_t lab5_times_num=0;



void Fun_lab5_show_text(void)
{
	Fun_lab1_show_text();
	
	POINT_COLOR=MY_DARKBLUE;
	BACK_COLOR=WHITE;
	
	LCD_DrawRectangle(108,295,180,320);
}

static void Fun_lab5_page_Screen(uint16_t period,void* p)
{
	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,1);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	Show_Str(20,40,color2,color1,"实验5：计数",16,mode);
	
	Show_Str(145,118,color2,color1,"计数：",16,mode);
	
	Show_Str(145,157,color2,color1,"计时：",16,mode);
	
//	for(uint8_t i=0;i<4;i++)
//	{
//		LCD_DrawLine(80,86+50*i,376,86+50*i);
//		LCD_DrawLine(80,122+50*i,376,122+50*i);
//	}
	
	
//	Show_Str(206,70,color2,color1,"|  s  |ms|us|",16,mode);
//	LCD_DrawLine(210,70,210+12*8,70);
	

	Fun_lab5_show_text();
	for(uint8_t i=0;i<3;i++)
	{
		POINT_COLOR=MY_DARKBLUE;
		LCD_DrawRectangle(420-xiankuangjiange,166+50*i-xiankuangjiange,420+16*2+xiankuangjiange,166+50*i+xiankuangjiange+16);
		Show_Str(420-xiankuangjiange-16,166+50*i,MY_DARKBLUE,color1,"魑",16,mode);
	}
	
	Show_Str(145,100+40*3+16+16+5,color2,color1,"按键 KEY_0 暂停和继续",16,mode);
	Show_Str(0,304,WHITE,BLACK,KEY_MANUAL,16,0);//按键说明
	
	POINT_COLOR=MY_DARKBLUE;
	BACK_COLOR=WHITE;
	
	LCD_DrawRectangle(108,295,180,320);
}

void touch_lab5_page(void)
{
	
	touch_lab1_page();
	if((108<tp_dev.x)&&(tp_dev.x<180)&&\
		(295<tp_dev.y)&&(tp_dev.y<320)&&key_direct)
	{
		key_direct=0;
		event_establish(EVENT_KEY0_PRESSED);
	}
	else if(!((108<tp_dev.x)&&(tp_dev.x<180)&&\
		(295<tp_dev.y)&&(tp_dev.y<320))||PEN==1)
	{
		key_direct=1;
	}
}

/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab5_page_Handle(event_type_t event)
{
	if(page_state_now == lab5  )
	{
		if(event == EVENT_KEY0_PRESSED)
		{
			static uint8_t tim=0;
			tim++;
			if(tim%2)
			{
				TIM_Cmd(TIM2, DISABLE); //暂停
				sx670_enable=0;
			}
			else
			{
				TIM_Cmd(TIM2, ENABLE); //继续
				sx670_enable=1;
			}
			
		}
		else if(event == EVENT_KEY1_PRESSED)
		{
			Fun_lab5_show_text();
			Show_Str(420,166,WHITE,MY_PURPLE,"开始",16,mode);
			EE_SX670_ENABLE();
		}
		else if(event == EVENT_KEY_UP_PRESSED)
		{
			Fun_lab5_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
			page_state_now=main_page;
		}
		else if(event == EVENT_TUOCH_START)
		{
			Fun_lab5_show_text();
			Show_Str(420,166,WHITE,MY_PURPLE,"开始",16,mode);
			EE_SX670_ENABLE();
		}
		else if(event == EVENT_TUOCH_STOP)
		{
			Fun_lab5_show_text();
			Show_Str(420,166+50,WHITE,MY_PURPLE,"清零",16,mode);		
			EE_SX670_DISENABLE();
			lab5_times_num=0;
		}
		else if(event == EVENT_TUOCH_BACK)
		{
			Fun_lab5_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
			page_state_now=main_page;
		}	
////////////////////////////////////////////////////////chuanganqi/////////////
		else if(event == EVENT_SENER1_IN)
		{
			lab5_times_num++;
//			times_us_old1=time_us;
//			printf("times_us_old1:%d\r\n",times_us_old1);
		}
		else if(event == EVENT_SENER1_OUT)
		{
//			sx670_parm.sensor1_us=time_us-times_us_old1;
//			sx670_parm.sensor1_us=sx670_parm.sensor1_us*10+(sx670_parm.sensor1_us/3)%10;
//			printf("sx670_parm.sensor1_us:%d\r\n",sx670_parm.sensor1_us);
		}
		else if(event == EVENT_SENER2_IN)
		{
			lab5_times_num++;
//			times_us_old2=time_us;
		}
		else if(event == EVENT_SENER2_OUT)
		{
//			sx670_parm.sensor2_us=time_us-times_us_old2;
//			sx670_parm.sensor2_us=sx670_parm.sensor2_us*10+(sx670_parm.sensor2_us/3)%10;
		}
		else if(event == EVENT_SENER4_IN)
		{
			lab5_times_num++;
//			times_us_old3=time_us;
		}
		else if(event == EVENT_SENER4_OUT)
		{
//			sx670_parm.sensor3_us=time_us-times_us_old3;
//			sx670_parm.sensor3_us=sx670_parm.sensor3_us*10+(sx670_parm.sensor3_us/3)%10;
		}
		else if(event == EVENT_SENER3_IN)
		{
			lab5_times_num++;
//			times_us_old4=time_us;
		}
		else if(event == EVENT_SENER3_OUT)
		{
//			sx670_parm.sensor4_us=time_us-times_us_old4;
//			sx670_parm.sensor4_us=sx670_parm.sensor4_us*10+(sx670_parm.sensor4_us/3)%10;
		}

	}
	return event;
}



void Fun_Init_lab5_page(void)
{	
	resgister_event_handle(Fun_lab5_page_Handle,EVENT_KEY0_PRESSED|EVENT_KEY1_PRESSED|EVENT_KEY_UP_PRESSED\
												|EVENT_TUOCH_START|EVENT_TUOCH_STOP|EVENT_TUOCH_BACK\
	
												|EVENT_SENER1_IN|EVENT_SENER2_IN | EVENT_SENER3_IN|EVENT_SENER4_IN\
												|EVENT_SENER1_OUT| EVENT_SENER2_OUT|EVENT_SENER3_OUT|EVENT_SENER4_OUT);
									
}


void Fun_Show_lab5_page(void)
{
	Fun_lab5_page_Screen(0,0);
}

void Fun_Close_lab5_page(void)
{
	LCD_Clear(WHITE);
	EE_SX670_DISENABLE();
	lab5_times_num=0;
}





