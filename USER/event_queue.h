#ifndef __EVENT_QUEUE_H
#define __EVENT_QUEUE_H	 
#include "sys.h"

typedef enum
{
	EVENT_KEY0_PRESSED				= 0x00000001,
	EVENT_KEY1_PRESSED				= 0x00000002,
	EVENT_KEY_UP_PRESSED			= 0x00000004,
	EVENT_TUOCH_PRESSED				= 0x00000008,
	EVENT_TUOCH_OK					= 0x00000010,
	EVENT_TUOCH_EXIT				= 0x00000020,
	EVENT_TUOCH_BACK				= 0x00000040,
	EVENT_TUOCH_START				= 0x00000080,
	EVENT_TUOCH_PAUSE				= 0x00000100,
	EVENT_TUOCH_CLERA				= 0x00000200,
	EVENT_TUOCH_TIP1				= 0x00000400,
	EVENT_TUOCH_TIP2				= 0x00000800,
	EVENT_TUOCH_LAB1				= 0x00001000,
	EVENT_TUOCH_LAB2				= 0x00002000,
	EVENT_TUOCH_LAB3				= 0x00004000,
	EVENT_TUOCH_LAB4				= 0x00008000,
	EVENT_TUOCH_LAB5				= 0x00010000,
	EVENT_TUOCH_LAB6				= 0x00020000,
	EVENT_TUOCH_SETING				= 0x00040000,
	EVENT_TUOCH_RES					= 0x00080000,
	
}event_type_t;


typedef event_type_t (*event_handle)(event_type_t event);

void Init_event_queue(void);
void resgister_event_handle(event_handle handle,int	interest_event_mask);
void event_establish(event_type_t event);

		 				    
#endif
