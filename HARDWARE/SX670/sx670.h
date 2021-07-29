#ifndef __SX670_H
#define __SX670_H	 
#include "stm32f10x.h"
#include "sys.h"

typedef struct
{
	uint32_t sensor1_us;
	uint32_t sensor2_us;
	uint32_t sensor3_us;
	uint32_t sensor4_us;
	
	int32_t sensor1_v;
	int32_t sensor2_v;
	int32_t sensor3_v;
	int32_t sensor4_v;
	
	uint32_t sensor12_us;
	uint32_t sensor34_us;
	int32_t sensor12_v;
	int32_t sensor34_v;
	
	uint32_t sensor_tip1_a1;
	uint32_t sensor_tip1_a2;
	uint32_t sensor_tip2_a;

}SX670_t;

extern SX670_t sx670_parm;
extern uint8_t sx670_enable;
extern int time_us_reset_flag;
#define sensor1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取传感器
#define sensor2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//读取传感器
#define sensor3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取传感器
#define sensor4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取传感器
 
 
 
 

void EE_SX670_INIT(void);//初始化
void EE_SX670_ENABLE(void);
void EE_SX670_DISENABLE(void);


#endif
