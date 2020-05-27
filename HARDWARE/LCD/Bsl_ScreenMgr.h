#ifndef BSL_SCREENMGR_H
#define BSL_SCREENMGR_H
#include "stdint.h"
typedef enum 
{
	SCREEN_PRI_LOW = 0,	///<��
	SCREEN_PRI_NORMAL,	///<��
	SCREEN_PRI_HEIGH,	///<��
}SCREEN_PRI;			///<��Ļ�ظ����

typedef enum
{
	SCREEN_TYPE_MAIN = 0, 	///<����Ļ
	SCREEN_TYPE_DIALOG,		///<��Ҫ�û�ȷ�ϵĶԻ���
	SCREEN_TYPE_MSSAGE,		///<��Ϣ
}SCREEN_TYPE;				///<��Ļ����

typedef struct
{
	void (*paint)(uint16_t period,void *p);	///<Ҫ��ʾ�ĺ���ָ��
	void *paint_p;							///<����ָ�룬��ǰ������
	SCREEN_TYPE type;						///<��Ļ����
	SCREEN_PRI  pri;						///<��Ļ�ظ����
	uint16_t show_time;						///<��ʾʱ��
}S_SCREEN;									///<��Ļ��ʾ�б�ṹ������ 

void Bsl_InitScreenMgr(void);
void Bsl_WakeupScreen(uint16_t time);
void Bsl_TurnOffScreen(void);
void Bsl_TurnOnScreen(void);
void Bsl_FlushScreen(void* p_context);
void Bsl_TipMessage(void (*paint)(uint16_t,void *),void *p,unsigned short tip_time,SCREEN_PRI pri);
void Bsl_CreateScreen(void (*paint)(uint16_t,void *),SCREEN_TYPE type,SCREEN_PRI pri);
void Bsl_ShowScreen(void (*paint)(uint16_t,void *),void *p);
void Bsl_CloseScreen(void (*paint)(uint16_t,void *));
unsigned char Bsl_IsScreenShowing(void (*paint)(uint16_t,void *));
unsigned char Bsl_IsShowingMainScreen(void);
uint8_t bsl_get_screen_show(void);
void bsl_turn_off_screen(void);

#endif


