    
#include "RS485.h"	 


//////////////////////////////////////////////////////////////////////////////////	 
/*
RS485�ײ�����
ʹ��usart2
DMA����\����
*/
//////////////////////////////////////////////////////////////////////////////////

extern QueueHandle_t RS485_buffer_Queue; //rs485��Ϣ���о��

#define RS485_DMA_Send_Len   256   //�������ݳ��� 
#define RS485_DMA_Rec_Len   256    //�������ݳ���
static u8 RS485_DMA_Send_Buf[RS485_DMA_Send_Len];	//�������ݻ�����
static u8 RS485_DMA_Receive_Buf[RS485_DMA_Rec_Len];	//�������ݻ�����


  



//��ʼ��IO ����2
//bound:������	  
void usart2_init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;		//GPIO��ʼ���ṹ��
  USART_InitTypeDef USART_InitStructure;  //USART2��ʼ���ṹ��
 
		/************USART/GPIO RCCʱ��ʹ��*********************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);//ʹ��GPIOA,Dʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
		/************gpio����************************/
	//PD7 RS485���շ���ģʽ����I/O
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PD7�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   //PA2��ʼ���ṹ���Ա������ֵ
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//PA3��ʼ���ṹ���Ա������ֵ
	
	/********************��������********** *****/
	USART_InitStructure.USART_BaudRate=bound;									//������115200
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //��Ӳ��������
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;		//�շ�ģʽ
	USART_InitStructure.USART_Parity=USART_Parity_No;		 		//����żУ��λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1��ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;       //���ݳ���Ϊ8λ
	//���ڳ�ʼ���ṹ���Ա������ֵ
	USART_Init(USART2,&USART_InitStructure);

	
	/********�����ж�����*************/

	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//�رս����ж�(���յ�һ���ֽ�ʱ�������ж�)
		USART_ITConfig(USART2,USART_IT_TC,DISABLE);  //�رմ�������ж� 
		
		
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE); //�������ڿ����ж�
	USART_ITConfig(USART2,USART_IT_ERR,ENABLE);	//���������ж�
	USART_ITConfig(USART2,USART_IT_PE,ENABLE);	  //������ż�����ж�

	/**********************��������********************/
	USART_Cmd(USART2,ENABLE);           //ʹ�ܴ���2
 
	

 
}




/*****************
DMA��ʼ��
USART2_RX->channel6
USART2_TX->channel7
******************/
//���͵�ַ�Լ����ͻ����С

//���յ�ַ�Լ����ջ����С

void DMA_USART2_Init(void)
{
	
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/****************DMA1 RCCʱ��ʹ��************************************/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA1rccʱ��
	/*******************USART2����DMA����(channel7)***************************/
	
	DMA_DeInit(DMA1_Channel7);  //��DMA��ͨ��7�Ĵ�������Ϊȱʡֵ

  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR; //DMA ����USART2��ַ

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RS485_DMA_Send_Buf;  //DMA�����ڴ����ַ

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݷ��򣬴��ڲ�������

  DMA_InitStructure.DMA_BufferSize = RS485_DMA_Send_Len;  //DMAͨ���Ļ����С

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������

  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ

  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ

  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ

  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ��ʹ�������ȼ�

  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA���ڴ浽�ڴ洫��

  DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //DMAͨ��7�ṹ�帳ֵ

	//�ж�����

DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);//ʹ�ܴ�������ж�

DMA_ITConfig(DMA1_Channel7, DMA_IT_TE, ENABLE); //ʹ�ܴ�������ж�
	


	//���ڷ���DMAʹ��

//USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);   //ʹ�ܴ���2 DMA����ʹ��
		
DMA_Cmd(DMA1_Channel7, DISABLE);//�رշ���ͨ�� ��ע�⡿ ����ʱ�ٿ�������ֹDMA���շ�������





	/***************USART2����DMA����(channel6)***************************/
	
	DMA_DeInit(DMA1_Channel6);   //��DMA��ͨ��6�Ĵ�������Ϊȱʡֵ

  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR; //DMA ����USART2��ַ

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RS485_DMA_Receive_Buf;  //DMA�����ڴ����ַ

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݷ��򣬴����赽�ڲ�

  DMA_InitStructure.DMA_BufferSize = RS485_DMA_Rec_Len;  //DMAͨ���Ļ����С

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������

  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ

  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ


  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ

  DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ��ʹ�ø����ȼ�

  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA���ڴ浽�ڴ洫��

  DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //DMAͨ��6�ṹ�帳ֵ

//�ж�����
DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);//ʹ�ܴ��仺������ж�

DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE); //ʹ�ܴ�������ж�

//���ڽ���DMAʹ�� ��ע��������NVIC��ʼ������

/*
USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);   //ʹ�ܴ���2DMA����ʹ��
	
DMA_Cmd(DMA1_Channel6, ENABLE);  //DMAͨ��6����ʹ��
*/
	
}




/***********************************
NVIC��ʼ��

����2NVIC
DMA1_channel6 NVIC
DMA1_channel7 NVIC

**************************/

void NVIC_USART2_cofig_Init(void)
{
		
	NVIC_InitTypeDef NVIC_InitStructure;            //NVIC��ʼ���ṹ��
	
	
	/********USART2 NVIC�жϳ�ʼ��***********/
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;				//�ж�ͨ��Ϊ����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=13;    //��ռ���ȼ�Ϊ13
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//��Ӧ���ȼ�Ϊ0
	//NVIC��ʼ���ṹ���Ա������ֵ
	NVIC_Init(&NVIC_InitStructure);
	
	 /****DMA ͨ��7 NVIC�ж�  ����*********/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);


	 /****DMA1 ͨ��6 NVIC�ж�  ����*********/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);

/****�ڴ˴���������2����DMA****/
USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);   //ʹ�ܴ���2DMA����ʹ��
	
DMA_Cmd(DMA1_Channel6, ENABLE);  //DMAͨ��6���մ���ʹ��

};

/************485��ʼ��************/
void RS485_init(u32 bound)
{

usart2_init(bound);
DMA_USART2_Init();
NVIC_USART2_cofig_Init();
//RS485ģʽѡ��
RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ

};










/********************
����2�жϷ�����
*********************/

typedef struct RS485buff
{
u32 RS485_lenth;
u8 RS485buff[256];
}RS485buff;

RS485buff RS485_tMsg;

void USART2_IRQHandler(void)  
{

	extern QueueHandle_t Wifi_buffer_Queue;
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//��ֵ�ź������
	RS485buff *receivebuff;
	BaseType_t xHigherPriorityTaskWoken;
	int16_t 	DATA_LEN;
	u16 i;
	DMA_Channel_TypeDef *DMA1_CH6 = DMA1_Channel6;
	USART_TypeDef *uart2 = USART2;
	
			receivebuff=&RS485_tMsg;//��Ϣ�ṹ���ʼ��
			receivebuff->RS485_lenth=0;//��Ϣ�ṹ���ʼ��
	

	DMA_Cmd(DMA1_Channel6,DISABLE); //�ر�DMA ��DMA��ֹ�жϴ����ڼ�������
	//printf("���1");
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)//���Ϊ����2�����ж�
	{
		DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);//�����־λ
	 //printf("���2");

			DATA_LEN = 256 - DMA_GetCurrDataCounter(DMA1_Channel6); 
			if(DATA_LEN > 0 && DATA_LEN <= 256)
			{	 
				//printf("���3");
			 DMA_Cmd(DMA1_Channel6, DISABLE); //�ı�ͨ��datasizeǰҪ��ֹͨ������
		   DMA1_CH6->CNDTR = 256; //�޸�DMA1���ݴ�����

				

			memset(receivebuff->RS485buff,0x00,sizeof(receivebuff->RS485buff));
			//������Ϣ����
					//printf("���4");
				//printf("485���߽��յ�����\r\n");
				receivebuff->RS485_lenth=DATA_LEN;
					//printf("���5");
				
				memcpy(receivebuff->RS485buff,RS485_DMA_Receive_Buf,256);
				xQueueOverwriteFromISR(RS485_buffer_Queue,(void *)&receivebuff,&xHigherPriorityTaskWoken);		
				
					//printf("���6");
				xSemaphoreGiveFromISR(BinarySemaphore_USART2ISR,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
				
//			uart_to_keyboard_msg.address = DMA_Receive_Buf;
//			uart_to_keyboard_msg.length = DATA_LEN;
//			total_rx++;
//			xQueueSendToBackFromISR(Keyboard_Queue,(void*)&uart_to_keyboard_msg,&xHigherPriorityTaskWoken);
			//xSemaphoreGiveFromISR(BinarySemaphore,&xHigherPriorityTaskWoken);
				

		 	//printf("���7");

			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				//�˳��ж��л�
			}//���������	
			
	      //�ȶ�SR���DR,���IDLE�����жϱ�־
	/****��ע����������жϺ󣬴���CR1�Ĵ�����IDLEλ��Ӳ����1�����Դ˶�������Ҫ����ֹ�����������жϣ���������жϲ���ʹ�ÿ⺯��������ʹ���ȶ�SR���DR�ķ�����****/
     i = uart2->SR;
     i = uart2->DR;
	   i = i;
				 //	printf("���8");
	}//�����ж��ж�
		
	else if(USART_GetITStatus(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE|USART_IT_ORE) != RESET)//����
	{
		USART_ClearITPendingBit(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE|USART_IT_ORE);//����ж�
		//�������ͼ���total_err2++;
		//printf("���� err2");
		i = uart2->SR;
		i = uart2->DR;
		
	}
	else if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) //��������ж�
	{
		//�������ͼ���total_err2++;
		//funcCode.all[FUNCCODE_P11_15] = total_err2;
		//printf("���� err3");

		
		

	//���봮��2��������ж�
		if (USART_GetFlagStatus(USART2,USART_FLAG_TC)==SET)
		{
		USART_ClearFlag(USART2,USART_FLAG_TC);//���������������жϱ�־���ڷ�������ж���ת������ģʽ
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		USART_ITConfig(USART2,USART_IT_TC,DISABLE);
		i = uart2->SR;
		i = uart2->DR;
		RS485_TX_EN=0;				//����Ϊ����ģʽ	
		}
		
		

	}
	else 
	{
		//�������ͼ���total_err2++;
		//funcCode.all[FUNCCODE_P11_15] = total_err2;
		//printf("���� err4");
		i = uart2->SR;
		i = uart2->DR;
			
	}
			 	//printf("���9");
	
	DMA_Cmd(DMA1_Channel6, DISABLE);//�ر�DMA
	DMA1_CH6->CNDTR = 256;//��װ��
	DMA_Cmd(DMA1_Channel6, ENABLE);//������ɣ�����DMA
		 //	printf("���10");
	
	//������б�־ ��ע�����˿⺯�������־һ��Ҫ�����沽�� ��ȡSR DR�Ĵ���ֵ
	USART_ClearITPendingBit(USART2, USART_IT_TC);
	USART_ClearITPendingBit(USART2, USART_IT_IDLE);
				// printf("���11");
			
}


 /****DMA1 ͨ��7  ��������ж�*********/
void DMA1_Channel7_IRQHandler(void)
{
	/*******ע�ⲻ����DMA��������ж�(TC)���л�485�շ�ģʽ********/
	//��ᵼ�����������ֽڷ��Ͳ���ȥ��������Ϊ��DMA��"��������ж�"�����ٸշ��͵���
	//�ڶ����ֽڵ���ʼλ�ã����ʱ���л�485���շ��������ն˲�����У��Ļ����������յ�0xFF ����һ�϶�Ҳ
	//�������������ʱ����ʹ�ô��ڷ�������ж���ʹ�� 485ģʽ�л�
	
	/**********��ע�⡿*********************/
	//���� DMAǰ���ȹر�UART��������жϣ��������������жϱ�־
	//��DMA��������жϺ����У�����UART��������ж�
	//��UART��������жϺ����У��л�RS485Ϊ��״̬
	
	//RS485_TX_EN=0;				//����Ϊ����ģʽ	
	
  
	
  DMA_ClearITPendingBit(DMA1_IT_TC7);
 
  DMA_ClearITPendingBit(DMA1_IT_TE7); //��������ж�

  DMA_Cmd(DMA1_Channel7, DISABLE);//DMA�ر�
	

  USART_ITConfig(USART2,USART_IT_TC,ENABLE);
  
}

 /****DMA1 ͨ��6  ��������ж�*********/
 //���ݻ����������
void DMA1_Channel6_IRQHandler(void)
{
		
	DMA_Channel_TypeDef *DMA1_CH6 = DMA1_Channel6;
	 	//printf("���12");
	DMA_ClearITPendingBit(DMA1_IT_TC6);

	DMA_ClearITPendingBit(DMA1_IT_TE6); //��������ж�

	DMA_Cmd(DMA1_Channel6, DISABLE);//DMA���չر�

	DMA1_CH6->CNDTR = 256;//����װ�����ݴ�С

	DMA_Cmd(DMA1_Channel6, ENABLE);//DMA���տ���
		 	//printf("���13");
}


/*****************
���ͽӿں���
���ͺ���������Ҫ����ʱ
vTaskDelay(time);
Ԥ��time=(len/40)+2

******************/
void RS485_send(u8 * buffer,u32 len)
{

	
	RS485_TX_EN=1;				//����Ϊ����ģʽ	
	

	
	DMA_ClearFlag(DMA1_FLAG_TC7);      //����DMA��������ж�
	DMA_ClearITPendingBit(DMA1_IT_TC7);
	
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);//����DMAǰ���ȹر�UART��������ж�
	USART_ClearFlag(USART2,USART_FLAG_TC);//���������������жϱ�־���ڷ�������ж���ת������ģʽ
	USART_ClearITPendingBit(USART2, USART_IT_TC);

	
	memset(RS485_DMA_Send_Buf, 0x00, sizeof(RS485_DMA_Send_Buf));//������0
	
	memcpy(RS485_DMA_Send_Buf,buffer,len);
	

	
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���2��DMA����      
	
	DMA_Cmd(DMA1_Channel7, DISABLE);//DMA �ر�
	
	
	DMA_SetCurrDataCounter(DMA1_Channel7,len);//DMAͨ����DMA����Ĵ�С
	
	
	
	

	
	DMA_Cmd(DMA1_Channel7, ENABLE);        //DMA����
	
	
	

	//total_tx++;

	
}

















