#include "main_page.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"
#include "control.h"
#include "led.h"
#include "touch.h" 


static uint8_t VERSION_NUMBER[]=VERSION;

static uint8_t KEY_MANUAL[]=" EXIT | BACK | UP | OK | DOWN ";

static uint8_t x=150,y=40,line=40,mode=0;
static uint16_t color1=WHITE,color2=BLACK;
static uint8_t  xiankuangjiange=10;
static uint8_t list_text_num[6]={6,6,11,8,2,7};

static lab_list_t page_state_select=close;
static lab_list_t page_state_select_last=close;

static uint8_t LAB1=1,LAB2=1,LAB3=1,LAB4=1,LAB5=1,LAB6=1,SETING=1,EXIT=1;//按键标志

static int8_t page_key=0;
static uint8_t page_key_one=1;

static uint8_t exit_cnt=0;

void Fun_main_page_show_text(void)
{
	Show_Str(x,y,color2,color1,"实验1：测量挡光时间",16,mode);
	Show_Str(x,y+line,color2,color1,"实验2：测量瞬时速度",16,mode);
	Show_Str(x,y+line+line,color2,color1,"实验3：测量匀变速运动的加速度",16,mode);
	Show_Str(x,y+line+line+line,color2,color1,"实验4：测量碰撞中的动量",16,mode);
	Show_Str(x,y+line+line+line+line,color2,color1,"实验5：计数",16,mode);
	Show_Str(x,y+line+line+line+line+line,color2,color1,"实验6：测量周期和频率",16,mode);
	
	//Show_Str(420,y+line+line+line+line+line,color2,color1,"设置",16,mode);	
	Show_Str(420,y+line+line+line+line+line+line,RED,color1,"退出",16,mode);
}



static void Fun_main_page_Screen(uint16_t period,void* p)
{	
	//Gu_Drawbmp16_320X480(0,0,gImage_480X320);
	//Gui_Drawbmp16(10,30,gImage_77777);
	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);
	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,1);//居中显示
	Show_Str(370,4,WHITE,BLACK,VERSION_NUMBER,16,1);//版本号
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	GUI_DrawFont32(30,100,MY_DARKBLUE,WHITE,"徽",0);
	Show_Str(30,100+70,WHITE,MY_DARKBLUE,"平川中学",16,mode);
	Fun_main_page_show_text();
	for(uint8_t list=0;list<6;list++)
	{	
		
		POINT_COLOR=MY_DARKBLUE;
		LCD_DrawRectangle(x-xiankuangjiange,y+line*list-xiankuangjiange,x+56+16*list_text_num[list]+xiankuangjiange,y+line*list+xiankuangjiange+16);
		Show_Str(x-40,y+list*line,MY_DARKBLUE,color1,"魑",16,mode);
	}
	
	//LCD_DrawRectangle(410,y+line*5-xiankuangjiange,420+32+xiankuangjiange,y+line*5+xiankuangjiange+16);	

	LCD_DrawRectangle(410,y+line*6-xiankuangjiange,420+32+xiankuangjiange,y+line*6+xiankuangjiange+16);	

	Show_Str(0,304,WHITE,BLACK,KEY_MANUAL,16,0);//按键说明
	
	LCD_LED=1;//加载完之后点亮背光	
}

void Fun_show_refsh_touch_key(lab_list_t res)
{
	page_state_select=res;
	Fun_main_page_show_text();
	
	
	if(page_state_select == lab1)
	{
		exit_cnt=0;
		Show_Str(x,y,WHITE,MY_DARKBLUE,"实验1：测量挡光时间",16,mode);
	}
	else if(page_state_select == lab2)
	{
		exit_cnt=0;
		Show_Str(x,y+line,WHITE,MY_DARKBLUE,"实验2：测量瞬时速度",16,mode);
	}
	else if(page_state_select == lab3)
	{
		exit_cnt=0;
		Show_Str(x,y+line+line,WHITE,MY_DARKBLUE,"实验3：测量匀变速运动的加速度",16,mode);
	}
	else if(page_state_select == lab4)
	{
		exit_cnt=0;
		Show_Str(x,y+line+line+line,WHITE,MY_DARKBLUE,"实验4：测量碰撞中的动量",16,mode);
	}
	else if(page_state_select == lab5)
	{
		exit_cnt=0;
		Show_Str(x,y+line+line+line+line,WHITE,MY_DARKBLUE,"实验5：计数",16,mode);
	}
	else if(page_state_select == lab6)
	{
		exit_cnt=0;
		Show_Str(x,y+line+line+line+line+line,WHITE,MY_DARKBLUE,"实验6：测量周期和频率",16,mode);
	}
	else if(page_state_select == seting)
	{
		exit_cnt=0;
		//Show_Str(420,y+line+line+line+line+line,WHITE,MY_DARKBLUE,"设置",16,mode);
	}
	else if(page_state_select == exiting)
	{
		
		Show_Str(420,y+line+line+line+line+line+line,WHITE,MY_DARKBLUE,"退出",16,mode);
	}
		
	
	if(page_state_select==page_state_select_last)
	{
		if(page_state_select==seting||page_state_select==exiting)
		{
			
			if(page_state_select==exiting)
			{
				exit_cnt++;
				if(exit_cnt==2)
					SCB->AIRCR =0X05FA0000|(u32)0x04;	
			}
				
		}
		else
		{
			page_state_now=res;
		}

	}
	page_state_select_last=page_state_select;	
	
}


/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_main_page_Handle(event_type_t event)
{
	if(page_state_now == main_page  )
	{
		if(event == EVENT_KEY0_PRESSED)
		{
			page_key--;
			if(page_key<1)page_key=1;
			if(page_key==1)
			{	
				if(page_key_one)
				{
					page_key_one=0;
					Fun_show_refsh_touch_key((lab_list_t)page_key);				
				}
			}
			else 
			{
				page_key_one=1;
				Fun_show_refsh_touch_key((lab_list_t)page_key);
			}
		}
		else if(event == EVENT_KEY1_PRESSED)
		{
			if((0<page_key)&&(page_key<7))
				Fun_show_refsh_touch_key((lab_list_t)page_key);
		}
		else if(event == EVENT_KEY_UP_PRESSED)
		{
			page_key++;		
			if(page_key>6)page_key=6;
			if(page_key==6)
			{	
				if(page_key_one)
				{
					page_key_one=0;
					Fun_show_refsh_touch_key((lab_list_t)page_key);				
				}
			}
			else 
			{
				page_key_one=1;
				Fun_show_refsh_touch_key((lab_list_t)page_key);
			}
		
		}		
		else if(event == EVENT_TUOCH_LAB1)
		{
			Fun_show_refsh_touch_key(lab1);
			page_key=1;
		}
		else if(event == EVENT_TUOCH_LAB2)
		{
			Fun_show_refsh_touch_key(lab2);
			page_key=2;
		}
		else if(event == EVENT_TUOCH_LAB3)
		{
			Fun_show_refsh_touch_key(lab3);
			page_key=3;
		}
		else if(event == EVENT_TUOCH_LAB4)
		{
			Fun_show_refsh_touch_key(lab4);
			page_key=4;
		}
		else if(event == EVENT_TUOCH_LAB5)
		{
			Fun_show_refsh_touch_key(lab5);
			page_key=5;
		}
		else if(event == EVENT_TUOCH_LAB6)
		{
			Fun_show_refsh_touch_key(lab6);
			page_key=6;
		}
		else if(event == EVENT_TUOCH_SETING)
		{
			Fun_show_refsh_touch_key(seting);
		}	
		else if(event == EVENT_TUOCH_EXIT)
		{
			Fun_show_refsh_touch_key(exiting);		
		}			
	}
	return event;
}

void touch_main_page(void)
{
	if((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[0]+xiankuangjiange)&&\
		(y+line*0-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*0+xiankuangjiange+16)&&LAB1)
	{
		LAB1=0;
		event_establish(EVENT_TUOCH_LAB1);
	}
	else if(!((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[0]+xiankuangjiange)&&\
		(y+line*0-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*0+xiankuangjiange+16))||PEN==1)
	{
		LAB1=1;
	}

	if((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[1]+xiankuangjiange)&&\
		(y+line*1-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*1+xiankuangjiange+16)&&LAB2)
	{
		LAB2=0;
		event_establish(EVENT_TUOCH_LAB2);
	}
	else if(!((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[1]+xiankuangjiange)&&\
		(y+line*1-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*1+xiankuangjiange+16))||PEN==1)
	{
		LAB2=1;
	}

	if((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[2]+xiankuangjiange)&&\
		(y+line*2-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*2+xiankuangjiange+16)&&LAB3)
	{
		LAB3=0;
		event_establish(EVENT_TUOCH_LAB3);
	}
	else if(!((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[2]+xiankuangjiange)&&\
		(y+line*2-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*2+xiankuangjiange+16))||PEN==1)
	{
		LAB3=1;
	}

	
	if((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[3]+xiankuangjiange)&&\
		(y+line*3-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*3+xiankuangjiange+16)&&LAB4)
	{
		LAB4=0;
		event_establish(EVENT_TUOCH_LAB4);
	}
	else if(!((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[3]+xiankuangjiange)&&\
		(y+line*3-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*3+xiankuangjiange+16))||PEN==1)
	{
		LAB4=1;
	}
	
	if((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[4]+xiankuangjiange)&&\
		(y+line*4-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*4+xiankuangjiange+16)&&LAB5)
	{
		LAB5=0;
		event_establish(EVENT_TUOCH_LAB5);
	}
	else if(!((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[4]+xiankuangjiange)&&\
		(y+line*4-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*4+xiankuangjiange+16))||PEN==1)
	{
		LAB5=1;
	}	
	
	if((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[5]+xiankuangjiange)&&\
		(y+line*5-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*5+xiankuangjiange+16)&&LAB6)
	{
		LAB6=0;
		event_establish(EVENT_TUOCH_LAB6);
	}
	else if(!((x-xiankuangjiange<tp_dev.x)&&(tp_dev.x<x+56+16*list_text_num[5]+xiankuangjiange)&&\
		(y+line*5-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*5+xiankuangjiange+16))||PEN==1)
	{
		LAB6=1;
	}	
	
	
	//设置
	
	if((410<tp_dev.x)&&(tp_dev.x<420+32+xiankuangjiange)&&\
		(y+line*5-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*5+xiankuangjiange+16)&&SETING)
	{
		SETING=0;
		event_establish(EVENT_TUOCH_SETING);
	}
	
	
	else if(!((410<tp_dev.x)&&(tp_dev.x<420+32+xiankuangjiange)&&\
		(y+line*5-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*5+xiankuangjiange+16))||PEN==1)
	{
		SETING=1;
	}	
	//退出
	if((410<tp_dev.x)&&(tp_dev.x<420+32+xiankuangjiange)&&\
		(y+line*6-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*6+xiankuangjiange+16)&&EXIT)
	{
		EXIT=0;
		event_establish(EVENT_TUOCH_EXIT);
	}
	else if(!((410<tp_dev.x)&&(tp_dev.x<420+32+xiankuangjiange)&&\
		(y+line*6-xiankuangjiange<tp_dev.y)&&(tp_dev.y<y+line*6+xiankuangjiange+16))||PEN==1)
	{
		EXIT=1;
	}		

}

void Fun_Init_main_page(void)
{	
	resgister_event_handle(Fun_main_page_Handle,EVENT_KEY0_PRESSED|EVENT_KEY1_PRESSED|EVENT_KEY_UP_PRESSED|EVENT_TUOCH_LAB1|EVENT_TUOCH_LAB2\
												|EVENT_TUOCH_LAB3|EVENT_TUOCH_LAB4|EVENT_TUOCH_LAB5\
												|EVENT_TUOCH_LAB6|EVENT_TUOCH_SETING|EVENT_TUOCH_EXIT\
												);
}


void Fun_Show_main_page(void)
{
	Fun_main_page_Screen(0,0);
}

void Fun_Close_main_page(void)
{
	LCD_Clear(WHITE);
	LAB1=1;
	LAB2=1;
	LAB3=1;
	LAB4=1;
	LAB5=1;
	LAB6=1;
	SETING=1;
	EXIT=1;//按键标志
	page_key=0;
	page_key_one=1;
	page_state_select=close;
	page_state_select_last=close;
	
}

