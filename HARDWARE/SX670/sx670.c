#include "sx670.h"
#include "led.h"
#include "timer.h"
#include "event_queue.h"

SX670_t sx670_parm;
static uint8_t sx670_enable=0;


void EE_SX670_INIT_PIN(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
}

void EE_SX670_ENABLE(void)
{
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx
	sx670_enable=1;
}

void EE_SX670_DISENABLE(void)
{
	TIM_Cmd(TIM2, DISABLE);  //使能TIMx
	time_us=0;
	sx670_enable=0;
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
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}
#include "lab6.h"
uint8_t run_one=1;
uint8_t num=0;
//外部中断0服务程序 
void EXTI2_IRQHandler(void)
{
	
	if(sx670_enable)
	{
		
		if(sensor1==1)	 //按键KEY0
		{
			event_establish(EVENT_SENER1_OUT);
		}		 
		else 	 //按键KEY0
		{
			event_establish(EVENT_SENER1_IN);
		}	
	
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE3上的中断标志位  
}
 

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	if(sx670_enable)
	{
		if(sensor2==1)	 //按键KEY0
		{
			event_establish(EVENT_SENER2_OUT);
		}		 
		else	 //按键KEY0
		{
			event_establish(EVENT_SENER2_IN);
		}
	}		
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

void EXTI4_IRQHandler(void)
{
	if(sx670_enable)
	{
		if(sensor3==1)	 //按键KEY0
		{
			event_establish(EVENT_SENER3_OUT);
		}		 
		else
		{
			event_establish(EVENT_SENER3_IN);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}

void EXTI9_5_IRQHandler(void)
{
	if(sx670_enable)
	{
		if(sensor4==1)	 //按键KEY0
		{
			event_establish(EVENT_SENER4_OUT);
		}		 
		else
		{
			event_establish(EVENT_SENER4_IN);
		} 		
	}
	EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE4上的中断标志位  
}


