#include "lab1.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"
#include "sx670.h"
#include "control.h"
#include "led.h"
#include "touch.h" 
#include "timer.h"

static uint16_t color1=WHITE,color2=BLACK;
static uint8_t  xiankuangjiange=5;
static uint8_t mode=0;
static uint32_t times_us_old1=0;
static uint32_t times_us_old2=0;
static uint32_t times_us_old3=0;
static uint32_t times_us_old4=0;


static uint8_t KEY_MANUAL[]=" EXIT | NONE | BACK | START | CLEAR ";

static uint8_t start=1,stop=1,back=1;//按键标志

void Fun_lab1_show_text(void)
{
	Show_Str(420,166,color2,color1,"开始",16,mode);
	Show_Str(420,166+50,color2,color1,"清零",16,mode);
	Show_Str(420,166+50+50,RED,color1,"返回",16,mode);
}

static void Fun_lab1_page_Screen(uint16_t period,void* p)
{
	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,1);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	

	
	Show_Str(20,40,color2,color1,"实验1：测量挡光时间",16,mode);
	
	Show_Str(270,40,color2,color1,"系统计时时间:           ms",16,mode);
	
	Show_Str(100+30,16+50+30,color2,color1,"传感器1:",16,mode);
	Show_Str(100+30,16+50+50+30,color2,color1,"传感器2:",16,mode);
	Show_Str(100+30,16+50+50+50+30,color2,color1,"传感器3:",16,mode);
	Show_Str(100+30,16+50+50+50+50+30,color2,color1,"传感器4:",16,mode);

	
	Show_Str(280+30+8,16+50+30,color2,color1,"ms",16,mode);
	Show_Str(280+30+8,16+50+50+30,color2,color1,"ms",16,mode);
	Show_Str(280+30+8,16+50+50+50+30,color2,color1,"ms",16,mode);
	Show_Str(280+30+8,16+50+50+50+50+30,color2,color1,"ms",16,mode);
	
	
	
	POINT_COLOR=MY_DARKBLUE;
	
	for(uint8_t i=0;i<4;i++)
	{
		LCD_DrawLine(80,86+50*i,376,86+50*i);
		LCD_DrawLine(80,122+50*i,376,122+50*i);
	}
	
	
//	Show_Str(206,70,color2,color1,"|    s   |ms|",16,mode);
//	LCD_DrawLine(210,70,210+12*8,70);
	
	LCD_DrawLine(80,86,80,122+50*3);
	LCD_DrawLine(376,86,376,122+50*3);

	Fun_lab1_show_text();
	
	for(uint8_t i=0;i<3;i++)
	{
		LCD_DrawRectangle(420-xiankuangjiange,166+50*i-xiankuangjiange,420+16*2+xiankuangjiange,166+50*i+xiankuangjiange+16);
		Show_Str(420-xiankuangjiange-16,166+50*i,MY_DARKBLUE,color1,"魑",16,mode);
	}
	

	Show_Str(0,304,WHITE,BLACK,KEY_MANUAL,16,0);//按键说明
}

void touch_lab1_page(void)
{
	if((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166+50*0-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166+50*0+xiankuangjiange+16)&&start)
	{
		start=0;
		event_establish(EVENT_TUOCH_START);
	}
	else if(!((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166+50*0-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166+50*0+xiankuangjiange+16))||PEN==1)
	{
		start=1;
	}
	
	if((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166+50*1-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166+50*1+xiankuangjiange+16)&&stop)
	{
		stop=0;
		event_establish(EVENT_TUOCH_STOP);
	}
	else if(!((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166+50*1-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166+50*1+xiankuangjiange+16))||PEN==1)
	{
		stop=1;
	}
	
	if((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166+50*2-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166+50*2+xiankuangjiange+16)&&back)
	{
		back=0;
		event_establish(EVENT_TUOCH_BACK);
	}
	else if(!((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166+50*2-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166+50*2+xiankuangjiange+16))||PEN==1)
	{
		back=1;
	}
	
}

/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab1_page_Handle(event_type_t event)
{
	if(page_state_now == lab1  )
	{
		if(event == EVENT_KEY0_PRESSED)
		{
			Fun_lab1_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);
			printf("lab1_key_back\r\n");
			page_state_now=main_page;
		}
		else if(event == EVENT_KEY1_PRESSED)
		{
			Fun_lab1_show_text();
			Show_Str(420,166,WHITE,MY_PURPLE,"开始",16,mode);
			printf("lab1_key_strat\r\n");
			EE_SX670_ENABLE();
		}
		else if(event == EVENT_KEY_UP_PRESSED)
		{
			Fun_lab1_show_text();
			Show_Str(420,166+50,WHITE,MY_PURPLE,"清零",16,mode);	
			printf("lab1_key_clear\r\n");
			EE_SX670_DISENABLE();
		}
		else if(event == EVENT_TUOCH_START)
		{
			Fun_lab1_show_text();
			Show_Str(420,166,WHITE,MY_PURPLE,"开始",16,mode);
			printf("lab1_touch_strat\r\n");
			EE_SX670_ENABLE();
		}
		else if(event == EVENT_TUOCH_STOP)
		{
			Fun_lab1_show_text();
			Show_Str(420,166+50,WHITE,MY_PURPLE,"清零",16,mode);
			printf("lab1_touch_clear\r\n");
			EE_SX670_DISENABLE();
		}
		else if(event == EVENT_TUOCH_BACK)
		{
			Fun_lab1_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);	
			printf("lab1_touch_back\r\n");
			page_state_now=main_page;
		}	
////////////////////////////////////////////////////////chuanganqi/////////////
		else if(event == EVENT_SENER1_IN)
		{
			times_us_old1=time_us;
		}
		else if(event == EVENT_SENER1_OUT)
		{
			sx670_parm.sensor1_us=time_us-times_us_old1;
			//sx670_parm.sensor1_us=sx670_parm.sensor1_us*10+(sx670_parm.sensor1_us/3)%10;
		}
		else if(event == EVENT_SENER2_IN)
		{
			times_us_old2=time_us;
		}
		else if(event == EVENT_SENER2_OUT)
		{
			sx670_parm.sensor2_us=time_us-times_us_old2;
			//sx670_parm.sensor2_us=sx670_parm.sensor2_us*10+(sx670_parm.sensor2_us/3)%10;
		}
		else if(event == EVENT_SENER4_IN)
		{
			times_us_old3=time_us;
		}
		else if(event == EVENT_SENER4_OUT)
		{
			sx670_parm.sensor3_us=time_us-times_us_old3;
			//sx670_parm.sensor3_us=sx670_parm.sensor3_us*10+(sx670_parm.sensor3_us/3)%10;
		}
		else if(event == EVENT_SENER3_IN)
		{
			times_us_old4=time_us;
		}
		else if(event == EVENT_SENER3_OUT)
		{
			sx670_parm.sensor4_us=time_us-times_us_old4;
			//sx670_parm.sensor4_us=sx670_parm.sensor4_us*10+(sx670_parm.sensor4_us/3)%10;
		}
		
	}
	return event;
}



void Fun_Init_lab1_page(void)
{	
	resgister_event_handle(Fun_lab1_page_Handle,EVENT_KEY0_PRESSED|EVENT_KEY1_PRESSED|EVENT_KEY_UP_PRESSED\
												|EVENT_TUOCH_START|EVENT_TUOCH_STOP|EVENT_TUOCH_BACK\
	
												|EVENT_SENER1_IN|EVENT_SENER2_IN | EVENT_SENER3_IN|EVENT_SENER4_IN\
												|EVENT_SENER1_OUT| EVENT_SENER2_OUT|EVENT_SENER3_OUT|EVENT_SENER4_OUT);
	                                          
	                              
												
}


void Fun_Show_lab1_page(void)
{
	printf("lab1_init\r\n");
	Fun_lab1_page_Screen(0,0);
}

void Fun_Close_lab1_page(void)
{
	printf("lab1_close\r\n");
	LCD_Clear(WHITE);
	EE_SX670_DISENABLE();
	start=stop=back=1;//按键标志
}





