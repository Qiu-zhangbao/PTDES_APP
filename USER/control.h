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
<<<<<<< Updated upstream
	main_page

}lab_list_t;
 


void main_page_init(void);

=======
	close,
}page_list_t;
 
>>>>>>> Stashed changes

void Control_Init(void);

void main_control(void);

#endif

