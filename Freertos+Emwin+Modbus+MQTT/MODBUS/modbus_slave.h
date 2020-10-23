#ifndef __MODBUS_SLAVE_H
#define __MODBUS_SLAVE_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h" 
#include "RS485.h"
#include "MODBUSconfig.h"
#include "crc.h"



void MODS_Poll(u8 *RS485_rec_buffer,int RS485_rec_Len,uint8_t *RS485_send_buffer, int RS485_send_Len);

#endif





