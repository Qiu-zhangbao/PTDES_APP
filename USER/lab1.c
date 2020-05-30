#include "lab1.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"
#include "sx670.h"
#include "control.h"
#include "led.h"

static uint16_t color1=BLUE,color2=BLACK;
//static uint8_t x=130,y=40,line=40,

static uint8_t mode=1;
SX670_t sx670_parm;


void Fun_lab1_page_init(void)
{



	POINT_COLOR=RED;
	LCD_ShowNum(180+30,16+50+30,sx670_parm.sensor1_us,12,16);
	LCD_ShowNum(180+30,16+50+50+30,sx670_parm.sensor2_us,12,16);
	LCD_ShowNum(180+30,16+50+50+50+30,sx670_parm.sensor3_us,12,16);
	LCD_ShowNum(180+30,16+50+50+50+50+30,sx670_parm.sensor4_us,12,16);
}

static void Fun_lab1_page_Screen(uint16_t period,void* p)
{


	LCD_Fill(0,0,lcddev.width,20,BLUE+200);

	Gui_StrCenter(0,2,WHITE,BLUE,"����ʱ����ʵ��ϵͳ",16,mode);//������ʾ
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	
	Show_Str(20,40,color2,color1,"ʵ��1����������ʱ��",16,mode);
	
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


}


/**@brief �¼�������
 *
 * @param[in] event �¼�
 */
event_type_t Fun_lab1_page_Handle(event_type_t event)
{
	if(event == EVENT_KEY0_PRESSED)
	{
	
	}
	
	return event;
}



void Fun_Init_lab1_page(void)
{	
	Bsl_CreateScreen(Fun_lab1_page_Screen,SCREEN_TYPE_MAIN,SCREEN_PRI_HEIGH);
	resgister_event_handle(Fun_lab1_page_Handle,EVENT_KEY0_PRESSED);
}


void Fun_Show_lab1_page(void)
{
//	Bsl_WakeupScreen(5);
//	Bsl_ShowScreen(Fun_lab1_page_Screen,0);
	Fun_lab1_page_Screen(0,0);
}

void Fun_Close_lab1_page(void)
{
//	Bsl_WakeupScreen(5);
//	Bsl_CloseScreen(Fun_lab1_page_Screen);
	LCD_Clear(WHITE);
}





