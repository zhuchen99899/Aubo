#include "task_MQTT_rec.h"


/**********消息队列参数**************/

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

/********Freertos软件定时器组句柄******************/




/***************信号量参数************/
	extern SemaphoreHandle_t BinarySemaphore_USART3ISR;	//二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄


	/****消息队列句柄**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern QueueHandle_t PINGREQ_Queue;
	extern QueueHandle_t Key_Queue;
	extern QueueHandle_t MODBUS_Var_Queue;

		/**************事件组句柄**********/


/*************************WIFI接收任务函数******************/
void MQTT_rec_task(void *pvParameters)
{
	//MODBUS参数
	VAR_T *MODBUS_reg;
VAR_T init_modbus_reg;
	
		/**********************SUBACK解包参数**********************/
	unsigned short DeserializeSuback_packetid;
	int DeserializeSuback_Maxcount=subscribe_count;
	int DeserializeSuback_Count;
	int grantedQoSs[subscribe_count];
	
	//显示参数
	int i;
	/**********************PUBLISH解包参数*********************/
		unsigned char DeserializePublish_dup;
		int DeserializePublish_qos;
		unsigned char DeserializePublish_retained;
		unsigned short DeserializePublish_packetid;
		MQTTString DeserializePublish_topicName;
		unsigned char* DeserializePublish_payload;
		int DeserializePublish_payloadlen;
	

							
									
		//申请动态内存指针
	char *topic1;
	char *payload1;
	
	
	/* 创建JSON结构体指针*/
	cJSON *json = NULL;
	cJSON *node = NULL;
	char *json_data = NULL;

	


	
	/********传递任务堆剩余变量*********/

/********************变量****************************/


	/***************信号量参数************/
	BaseType_t err=pdFALSE;
	/****消息队列参数**************/
	wifibuff *wifireceive;
	u8 PINGREQ_FLAG=0;

	


	
//	u8 key=0;
	MODBUS_reg=&init_modbus_reg;
	
/*******************阻塞等待DMA中断中传来的ISR信号量***************/
	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore_USART3ISR,portMAX_DELAY);	//获取串口3空闲中断二值信号量
	if(err==pdTRUE)										//获取信号量成功
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("接收到的数据为 %02x\r\n ,长度为 %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
				/************************CONNACK报文检测**************************************************/
			//	首字节固定为0x20  剩余字节固定为0x02 可变报头部分为 连接确认标志：0x00 以及连接返回码
		if((wifireceive->wifi_buffer[0]==0x20)&&(wifireceive->wifi_buffer[1]==0x02)&&(wifireceive->wifi_buffer[2]==0x00))    //如果收到4个字节，且第一个字节是0x20，表示是connect ack报文，进入if
		{
			printf("已接收服务器CONNACK报文\r\n");
			switch(wifireceive->wifi_buffer[3])	                                                     //判断收到的第4个字节
			{					
				case 0x00 : printf("CONNECT报文成功\r\n");


										PINGREQ_FLAG=1;   //开始发送PINGREQ报文标志
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										vTaskDelay(3000);
										xSemaphoreGive(BinarySemaphore_MQTTsubscribe);
										vTaskDelay(3000);

										
										/************SUBSCRIBE,publish*************/
							break;
				case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");
				/***************E1错误重启*********************/		
										
						break;
				case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");
				/***************E2错误重启*********************/	
								
						break;
				case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");
				/***************E3错误重启*********************/	

						break;
				case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");
				/***************E4错误重启*********************/	

						break;
				case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");
				/***************E5错误重启*********************/	

						break;
				default   : printf("连接已拒绝，未知状态，准备重启\r\n");
				/***************E6错误重启*********************/	

				    break;
		
			}				
		}//else if connack报文
		
		/*************收到PINGRESP报文******************************/
		//首字节固定为0xD0 剩余长度固定为 0x00 ，无可变报头
		else if((wifireceive->wifi_buffer[0]==0xD0)&&(wifireceive->wifi_buffer[1]==0x00)) 
		{
		
		printf("已接收服务器PINGRESP报文\r\n");

		}//else if PINGRESP报文
		
		/*****************SUBACK报文********************************/
		/*************如果增加subscribe，需要对wifi_buffer[2]剩余字节修改，增加一个加1，并且在下面主题判断中增加判断主题*****/
		else if(wifireceive->wifi_buffer[0]==0x90) 
		{
				printf("已接收服务器SUBACK报文\r\n");
			
						
			MQTTDeserialize_suback(&DeserializeSuback_packetid,DeserializeSuback_Maxcount,&DeserializeSuback_Count,grantedQoSs,(unsigned char*)(wifireceive->wifi_buffer),
									(int)(wifireceive->wifi_lenth));
			
			printf("包id为:%d\r\n",DeserializeSuback_packetid);
			printf("反回的报文主题过滤器计数%d\r\n",DeserializeSuback_Count);
			for (i=0;i<subscribe_count;i++)
			{
				printf("主题过滤器%d订阅成功,qos等级为:%d",i+1,grantedQoSs[i]);
			}
			


				
		}//else if SUBSCRIBE报文
		
		/***************************服务器publish报文************************/
		else if(wifireceive->wifi_buffer[0]==0x40)
		{

		printf("已接收服务器puback报文\r\n");

			
		}//else if puback报文

		else if((wifireceive->wifi_buffer[0]&0xF0)==0x30)
		{

		printf("已接收服务器PUBLISH报文\r\n");
			/******解包PUBLISH报文*****/
		MQTTDeserialize_publish(&DeserializePublish_dup,&DeserializePublish_qos,
									&DeserializePublish_retained,&DeserializePublish_packetid,
									&DeserializePublish_topicName,&DeserializePublish_payload,
									&DeserializePublish_payloadlen,(unsigned char*)(wifireceive->wifi_buffer),
									(int)(wifireceive->wifi_lenth));	

				/*************主题/负载判断****************/						
									
					printf("DUP:%d\r\n",DeserializePublish_dup);
					printf("qps:%d\r\n",DeserializePublish_qos);
					printf("retained:%d\r\n",DeserializePublish_retained);
		
							
									
									
//申请动态内存保存主题和负载							
topic1=pvPortMalloc(DeserializePublish_topicName.len);
payload1=pvPortMalloc(DeserializePublish_payloadlen);
memcpy(payload1,DeserializePublish_payload,DeserializePublish_payloadlen);
memcpy(topic1,DeserializePublish_topicName.string,DeserializePublish_topicName.len);
//清零字符串最后一位，防止乱码
payload1[DeserializePublish_payloadlen]=0;
topic1[DeserializePublish_topicName.len]=0;		

									
									
							/***********按键主题*******************/
			if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters1)==0))
			{
				printf("主题为Aubo主题\r\n");							
				printf("主题为%s\r\n",topic1);	
				printf("负载为\r\n");
				
				
								/****************有效载荷Cjson解包******************/
			
			
									/**********	完整输出一个JSON对象结构************/
								json = cJSON_Parse(payload1);                         //从数据缓冲区解析JASON对象结构
								json_data = cJSON_Print(json);                    //将传入的JASON对象转化为JASON字符串
								printf("data: %s\n", json_data);                  //输出JASON结构
								vPortFree(json_data);                                  //释放JASON结构

									/****************JASON树中键值对解析***************/
									//根据键名再JASON中查找子节点，查找出LED1对象
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
													//从LED1中获取结果

												printf("value:%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														MODBUS_reg->P01=1;	
														MODBUS_reg->P02=1;
														xQueueOverwrite(MODBUS_Var_Queue,&MODBUS_reg);
														printf("action1动作寄存器置1");
														}
														else if((node->valueint)==0)
														{
														MODBUS_reg->P01=0;	
														MODBUS_reg->P02=0;
														xQueueOverwrite(MODBUS_Var_Queue,&MODBUS_reg);
														printf("action1动作寄存器置0");
														}

											}//查找LED1值
											
										

													
									}//识别JASON树中LED0对象
			
			
				
			}//按键主题确认
			
				
			

			
			

			
			
			else
			{
			printf("已订阅的未知主题，请查看设置\r\n");
			}
			//主题确认
		
			
			//释放JASON树
			cJSON_Delete(json);
			
			//释放解包所申请的空间 
	vPortFree(payload1);	
	vPortFree(topic1);	
			
//将剩余堆传递给emwin	

		};//else if publish报文
		

	}//WIFI串口接收信号量
			
			/************串口未收到任何服务器返回(信号量持续阻塞)***********************/
			
		//else{
			/************************

			**************/
			
			
			
			//	}
			
	//事件标志组喂狗

	
   vTaskDelay(30);                           //延时3000ms
	}

}
