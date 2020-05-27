#include "main_page.h"
#include "lcd.h"
#include "event_queue.h"
#include "Bsl_ScreenMgr.h"
#include "gui.h"




static uint8_t x=130,y=40,line=40,mode=1;
//static uint8_t xiankuangjiange=10;
static uint16_t color1=BLUE,color2=BLACK;


	
void main_page_init(void)
{


	//Gu_Drawbmp16_320X480(0,0,gImage_480X320);
	//Gui_Drawbmp16(0,0,gImage_80X80);

	LCD_Fill(0,0,lcddev.width,20,BLUE+200);

	Gui_StrCenter(0,2,WHITE,BLUE,"����ʱ����ʵ��ϵͳ",16,mode);//������ʾ
	
	LCD_Fill(0,20,lcddev.width,lcddev.height,WHITE);
	
	Show_Str(x,y,color2,color1,"ʵ��1����������ʱ��",16,mode);
	Show_Str(x,y+line,color2,color1,"ʵ��2������˲ʱ�ٶ�",16,mode);
	Show_Str(x,y+line+line,color2,color1,"ʵ��3�������ȱ����˶��ļ��ٶ�",16,mode);
	Show_Str(x,y+line+line+line,color2,color1,"ʵ��4��������ײ�еĶ���",16,mode);
	Show_Str(x,y+line+line+line+line,color2,color1,"ʵ��5������",16,mode);
	Show_Str(x,y+line+line+line+line+line,color2,color1,"ʵ��6���������ں�Ƶ��",16,mode);
	
	Show_Str(420,y+line+line+line+line+line,color2,color1,"����",16,mode);
	
	Show_Str(420,y+line+line+line+line+line+line,color2,color1,"�˳�",16,mode);


}


static void Fun_main_page_Screen(uint16_t period,void* p)
{
	
	main_page_init();
//	uint8_t list=0;
//	uint8_t list_text_num[6]={6,6,11,8,2,7};
//	
//	if(!(tp_dev.sta&TP_PRES_DOWN))	ones=1;
//	if(tp_dev.sta&TP_PRES_DOWN)			//������������
//	{	
//		for(list=0;list<6;list++)
//		{	
//			if(tp_dev.x>(x-xiankuangjiange)&&tp_dev.x<(x+56+16*list_text_num[list]+xiankuangjiange))
//			{	
//				if(tp_dev.y>(y+line*list-xiankuangjiange)&&tp_dev.y<(y+line*list+xiankuangjiange+16))
//				{
//					lab_list=list;
//					if(ones==1)
//					{
//						LCD_Clear(WHITE);
//						main_page_init();
//						ones=0;
//					}
//					if(lab_list_last==lab_list)
//					{
//						LED1=0;
//						labrary=lab_list;
//					}
//					else
//					{
//						LED1=1;

//					}
//					
//					
//					
//					POINT_COLOR=RED;
//					LCD_DrawRectangle(x-xiankuangjiange,y+line*list-xiankuangjiange,x+56+16*list_text_num[list]+xiankuangjiange,y+line*list+xiankuangjiange+16);
//					lab_list_last=lab_list;
//					text_num_last=list_text_num[list];
//					
//				}
//			}
//		}
//	}


}


/**@brief �¼�������
 *
 * @param[in] event �¼�
 */
event_type_t Fun_main_page_Handle(event_type_t event)
{
	if(event == EVENT_KEY0_PRESSED)
	{
	
	}
	
	return event;
}



void Fun_Init_main_page(void)
{	
	Bsl_CreateScreen(Fun_main_page_Screen,SCREEN_TYPE_MAIN,SCREEN_PRI_HEIGH);
	resgister_event_handle(Fun_main_page_Handle,EVENT_KEY0_PRESSED);
}


void Fun_Show_main_page(void)
{
	Bsl_WakeupScreen(5);
	Bsl_ShowScreen(Fun_main_page_Screen,0);
}

void Fun_Close_main_page(void)
{
	Bsl_WakeupScreen(5);
	Bsl_CloseScreen(Fun_main_page_Screen);
}




