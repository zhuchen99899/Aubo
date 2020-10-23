#include "includes.h"
/***********************************************************
						变量定义
************************************************************/





/********************************消息队列句柄声明*********************************/
QueueHandle_t Key_Queue;
QueueHandle_t RS485_buffer_Queue;
QueueHandle_t Wifi_buffer_Queue;
QueueHandle_t PINGREQ_Queue;
/********************************信号量句柄*************************************/
SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
SemaphoreHandle_t BinarySemaphore_USART3ISR;  //USART3空闲中断二值信号量句柄
SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄
SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄
/***********************************************************
					任务函数声明
************************************************************/


void start_task(void *pvParameters);		//Start任务
void touch_task(void *pvParameters);		//TOUCH任务
void UserIf_task(void *pvParameters);		//空闲任务
void emwin_task(void *pvParameters); 	//emwin任务
void led_task(void *pvParameters);       //led任务
void key_task(void *pvParameters);       //key任务
void modbus_task(void *pvParameters);       //modbus_RTU任务
void MQTT_Connect_task(void *pvParameters);       // MQTTconnect任务
void MQTT_rec_task(void *pvParameters);				// MQTT接收任务
void MQTT_Pingreq_task(void *pvParameters);				// MQTTPINGREQ任务
void MQTT_Subscribe_task(void *pvParameters);				// MQTTSUBSCRIBE任务
/***********************************************************
						任务函数句柄
************************************************************/
//任务函数句柄
TaskHandle_t StartTask_Handler;			//Start任务
TaskHandle_t TouchTask_Handler;			//TOUCH任务
TaskHandle_t UserIfTask_Handler;		//空闲任务
TaskHandle_t EmwinTask_Handler;		//emwin任务
TaskHandle_t LedTask_Handler;		//led任务
TaskHandle_t KeyTask_Handler;		//Key任务
TaskHandle_t ModbusTask_Handler;		//modbus_RTU任务
TaskHandle_t MQTT_Connect_task_Handler;		  // MQTTconnect任务
TaskHandle_t MQTT_Rec_task_Handler;		  // MQTT接收任务
TaskHandle_t MQTT_PINGREQ_task_Handler;		  // MQTTPINGREQ任务
TaskHandle_t MQTT_SUBSCRIBE_task_Handler;				// MQTTSUBSCRIBE任务

/***********************************************************
						任务堆栈大小
************************************************************/
#define START_STK_SIZE 			256  	//Start任务
#define TOUCH_STK_SIZE			128		//TOUCH任务
#define USERIF_STK_SIZE			128		//空闲任务
#define EMWIN_STK_SIZE		512		//emwin任务
#define LED_STK_SIZE     128    //LED任务
#define KEY_STK_SIZE     128    //KEY任务
#define MODBUS_STK_SIZE     512    //modbus_RTU任务
#define MQTT_Connect_STK_SIZE		256	// MQTTconnect任务
#define MQTT_Rec_STK_SIZE		512	// MQTT接收任务
#define MQTT_PINGREQ_STK_SIZE		256	// MQTTPINGREQ任务
#define MQTT_SUBSCRIBE_STK_SIZE		256		// MQTTSUBSCRIBE任务
/***********************************************************
						 任务优先级(数值越小优先级越低)
************************************************************/
#define START_TASK_PRIO			5		//Start任务
#define TOUCH_TASK_PRIO			0		//TOUCH任务
#define USERIF_TASK_PRIO 		0	   //空闲任务
#define EMWIN_TASK_PRIO		0		//emwin任务
#define LED_TASK_PRIO        1    //led任务
#define KEY_TASK_PRIO        1    //Key任务
#define MODBUS_TASK_PRIO        2    //modbus_RTU任务
#define MQTT_Connect_TASK_PRIO		3		// MQTTCONNCET任务
#define MQTT_Rec_TASK_PRIO		2		// MQTT接收任务
#define MQTT_PINGREQ_TASK_PRIO		2		// MQTTPINGREQ任务
#define MQTT_SUBSCRIBE_TASK_PRIO		3		// MQTTSUBSCRIBE任务
/***********************************************************
						 主函数入口
************************************************************/

int main(void)
{
	
/*
 在启动调度前，为了防止初始化 STM32 外设时有中断服务程序执行，这里禁止全局中断(除了 NMI 和 HardFault)。
 这样做的好处是：
 1. 防止执行的中断服务程序中有 FreeRTOS 的 API 函数。
 2. 保证系统正常启动，不受别的中断影响。
 3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
 在移植文件 port.c 中的函数 prvStartFirstTask 中会重新开启全局中断。通过指令 cpsie i 开启，__set_PRIMASK(1)
 和 cpsie i 是等效的。
 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	 
	uart_init(115200);					//初始化串口1 :调试串口
	RS485_init(115200);					//初始化串口2 ：485总线
	wifi_init(115200);					//初始化串口3:  wifi串口
	LED_Init();		  					//初始化LED
  bsp_InitKey();	/* 初始化按键 */
	TFTLCD_Init();						//LCD初始化	
	FSMC_SRAM_Init();					//初始化SRAM
	TP_Init();							//触摸屏初始化
	TIM3_Int_Init(1799,0);
	my_mem_init(SRAMIN);            	//初始化内部内存池
	my_mem_init(SRAMEX);				//初始化外部内存池
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄                
    vTaskStartScheduler();          //开启任务调度
/*
 如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
 heap 空间不足造成创建失败，此要加大 FreeRTOSConfig.h 文件中定义的 heap 大小：
 #define configTOTAL_HEAP_SIZE ( ( size_t ) ( 17 * 1024 ) )
*/
	while(1);			
}

/***********************************************************
						 任务函数
************************************************************/

//开始任务任务函数
void start_task(void *pvParameters)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟
	GUI_Init();  					//STemWin初始化
	WM_SetCreateFlags(WM_CF_MEMDEV);
  taskENTER_CRITICAL();           //进入临界区
	
	
	
	OS_AppObjCreate();       //创建任务间通讯机制
	
	
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//发送MQTT Connack报文信号
	//创建触摸任务
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //创建空闲任务
    xTaskCreate((TaskFunction_t )UserIf_task,             
                (const char*    )"UserIf",           
                (uint16_t       )USERIF_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USERIF_TASK_PRIO,        
                (TaskHandle_t*  )&UserIfTask_Handler);  
    //创建gui任务
    xTaskCreate((TaskFunction_t )emwin_task,             
                (const char*    )"emwin_task",           
                (uint16_t       )EMWIN_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWIN_TASK_PRIO,        
                (TaskHandle_t*  )&EmwinTask_Handler);   	

		//创建led 任务
    xTaskCreate((TaskFunction_t )led_task,             
                (const char*    )"led_task",           
                (uint16_t       )LED_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED_TASK_PRIO,        
                (TaskHandle_t*  )&LedTask_Handler);   		
					

		//创建key任务
    xTaskCreate((TaskFunction_t )key_task,             
                (const char*    )"key_task",           
                (uint16_t       )KEY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&KeyTask_Handler); 

								
		//创建Modbus RTU任务
		xTaskCreate((TaskFunction_t )modbus_task,             
                (const char*    )"modbus_task",           
                (uint16_t       )MODBUS_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MODBUS_TASK_PRIO,        
                (TaskHandle_t*  )&ModbusTask_Handler); 
								
		//创建MQTT发送任务			
		   xTaskCreate((TaskFunction_t )MQTT_Connect_task,             
                (const char*    )"MQTT_Connect_Task",           
                (uint16_t       )MQTT_Connect_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Connect_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Connect_task_Handler);
				
			//创建MQTT接收任务			
		   xTaskCreate((TaskFunction_t )MQTT_rec_task,             
                (const char*    )"MQTT_Rec_Task",           
                (uint16_t       )MQTT_Rec_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Rec_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Rec_task_Handler);
			 //创建MQTT PINGREQ任务			
		   xTaskCreate((TaskFunction_t )MQTT_Pingreq_task,             
                (const char*    )"MQTT_PINGREQ_Task",           
                (uint16_t       )MQTT_PINGREQ_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_PINGREQ_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_PINGREQ_task_Handler);
				
				//创建MQTT SUBSCRIBE任务			
		   xTaskCreate((TaskFunction_t )MQTT_Subscribe_task,             
                (const char*    )"MQTT_SUBSCRIBE_Task",           
                (uint16_t       )MQTT_SUBSCRIBE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_SUBSCRIBE_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_SUBSCRIBE_task_Handler);
								
								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}



