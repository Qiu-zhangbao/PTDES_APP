#include "sx670.h"
#include "led.h"
#include "timer.h"
#include "event_queue.h"
#include "delay.h"
SX670_t sx670_parm;
uint8_t sx670_enable=0;


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
	sx670_enable=1;
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx
}

void EE_SX670_DISENABLE(void)
{
	TIM_Cmd(TIM2, DISABLE);  //ʹ��TIMx
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
	if(EXTI_GetITStatus(GPIO_Pin_2) != RESET)
	{
		if(sx670_enable)
		{
//			delay_ms(1);
			if(sensor1==1)	 //����KEY0
			{
				printf("1_OUT\r\n");
				LED0=1;
				event_establish(EVENT_SENER1_OUT);
			}		 
			else 	 //����KEY0
			{
				printf("1_IN\r\n");
				LED0=0;
				event_establish(EVENT_SENER1_IN);
			}	
		
		}
	
		EXTI_ClearFlag(EXTI_Line2);
		EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE3�ϵ��жϱ�־λ  
	}
}
 

//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(GPIO_Pin_3) != RESET)
	{
		if(sx670_enable)
		{
//			delay_ms(1);
			if(sensor2==1)	 //����KEY0
			{
				printf("2_OUT\r\n");
				LED0=1;
				event_establish(EVENT_SENER2_OUT);
			}		 
			else	 //����KEY0
			{
				printf("2_IN\r\n");
				LED0=0;
				event_establish(EVENT_SENER2_IN);
			}
		}		

		EXTI_ClearFlag(EXTI_Line3);
		EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
	}
	
}

void EXTI4_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(GPIO_Pin_4) != RESET)
	{
		if(sx670_enable)
		{
//			delay_ms(1);
			if(sensor3==1)	 //����KEY0
			{
				printf("4_OUT\r\n");
				LED0=1;
				event_establish(EVENT_SENER3_OUT);
			}		 
			else
			{
				printf("4_IN\r\n");
				LED0=0;
				event_establish(EVENT_SENER3_IN);
			}
		}
		EXTI_ClearFlag(EXTI_Line4);
		EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE3�ϵ��жϱ�־λ  
	}
	
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(GPIO_Pin_5) != RESET)
	{
		if(sx670_enable)
		{
//			delay_ms(1);
			if(sensor4==1)	 //����KEY0
			{
				printf("3_OUT\r\n");
				LED0=1;
				event_establish(EVENT_SENER4_OUT);
			}		 
			else
			{
				printf("3_IN\r\n");
				LED0=0;
				event_establish(EVENT_SENER4_IN);
			} 		
		}
		EXTI_ClearFlag(EXTI_Line5);
		EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE3�ϵ��жϱ�־λ  
	
	}		
}


