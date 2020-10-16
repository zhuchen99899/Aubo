#ifndef __wifi_H_
#define __wifi_H_
#include "sys.h"
#include "stdio.h"	
#include "string.h"

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "semphr.h"
#include "stm32f10x.h"





void usart3_init(u32 bound);
void DMA_USART3_Init(void);
void NVIC_USART3_cofig_Init(void);
void wifi_init(u32 bound);
void USART3_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void WIFI_send(u8 * buffer,u32 len);

#endif



