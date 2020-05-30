#include "Bsl_ScreenMgr.h"
#include "timer.h"
#include "string.h"
#include "event_queue.h"
#include "lcd.h"


#define FLUSH_PERIOD 100													/**< 屏幕点亮周期 */
#define MESSAGE_TIP_UPDATE_TIME 50											/**< 消息提示更新时间 */
#define SCREEN_LIST_CNT 20													/**< 屏幕列表个数统计 */
//APP_TIMER_DEF(m_drawScreenTime);											/**< 画屏幕定时器 */
//APP_TIMER_DEF(m_wakeup_screen_timer);										/**< 唤醒屏幕定时器 */
//APP_TIMER_DEF(m_tip_message_timer);											/**< 信息提示定时器 */

S_SCREEN screen_list[SCREEN_LIST_CNT];										/**< 屏幕显示列表结构体数组 */
void (*last_paint)(uint16_t,void *p) = 0;									/**< 上次画屏幕回调函数 */
const unsigned char flush_period = FLUSH_PERIOD;							/**< 点亮周期 */
uint8_t is_screen_show = 0;													/**< 是否在显示标志位 */
uint8_t want_screen_mgr_work = 0;											/**< 需要显示管理工作标志位 */

static void Bsl_WakeupScreenTimeout(void *p_context);						/**< 唤醒屏幕 */
static event_type_t Bsl_ScreenMgrEventHandle(event_type_t event);	/**< 处理显示事件 */

/**@brief 屏幕协调循环
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
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

/**@brief 屏幕显示列表数组中寻找空值
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
 * @return new_screen 屏幕显示列表指针
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
/**@brief 获取列表中与填入显示函数对应的地址
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
 * @return &screen_list[i] 对应显示函数的地址 
 * @retval 0 获取失败
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

/**@brief 定时消失的弹窗（无法按键关闭）
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
 * @param[in] tip_time 提示时间
 * @param[in] pri 屏幕重复间隔
 */
void Bsl_TipMessage(void (*paint)(uint16_t,void *),void *p,unsigned short tip_time,SCREEN_PRI pri)
{	
	S_SCREEN* screen = Bsl_FindEmptyScreen(paint);
	if(screen == 0)///<当前有内容显示，则不弹屏
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

/**@brief 创建屏幕
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
 * @param[in] type 屏幕类型
 * @param[in] pri 屏幕重复间隔
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

/**@brief 显示屏幕
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
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

/**@brief 关闭屏幕
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
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

///**@brief 弹窗超时
// *
// * @param[in] *p_context 备用指针，当前无意义
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
//	//停止
//	if(vaild_message_cnt == 0)
////		app_timer_stop(m_tip_message_timer);
//	if(invaild_message_cnt)
//		Bsl_ReSortScreen(0);
//	Bsl_FlushScreen(NULL);
//}


/**@brief 初始化屏幕管理
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

/**@brief 点亮屏幕
 *
 * @param[in] *p_context 备用指针，当前无意义
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

/**@brief 填入屏幕是否在显示
 *
 * @param[in] (*paint)(uint16_t,void *)  
 *				*paint 函数指针
 *				uint16_t 函数指针的入口参数，当前用于底部滑动文字
 * @return 显示状态
 *
 * @retval 1 正在显示
 * @retval 0 未显示
 */
unsigned char Bsl_IsScreenShowing(void (*paint)(uint16_t,void *))
{
	if(paint != 0 && screen_list[0].paint == paint  && screen_list[0].show_time != 0)
		return 1;
	else
		return 0;
}
/**@brief 主屏幕是否在显示
 *
 * @return 显示状态
 *
 * @retval 1 正在显示
 * @retval 0 未显示
 */
unsigned char Bsl_IsShowingMainScreen(void)
{
	if(screen_list[0].paint != 0 && screen_list[0].show_time != 0 && screen_list[0].type == SCREEN_TYPE_MAIN)
		return 1;
	else
		return 0;
}


/**@brief 唤醒屏幕一段时间
 *
 * @param[in] time 唤醒时间，单位s
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

/**@brief 关闭屏幕
 */
void Bsl_TurnOffScreen(void)
{
	Bsl_WakeupScreenTimeout(0);
	want_screen_mgr_work = 0;
}

/**@brief 打开屏幕
 */
void Bsl_TurnOnScreen(void)
{
	want_screen_mgr_work = 1;
}

/**@brief 唤醒屏幕超时
 *
 * @param[in] *p_context 备用指针，当前无意义
 */
static void Bsl_WakeupScreenTimeout(void *p_context)
{	
//	app_timer_stop(m_wakeup_screen_timer);
//	app_timer_stop(m_drawScreenTime);
//	Hdl_DisableSH1107();
	is_screen_show = 0;
	event_establish(EVENT_KEY0_PRESSED);
}

/**@brief 处理显示事件
 *
 * @param[in] event 事件
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
//			else if(screen_list[0].paint && screen_list[0].show_time != 0 && screen_list[0].type == SCREEN_TYPE_DIALOG)//点击退出弹屏
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


/**@brief 获得显示状态
 *
 * @return 是否正在显示
 *
 * @retval 1是 0否
 */
uint8_t bsl_get_screen_show(void)
{
	return is_screen_show;
}

/**@brief 熄灭屏幕
 */
void bsl_turn_off_screen(void)
{
	Bsl_WakeupScreenTimeout(0);
}
