#include "lab4.h"
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
static uint8_t lab4_x=13,lab4_y=100,lab4_line=40;

static uint8_t KEY_MANUAL[]=" EXIT | NONE | DIRECT | + | - ";

lab4_param_v_t lab4_param_v;
static uint8_t lab4_direct=0;
static int8_t lab4_distance=10;


void Fun_lab4_show_text(void)
{
	Fun_lab1_show_text();
}

static void Fun_lab4_page_Screen(uint16_t period,void* p)
{
	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,1);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	Show_Str(20,40,color2,color1,"实验4：测量碰撞中的动量",16,mode);
	Show_Str(270,40,color2,color1,"系统计时时间:          m s",16,mode);
	
	POINT_COLOR=WHITE;
	BACK_COLOR=MY_DARKBLUE;
	Show_Str(lab4_x,lab4_y,color2,color1,"经过光电传感器的12的瞬时速度V12:(单位: m m/s)",16,mode);
	Show_Str(lab4_x,lab4_y+lab4_line,color2,color1,"第1次:",16,mode);
	LCD_ShowNum(lab4_x+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_1,6,16);

	Show_Str(lab4_x+130,lab4_y+lab4_line,color2,color1,"第2次:",16,mode);
	LCD_ShowNum(lab4_x+130+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_2,6,16);
	
	Show_Str(lab4_x+260,lab4_y+lab4_line,color2,color1,"第3次:",16,mode);
	LCD_ShowNum(lab4_x+260+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_3,6,16);
	

	
	Show_Str(lab4_x,lab4_y+lab4_line*2,color2,color1,"经过光电传感器的34的瞬时速度V34:(单位: m m/s)",16,mode);
	
	Show_Str(lab4_x,lab4_y+lab4_line*3,color2,color1,"第1次:",16,mode);
	LCD_ShowNum(lab4_x+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_1,6,16);

	Show_Str(lab4_x+130,lab4_y+lab4_line*3,color2,color1,"第2次:",16,mode);
	LCD_ShowNum(lab4_x+130+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_2,6,16);
	
	Show_Str(lab4_x+260,lab4_y+lab4_line*3,color2,color1,"第3次:",16,mode);
	LCD_ShowNum(lab4_x+260+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_3,6,16);
	
	Show_Str(280,273,color2,color1,"距离：",16,mode);
	LCD_ShowNum_Cover(280+16*3,273,lab4_distance,2,16);//传感器距离
	Show_Str(280+16*4,273,color2,color1," m m",16,mode);
	
	POINT_COLOR=MY_DARKBLUE;
	LCD_DrawRectangle(lab4_x-5,lab4_y-16-5,lab4_x+260+16*7+5,lab4_y+lab4_line*3+16+16+5);
	
	Show_Str(20,273,color2,color1,"通过按键KEY_0切换速度方向",16,mode);
	
	
	Show_Str(400,166-80,color2,color1,"速度方向：",16,mode);

	if(lab4_direct)
		Show_Str(410,166-50,color2,color1,"21为正",16,mode);
	else
		Show_Str(410,166-50,color2,color1,"12为正",16,mode);
	
	
	Fun_lab4_show_text();
	for(uint8_t i=0;i<3;i++)
	{
		POINT_COLOR=MY_DARKBLUE;
		LCD_DrawRectangle(420-xiankuangjiange,166+50*i-xiankuangjiange,420+16*2+xiankuangjiange,166+50*i+xiankuangjiange+16);
		Show_Str(420-xiankuangjiange-16,166+50*i,MY_DARKBLUE,color1,"魑",16,mode);
	}
	Show_Str(0,304,WHITE,BLACK,KEY_MANUAL,16,0);//按键说明
}

void touch_lab4_page(void)
{
	touch_lab1_page();
}





static uint8_t lab4_direct_v_1to2=0xff;//实际方向 12：0    21：1
static uint8_t lab4_direct_v_2to1=0xff;//实际方向 12：0    21：1
static uint8_t lab4_sener_times=0;//次数
static uint8_t lab4_num_v12=0;//v12速度个数

static uint8_t lab4_direct_v_1to2_34=0xff;//实际方向 34：0    43：1
static uint8_t lab4_direct_v_2to1_34=0xff;//实际方向 34：0    43：1
static uint8_t lab4_sener_times_34=0;//次数
static uint8_t lab4_num_v34=0;//v34速度个数


void lab4_clear_param(void)
{
	lab4_param_v.v12_times_1=0;
	lab4_param_v.v12_times_2=0;
	lab4_param_v.v12_times_3=0;
	lab4_param_v.v34_times_1=0;
	lab4_param_v.v34_times_2=0;
	lab4_param_v.v34_times_3=0;

	lab4_direct_v_1to2=0xff;//实际方向 12：0    21：1
	lab4_direct_v_2to1=0xff;//实际方向 12：0    21：1
	lab4_sener_times=0;//次数
	lab4_num_v12=0;//v12速度个数

	lab4_direct_v_1to2_34=0xff;//实际方向 34：0    43：1
	lab4_direct_v_2to1_34=0xff;//实际方向 34：0    43：1
	lab4_sener_times_34=0;//次数
	lab4_num_v34=0;//v34速度个数
	
	lab4_distance=10;
	lab4_direct=0;
}


/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab4_page_Handle(event_type_t event)
{
	if(page_state_now == lab4  )
	{
		if(event == EVENT_KEY0_PRESSED)
		{
			static	uint8_t direct=0;
			direct++;
			if((direct%2)==0)
			{
				lab4_direct=0;
				Show_Str(410,166-50,color2,color1,"12为正",16,mode);
			
			}
			else
			{
				lab4_direct=1;
				Show_Str(410,166-50,color2,color1,"21为正",16,mode);
			}
		}
		else if(event == EVENT_KEY1_PRESSED)
		{
			lab4_distance++;
			if(lab4_distance>99)
				lab4_distance=99;
			LCD_ShowNum_Cover(280+16*3,273,lab4_distance,2,16);
		}
		else if(event == EVENT_KEY_UP_PRESSED)
		{
			lab4_distance--;
			if(lab4_distance<0)
				lab4_distance=0;
			LCD_ShowNum_Cover(280+16*3,273,lab4_distance,2,16);
	
		}
		else if(event == EVENT_TUOCH_START)
		{
			Fun_lab4_show_text();
			Show_Str(420,166,WHITE,MY_PURPLE,"开始",16,mode);
			EE_SX670_ENABLE();
		}
		else if(event == EVENT_TUOCH_STOP)
		{
			LCD_Clear(WHITE);
			EE_SX670_DISENABLE();
			lab4_clear_param();
			Fun_lab4_page_Screen(0,0);
			Show_Str(420,166+50,WHITE,MY_PURPLE,"清零",16,mode);
		}
		else if(event == EVENT_TUOCH_BACK)
		{
			Fun_lab4_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
			page_state_now=main_page;
		}	
/////////////////////////////////////////////////传感器/////////////////////////////////////////////////////////
		else if(event == EVENT_SENER1_IN)
		{
			lab4_sener_times++;
			
			 if(lab4_sener_times==1)
			{
				times_us_old1=time_us;
				lab4_direct_v_1to2=0;//实际方向为12
			}
			else if(lab4_sener_times==2)//记到两个反速度
			{
				lab4_direct_v_1to2=1;//实际方向为12
			}
			else if(lab4_sener_times==3)
			{
				sx670_enable=0;
			}
			
			if(lab4_direct_v_2to1==0)
			{
				lab4_direct_v_2to1=1;
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor12_us=time_us-times_us_old2;
				sx670_parm.sensor12_v=(1000*lab4_distance)/sx670_parm.sensor12_us;

				lab4_num_v12++;
				if(lab4_num_v12>3)
					lab4_num_v12=1;
				if(lab4_num_v12==1)
				{
					lab4_param_v.v12_times_1=sx670_parm.sensor12_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,' ',16,0);
					else
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,'-',16,0);
					LCD_ShowNum(lab4_x+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_1,6,16);
				}
				else if(lab4_num_v12==2)
				{
					lab4_param_v.v12_times_2=sx670_parm.sensor12_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,' ',16,0);
					else
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,'-',16,0);
					LCD_ShowNum(lab4_x+130+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_2,6,16);
				}
				else if(lab4_num_v12==3)
				{
					lab4_param_v.v12_times_3=sx670_parm.sensor12_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,' ',16,0);
					else
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,'-',16,0);
					LCD_ShowNum(lab4_x+260+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_3,6,16);
				}
				
			}
		}
		else if(event == EVENT_SENER2_IN)
		{

			lab4_direct_v_2to1=0;//实际方向为21
			times_us_old2=time_us;
			
			if(lab4_direct_v_1to2==0)
			{
				lab4_direct_v_1to2=1;
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor12_us=time_us-times_us_old1;
				sx670_parm.sensor12_v=(1000*lab4_distance)/sx670_parm.sensor12_us;
				
				lab4_num_v12++;
				if(lab4_num_v12>3)
					lab4_num_v12=1;
				if(lab4_num_v12==1)
				{
					lab4_param_v.v12_times_1=sx670_parm.sensor12_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,'-',16,0);
					else
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,' ',16,0);
					LCD_ShowNum(lab4_x+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_1,6,16);
				}
				else if(lab4_num_v12==2)
				{
					lab4_param_v.v12_times_2=sx670_parm.sensor12_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,'-',16,0);
					else
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,' ',16,0);
					LCD_ShowNum(lab4_x+130+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_2,6,16);
				}
				else if(lab4_num_v12==3)
				{
					lab4_param_v.v12_times_3=sx670_parm.sensor12_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,'-',16,0);
					else
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line,POINT_COLOR,BACK_COLOR,' ',16,0);
					LCD_ShowNum(lab4_x+260+16*4,lab4_y+lab4_line,lab4_param_v.v12_times_3,6,16);
				}
			}
			
		}
		
		else if(event == EVENT_SENER4_IN)
		{
			lab4_direct_v_2to1_34=0;//实际方向为21
			times_us_old3=time_us;
			
			if(lab4_direct_v_1to2_34==0)
			{
				lab4_direct_v_1to2_34=1;
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor34_us=time_us-times_us_old4;
				sx670_parm.sensor34_v=(1000*lab4_distance)/sx670_parm.sensor34_us;
				
				lab4_num_v34++;
				if(lab4_num_v34>3)
					lab4_num_v34=1;
				if(lab4_num_v34==1)
				{
					lab4_param_v.v34_times_1=sx670_parm.sensor34_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,' ',16,0);
					else
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,'-',16,0);
					LCD_ShowNum(lab4_x+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_1,6,16);
				}
				else if(lab4_num_v34==2)
				{
					lab4_param_v.v34_times_2=sx670_parm.sensor34_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,' ',16,0);
					else
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,'-',16,0);
					LCD_ShowNum(lab4_x+130+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_2,6,16);
				}
				else if(lab4_num_v34==3)
				{
					lab4_param_v.v34_times_3=sx670_parm.sensor34_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,' ',16,0);
					else
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,'-',16,0);
					LCD_ShowNum(lab4_x+260+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_3,6,16);
				}
			}
			
		}
		else if(event == EVENT_SENER3_IN)
		{
			lab4_sener_times_34++;
			 if(lab4_sener_times_34==1)
			{
				times_us_old4=time_us;
				lab4_direct_v_1to2_34=0;//实际方向为34
			}
			else if(lab4_sener_times_34==2)//记到两个反速度
			{
				lab4_direct_v_1to2_34=1;//实际方向为34
			}
			else if(lab4_sener_times_34==3)
			{
				sx670_enable=0;
			}
			
			
			if(lab4_direct_v_2to1_34==0)
			{
				lab4_direct_v_2to1_34=1;
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor34_us=time_us-times_us_old3;
				sx670_parm.sensor34_v=(1000*lab4_distance)/sx670_parm.sensor34_us;

				lab4_num_v34++;
				if(lab4_num_v34>3)
					lab4_num_v34=1;
				if(lab4_num_v34==1)
				{
					lab4_param_v.v34_times_1=sx670_parm.sensor34_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,'-',16,0);
					else
						LCD_ShowChar(lab4_x+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,' ',16,0);
					LCD_ShowNum(lab4_x+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_1,6,16);
				}
				else if(lab4_num_v34==2)
				{
					lab4_param_v.v34_times_2=sx670_parm.sensor34_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,'-',16,0);
					else
						LCD_ShowChar(lab4_x+130+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,' ',16,0);
					LCD_ShowNum(lab4_x+130+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_2,6,16);
				}
				else if(lab4_num_v34==3)
				{
					lab4_param_v.v34_times_3=sx670_parm.sensor34_v;
					if(lab4_direct)
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,'-',16,0);
					else
						LCD_ShowChar(lab4_x+260+16*4-8,lab4_y+lab4_line*3,POINT_COLOR,BACK_COLOR,' ',16,0);
					LCD_ShowNum(lab4_x+260+16*4,lab4_y+lab4_line*3,lab4_param_v.v34_times_3,6,16);
				}
				
			}
			
			
		}	
/////////////////////////////////////////////////传感器/////////////////////////////////////////////////////////
	}
	return event;
}



void Fun_Init_lab4_page(void)
{	
	resgister_event_handle(Fun_lab4_page_Handle,EVENT_KEY0_PRESSED|EVENT_KEY1_PRESSED|EVENT_KEY_UP_PRESSED\
												|EVENT_TUOCH_START|EVENT_TUOCH_STOP|EVENT_TUOCH_BACK\
	
												|EVENT_SENER1_IN|EVENT_SENER2_IN | EVENT_SENER3_IN|EVENT_SENER4_IN\
												|EVENT_SENER1_OUT| EVENT_SENER2_OUT|EVENT_SENER3_OUT|EVENT_SENER4_OUT);
	                                          
	                              
												
}


void Fun_Show_lab4_page(void)
{
	Fun_lab4_page_Screen(0,0);
}

void Fun_Close_lab4_page(void)
{
	LCD_Clear(WHITE);
	EE_SX670_DISENABLE();
	lab4_clear_param();
}





