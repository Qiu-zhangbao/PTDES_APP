#include "control.h"
#include "key.h"
#include "touch.h"
#include "main_page.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
#include "lab6.h"
#include "Bsl_ScreenMgr.h"

lab_list_t page_state=close;	
		
void Apc_InitFunCtrlSM(void)
{
	Fun_Init_main_page();
	Fun_Init_lab1_page();
	Fun_Init_lab2_page();
	Fun_Init_lab3_page();
	Fun_Init_lab4_page();
	Fun_Init_lab5_page();
	Fun_Init_lab6_page();
}



void Control_Init(void)
{





}

void Control_state_machine(lab_list_t state)
{
	if(state == page_state)
		return;
	
	switch(page_state)
	{
		case main_page:
			Fun_Close_main_page();
			break;
		
		case lab1:
			Fun_Close_lab1_page();
			break;
				
		case lab2:
			Fun_Close_lab2_page();
			break;
		
		case lab3:
			Fun_Close_lab3_page();
		
			break;

		case lab4:
			Fun_Close_lab4_page();
			break;
		
		case lab5:
			Fun_Close_lab5_page();
			break;		
		
		case lab6:
			Fun_Close_lab6_page();
			break;		
		case close:
			
			break;			
	}
	
	switch(state)
	{
		case main_page:
			Fun_Show_main_page();
			break;
		
		case lab1:
			Fun_Show_lab1_page();
			break;
		
		case lab2:
			Fun_Show_lab2_page();
			break;
		
		case lab3:
			Fun_Show_lab3_page();
			break;

		case lab4:
			Fun_Show_lab4_page();
			break;
		
		case lab5:
			Fun_Show_lab5_page();
			break;		
		
		case lab6:
			Fun_Show_lab6_page();
			break;	
		
		case close:
			
			break;	
	}
	page_state=state;
}



void main_control(void)
{
	static uint16_t period=0;
	KEY_Scan(0);	
	tp_dev.scan(0); 
	Control_state_machine(main_page);
	
	
	period++;
	if(period==10)
	{
		period=0;
		Bsl_FlushScreen(0);
	}
		
}




