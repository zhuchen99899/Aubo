#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "MODBUSConfig.h"





//消息队列创建
void QueueCreate(void);




//二值信号量创建
void BinarySemaphoreCreate(void);


//信号量创建
void SempaphoreCreate(void);

//任务通讯机制创建
void OS_AppObjCreate(void);

#endif 







