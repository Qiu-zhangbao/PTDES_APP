#include "lab6.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"
#include "sx670.h"
#include "control.h"
#include "led.h"


void Fun_lab6_page_init(void)
{

	
	
}

static void Fun_lab6_page_Screen(uint16_t period,void* p)
{





}


/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_lab6_page_Handle(event_type_t event)
{
	if(event == EVENT_KEY0_PRESSED)
	{
	
	}
	
	return event;
}



void Fun_Init_lab6_page(void)
{	
	Bsl_CreateScreen(Fun_lab6_page_Screen,SCREEN_TYPE_MAIN,SCREEN_PRI_HEIGH);
	resgister_event_handle(Fun_lab6_page_Handle,EVENT_KEY0_PRESSED);
}


void Fun_Show_lab6_page(void)
{
	Bsl_WakeupScreen(5);
	Bsl_ShowScreen(Fun_lab6_page_Screen,0);
}

void Fun_Close_lab6_page(void)
{
	Bsl_WakeupScreen(5);
	Bsl_CloseScreen(Fun_lab6_page_Screen);
}





