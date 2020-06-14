#include "sx670.h"
#include "led.h"
#include "timer.h"
#include "event_queue.h"

SX670_t sx670_parm;
static uint8_t sx670_enable=0;


void EE_SX670_INIT_PIN(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
}

void EE_SX670_ENABLE(void)
{
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx
	sx670_enable=1;
}

void EE_SX670_DISENABLE(void)
{
	TIM_Cmd(TIM2, DISABLE);  //ʹ��TIMx
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


//�ⲿ�ж�0�������
void EE_SX670_INIT(void)
{
   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    EE_SX670_INIT_PIN();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2|GPIO_PinSource3|GPIO_PinSource4|GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4|EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
}
#include "lab6.h"
uint8_t run_one=1;
uint8_t num=0;
//�ⲿ�ж�0������� 
void EXTI2_IRQHandler(void)
{
	
	if(sx670_enable)
	{
		
		if(sensor1==1)	 //����KEY0
		{
			event_establish(EVENT_SENER1_OUT);
		}		 
		else 	 //����KEY0
		{
			event_establish(EVENT_SENER1_IN);
		}	
	
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE3�ϵ��жϱ�־λ  
}
 

//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	if(sx670_enable)
	{
		if(sensor2==1)	 //����KEY0
		{
			event_establish(EVENT_SENER2_OUT);
		}		 
		else	 //����KEY0
		{
			event_establish(EVENT_SENER2_IN);
		}
	}		
	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}

void EXTI4_IRQHandler(void)
{
	if(sx670_enable)
	{
		if(sensor3==1)	 //����KEY0
		{
			event_establish(EVENT_SENER3_OUT);
		}		 
		else
		{
			event_establish(EVENT_SENER3_IN);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}

void EXTI9_5_IRQHandler(void)
{
	if(sx670_enable)
	{
		if(sensor4==1)	 //����KEY0
		{
			event_establish(EVENT_SENER4_OUT);
		}		 
		else
		{
			event_establish(EVENT_SENER4_IN);
		} 		
	}
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
}


