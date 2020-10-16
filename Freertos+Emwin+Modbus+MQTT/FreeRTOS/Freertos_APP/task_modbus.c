#include "task_modbus.h"

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
extern QueueHandle_t RS485_buffer_Queue; //rs485消息队列句柄



typedef struct RS485buff
{
u32 RS485_lenth;
u8 RS485_buffer[256];
}RS485buff;

//modbus_RTU任务
void modbus_task(void *p_arg)
{
	
	/**********485总线数据缓冲结构体**********/
	RS485buff *RS485receive;
		/***************信号量参数************/
	BaseType_t err=pdFALSE;
	
	
	
	
	
	
	
	//测试数组
	u8 test[]="485应答";
		while(1)
		{
	

			err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//获取串口2空闲中断二值信号量
				if(err==pdTRUE)										//获取信号量成功
			{
			
				printf("串口2DMA收到数据");
			  xQueuePeek(RS485_buffer_Queue,(void *)&RS485receive,portMAX_DELAY);
				printf("RS485:%s",RS485receive->RS485_buffer);
				
				RS485_send(test,sizeof(test));
				
				
			
			}
			
			
			
			vTaskDelay(500);
		}
		
		
}
