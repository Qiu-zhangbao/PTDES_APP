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

	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,mode);//居中显示
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	
	Show_Str(20,40,color2,color1,"实验一：测量挡光时间",16,mode);
	
	Show_Str(100+30,16+50+30,color2,color1,"传感器1:",16,mode);
	Show_Str(100+30,16+50+50+30,color2,color1,"传感器2:",16,mode);
	Show_Str(100+30,16+50+50+50+30,color2,color1,"传感器3:",16,mode);
	Show_Str(100+30,16+50+50+50+50+30,color2,color1,"传感器4:",16,mode);
	
	Show_Str(280+30,16+50+30,color2,color1,"us",16,mode);
	Show_Str(280+30,16+50+50+30,color2,color1,"us",16,mode);
	Show_Str(280+30,16+50+50+50+30,color2,color1,"us",16,mode);
	Show_Str(280+30,16+50+50+50+50+30,color2,color1,"us",16,mode);
	
	Show_Str(420,16+50+50+50,color2,RED,"开始",16,mode);
	Show_Str(420,16+50+50+50+50,color2,RED,"清零",16,mode);
	Show_Str(420,16+50+50+50+50+50,color2,RED,"返回",16,mode);

		POINT_COLOR=RED;
		LCD_ShowNum(180+30,16+50+30,sx670_parm.sensor1_us,12,16);
		LCD_ShowNum(180+30,16+50+50+30,sx670_parm.sensor2_us,12,16);
		LCD_ShowNum(180+30,16+50+50+50+30,sx670_parm.sensor3_us,12,16);
		LCD_ShowNum(180+30,16+50+50+50+50+30,sx670_parm.sensor4_us,12,16);
}


