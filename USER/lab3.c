#include "lab3.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"
#include "sx670.h"
#include "control.h"
#include "led.h"

static uint16_t color1=BLUE,color2=BLACK;
static uint8_t mode=1;

void Fun_lab3_page_init(void)
{

	
	
}

static void Fun_lab3_page_Screen(uint16_t period,void* p)
{
	Show_Str(20,40,color2,color1,"实验3：测量匀变速运动的加速度",16,mode);
	
	Show_Str(100+30,16+50+30,color2,color1,"传感器1:",16,mode);
	Show_Str(100+30,16+50+50+30,color2,color1,"传感器2:",16,mode);
	Show_Str(100+30,16+50+50+50+30,color2,color1,"传感器3:",16,mode);
	Show_Str(100+30,16+50+50+50+50+30,color2,color1,"传感器4:",16,mode);




}
void touch_lab3_page(void)
{




}

/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab3_page_Handle(event_type_t event)
{
	if(event == EVENT_KEY0_PRESSED)
	{
	
	}
	
	return event;
}



void Fun_Init_lab3_page(void)
{	
	Bsl_CreateScreen(Fun_lab3_page_Screen,SCREEN_TYPE_MAIN,SCREEN_PRI_HEIGH);
	resgister_event_handle(Fun_lab3_page_Handle,EVENT_KEY0_PRESSED);
}


void Fun_Show_lab3_page(void)
{
	Fun_lab3_page_Screen(0,0);
//	Bsl_WakeupScreen(5);
//	Bsl_ShowScreen(Fun_lab3_page_Screen,0);
}

void Fun_Close_lab3_page(void)
{
//	Bsl_WakeupScreen(5);
//	Bsl_CloseScreen(Fun_lab3_page_Screen);
	LCD_Clear(WHITE);
}





