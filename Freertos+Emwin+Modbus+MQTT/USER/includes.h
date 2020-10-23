#ifndef __INCLUDES_H
#define __INCLUDES_H	 





/***********************************
bsp ����
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
osӦ��
************************************/

#include "task_emwin.h"
#include "task_usrif.h"
#include "task_led.h"
#include "task_key.h"
#include "task_modbus.h"
#include "task_MQTT_connect.h"
#include "task_MQTT_pingreq.h"
#include "task_MQTT_subscribe.h"
//�����ͨѶ����
#include <OS_AppObjCreate.h>



//freertos����
#include "FreeRTOSConfig.h"


/***********************************
emwin Ӧ��
************************************/
//ICON ����
#include "iconvdesk.h"

//�����������APP
#include "ButtonController.h" 

//emwin����ɫ�ʿ�
#include "ST_GUI_Addons.h"




/***********************************
���ܿ�
************************************/


/***********ϵͳ����************/
#include "sys.h"
#include "delay.h"
#include "usart.h"


/***********GUI���***********/
#include "GUI.h"
#include "WM.h"


/*********MQTT��**************/

#include "MQTTconnect.h"
#include "MQTTpacket.h"
#include "MQTTpingreq.h"
#include "MQTTpublish.h"
#include "MQTTsubscribe.h"
#include "MQTTunpacket.h"


//MQTT����
#include "MQTT_init.h"
#include "MQTTConfig.h"


/**********CJSON��************/
#include "cJSON.h"


/*********MODBUS��************/
#include "modbus_slave.h"

//modbus����
#include "MODBUSConfig.h"
//CRCУ��
#include "crc.h"



#endif




