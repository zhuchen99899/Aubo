#include "task_MQTT_Connect.h"


/************Freertos�����ʱ�����********************/
//�����������ʱ��


/**********�ź���**********/
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������

/******MQTTconncet�ṹ��************/
MQTTPacket_connectData SETCONNECT;




/********************WIFI����CONNCET����*****************/

void MQTT_Connect_task(void *pvParameters)
{
BaseType_t err=pdFALSE;
 u8 temp_buff[256];
int len;

vTaskDelay(20000);
conncect_init(); //MQTTconncet ��ͷ��ʼ������
	while(1)
{	
	
	err=xSemaphoreTake(BinarySemaphore_MQTTconnect,portMAX_DELAY);	//��ȡCONNECT�����ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{		
			printf("WIFI���ڷ���CONNECT����\r\n");
			len=MQTT_Connect(temp_buff,256,&SETCONNECT);
			WIFI_send(temp_buff,len);

					/*******��ӡwifi����*******/
		

			
		printf("CONNECT���ķ������\r\n");
	

			}

	
      vTaskDelay(15000);                           //��ʱ15s

}

}


