#ifndef __TASK_EMWIN_H
#define __TASK_EMWIN_H	 
#include "freertos.h"
#include "task.h"
#include "iconvdesk.h"

#include "DIALOG.h"

void touch_task(void *pvParameters);
void emwin_task(void *pvParameters);



#endif
