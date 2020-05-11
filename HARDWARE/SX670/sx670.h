#ifndef __SX670_H
#define __SX670_H	 
#include "stm32f10x.h"
#include "sys.h"


#define sensor1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取传感器1
#define sensor2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//读取传感器1
#define sensor3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取传感器1
#define sensor4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取传感器1
 
 
 
 

void EE_SX670_INIT(void);//初始化

		 				    
#endif
