#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "MODBUSConfig.h"





//��Ϣ���д���
void QueueCreate(void);




//��ֵ�ź�������
void BinarySemaphoreCreate(void);


//�ź�������
void SempaphoreCreate(void);

//����ͨѶ���ƴ���
void OS_AppObjCreate(void);

#endif 







