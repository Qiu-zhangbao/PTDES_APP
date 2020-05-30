#include "main_page.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"
#include "control.h"
#include "led.h"

static uint8_t VERSION_NUMBER[]="Vison:1.0";

static uint8_t KEY_MANUAL[]="RESET | UP | OK | DOWN";

static uint8_t x=150,y=40,line=40,mode=1;
static uint16_t color1=BLUE,color2=BLACK;
static uint8_t  xiankuangjiange=10;
static uint8_t list_text_num[6]={6,6,11,8,2,7};

	
void main_page_init(void)
{
	

}


static void Fun_main_page_Screen(uint16_t period,void* p)
{	
	//Gu_Drawbmp16_320X480(0,0,gImage_480X320);
	//Gui_Drawbmp16(0,0,gImage_80X80);

	LCD_Fill(0,0,lcddev.width,20,MY_DARKBLUE);
	Gui_StrCenter(0,2,WHITE,BLUE,"光电计时数字实验系统",16,mode);//居中显示
	Show_Str(400,8,WHITE,BLACK,VERSION_NUMBER,12,mode);//版本号
	
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);

	Show_Str(x,y,color2,color1,"实验1：测量挡光时间",16,mode);
	Show_Str(x,y+line,color2,color1,"实验2：测量瞬时速度",16,mode);
	Show_Str(x,y+line+line,color2,color1,"实验3：测量匀变速运动的加速度",16,mode);
	Show_Str(x,y+line+line+line,color2,color1,"实验4：测量碰撞中的动量",16,mode);
	Show_Str(x,y+line+line+line+line,color2,color1,"实验5：计数",16,mode);
	Show_Str(x,y+line+line+line+line+line,color2,color1,"实验6：测量周期和频率",16,mode);
	
	for(uint8_t list=0;list<6;list++)
	{	
		
		POINT_COLOR=MY_DARKBLUE;
		LCD_DrawRectangle(x-xiankuangjiange,y+line*list-xiankuangjiange,x+56+16*list_text_num[list]+xiankuangjiange,y+line*list+xiankuangjiange+16);
		Show_Str(x-40,y+list*line,MY_DARKBLUE,color1,"魑",16,mode);
	}
	
	LCD_DrawRectangle(410,y+line*5-xiankuangjiange,420+32+xiankuangjiange,y+line*5+xiankuangjiange+16);	
	Show_Str(420,y+line+line+line+line+line,color2,color1,"设置",16,mode);
	LCD_DrawRectangle(410,y+line*6-xiankuangjiange,420+32+xiankuangjiange,y+line*6+xiankuangjiange+16);	
	Show_Str(420,y+line+line+line+line+line+line,RED,color1,"退出",16,mode);

	Show_Str(0,308,WHITE,BLACK,KEY_MANUAL,12,0);//按键说明
	
	LCD_LED=1;//加载完之后点亮背光	
}


/**@brief 事件处理函数
 *
 * @param[in] event 事件
 */
event_type_t Fun_main_page_Handle(event_type_t event)
{
	if(event == EVENT_KEY0_PRESSED)
	{
		if(page_state_now == main_page  )
		{
			LED1=!LED1;
		
		
		}
	}
	return event;
}



void Fun_Init_main_page(void)
{	
	resgister_event_handle(Fun_main_page_Handle,EVENT_KEY0_PRESSED);
}


void Fun_Show_main_page(void)
{
	Fun_main_page_Screen(0,0);
}

void Fun_Close_main_page(void)
{
	LCD_Clear(WHITE);
}

