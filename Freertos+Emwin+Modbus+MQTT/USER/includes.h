#ifndef __INCLUDES_H
#define __INCLUDES_H	 





/***********************************
bsp 驱动
************************************/
#include "led.h"
#include "timer.h"
#include "ILI93xx.h"
#include "key.h"
#include "timer.h"
#include "sram.h"
#include "24cxx.h"
#include "touch.h"
#include "malloc.h"
#include "w25qxx.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "limits.h"
#include "RS485.h"
#include "WIFI.h"
/***********************************
os应用
************************************/

#include "task_emwin.h"
#include "task_usrif.h"
#include "task_led.h"
#include "task_key.h"
#include "task_modbus.h"
#include "task_MQTT_connect.h"
#include "task_MQTT_pingreq.h"
#include "task_MQTT_subscribe.h"
//任务间通讯机制
#include <OS_AppObjCreate.h>



//freertos配设
#include "FreeRTOSConfig.h"


/***********************************
emwin 应用
************************************/
//ICON 桌面
#include "iconvdesk.h"

//按键控制面板APP
#include "ButtonController.h" 

//emwin渐变色彩库
#include "ST_GUI_Addons.h"




/***********************************
功能库
************************************/


/***********系统功能************/
#include "sys.h"
#include "delay.h"
#include "usart.h"


/***********GUI相关***********/
#include "GUI.h"
#include "WM.h"


/*********MQTT库**************/

#include "MQTTconnect.h"
#include "MQTTpacket.h"
#include "MQTTpingreq.h"
#include "MQTTpublish.h"
#include "MQTTsubscribe.h"
#include "MQTTunpacket.h"


//MQTT配设
#include "MQTT_init.h"
#include "MQTTConfig.h"


/**********CJSON库************/
#include "cJSON.h"


/*********MODBUS库************/
#include "modbus_slave.h"

//modbus配设
#include "MODBUSConfig.h"
//CRC校验
#include "crc.h"



#endif




