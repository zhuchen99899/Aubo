#include "task_MQTT_rec.h"


/**********��Ϣ���в���**************/

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[256];
}wifibuff;


typedef  struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;
extern SETMSG g_tMsg;

/********Freertos�����ʱ������******************/




/***************�ź�������************/
	extern SemaphoreHandle_t BinarySemaphore_USART3ISR;	//��ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������


	/****��Ϣ���о��**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern QueueHandle_t PINGREQ_Queue;
	extern QueueHandle_t Key_Queue;
	extern QueueHandle_t MODBUS_Var_Queue;

		/**************�¼�����**********/


/*************************WIFI����������******************/
void MQTT_rec_task(void *pvParameters)
{
	//MODBUS����
	VAR_T *MODBUS_reg;
VAR_T init_modbus_reg;
	
		/**********************SUBACK�������**********************/
	unsigned short DeserializeSuback_packetid;
	int DeserializeSuback_Maxcount=subscribe_count;
	int DeserializeSuback_Count;
	int grantedQoSs[subscribe_count];
	
	//��ʾ����
	int i;
	/**********************PUBLISH�������*********************/
		unsigned char DeserializePublish_dup;
		int DeserializePublish_qos;
		unsigned char DeserializePublish_retained;
		unsigned short DeserializePublish_packetid;
		MQTTString DeserializePublish_topicName;
		unsigned char* DeserializePublish_payload;
		int DeserializePublish_payloadlen;
	

							
									
		//���붯̬�ڴ�ָ��
	char *topic1;
	char *payload1;
	
	
	/* ����JSON�ṹ��ָ��*/
	cJSON *json = NULL;
	cJSON *node = NULL;
	char *json_data = NULL;

	


	
	/********���������ʣ�����*********/

/********************����****************************/


	/***************�ź�������************/
	BaseType_t err=pdFALSE;
	/****��Ϣ���в���**************/
	wifibuff *wifireceive;
	u8 PINGREQ_FLAG=0;

	


	
//	u8 key=0;
	MODBUS_reg=&init_modbus_reg;
	
/*******************�����ȴ�DMA�ж��д�����ISR�ź���***************/
	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore_USART3ISR,portMAX_DELAY);	//��ȡ����3�����ж϶�ֵ�ź���
	if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("���յ�������Ϊ %02x\r\n ,����Ϊ %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
				/************************CONNACK���ļ��**************************************************/
			//	���ֽڹ̶�Ϊ0x20  ʣ���ֽڹ̶�Ϊ0x02 �ɱ䱨ͷ����Ϊ ����ȷ�ϱ�־��0x00 �Լ����ӷ�����
		if((wifireceive->wifi_buffer[0]==0x20)&&(wifireceive->wifi_buffer[1]==0x02)&&(wifireceive->wifi_buffer[2]==0x00))    //����յ�4���ֽڣ��ҵ�һ���ֽ���0x20����ʾ��connect ack���ģ�����if
		{
			printf("�ѽ��շ�����CONNACK����\r\n");
			switch(wifireceive->wifi_buffer[3])	                                                     //�ж��յ��ĵ�4���ֽ�
			{					
				case 0x00 : printf("CONNECT���ĳɹ�\r\n");


										PINGREQ_FLAG=1;   //��ʼ����PINGREQ���ı�־
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										vTaskDelay(3000);
										xSemaphoreGive(BinarySemaphore_MQTTsubscribe);
										vTaskDelay(3000);

										
										/************SUBSCRIBE,publish*************/
							break;
				case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");
				/***************E1��������*********************/		
										
						break;
				case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");
				/***************E2��������*********************/	
								
						break;
				case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");
				/***************E3��������*********************/	

						break;
				case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");
				/***************E4��������*********************/	

						break;
				case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");
				/***************E5��������*********************/	

						break;
				default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");
				/***************E6��������*********************/	

				    break;
		
			}				
		}//else if connack����
		
		/*************�յ�PINGRESP����******************************/
		//���ֽڹ̶�Ϊ0xD0 ʣ�೤�ȹ̶�Ϊ 0x00 ���޿ɱ䱨ͷ
		else if((wifireceive->wifi_buffer[0]==0xD0)&&(wifireceive->wifi_buffer[1]==0x00)) 
		{
		
		printf("�ѽ��շ�����PINGRESP����\r\n");

		}//else if PINGRESP����
		
		/*****************SUBACK����********************************/
		/*************�������subscribe����Ҫ��wifi_buffer[2]ʣ���ֽ��޸ģ�����һ����1�����������������ж��������ж�����*****/
		else if(wifireceive->wifi_buffer[0]==0x90) 
		{
				printf("�ѽ��շ�����SUBACK����\r\n");
			
						
			MQTTDeserialize_suback(&DeserializeSuback_packetid,DeserializeSuback_Maxcount,&DeserializeSuback_Count,grantedQoSs,(unsigned char*)(wifireceive->wifi_buffer),
									(int)(wifireceive->wifi_lenth));
			
			printf("��idΪ:%d\r\n",DeserializeSuback_packetid);
			printf("���صı����������������%d\r\n",DeserializeSuback_Count);
			for (i=0;i<subscribe_count;i++)
			{
				printf("���������%d���ĳɹ�,qos�ȼ�Ϊ:%d",i+1,grantedQoSs[i]);
			}
			


				
		}//else if SUBSCRIBE����
		
		/***************************������publish����************************/
		else if(wifireceive->wifi_buffer[0]==0x40)
		{

		printf("�ѽ��շ�����puback����\r\n");

			
		}//else if puback����

		else if((wifireceive->wifi_buffer[0]&0xF0)==0x30)
		{

		printf("�ѽ��շ�����PUBLISH����\r\n");
			/******���PUBLISH����*****/
		MQTTDeserialize_publish(&DeserializePublish_dup,&DeserializePublish_qos,
									&DeserializePublish_retained,&DeserializePublish_packetid,
									&DeserializePublish_topicName,&DeserializePublish_payload,
									&DeserializePublish_payloadlen,(unsigned char*)(wifireceive->wifi_buffer),
									(int)(wifireceive->wifi_lenth));	

				/*************����/�����ж�****************/						
									
					printf("DUP:%d\r\n",DeserializePublish_dup);
					printf("qps:%d\r\n",DeserializePublish_qos);
					printf("retained:%d\r\n",DeserializePublish_retained);
		
							
									
									
//���붯̬�ڴ汣������͸���							
topic1=pvPortMalloc(DeserializePublish_topicName.len);
payload1=pvPortMalloc(DeserializePublish_payloadlen);
memcpy(payload1,DeserializePublish_payload,DeserializePublish_payloadlen);
memcpy(topic1,DeserializePublish_topicName.string,DeserializePublish_topicName.len);
//�����ַ������һλ����ֹ����
payload1[DeserializePublish_payloadlen]=0;
topic1[DeserializePublish_topicName.len]=0;		

									
									
							/***********��������*******************/
			if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters1)==0))
			{
				printf("����ΪAubo����\r\n");							
				printf("����Ϊ%s\r\n",topic1);	
				printf("����Ϊ\r\n");
				
				
								/****************��Ч�غ�Cjson���******************/
			
			
									/**********	�������һ��JSON����ṹ************/
								json = cJSON_Parse(payload1);                         //�����ݻ���������JASON����ṹ
								json_data = cJSON_Print(json);                    //�������JASON����ת��ΪJASON�ַ���
								printf("data: %s\n", json_data);                  //���JASON�ṹ
								vPortFree(json_data);                                  //�ͷ�JASON�ṹ

									/****************JASON���м�ֵ�Խ���***************/
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�LED1����
									node = cJSON_GetObjectItem(json, "action1");
									if (node == NULL)
									{
									printf("action1: no\r\n");
										
									}	
									else
									{
									printf("action1: ok\r\n");
										 if( node->type == cJSON_Number )

											{
													//��LED1�л�ȡ���

												printf("value:%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														MODBUS_reg->P01=1;	
														MODBUS_reg->P02=1;
														xQueueOverwrite(MODBUS_Var_Queue,&MODBUS_reg);
														printf("action1�����Ĵ�����1");
														}
														else if((node->valueint)==0)
														{
														MODBUS_reg->P01=0;	
														MODBUS_reg->P02=0;
														xQueueOverwrite(MODBUS_Var_Queue,&MODBUS_reg);
														printf("action1�����Ĵ�����0");
														}

											}//����LED1ֵ
											
										

													
									}//ʶ��JASON����LED0����
			
			
				
			}//��������ȷ��
			
				
			

			
			

			
			
			else
			{
			printf("�Ѷ��ĵ�δ֪���⣬��鿴����\r\n");
			}
			//����ȷ��
		
			
			//�ͷ�JASON��
			cJSON_Delete(json);
			
			//�ͷŽ��������Ŀռ� 
	vPortFree(payload1);	
	vPortFree(topic1);	
			
//��ʣ��Ѵ��ݸ�emwin	

		};//else if publish����
		

	}//WIFI���ڽ����ź���
			
			/************����δ�յ��κη���������(�ź�����������)***********************/
			
		//else{
			/************************

			**************/
			
			
			
			//	}
			
	//�¼���־��ι��

	
   vTaskDelay(30);                           //��ʱ3000ms
	}

}
