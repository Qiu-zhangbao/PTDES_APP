#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "sys.h"
#include "control.h"


#define	BEBUG_MODE			0	//调试模式
#define	OFFICIAL_MODE		1	//正式模式

#define USER_MODE	OFFICIAL_MODE	//用户模式

#if	( USER_MODE == BEBUG_MODE )

	#define BEBUG_PAGE	lab3	//调试页面选择

#endif



#define VERSION "release:2.5"	//版本号


/*
版本记录

2.5-20200703
1、实验3方法2的速度减小一个数量级


2.4-20200702
1、实验3修改数值的排列
2、实验3计算加速度的正负性
3、实验3方法二加t13显示

2.3-20200701
1、实验3的界面和加速度的计算方法
2、实验4屏幕右上角把“12为正”改为“1234为正”、删去”通过“、在”距离“前加”传感器“两个字
3、实验5和实验6把KEY_0设置成暂停和继续、删去”通过“两个字
4、所有实验keyup为加，key1为减，屏幕触控也要对应

2.2-20200630
1、实验4更改速度的方向限制
2、实验6加触发N次为一周期

2.1-20200629
主页：去掉屏幕右下角的“设置”按钮
实验1：的传感器1数值上去掉ms、s
实验2：挡光片宽度和传感器距离的设定都用在屏幕上加触控按键、默认值改为7
实验3：挡光片宽度和传感器距离的设定都用在屏幕上加触控按键、默认值改为7
实验4：传感器距离的设定在屏幕上加触控按键
实验5：界面加“计数”和“计时”文字注明数值的意义、暂停时停止计数
实验6：暂停时停止计数

2.1
程序大致完工，版本记录开始

*/



#endif





