#include "includes.h"
/***********************************************************
						��������
************************************************************/





/********************************��Ϣ���о������*********************************/
QueueHandle_t Key_Queue;
QueueHandle_t RS485_buffer_Queue;
QueueHandle_t Wifi_buffer_Queue;
QueueHandle_t PINGREQ_Queue;
/********************************�ź������*************************************/
SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
SemaphoreHandle_t BinarySemaphore_USART3ISR;  //USART3�����ж϶�ֵ�ź������
SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������
SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
/***********************************************************
					����������
************************************************************/


void start_task(void *pvParameters);		//Start����
void touch_task(void *pvParameters);		//TOUCH����
void UserIf_task(void *pvParameters);		//��������
void emwin_task(void *pvParameters); 	//emwin����
void led_task(void *pvParameters);       //led����
void key_task(void *pvParameters);       //key����
void modbus_task(void *pvParameters);       //modbus_RTU����
void MQTT_Connect_task(void *pvParameters);       // MQTTconnect����
void MQTT_rec_task(void *pvParameters);				// MQTT��������
void MQTT_Pingreq_task(void *pvParameters);				// MQTTPINGREQ����
void MQTT_Subscribe_task(void *pvParameters);				// MQTTSUBSCRIBE����
/***********************************************************
						���������
************************************************************/
//���������
TaskHandle_t StartTask_Handler;			//Start����
TaskHandle_t TouchTask_Handler;			//TOUCH����
TaskHandle_t UserIfTask_Handler;		//��������
TaskHandle_t EmwinTask_Handler;		//emwin����
TaskHandle_t LedTask_Handler;		//led����
TaskHandle_t KeyTask_Handler;		//Key����
TaskHandle_t ModbusTask_Handler;		//modbus_RTU����
TaskHandle_t MQTT_Connect_task_Handler;		  // MQTTconnect����
TaskHandle_t MQTT_Rec_task_Handler;		  // MQTT��������
TaskHandle_t MQTT_PINGREQ_task_Handler;		  // MQTTPINGREQ����
TaskHandle_t MQTT_SUBSCRIBE_task_Handler;				// MQTTSUBSCRIBE����

/***********************************************************
						�����ջ��С
************************************************************/
#define START_STK_SIZE 			256  	//Start����
#define TOUCH_STK_SIZE			128		//TOUCH����
#define USERIF_STK_SIZE			128		//��������
#define EMWIN_STK_SIZE		512		//emwin����
#define LED_STK_SIZE     128    //LED����
#define KEY_STK_SIZE     128    //KEY����
#define MODBUS_STK_SIZE     512    //modbus_RTU����
#define MQTT_Connect_STK_SIZE		256	// MQTTconnect����
#define MQTT_Rec_STK_SIZE		512	// MQTT��������
#define MQTT_PINGREQ_STK_SIZE		256	// MQTTPINGREQ����
#define MQTT_SUBSCRIBE_STK_SIZE		256		// MQTTSUBSCRIBE����
/***********************************************************
						 �������ȼ�(��ֵԽС���ȼ�Խ��)
************************************************************/
#define START_TASK_PRIO			5		//Start����
#define TOUCH_TASK_PRIO			0		//TOUCH����
#define USERIF_TASK_PRIO 		0	   //��������
#define EMWIN_TASK_PRIO		0		//emwin����
#define LED_TASK_PRIO        1    //led����
#define KEY_TASK_PRIO        1    //Key����
#define MODBUS_TASK_PRIO        2    //modbus_RTU����
#define MQTT_Connect_TASK_PRIO		3		// MQTTCONNCET����
#define MQTT_Rec_TASK_PRIO		2		// MQTT��������
#define MQTT_PINGREQ_TASK_PRIO		2		// MQTTPINGREQ����
#define MQTT_SUBSCRIBE_TASK_PRIO		3		// MQTTSUBSCRIBE����
/***********************************************************
						 ���������
************************************************************/

int main(void)
{
	
/*
 ����������ǰ��Ϊ�˷�ֹ��ʼ�� STM32 ����ʱ���жϷ������ִ�У������ֹȫ���ж�(���� NMI �� HardFault)��
 �������ĺô��ǣ�
 1. ��ִֹ�е��жϷ���������� FreeRTOS �� API ������
 2. ��֤ϵͳ�������������ܱ���ж�Ӱ�졣
 3. �����Ƿ�ر�ȫ���жϣ���Ҹ����Լ���ʵ��������ü��ɡ�
 ����ֲ�ļ� port.c �еĺ��� prvStartFirstTask �л����¿���ȫ���жϡ�ͨ��ָ�� cpsie i ������__set_PRIMASK(1)
 �� cpsie i �ǵ�Ч�ġ�
 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	uart_init(115200);					//��ʼ������1 :���Դ���
	RS485_init(115200);					//��ʼ������2 ��485����
	wifi_init(115200);					//��ʼ������3:  wifi����
	LED_Init();		  					//��ʼ��LED
  bsp_InitKey();	/* ��ʼ������ */
	TFTLCD_Init();						//LCD��ʼ��	
	FSMC_SRAM_Init();					//��ʼ��SRAM
	TP_Init();							//��������ʼ��
	TIM3_Int_Init(1799,0);
	my_mem_init(SRAMIN);            	//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);				//��ʼ���ⲿ�ڴ��
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������                
    vTaskStartScheduler();          //�����������
/*
 ���ϵͳ���������ǲ������е�����ģ����е����Ｋ�п��������ڶ�ʱ��������߿��������
 heap �ռ䲻����ɴ���ʧ�ܣ���Ҫ�Ӵ� FreeRTOSConfig.h �ļ��ж���� heap ��С��
 #define configTOTAL_HEAP_SIZE ( ( size_t ) ( 17 * 1024 ) )
*/
	while(1);			
}

/***********************************************************
						 ������
************************************************************/

//��ʼ����������
void start_task(void *pvParameters)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��
	GUI_Init();  					//STemWin��ʼ��
	WM_SetCreateFlags(WM_CF_MEMDEV);
  taskENTER_CRITICAL();           //�����ٽ���
	
	
	
	OS_AppObjCreate();       //���������ͨѶ����
	
	
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//����MQTT Connack�����ź�
	//������������
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //������������
    xTaskCreate((TaskFunction_t )UserIf_task,             
                (const char*    )"UserIf",           
                (uint16_t       )USERIF_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USERIF_TASK_PRIO,        
                (TaskHandle_t*  )&UserIfTask_Handler);  
    //����gui����
    xTaskCreate((TaskFunction_t )emwin_task,             
                (const char*    )"emwin_task",           
                (uint16_t       )EMWIN_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWIN_TASK_PRIO,        
                (TaskHandle_t*  )&EmwinTask_Handler);   	

		//����led ����
    xTaskCreate((TaskFunction_t )led_task,             
                (const char*    )"led_task",           
                (uint16_t       )LED_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED_TASK_PRIO,        
                (TaskHandle_t*  )&LedTask_Handler);   		
					

		//����key����
    xTaskCreate((TaskFunction_t )key_task,             
                (const char*    )"key_task",           
                (uint16_t       )KEY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&KeyTask_Handler); 

								
		//����Modbus RTU����
		xTaskCreate((TaskFunction_t )modbus_task,             
                (const char*    )"modbus_task",           
                (uint16_t       )MODBUS_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MODBUS_TASK_PRIO,        
                (TaskHandle_t*  )&ModbusTask_Handler); 
								
		//����MQTT��������			
		   xTaskCreate((TaskFunction_t )MQTT_Connect_task,             
                (const char*    )"MQTT_Connect_Task",           
                (uint16_t       )MQTT_Connect_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Connect_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Connect_task_Handler);
				
			//����MQTT��������			
		   xTaskCreate((TaskFunction_t )MQTT_rec_task,             
                (const char*    )"MQTT_Rec_Task",           
                (uint16_t       )MQTT_Rec_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Rec_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Rec_task_Handler);
			 //����MQTT PINGREQ����			
		   xTaskCreate((TaskFunction_t )MQTT_Pingreq_task,             
                (const char*    )"MQTT_PINGREQ_Task",           
                (uint16_t       )MQTT_PINGREQ_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_PINGREQ_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_PINGREQ_task_Handler);
				
				//����MQTT SUBSCRIBE����			
		   xTaskCreate((TaskFunction_t )MQTT_Subscribe_task,             
                (const char*    )"MQTT_SUBSCRIBE_Task",           
                (uint16_t       )MQTT_SUBSCRIBE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_SUBSCRIBE_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_SUBSCRIBE_task_Handler);
								
								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}



