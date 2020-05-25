#include "lab1.h"
#include "gui.h"
#include "lcd.h"
#include "test.h"
#include "sx670.h"

static uint16_t color1=BLUE,color2=BLACK;
static uint8_t x=130,y=40,line=40,mode=1;
SX670_t sx670_parm;
void lab1_page(void)
{

	LCD_Fill(0,0,lcddev.width,20,BLUE+200);

	Gui_StrCenter(0,2,WHITE,BLUE,"����ʱ����ʵ��ϵͳ",16,mode);//������ʾ
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	
	Show_Str(20,40,color2,color1,"ʵ��һ����������ʱ��",16,mode);
	
	Show_Str(100+30,16+50+30,color2,color1,"������1:",16,mode);
	Show_Str(100+30,16+50+50+30,color2,color1,"������2:",16,mode);
	Show_Str(100+30,16+50+50+50+30,color2,color1,"������3:",16,mode);
	Show_Str(100+30,16+50+50+50+50+30,color2,color1,"������4:",16,mode);
	
	Show_Str(280+30,16+50+30,color2,color1,"us",16,mode);
	Show_Str(280+30,16+50+50+30,color2,color1,"us",16,mode);
	Show_Str(280+30,16+50+50+50+30,color2,color1,"us",16,mode);
	Show_Str(280+30,16+50+50+50+50+30,color2,color1,"us",16,mode);
	
	Show_Str(420,16+50+50+50,color2,RED,"��ʼ",16,mode);
	Show_Str(420,16+50+50+50+50,color2,RED,"����",16,mode);
	Show_Str(420,16+50+50+50+50+50,color2,RED,"����",16,mode);

		POINT_COLOR=RED;
		LCD_ShowNum(180+30,16+50+30,sx670_parm.sensor1_us,12,16);
		LCD_ShowNum(180+30,16+50+50+30,sx670_parm.sensor2_us,12,16);
		LCD_ShowNum(180+30,16+50+50+50+30,sx670_parm.sensor3_us,12,16);
		LCD_ShowNum(180+30,16+50+50+50+50+30,sx670_parm.sensor4_us,12,16);
}


