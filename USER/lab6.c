#include "lab6.h"
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
static uint16_t color1=WHITE,color2=BLACK;
static uint8_t  xiankuangjiange=5;
static uint8_t mode=0;
static uint8_t x=40;

static uint8_t KEY_MANUAL[]=" EXIT | NONE | PAUSE  |    -    |    +    ";
static uint8_t key_direct=1,key_jia=1,key_jian=1;
lab6_parm_t lab6_parm;


void Fun_lab6_show_text(void)
{
	Fun_lab1_show_text();
	
	POINT_COLOR=MY_DARKBLUE;
	BACK_COLOR=WHITE;
	
	LCD_DrawRectangle(108,295,180,320);
	LCD_DrawRectangle(180,295,260,320);
	LCD_DrawRectangle(260,295,336,320);
}

static void Fun_lab6_page_Screen(uint16_t period,void* p)
{
	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,1);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	Show_Str(20,40,color2,color1,"实验6：测量周期和频率",16,mode);
	
	
//	Show_Str(x,80,color2,color1,"魑周期数：       ",16,mode);
	Show_Str(x,80,color2,color1,"魑时间 t=         ms",16,mode);
	Show_Str(x,80+50,color2,color1,"魑周期 T=         ms",16,mode);
	Show_Str(x,80+50+50,color2,color1,"魑频率 f=         Hz",16,mode);
	
	Show_Str(x+200,80,color2,color1,"魑触发        次为1周期",16,mode);
	Show_Str(x+200,80+50,color2,color1,"魑次数",16,mode);
	Show_Str(x+200,80+50+50,color2,color1,"魑周期数：       ",16,mode);
	 
	
	POINT_COLOR=WHITE;
	BACK_COLOR=MY_DARKBLUE;
	lab6_parm.period_uint=1;
	LCD_ShowNum(x+200+55,80,lab6_parm.period_uint,6,16);
	
	LCD_ShowNum(x+200+55,80+50,lab6_parm.cnt,6,16);
	
	Fun_lab6_show_text();
	for(uint8_t i=0;i<3;i++)
	{
		POINT_COLOR=MY_DARKBLUE;
		LCD_DrawRectangle(420-xiankuangjiange,166+50*i-xiankuangjiange,420+16*2+xiankuangjiange,166+50*i+xiankuangjiange+16);
		Show_Str(420-xiankuangjiange-16,166+50*i,MY_DARKBLUE,color1,"魑",16,mode);
	}
	
	Show_Str(x,100+40*3+16+16+16+5,color2,color1,"按键 KEY_0 暂停和继续",16,mode);
	Show_Str(0,304,WHITE,BLACK,KEY_MANUAL,16,0);//按键说明
	
	POINT_COLOR=MY_DARKBLUE;
	BACK_COLOR=WHITE;
	
	LCD_DrawRectangle(108,295,180,320);
	LCD_DrawRectangle(180,295,260,320);
	LCD_DrawRectangle(260,295,336,320);
}

void touch_lab6_page(void)
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
	
		if((180<tp_dev.x)&&(tp_dev.x<260)&&\
		(295<tp_dev.y)&&(tp_dev.y<320)&&key_jia)
	{
		key_jia=0;
		event_establish(EVENT_KEY1_PRESSED);
	}
	else if(!((180<tp_dev.x)&&(tp_dev.x<260)&&\
		(295<tp_dev.y)&&(tp_dev.y<320))||PEN==1)
	{
		key_jia=1;
	}
	
		if((260<tp_dev.x)&&(tp_dev.x<336)&&\
		(295<tp_dev.y)&&(tp_dev.y<320)&&key_jian)
	{
		key_jian=0;
		event_establish(EVENT_KEY_UP_PRESSED);
	}
	else if(!((260<tp_dev.x)&&(tp_dev.x<336)&&\
		(295<tp_dev.y)&&(tp_dev.y<320))||PEN==1)
	{
		key_jian=1;
	}
}

/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab6_page_Handle(event_type_t event)
{
	if(page_state_now == lab6  )
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
			lab6_parm.period_uint--;
			if(lab6_parm.period_uint<1)
				lab6_parm.period_uint=1;
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(x+200+55,80,lab6_parm.period_uint,6,16);
		}
		else if(event == EVENT_KEY_UP_PRESSED)
		{
			lab6_parm.period_uint++;
			if(lab6_parm.period_uint>999)
				lab6_parm.period_uint=999;
			POINT_COLOR=WHITE;
			BACK_COLOR=MY_DARKBLUE;
			LCD_ShowNum(x+200+55,80,lab6_parm.period_uint,6,16);
			
		}
		else if(event == EVENT_TUOCH_START)
		{
			Fun_lab6_show_text();
			Show_Str(420,166,WHITE,MY_PURPLE,"开始",16,mode);
			EE_SX670_ENABLE();
		}
		else if(event == EVENT_TUOCH_STOP)
		{
			LCD_Clear(WHITE);
			Fun_lab6_page_Screen(0,0);
			Show_Str(420,166+50,WHITE,MY_PURPLE,"清零",16,mode);	
			lab6_parm.cnt=0;		
			lab6_parm.period_uint=1;		
			EE_SX670_DISENABLE();
		}
		else if(event == EVENT_TUOCH_BACK)
		{
			Fun_lab6_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
			page_state_now=main_page;
		}	
////////////////////////////////////////////////////////chuanganqi/////////////
		else if(event == EVENT_SENER1_IN)
		{
			lab6_parm.cnt++;
		}
		else if(event == EVENT_SENER2_IN)
		{
			lab6_parm.cnt++;
		}

		else if(event == EVENT_SENER4_IN)
		{
			lab6_parm.cnt++;
		}
		else if(event == EVENT_SENER3_IN)
		{
			lab6_parm.cnt++;
		}

	}
	return event;
}



void Fun_Init_lab6_page(void)
{	
	resgister_event_handle(Fun_lab6_page_Handle,EVENT_KEY0_PRESSED|EVENT_KEY1_PRESSED|EVENT_KEY_UP_PRESSED\
												|EVENT_TUOCH_START|EVENT_TUOCH_STOP|EVENT_TUOCH_BACK\
	
												|EVENT_SENER1_IN|EVENT_SENER2_IN | EVENT_SENER3_IN|EVENT_SENER4_IN\
												|EVENT_SENER1_OUT| EVENT_SENER2_OUT|EVENT_SENER3_OUT|EVENT_SENER4_OUT);
	                                          
	                              
												
}


void Fun_Show_lab6_page(void)
{
	Fun_lab6_page_Screen(0,0);
}

void Fun_Close_lab6_page(void)
{
	LCD_Clear(WHITE);
	EE_SX670_DISENABLE();
	lab6_parm.cnt=0;		
	lab6_parm.period_uint=1;
}




