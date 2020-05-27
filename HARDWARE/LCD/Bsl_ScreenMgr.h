#ifndef BSL_SCREENMGR_H
#define BSL_SCREENMGR_H
#include "stdint.h"
typedef enum 
{
	SCREEN_PRI_LOW = 0,	///<低
	SCREEN_PRI_NORMAL,	///<中
	SCREEN_PRI_HEIGH,	///<高
}SCREEN_PRI;			///<屏幕重复间隔

typedef enum
{
	SCREEN_TYPE_MAIN = 0, 	///<主屏幕
	SCREEN_TYPE_DIALOG,		///<需要用户确认的对话框
	SCREEN_TYPE_MSSAGE,		///<消息
}SCREEN_TYPE;				///<屏幕类型

typedef struct
{
	void (*paint)(uint16_t period,void *p);	///<要显示的函数指针
	void *paint_p;							///<备用指针，当前无意义
	SCREEN_TYPE type;						///<屏幕类型
	SCREEN_PRI  pri;						///<屏幕重复间隔
	uint16_t show_time;						///<显示时间
}S_SCREEN;									///<屏幕显示列表结构体数组 

extern void Bsl_InitScreenMgr(void);
extern void Bsl_WakeupScreen(uint16_t time);
extern void Bsl_TurnOffScreen(void);
extern void Bsl_TurnOnScreen(void);

extern void Bsl_TipMessage(void (*paint)(uint16_t,void *),void *p,unsigned short tip_time,SCREEN_PRI pri);
extern void Bsl_CreateScreen(void (*paint)(uint16_t,void *),SCREEN_TYPE type,SCREEN_PRI pri);
extern void Bsl_ShowScreen(void (*paint)(uint16_t,void *),void *p);
extern void Bsl_CloseScreen(void (*paint)(uint16_t,void *));
extern unsigned char Bsl_IsScreenShowing(void (*paint)(uint16_t,void *));
extern unsigned char Bsl_IsShowingMainScreen(void);
extern uint8_t bsl_get_screen_show(void);
extern void bsl_turn_off_screen(void);
#endif
