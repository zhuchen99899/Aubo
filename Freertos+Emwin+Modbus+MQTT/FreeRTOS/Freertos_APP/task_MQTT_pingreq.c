#include "task_MQTT_Pingreq.h"




/************Freertos软件定时器句柄********************/
//周期性软件定时器


/******************消息队列句柄********************/
	extern QueueHandle_t PINGREQ_Queue;
/********************WIFI发送PINGREQ任务*****************/

	
void MQTT_Pingreq_task(void *pvParameters)
{
u8 PINGREQ_FLAG=0;
u8 temp_buff[256];
int len;




	while(1)
	{	
	 xQueuePeek(PINGREQ_Queue,&PINGREQ_FLAG,portMAX_DELAY);
	if (PINGREQ_FLAG)
		{
			
			printf("WIFI正在发送PINGREQ报文\r\n");
			len=MQTT_pingreq(temp_buff);
			WIFI_send(temp_buff,len);
			
		/*******打印wifi发送*******/
		

		printf("发送PINGREQ报文完成\r\n");
		
		

		
		}

      vTaskDelay(50000);                           //延时50s
	}//while
};// pingreq_task


