#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93xx.h"
#include "usart.h"	 
#include "24cxx.h"
#include "flash.h"
#include "touch.h"
#include "sram.h"
#include "timer.h"
#include "malloc.h"
#include "GUI.h"
#include "frame.h"
/************************************************
 ALIENTEK精英STM32开发板STemWin实验
 STemWin 移植实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

void emwin_texttest(void)
{
	int i;
	char acText[]	= "This example demostrates text wrapping";
	GUI_RECT Rect	={200,240,259,299};	//定义矩形显示区域
	GUI_WRAPMODE aWm[] = {GUI_WRAPMODE_NONE,
						  GUI_WRAPMODE_CHAR,
						  GUI_WRAPMODE_WORD};
				  
	GUI_SetBkColor(GUI_BLUE);		//设置背景颜色
	GUI_Clear();					//清屏
	GUI_SetFont(&GUI_Font24_ASCII); //设置字体
	GUI_SetColor(GUI_YELLOW);       //设置前景色(如文本，画线等颜色)
	GUI_DispString("HELLO WORD!");
	
	GUI_SetFont(&GUI_Font8x16);		//设置字体
	GUI_SetPenSize(10);				//设置笔大小
	GUI_SetColor(GUI_RED);			//红色字体
	GUI_DrawLine(300,50,500,130);	//绘线
	GUI_DrawLine(300,130,500,50);	//绘线
	GUI_SetBkColor(GUI_BLACK);		//设置黑色背景
	GUI_SetColor(GUI_WHITE);		//设置字体颜色为白色
	GUI_SetTextMode(GUI_TM_NORMAL);	//正常模式
	GUI_DispStringHCenterAt("GUI_TM_NORMAL",400,50);
	GUI_SetTextMode(GUI_TM_REV);	//反转文本
	GUI_DispStringHCenterAt("GUI_TM_REV"   ,400,66);
	GUI_SetTextMode(GUI_TM_TRANS);	//透明文本
	GUI_DispStringHCenterAt("GUI_TM_TRANS" ,400,82);
	GUI_SetTextMode(GUI_TM_XOR);   	//异或文本
	GUI_DispStringHCenterAt("GUI_TM_XOR"   ,400,98);
	GUI_SetTextMode(GUI_TM_TRANS|GUI_TM_REV);//透明反转文本
	GUI_DispStringHCenterAt("GUI_EM_TRANS|GUI_TM_REV",400,114);
	
	GUI_SetTextMode(GUI_TM_TRANS);  //透明文本
	for(i=0;i<3;i++)
	{
		GUI_SetColor(GUI_WHITE);
		GUI_FillRectEx(&Rect);
		GUI_SetColor(GUI_BLACK);
		GUI_DispStringInRectWrap(acText,&Rect,GUI_TA_LEFT,aWm[i]);//在当前窗口指定的矩形区域内显示字符串(并可自动换行)
		Rect.x0 += 70;
		Rect.x1 += 70;
	}	
}


void Touch_MainTask(void) {
 
  GUI_CURSOR_Show();
  GUI_CURSOR_Select(&GUI_CursorCrossL);
  GUI_SetBkColor(GUI_WHITE);
  GUI_SetColor(GUI_BLACK);
  GUI_Clear();
  GUI_DispString("Measurement of\nA/D converter values");
  while (1) {
    GUI_PID_STATE TouchState;
    int xPhys, yPhys;
    GUI_TOUCH_GetState(&TouchState);  /* Get the touch position in pixel */
    xPhys = GUI_TOUCH_GetxPhys();     /* Get the A/D mesurement result in x */
    yPhys = GUI_TOUCH_GetyPhys();     /* Get the A/D mesurement result in y */
    /* Display the measurement result */
    GUI_SetColor(GUI_BLUE);
    GUI_DispStringAt("Analog input:\n", 0, 20);
    GUI_GotoY(GUI_GetDispPosY() + 2);
    GUI_DispString("x:");
    GUI_DispDec(xPhys, 4);
    GUI_DispString(", y:");
    GUI_DispDec(yPhys, 4);
    /* Display the according position */
    GUI_SetColor(GUI_RED);
    GUI_GotoY(GUI_GetDispPosY() + 4);
    GUI_DispString("\nPosition:\n");
    GUI_GotoY(GUI_GetDispPosY() + 2);
    GUI_DispString("x:");
    GUI_DispDec(TouchState.x,4);
    GUI_DispString(", y:");
    GUI_DispDec(TouchState.y,4);
    /* Wait a while */
    GUI_Delay(100);
  };
}
int main(void)
{	 			
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			    //LED端口初始化
	TFTLCD_Init();			//LCD初始化	
	KEY_Init();	 			//按键初始化
 	TP_Init();				//触摸屏初始化
	FSMC_SRAM_Init();		//初始化SRAM
	TIM3_Int_Init(999,71);	//1KHZ 定时器1ms 
	TIM6_Int_Init(999,719);	//10ms中断
	
	my_mem_init(SRAMIN); 		//初始化内部内存池
	my_mem_init(SRAMEX);  		//初始化外部内存池
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//使能CRC时钟，否则STemWin不能使用 
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	CreateFramewin();
	GUI_CURSOR_Show(); //显示鼠标
	while(1)
	{
		GUI_Delay(100);
	}

}

