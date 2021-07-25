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



static uint8_t hangju=32,x=50,y=80,y_tips1=54,x1=200;

static uint8_t key_t1=1,key_t2=1,key_jia=1,key_jian=1,key_direct=1;

static uint8_t res=0;
	
static int16_t width_s=7;
static int16_t dis=7;

/////////////////////////////tips1/////////////
static uint8_t Tips=1;
static uint32_t t_record1=0;
static uint32_t t_record2=0;
static uint32_t t_record3=0;
static uint32_t t_record4=0;

static uint32_t t12=0;
static uint32_t t34=0;

/////////////////////////////tips2/////////////

static uint32_t t2_record1=0;
static uint32_t t2_record3=0;
static uint32_t t13=0;
static uint8_t lab2_direct=0;

void Fun_lab2_show_text(void)
{
	Fun_lab1_show_text();
	
	POINT_COLOR=WHITE;
	BACK_COLOR=MY_DARKBLUE;
	if(Tips==1)
	{
		Show_Str(420,166-100,WHITE,MY_YELLOW,"方法1",16,mode);
		Show_Str(420,166-50,color2,color1,"方法2",16,mode);
		
		if(page_state_now == lab2)
		{
			hangju=40;
			Show_Str(x,y_tips1,color2,color1,"t1:           ms",16,mode);
			Show_Str(x,y_tips1+hangju*1,color2,color1,"t2:           ms",16,mode);
			Show_Str(x,y_tips1+hangju*2,color2,color1,"t3:           ms",16,mode);
			Show_Str(x,y_tips1+hangju*3,color2,color1,"t4:           ms",16,mode);

			Show_Str(x1,y_tips1,color2,color1,"v1:           m/s",16,mode);
			Show_Str(x1,y_tips1+hangju*1,color2,color1,"v2:           m/s",16,mode);
			Show_Str(x1,y_tips1+hangju*2,color2,color1,"v3:           m/s",16,mode);
			Show_Str(x1,y_tips1+hangju*3,color2,color1,"v4:           m/s",16,mode);
			
			LCD_ShowNum(x+28,y_tips1+hangju*0,sx670_parm.sensor1_us/100,7,16);
			LCD_ShowChar(x+28+8*7,y_tips1+hangju*0,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*0,sx670_parm.sensor1_us%100,2,16);
			
			LCD_ShowNum(x+28,y_tips1+hangju*1,sx670_parm.sensor2_us/100,7,16);
			LCD_ShowChar(x+28+8*7,y_tips1+hangju*1,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*1,sx670_parm.sensor2_us%100,2,16);
			
			LCD_ShowNum(x+28,y_tips1+hangju*2,sx670_parm.sensor3_us/100,7,16);
			LCD_ShowChar(x+28+8*7,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*2,sx670_parm.sensor3_us%100,2,16);
			
			LCD_ShowNum(x+28,y_tips1+hangju*3,sx670_parm.sensor4_us/100,7,16);
			LCD_ShowChar(x+28+8*7,y_tips1+hangju*3,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*3,sx670_parm.sensor4_us%100,2,16);
			

			LCD_ShowNum(x1+28,y_tips1+hangju*0,sx670_parm.sensor1_v/1000,6,16);
			LCD_ShowChar(x1+28+6*8,y_tips1+hangju*0,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*0,sx670_parm.sensor1_v%1000,3,16);
			
			LCD_ShowNum(x1+28,y_tips1+hangju*1,sx670_parm.sensor2_v/1000,6,16);
			LCD_ShowChar(x1+28+6*8,y_tips1+hangju*1,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*1,sx670_parm.sensor2_v%1000,3,16);
			
			LCD_ShowNum(x1+28,y_tips1+hangju*2,sx670_parm.sensor3_v/1000,6,16);
			LCD_ShowChar(x1+28+6*8,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*2,sx670_parm.sensor3_v%1000,3,16);
			
			LCD_ShowNum(x1+28,y_tips1+hangju*3,sx670_parm.sensor4_v/1000,6,16);
			LCD_ShowChar(x1+28+6*8,y_tips1+hangju*3,POINT_COLOR,BACK_COLOR,'.',16,mode);
			LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*3,sx670_parm.sensor4_v%1000,3,16);
		
			
			Show_Str(x+24,y+hangju*4-16,color2,color1,"速度方向：",16,mode);
			if(lab2_direct)
				Show_Str(x+50+52,y+hangju*4-16,color2,color1,"4321为正",16,mode);
			else
				Show_Str(x+50+52,y+hangju*4-16,color2,color1,"1234为正",16,mode);
			
		}
		else if(page_state_now == lab3)
		{
			hangju=30;
			
			Show_Str(x,y_tips1,color2,color1,"t1:          ms",16,mode);
			Show_Str(x,y_tips1+hangju*1,color2,color1,"t2:          ms",16,mode);
			Show_Str(x,y_tips1+hangju*3,color2,color1,"t3:          ms",16,mode);
			Show_Str(x,y_tips1+hangju*4,color2,color1,"t4:          ms",16,mode);

			Show_Str(x1,y_tips1,color2,color1,"v1:          m/s",16,mode);
			Show_Str(x1,y_tips1+hangju*1,color2,color1,"v2:          m/s",16,mode);
			Show_Str(x1,y_tips1+hangju*3,color2,color1,"v3:          m/s",16,mode);
			Show_Str(x1,y_tips1+hangju*4,color2,color1,"v4:          m/s",16,mode);
			
			LCD_ShowNum(x+28,y_tips1+hangju*0,sx670_parm.sensor1_us,9,16);
			LCD_ShowNum(x+28,y_tips1+hangju*1,sx670_parm.sensor2_us,9,16);
			LCD_ShowNum(x+28,y_tips1+hangju*3,sx670_parm.sensor3_us,9,16);
			LCD_ShowNum(x+28,y_tips1+hangju*4,sx670_parm.sensor4_us,9,16);

			LCD_ShowNum(x1+28,y_tips1+hangju*0,sx670_parm.sensor1_v/10000,9,16);
			LCD_ShowNum(x1+28,y_tips1+hangju*1,sx670_parm.sensor2_v/10000,9,16);
			LCD_ShowNum(x1+28,y_tips1+hangju*3,sx670_parm.sensor3_v/10000,9,16);
			LCD_ShowNum(x1+28,y_tips1+hangju*4,sx670_parm.sensor4_v/10000,9,16);
			
			
			
			Show_Str(x-8,y_tips1+hangju*2,color2,color1,"t12:          ms",16,mode);
			LCD_ShowNum(x+28,y_tips1+hangju*2,t12,9,16);
			Show_Str(x1+10-8,y_tips1+hangju*2,color2,color1,"a1:          m/s2",16,mode);
			LCD_ShowChar(x1+28,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,' ',16,0);
			LCD_ShowNum(x1+28+8,y_tips1+hangju*2,sx670_parm.sensor_tip1_a1/10000,8,16);
			
			Show_Str(x-8,y_tips1+hangju*5,color2,color1,"t34:          ms",16,mode);
			LCD_ShowNum(x+28,y_tips1+hangju*5,t34,9,16);
			Show_Str(x1+10-8,y_tips1+hangju*5,color2,color1,"a2:          m/s2",16,mode);
			LCD_ShowChar(x1+28,y_tips1+hangju*5,POINT_COLOR,BACK_COLOR,' ',16,0);
			LCD_ShowNum(x1+28+8,y_tips1+hangju*5,sx670_parm.sensor_tip1_a2/10000,8,16);
	
		}

		Show_Str(x+24,y+40*5-18,color2,color1,"挡光片宽度:",16,mode);
		POINT_COLOR=WHITE;
		BACK_COLOR=MY_DARKBLUE;
		LCD_ShowNum_32(220,y+40*5-38,width_s,2,32);
		Show_Str(280,y+40*5-18,color2,color1,"m m",16,mode);
		
	}
	else
	{
		Show_Str(420,166-100,color2,color1,"方法1",16,mode);
		Show_Str(420,166-50,WHITE,MY_YELLOW,"方法2",16,mode);
		
		hangju=32;
		Show_Str(x,y,color2,color1,"t12:          ms",16,mode);
		
		Show_Str(x,y+hangju*2,color2,color1,"t34:          ms",16,mode);
		

		Show_Str(x1,y,color2,color1,"v12:          m/s",16,mode);
		
		Show_Str(x1,y+hangju*2,color2,color1,"v34:          m/s",16,mode);
		
		LCD_ShowNum(x+33,y+hangju*0,sx670_parm.sensor12_us,9,16);
				
		LCD_ShowNum(x+33,y+hangju*2,sx670_parm.sensor34_us,9,16);
		
		
		LCD_ShowNum(x1+33,y+hangju*0,sx670_parm.sensor12_v/10000,9,16);

		LCD_ShowNum(x1+33,y+hangju*2,sx670_parm.sensor34_v/10000,9,16);

	
		
		
		if(page_state_now == lab3)
		{
			Show_Str(x,y+32*4,color2,color1,"t13:          ms",16,mode);
			LCD_ShowNum(x+33,y+32*4,t13,9,16);
			
			Show_Str(x1+16,y+hangju*4,color2,color1,"a:          m/s2",16,mode);
			LCD_ShowChar(x1+33,y+hangju*4,POINT_COLOR,BACK_COLOR,' ',16,0);
			LCD_ShowNum(x1+33+8,y+hangju*4,sx670_parm.sensor_tip2_a/10000,8,16);
		}
		
		if(page_state_now == lab2)
		{
			Show_Str(x+50,y+hangju*4,color2,color1,"速度方向：",16,mode);
			if(lab2_direct)
			Show_Str(x+50+72,y+hangju*4,color2,color1,"4321为正",16,mode);
			else
			Show_Str(x+50+72,y+hangju*4,color2,color1,"1234为正",16,mode);
		}
		
		Show_Str(x+50,y+hangju*6-8,color2,color1,"传感器距离:",16,mode);
		POINT_COLOR=WHITE;
		BACK_COLOR=MY_DARKBLUE;
		LCD_ShowNum_32(x+50+100,y+hangju*6-28,dis,2,32);
		
		Show_Str(250,y+hangju*6-8,color2,color1,"m m",16,mode);
	}

}

static void Fun_lab2_page_Screen(uint16_t period,void* p)
{
	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,1);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	if(page_state_now == lab2)
	{
		Show_Str(20,22,color2,color1,"实验2：测量瞬时速度",16,mode);
	}
	else if(page_state_now == lab3)
	{
		Show_Str(20,22,color2,color1,"实验3：测量匀变速运动的加速度",16,mode);
	}
	
	Show_Str(270,22,color2,color1,"系统计时时间:           ms",16,mode);
	



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
	if(page_state_now == lab3)
	{
		static uint8_t KEY_MANUAL[]=" EXIT | NONE | BACK |    -    |    +    ";
		Show_Str(0,304,WHITE,BLACK,KEY_MANUAL,16,0);//按键说明
		POINT_COLOR=MY_DARKBLUE;
		BACK_COLOR=WHITE;
		LCD_DrawRectangle(164,295,244,320);
		LCD_DrawRectangle(245,295,320,320);
	}
	else if(page_state_now == lab2)
	{
		static uint8_t KEY_MANUAL[]=" EXIT | NONE | DIRECT |    -    |    +    ";
		Show_Str(0,304,WHITE,BLACK,KEY_MANUAL,16,0);//按键说明
		POINT_COLOR=MY_DARKBLUE;
		BACK_COLOR=WHITE;
		LCD_DrawRectangle(108,295,180,320);
		LCD_DrawRectangle(180,295,260,320);
		LCD_DrawRectangle(260,295,336,320);
	}

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
	
	if(page_state_now == lab3)
	{
			if((164<tp_dev.x)&&(tp_dev.x<244)&&\
				(285<tp_dev.y)&&(tp_dev.y<320)&&key_jia)
			{
				key_jia=0;
				event_establish(EVENT_KEY1_PRESSED);
			}
			else if(!((164<tp_dev.x)&&(tp_dev.x<244)&&\
				(285<tp_dev.y)&&(tp_dev.y<320))||PEN==1)
			{
				key_jia=1;
			}
			
				if((245<tp_dev.x)&&(tp_dev.x<320)&&\
				(285<tp_dev.y)&&(tp_dev.y<320)&&key_jian)
			{
				key_jian=0;
				event_establish(EVENT_KEY_UP_PRESSED);
			}
			else if(!((245<tp_dev.x)&&(tp_dev.x<320)&&\
				(285<tp_dev.y)&&(tp_dev.y<320))||PEN==1)
			{
				key_jian=1;
			}
	}
	else if(page_state_now == lab2)
	{
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

	

	

}


static uint8_t sener1_cnt=0;
static uint8_t sener2_cnt=0;
static uint8_t sener3_cnt=0;
static uint8_t sener4_cnt=0;

static uint32_t tip2_time1_cnt=0;
static uint32_t tip2_time2_cnt=0;
static uint32_t tip2_time3_cnt=0;
static uint32_t tip2_time4_cnt=0;


/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab2_page_Handle(event_type_t event)
{
	if((page_state_now == lab2)||(page_state_now == lab3)  )
	{
		if(event == EVENT_KEY0_PRESSED)
		{
			
			if(page_state_now == lab3)
			{
				Fun_lab2_show_text();
				Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
				page_state_now=main_page;
			}
			else if(page_state_now == lab2)
			{
				static	uint8_t direct=0;
					direct++;
				
				if(Tips==1)
				{
					
					Show_Str(x+24,y+hangju*4-16,color2,color1,"速度方向：",16,mode);
					if((direct%2)==0)
					{
						lab2_direct=0;
						Show_Str(x+50+52,y+hangju*4-16,color2,color1,"1234为正",16,mode);
					}
					else
					{
						lab2_direct=1;
						Show_Str(x+50+52,y+hangju*4-16,color2,color1,"4321为正",16,mode);
					}
				
				}
				else
				{
					Show_Str(x+50,y+hangju*4,color2,color1,"速度方向：",16,mode);
					if((direct%2)==0)
					{
						lab2_direct=0;
						Show_Str(x+50+72,y+hangju*4,color2,color1,"1234为正",16,mode);
					}
					else
					{
						lab2_direct=1;
						Show_Str(x+50+72,y+hangju*4,color2,color1,"4321为正",16,mode);
					}
				
				
				
				}
				
			}			
			
		}
		else if(event == EVENT_KEY1_PRESSED)
		{
			if(Tips==1)
			{
				width_s--;
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				if(width_s<0)width_s=0;
				LCD_ShowNum_32(220,y+40*5-38,width_s,2,32);
				
			}
			else
			{
				dis--;
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				if(dis<0)dis=0;
				LCD_ShowNum_32(x+50+100,y+hangju*6-28,dis,2,32);
				
			}
		}
		else if(event == EVENT_KEY_UP_PRESSED)
		{
			if(Tips==1)
			{
				width_s++;
				
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				if(width_s>99)width_s=200;
				LCD_ShowNum_32(220,y+40*5-38,width_s,2,32);
				
			}
			else
			{
				dis++;
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				if(dis>99)dis=0;
				LCD_ShowNum_32(x+50+100,y+hangju*6-28,dis,2,32);
				
	
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
			LCD_Clear(WHITE);
			t12=0;
			t34=0;
			t13=0;
			tip2_time1_cnt=0;
			tip2_time2_cnt=0;
			tip2_time3_cnt=0;
			tip2_time4_cnt=0;
			sener1_cnt=0;
			sener2_cnt=0;
			sener3_cnt=0;
			sener4_cnt=0;
			
			EE_SX670_DISENABLE();
			Fun_lab2_page_Screen(0,0);
			Show_Str(420,166+50,WHITE,MY_PURPLE,"清零",16,mode);		
			
		}
		else if(event == EVENT_TUOCH_BACK)
		{
			Fun_lab2_show_text();
			Show_Str(420,166+50+50,WHITE,MY_PURPLE,"返回",16,mode);		
			page_state_now=main_page;
		}	
/////////////////////////////////////////////////传感器/////////////////////////////////////////////////////////
		
		
		else if(event == EVENT_SENER1_IN)
		{
			if(Tips==1)
			{
				t_record1=times_us_old1=time_us;
			}
			else 
			{
				if(page_state_now == lab3)
				{
					res=1;
					t2_record1=times_us_old1=time_us;
				}
				else if(page_state_now == lab2)
				{
					sener1_cnt++;
					tip2_time1_cnt=time_us;
				}
			}
			
		}
		else if(event == EVENT_SENER1_OUT)
		{
			if(Tips==1)
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor1_us=time_us-times_us_old1;
	
				LCD_ShowNum(x+28,y_tips1+hangju*0,sx670_parm.sensor1_us/100,7,16);
				LCD_ShowChar(x+28+8*7,y_tips1+hangju*0,POINT_COLOR,BACK_COLOR,'.',16,mode);
				LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*0,sx670_parm.sensor1_us%100,2,16);
				
				sx670_parm.sensor1_v=1000*100*width_s/sx670_parm.sensor1_us;
				
				LCD_ShowNum(x1+28,y_tips1+hangju*0,sx670_parm.sensor1_v/1000,6,16);
				LCD_ShowChar(x1+28+6*8,y_tips1+hangju*0,POINT_COLOR,BACK_COLOR,'.',16,mode);
				LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*0,sx670_parm.sensor1_v%1000,3,16);
				
			}
			
		}
		else if(event == EVENT_SENER2_IN)
		{
			if(Tips==1)
			{
				t_record2=times_us_old2=time_us;
				t12=t_record2-t_record1;
			}
			else 
			{
				
				
				if(page_state_now == lab3)
				{
					if(res)
					{
						res=0;
						POINT_COLOR=WHITE;
						BACK_COLOR=MY_DARKBLUE;
						sx670_parm.sensor12_us=time_us-times_us_old1;
						sx670_parm.sensor12_v=1000*dis/sx670_parm.sensor12_us;
						LCD_ShowNum(x+33,y+hangju*0,sx670_parm.sensor12_us,9,16);
						LCD_ShowNum(x1+33,y+hangju*0,sx670_parm.sensor12_v/10000,9,16);
						
					}
				}
				else if(page_state_now == lab2)
				{
					sener2_cnt++;
					tip2_time2_cnt=time_us;
					
				}
				
			}
			
		}
		else if(event == EVENT_SENER2_OUT)
		{
			if(Tips==1)
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor2_us=time_us-times_us_old2;
				
				LCD_ShowNum(x+28,y_tips1+hangju*1,sx670_parm.sensor2_us/100,7,16);
				LCD_ShowChar(x+28+8*7,y_tips1+hangju*1,POINT_COLOR,BACK_COLOR,'.',16,mode);
				LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*1,sx670_parm.sensor2_us%100,2,16);
				
				sx670_parm.sensor2_v=1000*100*width_s/sx670_parm.sensor2_us;
				
				LCD_ShowNum(x1+28,y_tips1+hangju*1,sx670_parm.sensor2_v/1000,6,16);
				LCD_ShowChar(x1+28+6*8,y_tips1+hangju*1,POINT_COLOR,BACK_COLOR,'.',16,mode);
				LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*1,sx670_parm.sensor2_v%1000,3,16);
				
				
				/*方法一的加速度在传感器退出的时候算*/
				if(page_state_now == lab3)
				{
					
					LCD_ShowNum(x+28,y_tips1+hangju*2,t12,9,16);
					
					if((sx670_parm.sensor2_v-sx670_parm.sensor1_v>0)||(sx670_parm.sensor2_v-sx670_parm.sensor1_v==0))
					{
						sx670_parm.sensor_tip1_a1=((sx670_parm.sensor2_v-sx670_parm.sensor1_v)*1000)/t12;
						if(sx670_parm.sensor_tip1_a2<99999)
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,' ',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*2,sx670_parm.sensor_tip1_a1/10000,8,16);
						}
						else
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,' ',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*2,99999,8,16);
						}
					}
					else
					{
						sx670_parm.sensor_tip1_a1=((sx670_parm.sensor1_v-sx670_parm.sensor2_v)*1000)/t12;
						if(sx670_parm.sensor_tip1_a1<99999)
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,'-',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*2,sx670_parm.sensor_tip1_a1/10000,8,16);
						}
						else
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,'-',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*2,99999,8,16);
						}
					}
					
				}				
			}

		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		else if(event == EVENT_SENER4_IN)
		{
			if(Tips==1)
			{
				t_record3=times_us_old3=time_us;
			}
			else 
			{
				if(page_state_now == lab3)
				{
					res=1;
					t2_record3=times_us_old3=time_us;
				}
				else if(page_state_now == lab2)
				{
					sener3_cnt++;
					tip2_time3_cnt=time_us;
				}
			}
		}
		else if(event == EVENT_SENER4_OUT)
		{
			if(Tips==1)
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor3_us=time_us-times_us_old3;
				
				if(page_state_now == lab3)
				{
					sx670_parm.sensor3_v=1000*width_s/sx670_parm.sensor3_us;
					LCD_ShowNum(x+28,y_tips1+hangju*3,sx670_parm.sensor3_us,9,16);
					LCD_ShowNum(x1+28,y_tips1+hangju*3,sx670_parm.sensor3_v/10000,9,16);
				}
				else
				{
					LCD_ShowNum(x+28,y_tips1+hangju*2,sx670_parm.sensor3_us/100,7,16);
					LCD_ShowChar(x+28+8*7,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,'.',16,mode);
					LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*2,sx670_parm.sensor3_us%100,2,16);
					
					
					sx670_parm.sensor3_v=1000*100*width_s/sx670_parm.sensor3_us;
				
					LCD_ShowNum(x1+28,y_tips1+hangju*2,sx670_parm.sensor3_v/1000,6,16);
					LCD_ShowChar(x1+28+6*8,y_tips1+hangju*2,POINT_COLOR,BACK_COLOR,'.',16,mode);
					LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*2,sx670_parm.sensor3_v%1000,3,16);
					
	
				}
			}

		}
		else if(event == EVENT_SENER3_IN)
		{
			if(Tips==1)
			{
				t_record4=times_us_old4=time_us;
				t34=t_record4-t_record3;
			}
			else 
			{
				if(page_state_now == lab3)
				{
					if(res)
					{
						res=0;
						POINT_COLOR=WHITE;
						BACK_COLOR=MY_DARKBLUE;
						sx670_parm.sensor34_us=time_us-times_us_old3;
						sx670_parm.sensor34_v=1000*dis/sx670_parm.sensor34_us;
						LCD_ShowNum(x+33,y+hangju*2,sx670_parm.sensor34_us,9,16);
						LCD_ShowNum(x1+33,y+hangju*2,sx670_parm.sensor34_v/10000,9,16);
						/*方法二的加速度在进入最后一个传感器的时候算*/
						if(page_state_now == lab3)
						{
							t13=t2_record3-t2_record1;
							LCD_ShowNum(x+33,y+32*4,t13,9,16);
							if((sx670_parm.sensor34_v-sx670_parm.sensor12_v>0)||(sx670_parm.sensor34_v-sx670_parm.sensor12_v==0))
							{
								sx670_parm.sensor_tip2_a=((sx670_parm.sensor34_v-sx670_parm.sensor12_v)*1000)/t13;
								if(sx670_parm.sensor_tip2_a<99999)
								{
									LCD_ShowChar(x1+33,y+hangju*4,POINT_COLOR,BACK_COLOR,' ',16,0);
									LCD_ShowNum(x1+33+8,y+hangju*4,sx670_parm.sensor_tip2_a/10000,8,16);
								}
								else
								{
									LCD_ShowChar(x1+33,y+hangju*4,POINT_COLOR,BACK_COLOR,' ',16,0);
									LCD_ShowNum(x1+33+8,y+hangju*4,99999,8,16);
								}
							}
							else
							{
								sx670_parm.sensor_tip2_a=((sx670_parm.sensor12_v-sx670_parm.sensor34_v)*1000)/t13;
								if(sx670_parm.sensor_tip2_a<99999)
								{
									LCD_ShowChar(x1+33,y+hangju*4,POINT_COLOR,BACK_COLOR,'-',16,0);
									LCD_ShowNum(x1+33+8,y+hangju*4,sx670_parm.sensor_tip2_a/10000,8,16);
								}
								else
								{
									LCD_ShowChar(x1+33,y+hangju*4,POINT_COLOR,BACK_COLOR,'-',16,0);
									LCD_ShowNum(x1+33+8,y+hangju*4,99999,8,16);
								}
							}
						}	
					}	
				}
				else if(page_state_now == lab2)
				{
					sener4_cnt++;
					tip2_time4_cnt=time_us;
				}				
			}		
		}
		else if(event == EVENT_SENER3_OUT)
		{
			if(Tips==1)
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor4_us=time_us-times_us_old4;
				
				/*方法一的加速度在传感器退出的时候算*/
				if(page_state_now == lab3)
				{
					sx670_parm.sensor4_v=1000*width_s/sx670_parm.sensor4_us;
					LCD_ShowNum(x+28,y_tips1+hangju*4,sx670_parm.sensor4_us,9,16);
					LCD_ShowNum(x1+28,y_tips1+hangju*4,sx670_parm.sensor4_v/10000,9,16);
					LCD_ShowNum(x+28,y_tips1+hangju*5,t34,9,16);
					
					if((sx670_parm.sensor4_v-sx670_parm.sensor3_v>0)||(sx670_parm.sensor4_v-sx670_parm.sensor3_v==0))
					{
						sx670_parm.sensor_tip1_a2=((sx670_parm.sensor4_v-sx670_parm.sensor3_v)*1000)/t34;
						if(sx670_parm.sensor_tip1_a2<99999)
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*5,POINT_COLOR,BACK_COLOR,' ',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*5,sx670_parm.sensor_tip1_a2/10000,8,16);
						}
						else
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*5,POINT_COLOR,BACK_COLOR,' ',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*5,99999,8,16);
						}
					}
					else
					{
						sx670_parm.sensor_tip1_a2=((sx670_parm.sensor3_v-sx670_parm.sensor4_v)*1000)/t34;
						if(sx670_parm.sensor_tip1_a2<99999)
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*5,POINT_COLOR,BACK_COLOR,'-',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*5,sx670_parm.sensor_tip1_a2/10000,8,16);
						}
						else
						{
							LCD_ShowChar(x1+28,y_tips1+hangju*5,POINT_COLOR,BACK_COLOR,'-',16,0);
							LCD_ShowNum(x1+28+8,y_tips1+hangju*5,99999,8,16);
						}
					}
					
				}	
				else
				{
					LCD_ShowNum(x+28,y_tips1+hangju*3,sx670_parm.sensor4_us/100,7,16);
					LCD_ShowChar(x+28+8*7,y_tips1+hangju*3,POINT_COLOR,BACK_COLOR,'.',16,mode);
					LCD_ShowNum_Cover(x+28+8*8,y_tips1+hangju*3,sx670_parm.sensor4_us%100,2,16);
					
					sx670_parm.sensor4_v=1000*100*width_s/sx670_parm.sensor4_us;
				
					LCD_ShowNum(x1+28,y_tips1+hangju*3,sx670_parm.sensor4_v/1000,6,16);
					LCD_ShowChar(x1+28+6*8,y_tips1+hangju*3,POINT_COLOR,BACK_COLOR,'.',16,mode);
					LCD_ShowNum_Cover(x1+28+7*8,y_tips1+hangju*3,sx670_parm.sensor4_v%1000,3,16);
					
				}
			}
/////////////////////////////////////////////////方法一/////////////////////////////////////////////////////////
			
		}
		else if(event == EVENT_TUOCH_TIP1)
		{
			Tips=1;
			EE_SX670_DISENABLE();
			t12=0;
			t34=0;
			t13=0;
			tip2_time1_cnt=0;
			tip2_time2_cnt=0;
			tip2_time3_cnt=0;
			tip2_time4_cnt=0;
			sener1_cnt=0;
			sener2_cnt=0;
			sener3_cnt=0;
			sener4_cnt=0;
			LCD_Fill(x-30,44,370,lcddev.height-30,WHITE);
			Fun_lab2_show_text();
		}
		else if(event == EVENT_TUOCH_TIP2)
		{
			Tips=2;
			EE_SX670_DISENABLE();
			t12=0;
			t34=0;
			t13=0;
			tip2_time1_cnt=0;
			tip2_time2_cnt=0;
			tip2_time3_cnt=0;
			tip2_time4_cnt=0;
			sener1_cnt=0;
			sener2_cnt=0;
			sener3_cnt=0;
			sener4_cnt=0;
			LCD_Fill(x-30,44,370,lcddev.height-30,WHITE);
			Fun_lab2_show_text();			
		}
		
			////////**************/////////////////	
		
		if(sener1_cnt==sener2_cnt)
		{
			if(tip2_time2_cnt>tip2_time1_cnt)//正
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor12_us=tip2_time2_cnt-tip2_time1_cnt;
				sx670_parm.sensor12_v=1000*dis/sx670_parm.sensor12_us;
				LCD_ShowNum(x+33,y+hangju*0,sx670_parm.sensor12_us,9,16);
				if(lab2_direct == 0)//1234为正
					LCD_ShowChar(x1+33,y+hangju*0,POINT_COLOR,BACK_COLOR,' ',16,0);
				else
					LCD_ShowChar(x1+33,y+hangju*0,POINT_COLOR,BACK_COLOR,'-',16,0);
				LCD_ShowNum(x1+33+8,y+hangju*0,sx670_parm.sensor12_v/10000,8,16);
				
			}
			else if(tip2_time2_cnt<tip2_time1_cnt)//正
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor12_us=tip2_time1_cnt-tip2_time2_cnt;
				sx670_parm.sensor12_v=1000*dis/sx670_parm.sensor12_us;
				LCD_ShowNum(x+33,y+hangju*0,sx670_parm.sensor12_us,9,16);
				if(lab2_direct == 1)//1234为正
					LCD_ShowChar(x1+33,y+hangju*0,POINT_COLOR,BACK_COLOR,' ',16,0);
				else
					LCD_ShowChar(x1+33,y+hangju*0,POINT_COLOR,BACK_COLOR,'-',16,0);
				LCD_ShowNum(x1+33+8,y+hangju*0,sx670_parm.sensor12_v/10000,8,16);
			}
		}
		
		
		if(sener3_cnt==sener4_cnt)
		{
			if(tip2_time4_cnt>tip2_time3_cnt)//正
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor34_us=tip2_time4_cnt-tip2_time3_cnt;
				sx670_parm.sensor34_v=1000*dis/sx670_parm.sensor34_us;
				LCD_ShowNum(x+33,y+hangju*2,sx670_parm.sensor34_us,9,16);
				
				if(lab2_direct == 0)//1234为正
					LCD_ShowChar(x1+33,y+hangju*2,POINT_COLOR,BACK_COLOR,' ',16,0);
				else
					LCD_ShowChar(x1+33,y+hangju*2,POINT_COLOR,BACK_COLOR,'-',16,0);
				LCD_ShowNum(x1+33+8,y+hangju*2,sx670_parm.sensor34_v/10000,8,16);
			}
			else if(tip2_time4_cnt<tip2_time3_cnt)//正
			{
				POINT_COLOR=WHITE;
				BACK_COLOR=MY_DARKBLUE;
				sx670_parm.sensor34_us=tip2_time3_cnt-tip2_time4_cnt;
				sx670_parm.sensor34_v=1000*dis/sx670_parm.sensor34_us;
				LCD_ShowNum(x+33,y+hangju*2,sx670_parm.sensor34_us,9,16);
				
				if(lab2_direct == 1)//1234为正
					LCD_ShowChar(x1+33,y+hangju*2,POINT_COLOR,BACK_COLOR,' ',16,0);
				else
					LCD_ShowChar(x1+33,y+hangju*2,POINT_COLOR,BACK_COLOR,'-',16,0);
				LCD_ShowNum(x1+33+8,y+hangju*2,sx670_parm.sensor34_v/10000,8,16);
			}
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
	Tips=1;
	t12=0;
	t34=0;
	t13=0;
	lab2_direct=0;
	
	tip2_time1_cnt=0;
    tip2_time2_cnt=0;
    tip2_time3_cnt=0;
    tip2_time4_cnt=0;
	sener1_cnt=0;
	sener2_cnt=0;
	sener3_cnt=0;
	sener4_cnt=0;
	
}



