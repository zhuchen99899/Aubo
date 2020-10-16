#include <OS_AppObjCreate.h>


/***********************************************************
						��������
************************************************************/

//��Ϣ���ݽṹ�嶨��
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
						�ź������
************************************************************/

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_USART3ISR;	//USART3�����ж϶�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
/***********************************************************
						 ������Ϣ����
************************************************************/


#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define RS485MESSAGE_buffer_Q_NUM    1 //RS485��Ϣ��������
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi�������ݵ���Ϣ���е����� 
#define PINGREQ_Q_NUM   1   	//PINGREQ����Ϣ���е�����  
//������Ϣ����
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("������Ϣ���д����ɹ�");
	}
}


void RS485_buffer_QueueCreat(void)
{
	extern QueueHandle_t RS485_buffer_Queue;
	RS485_buffer_Queue =xQueueCreate(RS485MESSAGE_buffer_Q_NUM,sizeof(struct RS485MSG *));
	
	if (RS485_buffer_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("RS485������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("RS485������Ϣ���д����ɹ�");
	}
}


void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFIMSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("MQTT������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("MQTT������Ϣ���д����ɹ�");
	}
}


void PINGREQ_QueueCreat(void)
{
	extern QueueHandle_t PINGREQ_Queue;
	PINGREQ_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(uint8_t));
	
	if (PINGREQ_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("PINGREQ��Ϣ���д���ʧ��");
	}
	else 
	{
	printf("PINGREQ��Ϣ���д����ɹ�");
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
						 �ź�������
************************************************************/
/**************��ֵ�ź�������****************/

void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_USART3ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();
	BinarySemaphore_MQTTsubscribe=xSemaphoreCreateBinary();
};

/****************�ź�������*****************/
void SempaphoreCreate(void)
{

BinarySemaphoreCreate();
};




/***********************************************************
						OSͨѶ���Ƴ�ʼ��
************************************************************/

void OS_AppObjCreate(void)
{
//��Ϣ���д���
QueueCreate();
SempaphoreCreate();
	
	
}

 
 
 
 
 
 
 
 
 
 
 
 







