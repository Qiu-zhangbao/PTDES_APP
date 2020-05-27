#include "control.h"
#include "gui.h"
#include "lcd.h"
#include "test.h"
#include "pic.h"
#include "key.h"
#include "touch.h"
#include "delay.h"
#include "led.h"

#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
#include "lab6.h"
vu8 key=0;	

static uint8_t x=130,y=40,line=40,mode=1;
uint8_t xiankuangjiange=10;

static uint16_t color1=BLUE,color2=BLACK;

uint8_t lab_list=0,lab_list_last=0; 

uint8_t text_num_last;
uint8_t ones=1;
uint8_t labrary=0xff;
	
lab_list_t page_last;	
	
lab_list_t page_now;	
	
void main_page_init(void)
{

	//Gu_Drawbmp16_320X480(0,0,gImage_480X320);
	//Gui_Drawbmp16(0,0,gImage_80X80);

	LCD_Fill(0,0,lcddev.width,20,BLUE+200);

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,mode);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	Show_Str(x,y,color2,color1,"实验1：测量挡光时间",16,mode);
	Show_Str(x,y+line,color2,color1,"实验2：测量瞬时速度",16,mode);
	Show_Str(x,y+line+line,color2,color1,"实验3：测量匀变速运动的加速度",16,mode);
	Show_Str(x,y+line+line+line,color2,color1,"实验4：测量碰撞中的动量",16,mode);
	Show_Str(x,y+line+line+line+line,color2,color1,"实验5：计数",16,mode);
	Show_Str(x,y+line+line+line+line+line,color2,color1,"实验6：测量周期和频率",16,mode);
	
	Show_Str(420,y+line+line+line+line+line,color2,color1,"设置",16,mode);
	
	Show_Str(420,y+line+line+line+line+line+line,color2,color1,"退出",16,mode);


}


void main_page_refresh(void)
{
	uint8_t list=0;
	uint8_t list_text_num[6]={6,6,11,8,2,7};
	
	if(!(tp_dev.sta&TP_PRES_DOWN))	ones=1;
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{	
		for(list=0;list<6;list++)
		{	
			if(tp_dev.x>(x-xiankuangjiange)&&tp_dev.x<(x+56+16*list_text_num[list]+xiankuangjiange))
			{	
				if(tp_dev.y>(y+line*list-xiankuangjiange)&&tp_dev.y<(y+line*list+xiankuangjiange+16))
				{
					lab_list=list;
					if(ones==1)
					{
						LCD_Clear(WHITE);
						main_page_init();
						ones=0;
					}
					if(lab_list_last==lab_list)
					{
						LED1=0;
						labrary=lab_list;
					}
					else
					{
						LED1=1;

					}
					
					
					
					POINT_COLOR=RED;
					LCD_DrawRectangle(x-xiankuangjiange,y+line*list-xiankuangjiange,x+56+16*list_text_num[list]+xiankuangjiange,y+line*list+xiankuangjiange+16);
					lab_list_last=lab_list;
					text_num_last=list_text_num[list];
					
				}
			}
		}
	}


}

void main_page_close(void)
{










}

void main_work_turn_sm(lab_list_t state)
{
	if(state == page_last)
		return;
	
	switch(page_last)
	{
		case lab1:
			
			break;
	}
	
	switch(state)
	{
		case lab1:
			lab1_page_init();	
			break;
	}



	page_last=state;


}



void main_control(void)
{
	
	key = KEY_Scan(0);	
	tp_dev.scan(0); 
	main_work_turn_sm(page_now);
	

	//main_work_turn_sm();
	
}




