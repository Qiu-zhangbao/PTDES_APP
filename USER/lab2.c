#include "lab2.h"
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
static uint32_t times_us_old1=0;
static uint32_t times_us_old2=0;
static uint32_t times_us_old3=0;
static uint32_t times_us_old4=0;

static uint8_t KEY_MANUAL[]=" EXIT | NONE | BACK | + | - ";

static uint8_t hangju=32,x=50,y=80,x1=200;

static uint8_t key_t1=1,key_t2=1;

int16_t width_s=0;
int16_t dis=0;


uint8_t Tips=1;




void Fun_lab2_show_text(void)
{
	Fun_lab1_show_text();
	
	POINT_COLOR=WHITE;
	BACK_COLOR=MY_DARKBLUE;
	if(Tips==1)
	{
		Show_Str(420,166-100,WHITE,MY_YELLOW,"方法1",16,mode);
		Show_Str(420,166-50,color2,color1,"方法2",16,mode);
		
		hangju=40;
		Show_Str(x,y,color2,color1,"t1:          us",16,mode);
		Show_Str(x,y+hangju*1,color2,color1,"t2:          us",16,mode);
		Show_Str(x,y+hangju*2,color2,color1,"t3:          us",16,mode);
		Show_Str(x,y+hangju*3,color2,color1,"t4:          us",16,mode);

		Show_Str(x1,y,color2,color1,"v1:          mm/s",16,mode);
		Show_Str(x1,y+hangju*1,color2,color1,"v2:          mm/s",16,mode);
		Show_Str(x1,y+hangju*2,color2,color1,"v3:          mm/s",16,mode);
		Show_Str(x1,y+hangju*3,color2,color1,"v4:          mm/s",16,mode);
		
		LCD_ShowNum_Cover(x+28,y+hangju*0,sx670_parm.sensor1_us,9,16);
		LCD_ShowNum_Cover(x+28,y+hangju*1,sx670_parm.sensor2_us,9,16);
		LCD_ShowNum_Cover(x+28,y+hangju*2,sx670_parm.sensor3_us,9,16);
		LCD_ShowNum_Cover(x+28,y+hangju*3,sx670_parm.sensor4_us,9,16);

		LCD_ShowNum_Cover(x1+28,y+hangju*0,sx670_parm.sensor1_v,9,16);
		LCD_ShowNum_Cover(x1+28,y+hangju*1,sx670_parm.sensor2_v,9,16);
		LCD_ShowNum_Cover(x1+28,y+hangju*2,sx670_parm.sensor3_v,9,16);
		LCD_ShowNum_Cover(x1+28,y+hangju*3,sx670_parm.sensor4_v,9,16);
		
		Show_Str(x-20,y+hangju*5-8,color2,color1,"挡光片宽度:",16,mode);
		Show_Str(230,y+hangju*5-8,color2,color1,"    mm",16,mode);
		
	}
	else
	{
		Show_Str(420,166-100,color2,color1,"方法1",16,mode);
		Show_Str(420,166-50,WHITE,MY_YELLOW,"方法2",16,mode);
		
		hangju=32;
		Show_Str(x,y,color2,color1,"t12:          us",16,mode);
		
		Show_Str(x,y+hangju*2,color2,color1,"t34:          us",16,mode);
		

		Show_Str(x1,y,color2,color1,"v12:          mm/s",16,mode);
		
		Show_Str(x1,y+hangju*2,color2,color1,"v34:          mm/s",16,mode);
		
		LCD_ShowNum_Cover(x+28,y+hangju*0,sx670_parm.sensor12_us,9,16);
				
		LCD_ShowNum_Cover(x+28,y+hangju*2,sx670_parm.sensor34_us,9,16);
		
		
		LCD_ShowNum_Cover(x1+28,y+hangju*0,sx670_parm.sensor12_v,9,16);
	
		LCD_ShowNum_Cover(x1+28,y+hangju*2,sx670_parm.sensor34_v,9,16);

		Show_Str(x-20,y+hangju*5-8,color2,color1,"传感器距离:",16,mode);
		Show_Str(230,y+hangju*5-8,color2,color1,"    cm",16,mode);
	
	}

}

static void Fun_lab2_page_Screen(uint16_t period,void* p)
{
	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,1);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	Show_Str(20,22,color2,color1,"实验2：测量瞬时速度",16,mode);
	Show_Str(270,22,color2,color1,"系统计时时间:          us",16,mode);
	



	Fun_lab2_show_text();
	for(uint8_t i=0;i<5;i++)
	{
		POINT_COLOR=MY_DARKBLUE;
		if(i<2)
			LCD_DrawRectangle(420-xiankuangjiange,66+50*i-xiankuangjiange,420+16*2+xiankuangjiange+8,66+50*i+xiankuangjiange+16);
		else
			LCD_DrawRectangle(420-xiankuangjiange,66+50*i-xiankuangjiange,420+16*2+xiankuangjiange,66+50*i+xiankuangjiange+16);
		Show_Str(420-xiankuangjiange-16,66+50*i,MY_DARKBLUE,color1,"魑",16,mode);
	}
	Show_Str(0,308,WHITE,BLACK,KEY_MANUAL,12,0);//按键说明
}

void touch_lab2_page(void)
{
	touch_lab1_page();
	if((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166-50-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166-50+xiankuangjiange+16)&&key_t1)
	{
		key_t1=0;
		event_establish(EVENT_TUOCH_TIP2);
	}
	else if(!((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166-50-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166+-50+xiankuangjiange+16))||PEN==1)
	{
		key_t1=1;
	}
	
		if((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166-100-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166-100+xiankuangjiange+16)&&key_t2)
	{
		key_t2=0;
		event_establish(EVENT_TUOCH_TIP1);
	}
	else if(!((420-xiankuangjiange<tp_dev.x)&&(tp_dev.x<420+16*2+xiankuangjiange)&&\
		(166-100-xiankuangjiange<tp_dev.y)&&(tp_dev.y<166-100+xiankuangjiange+16))||PEN==1)
	{
		key_t2=1;
	}
}

/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab2_page_Handle(event_type_t event)
{
	if(page_state_now == lab2  )
	{
		if(event == EVENT_KEY0_PRESSED)
		{
			Fun_lab2_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
			page_state_now=main_page;
		}
		else if(event == EVENT_KEY1_PRESSED)
		{
			if(Tips==1)
			{
				width_s++;
				if(width_s>200)width_s=200;
				LCD_ShowNum(230,y+40*5-8,width_s,3,16);
			}
			else
			{
				dis++;
				if(dis>200)dis=0;
				LCD_ShowNum(230,y+40*5-8,dis,3,16);
	
			}
		}
		else if(event == EVENT_KEY_UP_PRESSED)
		{
			if(Tips==1)
			{
				width_s--;
				if(width_s<0)width_s=0;
				LCD_ShowNum(230,y+40*5-8,width_s,3,16);
			}
			else
			{
				dis++;
				if(dis>200)dis=0;
				LCD_ShowNum(230,y+40*5-8,dis,3,16);
			}
		}
		else if(event == EVENT_TUOCH_START)
		{
			Fun_lab2_show_text();
			Show_Str(420,166,WHITE,MY_PURPLE,"开始",16,mode);
			EE_SX670_ENABLE();
		}
		else if(event == EVENT_TUOCH_STOP)
		{
			Fun_lab2_show_text();
			Show_Str(420,166+50,WHITE,MY_PURPLE,"清零",16,mode);		
			EE_SX670_DISENABLE();
		}
		else if(event == EVENT_TUOCH_BACK)
		{
			Fun_lab2_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
			page_state_now=main_page;
		}	
////////////////////////////////////////////////////////chuanganqi/////////////
		else if(event == EVENT_SENER1_IN)
		{
			times_us_old1=time_us;
		}
		else if(event == EVENT_SENER1_OUT)
		{
			//2mm/1706us  0.001 706
			sx670_parm.sensor1_us=time_us-times_us_old1;
			sx670_parm.sensor1_us=sx670_parm.sensor1_us*10+(sx670_parm.sensor1_us/3)%10;
			sx670_parm.sensor1_v=(width_s/sx670_parm.sensor1_us)*1000;
			LCD_ShowNum_Cover(x+28,y+hangju*0,sx670_parm.sensor1_us,9,16);
			//LCD_ShowNum_Cover(x1+28,y+sx670_parm.sensor1_v*0,9876,9,16);
		}
		else if(event == EVENT_SENER2_IN)
		{
			times_us_old2=time_us;
		}
		else if(event == EVENT_SENER2_OUT)
		{
			sx670_parm.sensor2_us=time_us-times_us_old2;
			sx670_parm.sensor2_us=sx670_parm.sensor2_us*10+(sx670_parm.sensor2_us/3)%10;
			sx670_parm.sensor2_v=(width_s/sx670_parm.sensor2_us)*1000;
			LCD_ShowNum_Cover(x+28,y+hangju*1,sx670_parm.sensor2_us,9,16);
			//LCD_ShowNum_Cover(x1+28,y+hangju*1,sx670_parm.sensor2_v,9,16);
		}
		else if(event == EVENT_SENER4_IN)
		{
			times_us_old3=time_us;
		}
		else if(event == EVENT_SENER4_OUT)
		{
			sx670_parm.sensor3_us=time_us-times_us_old3;
			sx670_parm.sensor3_us=sx670_parm.sensor3_us*10+(sx670_parm.sensor3_us/3)%10;
			sx670_parm.sensor3_v=(width_s/sx670_parm.sensor3_us)*1000;
			LCD_ShowNum_Cover(x+28,y+hangju*2,sx670_parm.sensor3_us,9,16);
			//LCD_ShowNum_Cover(x1+28,y+hangju*2,sx670_parm.sensor3_v,9,16);
		}
		else if(event == EVENT_SENER3_IN)
		{
			times_us_old4=time_us;
		}
		else if(event == EVENT_SENER3_OUT)
		{
			sx670_parm.sensor4_us=time_us-times_us_old4;
			sx670_parm.sensor4_us=sx670_parm.sensor4_us*10+(sx670_parm.sensor4_us/3)%10;
			sx670_parm.sensor4_v=(width_s/sx670_parm.sensor4_us)*1000;
			LCD_ShowNum_Cover(x+28,y+hangju*3,sx670_parm.sensor4_us,9,16);
			//LCD_ShowNum_Cover(x1+28,y+hangju*3,sx670_parm.sensor4_v,9,16);
		}
		else if(event == EVENT_TUOCH_TIP1)
		{
			Tips=1;
			LCD_Fill(x-30,y,350,lcddev.height-30,WHITE);
			Fun_lab2_show_text();
		}
		else if(event == EVENT_TUOCH_TIP2)
		{
			Tips=2;
			LCD_Fill(x-30,y,350,lcddev.height-30,WHITE);
			Fun_lab2_show_text();			
		}
		
		
	}
	return event;
}



void Fun_Init_lab2_page(void)
{	
	resgister_event_handle(Fun_lab2_page_Handle,EVENT_KEY0_PRESSED|EVENT_KEY1_PRESSED|EVENT_KEY_UP_PRESSED\
												|EVENT_TUOCH_START|EVENT_TUOCH_STOP|EVENT_TUOCH_BACK\
	
												|EVENT_SENER1_IN|EVENT_SENER2_IN | EVENT_SENER3_IN|EVENT_SENER4_IN\
												|EVENT_SENER1_OUT| EVENT_SENER2_OUT|EVENT_SENER3_OUT|EVENT_SENER4_OUT\
												
												|EVENT_TUOCH_TIP1|EVENT_TUOCH_TIP2);
	                                          
	                              
												
}


void Fun_Show_lab2_page(void)
{
	Fun_lab2_page_Screen(0,0);
}

void Fun_Close_lab2_page(void)
{
	LCD_Clear(WHITE);
	EE_SX670_DISENABLE();
}





