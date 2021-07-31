#include "delay.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ��������ԭ��ս�� STM32������
//��˾������ԭ�ӹ����ҽ��ܺ���,QDtechȫϵ��Һ��ģ����ȫ��������ԭ��ȫϵ�п����壬ֱ��ֱ�ã��ǳ�����
//3.5��TFTҺ������
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtech.net
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:QDtech2008@gmail.com 
//Skype:QDtech2008
//��������:2013/5/13
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.1�޸�˵��
//���������ʾ���룬ֱ���ڳ�ʼ�����л�������ģʽ
////////////////////////////////////////////////////////////////////////////////// 
//static u8  fac_us=0;//us��ʱ������
//static u16 fac_ms=0;//ms��ʱ������
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
//systick�жϷ�����,ʹ��ucosʱ�õ�
void SysTick_Handler(void)
{				   
	OSIntEnter();		//�����ж�
    OSTimeTick();       //����ucos��ʱ�ӷ������               
    OSIntExit();        //���������л����ж�
}
#endif


#define EACH_PER_MS    1   /*!<ÿ�� 25 ms �ж�һ��  systick ��ʱ����24λ���¼����Ķ�ʱ��  ���װ��ֵ16777215 */ 

/** 
  * @brief һ��ʱ��ṹ��
  * @note  �ڲ�����
  */
struct time{
	
    uint32_t fac_us;                  /*!<us��Ƶϵ��         */
	uint32_t fac_ms;                  /*!<ms��Ƶϵ��         */
	volatile uint32_t millisecond;    /*!<��ǰmsֵ           */
	uint8_t ms_per_tick;              /*!<ÿ������ms�ж�һ�� */
	
}timer;



//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(void)	 
{

#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//ѡ���ⲿʱ��  HCLK/8
	timer.fac_us=SystemCoreClock/1000000;	//Ϊϵͳʱ�ӵ�1/8  
	 
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	reload=SystemCoreClock/8000000;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/OS_TICKS_PER_SEC;//����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
#else
	timer.fac_ms=(u16)timer.fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
#endif
	
	timer.ms_per_tick = EACH_PER_MS;
    timer.millisecond = 0;
	
	SysTick_Config((SystemCoreClock / 1000) * timer.ms_per_tick );   //����systick�ж�
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,0));
	
	
}								    

#ifdef OS_CRITICAL_METHOD	//ʹ����ucos
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
	tcnt=0;
	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}; 									    
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	
	if(OSRunning==TRUE)//���os�Ѿ�������	    
	{		  
		if(nms>=fac_ms)//��ʱ��ʱ�����ucos������ʱ������ 
		{
   			OSTimeDly(nms/fac_ms);//ucos��ʱ
		}
		nms%=fac_ms;				//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));	//��ͨ��ʽ��ʱ,��ʱucos�޷���������.
}
#else//����ucosʱ
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
//	u32 temp;	    	 
//	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
//	SysTick->VAL=0x00;        //��ռ�����
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
//	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
//	SysTick->VAL =0x00;           //��ռ�����
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
//	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
#endif




/////////////////////////////////////////










void systime_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	timer.fac_us = SystemCoreClock / 1000000;
	timer.fac_ms = timer.fac_us*1000;
	timer.ms_per_tick = EACH_PER_MS;
    timer.millisecond = 0;
	SysTick_Config(timer.fac_ms * timer.ms_per_tick );   //����systick�ж�
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,0));
}

extern uint8_t open_exti_flag;
extern uint16_t open_exti_delay_ms;


void SysTick_Handler(void)
{
	timer.millisecond += timer.ms_per_tick;
	
	if(open_exti_flag==DISABLE)
	open_exti_delay_ms--;
	if(open_exti_delay_ms==0)
	{
		open_exti_flag=ENABLE;
		open_exti_delay_ms=0xff;
	}

	
}


uint32_t systime_get_current_time_ms(void)
{
    register uint32_t val=0, ms=0;
    do
    {
        ms  = timer.millisecond;
        val = SysTick->VAL; 
    }while(ms != timer.millisecond);
    
	return ms  -  val/timer.fac_ms;
}



uint32_t systime_get_current_time_us(void)
{
    register uint32_t val=0, ms=0;
    do
    {
        ms  = timer.millisecond;
        val = SysTick->VAL;
    }while(ms != timer.millisecond);
	return (uint32_t)((uint32_t)(ms * 1000) -  val / timer.fac_us);
}


void systime_delay_us(uint32_t us)     
{
    uint32_t now = systime.get_time_us();
	uint32_t end_time = now + us - 3;
	while( systime.get_time_us() <= end_time)
    {
        ;
    }
}




void systime_delay_ms(uint32_t ms) 
{
    while(ms--)
    {
        systime.delay_us(1000);
    }
	
}

systime_t  systime = 
{
	systime_init,
	systime_get_current_time_us,
	systime_get_current_time_ms,
	systime_delay_us,
	systime_delay_ms
};











