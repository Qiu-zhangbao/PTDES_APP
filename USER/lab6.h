#ifndef __LAB6_H
#define __LAB6_H
#include "sys.h"


typedef struct
{
	uint32_t cnt;
	uint32_t period_num;
	uint32_t period;
	uint32_t  frequency;
	uint32_t time_ms;
	uint16_t period_uint;
}lab6_parm_t;

extern lab6_parm_t lab6_parm;


void Fun_Close_lab6_page(void);
void Fun_Show_lab6_page(void);
void Fun_Init_lab6_page(void);
void touch_lab6_page(void);


#endif





