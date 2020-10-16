#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 	
#include "sys.h"		
#include "string.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "semphr.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//RS485���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	


//ģʽ����
#define RS485_TX_EN		PDout(7)	//485ģʽ����.0,����;1,����.





void usart2_init(u32 bound);
void DMA_USART2_Init(void);
void NVIC_USART2_cofig_Init(void);
void RS485_init(u32 bound);


void USART2_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void RS485_send(u8 * buffer,u32 len);

#endif	   



