#include "task_emwin.h"


//EMWINDEMO任务
void emwin_task(void *pvParameters)
{
	
	MainTask(); 
	
}

//触摸任务的任务函数
void touch_task(void *pvParameters)
{
	while(1)
	{
		GUI_TOUCH_Exec();	
		vTaskDelay(5);		//延时5ms
	}
}



