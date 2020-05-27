#ifndef __EVENT_QUEUE_H
#define __EVENT_QUEUE_H	 
#include "sys.h"

typedef enum
{
	EVENT_KEY_PRESSED				= 0x00000001,
	EVENT_KEY_LONGPRESSED			= 0x00000002,
	EVENT_PWR_EMPTY					= 0x00000004,
	EVENT_PWR_SHORTAGE				= 0x00000008,
	EVENT_PWR_NORMAL				= 0x00000010,
	EVENT_CAR_FRONT					= 0x00000020,
	EVENT_CAR_TURN_RIGHT			= 0x00000040,
	EVENT_CAR_TURN_LIEFT			= 0x00000080,
	EVENT_CAR_BACK					= 0x00000100,
	usarterr						= 0x00000200,
	
}event_type_t;


typedef event_type_t (*event_handle)(event_type_t event);

void Init_event_queue(void);
void resgister_event_handle(event_handle handle,int	interest_event_mask);
void event_establish(event_type_t event);

		 				    
#endif
