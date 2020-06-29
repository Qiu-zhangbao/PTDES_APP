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
lab_list_t page_state_now=close;	
		
void Apc_InitFunCtrlSM(void)
{
	Fun_Init_main_page();
	Fun_Init_lab1_page();
	Fun_Init_lab2_page();
//	Fun_Init_lab3_page();
	Fun_Init_lab4_page();
	Fun_Init_lab5_page();
	Fun_Init_lab6_page();
}



void Control_Init(void)
{
//	page_state_now=main_page;
	page_state_now=lab2;
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
			Fun_Close_lab2_page();
		
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
		case seting:
			
			break;	
		case exiting:
			
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
			Fun_Show_lab2_page();
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
		case seting:
			
			break;	
		case exiting:
			
			break;	
	}
	page_state=state;
}



void Control_touch(void)
{
	switch(page_state_now)
	{
		case main_page:
			touch_main_page();
			break;
		
		case lab1:
			touch_lab1_page();
			break;
		
		case lab2:
			touch_lab2_page();
			break;
		
		case lab3:
			touch_lab2_page();
			break;

		case lab4:
			touch_lab4_page();
			break;
		
		case lab5:
			touch_lab5_page();
			break;		
		
		case lab6:
			touch_lab6_page();
			break;	
		
		case close:
			
			break;	
		case seting:
			
			break;	
		case exiting:
			
			break;	
	}
}


void main_control(void)
{
	tp_dev.scan(0); 
	Control_state_machine(page_state_now);	
	Control_touch();
}





