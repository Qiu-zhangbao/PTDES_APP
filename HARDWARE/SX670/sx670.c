#include "sx670.h"
#include "led.h"
#include "timer.h"
#include "event_queue.h"
#include "delay.h"
SX670_t sx670_parm;
uint8_t sx670_enable=0;

uint8_t open_exti_flag=1;
uint16_t open_exti_delay_ms=0;



int time_us_reset_flag=0;

void EE_SX670_INIT_PIN(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟

	//初始化 WK_UP-->GPIOA.0
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EE_SX670_ENABLE(void)
{
	sx670_enable=1;
	time_us_reset_flag=1;

}

void EE_SX670_DISENABLE(void)
{

	sx670_enable=0;
	time_us=0;
	sx670_parm.sensor1_us=0;
	sx670_parm.sensor2_us=0;
	sx670_parm.sensor3_us=0;
	sx670_parm.sensor4_us=0;
	sx670_parm.sensor12_us=0;
	sx670_parm.sensor34_us=0;
	
	sx670_parm.sensor12_v=0;
	sx670_parm.sensor34_v=0;
	sx670_parm.sensor1_v=0;
	sx670_parm.sensor2_v=0;
	sx670_parm.sensor3_v=0;
	sx670_parm.sensor4_v=0;
	
	sx670_parm.sensor_tip1_a1=0;
	sx670_parm.sensor_tip1_a2=0;
	sx670_parm.sensor_tip2_a=0;
}


//外部中断0服务程序
void EE_SX670_INIT(void)
{
   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    EE_SX670_INIT_PIN();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2|GPIO_PinSource3|GPIO_PinSource4|GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4|EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	printf("sx670 init\r\n");
 
}

u32 sensor1_in_time_us=0;

u32 sensor1_out_time_us=0;

void open_exti_flag_after_time(u16 after_time_ms)
{
	open_exti_flag=DISABLE;
	
	open_exti_delay_ms=after_time_ms;

}


//外部中断0服务程序 
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(GPIO_Pin_2) != RESET)
	{
		if(sx670_enable&&open_exti_flag)
		{
			open_exti_flag_after_time(2);
			if(sensor1==1)	 //按键KEY0
			{
				LED0=1;
				event_establish(EVENT_SENER1_OUT);
			}		 
			else	 //按键KEY0
			{
				LED0=0;
				event_establish(EVENT_SENER1_IN);
			}
		}		
		EXTI_ClearFlag(EXTI_Line2);
		EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE3上的中断标志位  
	}
}
 

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(GPIO_Pin_3) != RESET)
	{
		if(sx670_enable&&open_exti_flag)
		{
			open_exti_flag_after_time(2);
			if(sensor2==1)	 //按键KEY0
			{
				LED0=1;
				event_establish(EVENT_SENER2_OUT);
			}		 
			else	 //按键KEY0
			{
				LED0=0;
				event_establish(EVENT_SENER2_IN);
			}
		}		

		EXTI_ClearFlag(EXTI_Line3);
		EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
	}
	
}

void EXTI4_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(GPIO_Pin_4) != RESET)
	{
		if(sx670_enable&&open_exti_flag)
		{
			open_exti_flag_after_time(2);
			if(sensor3==1)	 //按键KEY0
			{
				LED0=1;
				event_establish(EVENT_SENER3_OUT);
			}		 
			else
			{
				LED0=0;
				event_establish(EVENT_SENER3_IN);
			}
		}
		EXTI_ClearFlag(EXTI_Line4);
		EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE3上的中断标志位  
	}
	
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(GPIO_Pin_5) != RESET)
	{
		if(sx670_enable&&open_exti_flag)
		{
			open_exti_flag_after_time(2);
			if(sensor4==1)	 //按键KEY0
			{
				LED0=1;
				event_establish(EVENT_SENER4_OUT);
			}		 
			else
			{
				LED0=0;
				event_establish(EVENT_SENER4_IN);
			} 		
		}
		EXTI_ClearFlag(EXTI_Line5);
		EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE3上的中断标志位  
	
	}		
}


