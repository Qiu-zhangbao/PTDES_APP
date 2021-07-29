#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
//修改日期:2010/5/27
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 全动电子 2009-2019
//All rights reserved
//********************************************************************************
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
//////////////////////////////////////////////////////////////////////////////////	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);


/** 
  * @brief 一个系统时间结构体
  */
typedef struct
{	
	void (* init) (void);  
	uint32_t (* get_time_us) (void);
	uint32_t (* get_time_ms) (void);
	void (* delay_us)(uint32_t);
	void (* delay_ms)(uint32_t);
}systime_t;

/** 声明 systime */
extern systime_t  systime;

#endif





























