#include "task_modbus.h"

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
extern QueueHandle_t RS485_buffer_Queue; //rs485��Ϣ���о��



typedef struct RS485buff
{
u32 RS485_lenth;
u8 RS485_buffer[256];
}RS485buff;

//modbus_RTU����
void modbus_task(void *p_arg)
{
	
	/**********485�������ݻ���ṹ��**********/
	RS485buff *RS485receive;
		/***************�ź�������************/
	BaseType_t err=pdFALSE;
	
	
	
	
	
	
	
	//��������
	u8 test[]="485Ӧ��";
		while(1)
		{
	

			err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//��ȡ����2�����ж϶�ֵ�ź���
				if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
			
				printf("����2DMA�յ�����");
			  xQueuePeek(RS485_buffer_Queue,(void *)&RS485receive,portMAX_DELAY);
				printf("RS485:%s",RS485receive->RS485_buffer);
				
				RS485_send(test,sizeof(test));
				
				
			
			}
			
			
			
			vTaskDelay(500);
		}
		
		
}
