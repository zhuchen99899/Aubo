#ifndef __MODBUS_SLAVE_H
#define __MODBUS_SLAVE_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h" 
#include "MODBUSconfig.h"
#include "crc.h"
#include "modbus_packet.h"





int MODS_Poll(u8 *RS485_rec_buffer,int RS485_rec_Len);
ModbusSlaveSolve_T MODS_03H(u8 *RxBuf,int Rxlen);

#endif





