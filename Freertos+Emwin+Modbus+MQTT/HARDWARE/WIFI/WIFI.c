
#include "wifi.h"

/*����3��������WIFIģ�� RXD->PB11 TXD->PB10
�������Ƽ�96000 
*/


extern QueueHandle_t Wifi_buffer_Queue;
extern SemaphoreHandle_t BinarySemaphore_USART3ISR;	//USART3�����ж϶�ֵ�ź������

#define WIFI_DMA_Send_Len   256   //�������ݳ��� 
#define WIFI_DMA_Rec_Len   256    //�������ݳ���
static u8 WIFI_DMA_Send_Buf[WIFI_DMA_Send_Len];	//�������ݻ�����
static u8 WIFI_DMA_Receive_Buf[WIFI_DMA_Rec_Len];	//�������ݻ�����



void usart3_init(u32 bound)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;			//GPIO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;		//USART3��ʼ���ṹ��

	/************USART/GPIO RCCʱ��ʹ��*********************/
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����3����TX,RX(PB10,PB11)ʱ��ʹ��
	
	/************gpio����************************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //����3TX ��PB10��Ҫ��Ϊ���츴��
	//PA9��ʼ���ṹ���Ա������ֵ
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //����3RX( PB11)Ҫ��Ϊ�����������������
	//PA10��ʼ���ṹ���Ա������ֵ
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/********************��������********** *****/
	USART_InitStructure.USART_BaudRate=bound;									//������115200
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //��Ӳ��������
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;		//�������ģʽ
	USART_InitStructure.USART_Parity=USART_Parity_No;		 		//����żУ��λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1��ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;       //���ݳ���Ϊ8λ
	//���ڳ�ʼ���ṹ���Ա������ֵ
	USART_Init(USART3,&USART_InitStructure);

	
	/********�����ж�����*************/
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);  //�رմ�������ж�
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//�رս����ж�(���յ�һ���ֽ�ʱ�������ж�)
	
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); //�������ڿ����ж�
	USART_ITConfig(USART3,USART_IT_ERR,ENABLE);	//���������ж�
	USART_ITConfig(USART3,USART_IT_PE,ENABLE);	  //������ż�����ж�

	/**********************��������********************/
	USART_Cmd(USART3,ENABLE);           //ʹ�ܴ���3
}


/*****************
DMA��ʼ��
USART3_RX->channel3
USART3_TX->channel2
******************/
//���͵�ַ�Լ����ͻ����С

//���յ�ַ�Լ����ջ����С

void    DMA_USART3_Init(void)
{
	
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/****************DMA1 RCCʱ��ʹ��************************************/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA1rccʱ��
	/*******************USART3����DMA����(channel2)***************************/
	
	DMA_DeInit(DMA1_Channel2);  //��DMA��ͨ��2�Ĵ�������Ϊȱʡֵ

  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR; //DMA ����USART3��ַ

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)WIFI_DMA_Send_Buf;  //DMA�����ڴ����ַ

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݷ��򣬴��ڲ�������

  DMA_InitStructure.DMA_BufferSize = WIFI_DMA_Send_Len;  //DMAͨ���Ļ����С

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������

  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ

  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ

  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ

  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ��ʹ�������ȼ�

  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA���ڴ浽�ڴ洫��

  DMA_Init(DMA1_Channel2, &DMA_InitStructure);  //DMAͨ��2�ṹ�帳ֵ

	//�ж�����

DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);//ʹ�ܴ�������ж�

DMA_ITConfig(DMA1_Channel2, DMA_IT_TE, ENABLE); //ʹ�ܴ�������ж�
	
DMA_Cmd(DMA1_Channel2, ENABLE);  //DMAͨ��2����ʹ��

	//���ڷ���DMAʹ��

//USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);   //ʹ�ܴ���3 DMA����ʹ��
		
DMA_Cmd(DMA1_Channel2, DISABLE);//�رշ���ͨ�� ��ע�⡿ ����ʱ�ٿ�������ֹDMA���շ�������





	/***************USART3����DMA����(channel3)***************************/
	
	DMA_DeInit(DMA1_Channel3);   //��DMA��ͨ��3�Ĵ�������Ϊȱʡֵ

  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR; //DMA ����USART3��ַ

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)WIFI_DMA_Receive_Buf;  //DMA�����ڴ����ַ

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݷ��򣬴����赽�ڲ�

  DMA_InitStructure.DMA_BufferSize = WIFI_DMA_Rec_Len;  //DMAͨ���Ļ����С

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������

  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ

  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ


  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ

  DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ��ʹ�ø����ȼ�

  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA���ڴ浽�ڴ洫��

  DMA_Init(DMA1_Channel3, &DMA_InitStructure);  //DMAͨ��3�ṹ�帳ֵ

//�ж�����
DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);//ʹ�ܴ��仺������ж�

DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE); //ʹ�ܴ�������ж�

//���ڽ���DMAʹ�� ��ע��������NVIC��ʼ������

/*
USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);   //ʹ�ܴ���3DMA����ʹ��
	
DMA_Cmd(DMA1_Channel3, ENABLE);  //DMAͨ��3����ʹ��
*/
	
}




/***********************************
NVIC��ʼ��

����3NVIC
DMA1_channel3 NVIC
DMA1_channel2 NVIC

**************************/

void NVIC_USART3_cofig_Init(void)
{
		
	NVIC_InitTypeDef NVIC_InitStructure;            //NVIC��ʼ���ṹ��
	
	
	/********USART3 NVIC�жϳ�ʼ��***********/
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;				//�ж�ͨ��Ϊ����3�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=12;    //��ռ���ȼ�Ϊ13
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//��Ӧ���ȼ�Ϊ0
	//NVIC��ʼ���ṹ���Ա������ֵ
	NVIC_Init(&NVIC_InitStructure);
	
	 /****DMA ͨ��2 NVIC�ж�  ����*********/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);


	 /****DMA1 ͨ��3 NVIC�ж�  ����*********/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);

/****�ڴ˴���������3����DMA****/
USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);   //ʹ�ܴ���3DMA����ʹ��
	
DMA_Cmd(DMA1_Channel3, ENABLE);  //DMAͨ��3���մ���ʹ��

};




/************wifi��ʼ��************/
void wifi_init(u32 bound)
{

usart3_init(bound);
DMA_USART3_Init();
NVIC_USART3_cofig_Init();
};


/********************
����3�жϷ�����
*********************/

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[256];
}wifibuff;

wifibuff wifi_tMsg;

void USART3_IRQHandler(void)  
{

	extern QueueHandle_t Wifi_buffer_Queue;
	extern SemaphoreHandle_t BinarySemaphore_USART3ISR;	//��ֵ�ź������
	wifibuff *receivebuff;
	BaseType_t xHigherPriorityTaskWoken;
	int16_t 	DATA_LEN;
	u16 i;
	DMA_Channel_TypeDef *DMA1_CH3 = DMA1_Channel3;
	USART_TypeDef *uart3 = USART3;
	
			receivebuff=&wifi_tMsg;//��Ϣ�ṹ���ʼ��
			receivebuff->wifi_lenth=0;//��Ϣ�ṹ���ʼ��
	

	DMA_Cmd(DMA1_Channel3,DISABLE); //�ر�DMA ��DMA��ֹ�жϴ����ڼ�������
	//printf("���1");
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)//���Ϊ����3�����ж�
	{
		DMA_ClearFlag(DMA1_FLAG_GL3 | DMA1_FLAG_TC3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3);//�����־λ
	 //printf("���2");

			DATA_LEN = 256 - DMA_GetCurrDataCounter(DMA1_Channel3); 
			if(DATA_LEN > 0 && DATA_LEN <= 256)
			{	 
				//printf("���3");
			 DMA_Cmd(DMA1_Channel3, DISABLE); //�ı�ͨ��datasizeǰҪ��ֹͨ������
		   DMA1_CH3->CNDTR = 256; //�޸�DMA1���ݴ�����

				

			memset(receivebuff->wifi_buffer,0x00,sizeof(receivebuff->wifi_buffer));
			//������Ϣ����
					//printf("���4");
				printf("WIFIģ����յ�����\r\n");
				receivebuff->wifi_lenth=DATA_LEN;
					//printf("���5");
				
				memcpy(receivebuff->wifi_buffer,WIFI_DMA_Receive_Buf,256);
				xQueueOverwriteFromISR(Wifi_buffer_Queue,(void *)&receivebuff,&xHigherPriorityTaskWoken);		
				
					//printf("���6");
				xSemaphoreGiveFromISR(BinarySemaphore_USART3ISR,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
				
//			uart_to_keyboard_msg.address = DMA_Receive_Buf;
//			uart_to_keyboard_msg.length = DATA_LEN;
//			total_rx++;
//			xQueueSendToBackFromISR(Keyboard_Queue,(void*)&uart_to_keyboard_msg,&xHigherPriorityTaskWoken);
			//xSemaphoreGiveFromISR(BinarySemaphore,&xHigherPriorityTaskWoken);
				

		 	//printf("���2");

			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				//�˳��ж��л�
			}//���������	
			
	      //�ȶ�SR���DR,���IDLE�����жϱ�־
	/****��ע����������жϺ󣬴���CR1�Ĵ�����IDLEλ��Ӳ����1�����Դ˶�������Ҫ����ֹ�����������жϣ���������жϲ���ʹ�ÿ⺯��������ʹ���ȶ�SR���DR�ķ�����****/
     i = uart3->SR;
     i = uart3->DR;
	   i = i;
				 //	printf("���8");
	}//�����ж��ж�
		
	else if(USART_GetITStatus(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE|USART_IT_ORE) != RESET)//����
	{
		USART_ClearITPendingBit(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE|USART_IT_ORE);//����ж�
		//�������ͼ���total_err2++;
		//printf("���� err2");
		i = uart3->SR;
		i = uart3->DR;
		
	}
	else
	{
		//�������ͼ���total_err2++;
		//funcCode.all[FUNCCODE_P11_15] = total_err2;
		//printf("���� err3");
		i = uart3->SR;
		i = uart3->DR;
			
	}
			 	//printf("���9");
	
	DMA_Cmd(DMA1_Channel3, DISABLE);//�ر�DMA
	DMA1_CH3->CNDTR = 256;//��װ��
	DMA_Cmd(DMA1_Channel3, ENABLE);//������ɣ�����DMA
		 //	printf("���10");
	
	//������б�־ ��ע�����˿⺯�������־һ��Ҫ�����沽�� ��ȡSR DR�Ĵ���ֵ
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
				// printf("���11");
			
}


 /****DMA1 ͨ��2  ��������ж�*********/
void DMA1_Channel2_IRQHandler(void)
{
 
  DMA_ClearITPendingBit(DMA1_IT_TC2);
 
  DMA_ClearITPendingBit(DMA1_IT_TE2); //��������ж�
 
  DMA_Cmd(DMA1_Channel2, DISABLE);//DMA�ر�

  
}

 /****DMA1 ͨ��3  ��������ж�*********/
 //���ݻ����������
void DMA1_Channel3_IRQHandler(void)
{
		
	DMA_Channel_TypeDef *DMA1_CH3 = DMA1_Channel3;
	 	//printf("���12");
	DMA_ClearITPendingBit(DMA1_IT_TC3);

	DMA_ClearITPendingBit(DMA1_IT_TE3); //��������ж�

	DMA_Cmd(DMA1_Channel3, DISABLE);//DMA���չر�

	DMA1_CH3->CNDTR = 256;//����װ�����ݴ�С

	DMA_Cmd(DMA1_Channel3, ENABLE);//DMA���տ���
		 	//printf("���13");
}


/*****************
���ͽӿں���
���ͺ���������Ҫ����ʱ
vTaskDelay(time);
Ԥ��time=(len/40)+2

******************/
void WIFI_send(u8 * buffer,u32 len)
{


	
	memset(WIFI_DMA_Send_Buf, 0x00, sizeof(WIFI_DMA_Send_Buf));//������0
	
	memcpy(WIFI_DMA_Send_Buf,buffer,len);
	
	
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���3��DMA����      
	
	DMA_Cmd(DMA1_Channel2, DISABLE);//DMA �ر�
	DMA_SetCurrDataCounter(DMA1_Channel2,len);//DMAͨ����DMA����Ĵ�С
	DMA_Cmd(DMA1_Channel2, ENABLE);        //DMA����

	//total_tx++;


}


/**************
���սӿں���
������
�Ѿ��Ӵ��ڿ����ж�����Ϣ���д���
***************/
/*
void KeyboardRs485RxFrame(unsigned char *buffer,unsigned int timeout)
{
	unsigned int len = 0;
	unsigned int i = 0;

	msg_object_t msg;
	KeyboardRs485RxMode();	

	if(xQueueReceive(Keyboard_Queue,&msg,timeout) == pdFALSE)
	//if(xSemaphoreTake(BinarySemaphore, timeout) == pdFALSE)
	{
		
		return len;
	}

	len = msg.length;
	//len = uart_to_keyboard_msg.length;
	//copy to buffer
	for(i = 0; i < len; i++)
	{
		buffer[i] = msg.address[i];
		//buffer[i] = uart_to_keyboard_msg.address[i];
	}
	

};
*/




