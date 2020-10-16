#include <OS_AppObjCreate.h>


/***********************************************************
						变量定义
************************************************************/

//消息传递结构体定义
typedef struct RS485MSG
{

char  address[256];
u32 length;

} RS485MSG;
	

typedef struct WIFIMSG
{

char  address[256];
u32 length;

} WIFIMSG;
	


/***********************************************************
						信号量句柄
************************************************************/

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_USART3ISR;	//USART3空闲中断二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄
/***********************************************************
						 创建消息队列
************************************************************/


#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define RS485MESSAGE_buffer_Q_NUM    1 //RS485消息队列数量
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi接收数据的消息队列的数量 
#define PINGREQ_Q_NUM   1   	//PINGREQ的消息队列的数量  
//按键消息队列
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("按键消息队列创建失败");
	}
	else 
	{
	printf("按键消息队列创建成功");
	}
}


void RS485_buffer_QueueCreat(void)
{
	extern QueueHandle_t RS485_buffer_Queue;
	RS485_buffer_Queue =xQueueCreate(RS485MESSAGE_buffer_Q_NUM,sizeof(struct RS485MSG *));
	
	if (RS485_buffer_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("RS485接收消息队列创建失败");
	}
	else 
	{
	printf("RS485接收消息队列创建成功");
	}
}


void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFIMSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("MQTT接收消息队列创建失败");
	}
	else 
	{
	printf("MQTT接收消息队列创建成功");
	}
}


void PINGREQ_QueueCreat(void)
{
	extern QueueHandle_t PINGREQ_Queue;
	PINGREQ_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(uint8_t));
	
	if (PINGREQ_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("PINGREQ消息队列创建失败");
	}
	else 
	{
	printf("PINGREQ消息队列创建成功");
	}
}



void QueueCreate(void)
{

Key_QueueCreat();
RS485_buffer_QueueCreat();	
WIFI_buffer_QueueCreat();
PINGREQ_QueueCreat();
}


/***********************************************************
						 信号量创建
************************************************************/
/**************二值信号量创建****************/

void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_USART3ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();
	BinarySemaphore_MQTTsubscribe=xSemaphoreCreateBinary();
};

/****************信号量创建*****************/
void SempaphoreCreate(void)
{

BinarySemaphoreCreate();
};




/***********************************************************
						OS通讯机制初始化
************************************************************/

void OS_AppObjCreate(void)
{
//消息队列创建
QueueCreate();
SempaphoreCreate();
	
	
}

 
 
 
 
 
 
 
 
 
 
 
 







