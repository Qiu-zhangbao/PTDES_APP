#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

typedef enum
{
	main_page=0,
	lab1,
	lab2,
	lab3,
	lab4,
	lab5,
	lab6,
	close,
	seting,
	exiting,

}lab_list_t;
 
extern lab_list_t page_state_now;

void main_page_init(void);

void Control_Init(void);

void main_control(void);

void Control_state_machine(lab_list_t state);

void Control_touch(void);

#endif

