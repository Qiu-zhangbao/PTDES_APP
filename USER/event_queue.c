#include "event_queue.h"
#include "string.h"

#define EVENT_HANDLE_LIST_CNT	50

uint8_t Event_HandlerCnt;

typedef struct
{
	event_handle Event_Handle;		//函数指针
	int	Interest_Event_Mask;		//触发事件的掩码
	
}EVENT_HANDLE_ITEM;//事件句柄结构体

EVENT_HANDLE_ITEM Event_HandlerList[EVENT_HANDLE_LIST_CNT];

void Init_event_queue(void)
{
	memset(Event_HandlerList,0,sizeof(Event_HandlerList));
	printf("event queue init\r\n");
}

void resgister_event_handle(event_handle handle,int	interest_event_mask)
{
	for(uint8_t i=0;i<EVENT_HANDLE_LIST_CNT;i++)
	{
		if(Event_HandlerList[i].Event_Handle == 0 )
		{
			Event_HandlerList[i].Event_Handle = handle;
			Event_HandlerList[i].Interest_Event_Mask = interest_event_mask;
			Event_HandlerCnt++;
			return;
		}
	}
}

void event_establish(event_type_t event)
{
	for(int i=0;i<Event_HandlerCnt;i++)
	{
		if(Event_HandlerList[i].Interest_Event_Mask & event)
		{
			event=Event_HandlerList[i].Event_Handle(event);
			if(event == 0)
				return;
		}
	}
}


