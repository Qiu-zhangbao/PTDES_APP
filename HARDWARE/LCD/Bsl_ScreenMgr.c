#include "Bsl_ScreenMgr.h"
#include "timer.h"
#include "string.h"
#include "event_queue.h"
#include "lcd.h"


#define FLUSH_PERIOD 100													/**< ��Ļ�������� */
#define MESSAGE_TIP_UPDATE_TIME 50											/**< ��Ϣ��ʾ����ʱ�� */
#define SCREEN_LIST_CNT 20													/**< ��Ļ�б����ͳ�� */
//APP_TIMER_DEF(m_drawScreenTime);											/**< ����Ļ��ʱ�� */
//APP_TIMER_DEF(m_wakeup_screen_timer);										/**< ������Ļ��ʱ�� */
//APP_TIMER_DEF(m_tip_message_timer);											/**< ��Ϣ��ʾ��ʱ�� */

S_SCREEN screen_list[SCREEN_LIST_CNT];										/**< ��Ļ��ʾ�б�ṹ������ */
void (*last_paint)(uint16_t,void *p) = 0;									/**< �ϴλ���Ļ�ص����� */
const unsigned char flush_period = FLUSH_PERIOD;							/**< �������� */
uint8_t is_screen_show = 0;													/**< �Ƿ�����ʾ��־λ */
uint8_t want_screen_mgr_work = 0;											/**< ��Ҫ��ʾ��������־λ */

static void Bsl_WakeupScreenTimeout(void *p_context);						/**< ������Ļ */
static event_type_t Bsl_ScreenMgrEventHandle(event_type_t event);	/**< ������ʾ�¼� */

/**@brief ��ĻЭ��ѭ��
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 */
static void Bsl_ReSortScreen(void (*paint)(uint16_t,void *))
{
	for(int i=0;i<SCREEN_LIST_CNT;++i)
	{
		S_SCREEN* screen_a = &screen_list[i];
		for(int j = i+1;j<SCREEN_LIST_CNT;++j)
		{
			S_SCREEN* screen_b = &screen_list[j];
			
			if(screen_a->paint == 0 && screen_b->paint != 0)
			{
				S_SCREEN temp;
				memcpy(&temp,screen_a,sizeof(S_SCREEN));
				memcpy(screen_a,screen_b,sizeof(S_SCREEN));
				memcpy(screen_b,&temp,sizeof(S_SCREEN));
			}
			else if(screen_a->paint != 0 && screen_b->paint != 0)
			{
				if(screen_a->show_time == 0 && screen_b->show_time != 0)
				{
					S_SCREEN temp;
					memcpy(&temp,screen_a,sizeof(S_SCREEN));
					memcpy(screen_a,screen_b,sizeof(S_SCREEN));
					memcpy(screen_b,&temp,sizeof(S_SCREEN));
				}
				else if(screen_a->show_time != 0 && screen_b->show_time != 0)
				{
					if(screen_a->type < screen_b->type)
					{
						S_SCREEN temp;
						memcpy(&temp,screen_a,sizeof(S_SCREEN));
						memcpy(screen_a,screen_b,sizeof(S_SCREEN));
						memcpy(screen_b,&temp,sizeof(S_SCREEN));
					}
					else if(screen_a->type == screen_b->type)
					{
						if(screen_a->pri < screen_a->pri)
						{
							S_SCREEN temp;
							memcpy(&temp,screen_a,sizeof(S_SCREEN));
							memcpy(screen_a,screen_b,sizeof(S_SCREEN));
							memcpy(screen_b,&temp,sizeof(S_SCREEN));
						}
						else if(screen_a->pri == screen_a->pri)
						{
							if(screen_b->paint == paint)
							{
								S_SCREEN temp;
								memcpy(&temp,screen_a,sizeof(S_SCREEN));
								memcpy(screen_a,screen_b,sizeof(S_SCREEN));
								memcpy(screen_b,&temp,sizeof(S_SCREEN));
							}
						}
					}
				}
			}
		}
	}
}

/**@brief ��Ļ��ʾ�б�������Ѱ�ҿ�ֵ
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 * @return new_screen ��Ļ��ʾ�б�ָ��
 */
S_SCREEN* Bsl_FindEmptyScreen(void (*paint)(uint16_t,void *))
{
	S_SCREEN* new_screen = 0;
	for(int i=0;i<SCREEN_LIST_CNT;++i)
	{
		if(screen_list[i].paint == paint)
		{
			return &screen_list[i];
		}
		else if(new_screen == 0 && screen_list[i].paint == 0)
		{	
			new_screen = &screen_list[i];
		}
	}
	return new_screen;
}
/**@brief ��ȡ�б�����������ʾ������Ӧ�ĵ�ַ
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 * @return &screen_list[i] ��Ӧ��ʾ�����ĵ�ַ 
 * @retval 0 ��ȡʧ��
 */
S_SCREEN* Bsl_GetScreen(void (*paint)(uint16_t,void *))
{
	for(int i=0;i<SCREEN_LIST_CNT;++i)
	{
		if(screen_list[i].paint == paint)
		{
			return &screen_list[i];
		}
	}
	return 0;
}

/**@brief ��ʱ��ʧ�ĵ������޷������رգ�
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 * @param[in] tip_time ��ʾʱ��
 * @param[in] pri ��Ļ�ظ����
 */
void Bsl_TipMessage(void (*paint)(uint16_t,void *),void *p,unsigned short tip_time,SCREEN_PRI pri)
{	
	S_SCREEN* screen = Bsl_FindEmptyScreen(paint);
	if(screen == 0)///<��ǰ��������ʾ���򲻵���
		return;
	
//	app_timer_stop(m_tip_message_timer);
	
	screen->paint = paint;
	screen->paint_p = p;
	screen->type = SCREEN_TYPE_MSSAGE;
	screen->pri = pri;
	screen->show_time = tip_time+MESSAGE_TIP_UPDATE_TIME;
	
//	app_timer_start(m_tip_message_timer,APP_TIMER_TICKS(MESSAGE_TIP_UPDATE_TIME),NULL);
	
	Bsl_ReSortScreen(paint);
	Bsl_FlushScreen(NULL);
}

/**@brief ������Ļ
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 * @param[in] type ��Ļ����
 * @param[in] pri ��Ļ�ظ����
 */
void Bsl_CreateScreen(void (*paint)(uint16_t,void *),SCREEN_TYPE type,SCREEN_PRI pri)
{
	S_SCREEN* screen = Bsl_FindEmptyScreen(paint);
	if(screen == 0)
		return;
	
	screen->paint = paint;
	screen->paint_p = 0;
	screen->type = type;
	screen->pri = pri;
	screen->show_time = 0;
}

/**@brief ��ʾ��Ļ
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 */
void Bsl_ShowScreen(void (*paint)(uint16_t,void *),void *p)
{
	S_SCREEN* screen = Bsl_GetScreen(paint);
	if(screen == 0)
		return;
	
	screen->paint_p = p;
	screen->show_time = 0xFFFF;
	Bsl_ReSortScreen(paint);
//	for(uint8_t i=0;i<100;i++)
//	{
//	Bsl_FlushScreen(NULL);
//	
//	}
	
}

/**@brief �ر���Ļ
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 */
void Bsl_CloseScreen(void (*paint)(uint16_t,void *))
{
	S_SCREEN* screen = Bsl_GetScreen(paint);
	if(screen == 0)
		return;
	
	screen->show_time = 0;
	Bsl_ReSortScreen(paint);
	Bsl_FlushScreen(NULL);
}

///**@brief ������ʱ
// *
// * @param[in] *p_context ����ָ�룬��ǰ������
// */
//static void Bsl_TipMessageTimeout(void *p_context)
//{
//	unsigned char vaild_message_cnt = 0;
//	unsigned char invaild_message_cnt = 0;
//	for(int i= 0;i<SCREEN_LIST_CNT;++i)
//	{
//		if(screen_list[i].type == SCREEN_TYPE_MSSAGE)
//		{
//			screen_list[i].show_time -= MESSAGE_TIP_UPDATE_TIME;
//			if(screen_list[i].show_time<MESSAGE_TIP_UPDATE_TIME)
//			{
//				memset(&screen_list[i],0,sizeof(screen_list[i]));
//				invaild_message_cnt ++;
//			}
//			else
//				vaild_message_cnt ++;
//		}
//	}
//	//ֹͣ
//	if(vaild_message_cnt == 0)
////		app_timer_stop(m_tip_message_timer);
//	if(invaild_message_cnt)
//		Bsl_ReSortScreen(0);
//	Bsl_FlushScreen(NULL);
//}


/**@brief ��ʼ����Ļ����
 */
void Bsl_InitScreenMgr(void)
{

//	err_code = app_timer_create(&m_drawScreenTime, APP_TIMER_MODE_REPEATED, Bsl_FlushScreen);
//	APP_ERROR_CHECK(err_code);
//	
//	err_code = app_timer_create(&m_wakeup_screen_timer,APP_TIMER_MODE_SINGLE_SHOT, Bsl_WakeupScreenTimeout);
//	APP_ERROR_CHECK(err_code);
//	
//	err_code = app_timer_create(&m_tip_message_timer,APP_TIMER_MODE_REPEATED, Bsl_TipMessageTimeout);
//	APP_ERROR_CHECK(err_code);
	
	resgister_event_handle(Bsl_ScreenMgrEventHandle,	EVENT_KEY0_PRESSED);
}

/**@brief ������Ļ
 *
 * @param[in] *p_context ����ָ�룬��ǰ������
 */
void Bsl_FlushScreen(void* p_context)
{
//	LCD_Clear(WHITE);
	if(screen_list[0].paint /*&& screen_list[0].show_time != 0*/)
	{
		if(p_context)
			screen_list[0].paint(*(uint16_t*)p_context,screen_list[0].paint_p);
		else
			screen_list[0].paint(0,screen_list[0].paint_p);
	}
//	Hdl_FlushScreen();
}

/**@brief ������Ļ�Ƿ�����ʾ
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint ����ָ��
 *				uint16_t ����ָ�����ڲ�������ǰ���ڵײ���������
 * @return ��ʾ״̬
 *
 * @retval 1 ������ʾ
 * @retval 0 δ��ʾ
 */
unsigned char Bsl_IsScreenShowing(void (*paint)(uint16_t,void *))
{
	if(paint != 0 && screen_list[0].paint == paint  && screen_list[0].show_time != 0)
		return 1;
	else
		return 0;
}
/**@brief ����Ļ�Ƿ�����ʾ
 *
 * @return ��ʾ״̬
 *
 * @retval 1 ������ʾ
 * @retval 0 δ��ʾ
 */
unsigned char Bsl_IsShowingMainScreen(void)
{
	if(screen_list[0].paint != 0 && screen_list[0].show_time != 0 && screen_list[0].type == SCREEN_TYPE_MAIN)
		return 1;
	else
		return 0;
}


/**@brief ������Ļһ��ʱ��
 *
 * @param[in] time ����ʱ�䣬��λs
 */
void Bsl_WakeupScreen(uint16_t time)
{
	if(want_screen_mgr_work == 0)
		return;
	LCD_LED=1;
	
//	Hdl_ReInitSH1107();
//	
//	
//	app_timer_stop(m_wakeup_screen_timer);
//	err_code = app_timer_start(m_wakeup_screen_timer, APP_TIMER_TICKS(time*1000), NULL);
//	APP_ERROR_CHECK(err_code);
//	
//	err_code = app_timer_start(m_drawScreenTime, APP_TIMER_TICKS(FLUSH_PERIOD), (void*)&flush_period);
//	APP_ERROR_CHECK(err_code);	
//	if(is_screen_show == 0)
//	{
//		is_screen_show = 1;
//		event_establish(EVENT_SCREEN_WAKEUPED);
//	}
}

/**@brief �ر���Ļ
 */
void Bsl_TurnOffScreen(void)
{
	Bsl_WakeupScreenTimeout(0);
	want_screen_mgr_work = 0;
}

/**@brief ����Ļ
 */
void Bsl_TurnOnScreen(void)
{
	want_screen_mgr_work = 1;
}

/**@brief ������Ļ��ʱ
 *
 * @param[in] *p_context ����ָ�룬��ǰ������
 */
static void Bsl_WakeupScreenTimeout(void *p_context)
{	
//	app_timer_stop(m_wakeup_screen_timer);
//	app_timer_stop(m_drawScreenTime);
//	Hdl_DisableSH1107();
	is_screen_show = 0;
	event_establish(EVENT_KEY0_PRESSED);
}

/**@brief ������ʾ�¼�
 *
 * @param[in] event �¼�
 */
static event_type_t Bsl_ScreenMgrEventHandle(event_type_t event)
{
//	if(want_screen_mgr_work == 0)
//		return event;
//	switch(event)
//	{
//		case EVENT_KEY0_PRESSED:
//			if(is_screen_show == 0)
//			{
//				#ifdef ENABLE_IQS211
//				Hdl_DiscardKeyThisTime();
//				#endif
//			}
//			else if(screen_list[0].paint && screen_list[0].show_time != 0 && screen_list[0].type == SCREEN_TYPE_DIALOG)//����˳�����
//			{
//				Bsl_CloseScreen(screen_list[0].paint);
//				#ifdef ENABLE_IQS211
//				Hdl_DiscardKeyThisTime();
//				#endif
//			}
//			//fun_close_sendentary();
//			Bsl_WakeupScreen(10);
//			break;
//		default:
//			break;
//	}
	return event;
}


/**@brief �����ʾ״̬
 *
 * @return �Ƿ�������ʾ
 *
 * @retval 1�� 0��
 */
uint8_t bsl_get_screen_show(void)
{
	return is_screen_show;
}

/**@brief Ϩ����Ļ
 */
void bsl_turn_off_screen(void)
{
	Bsl_WakeupScreenTimeout(0);
}
