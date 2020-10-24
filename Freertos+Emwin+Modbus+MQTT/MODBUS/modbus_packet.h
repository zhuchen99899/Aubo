#ifndef __MODBUS_PACKET_H
#define __MODBUS_PACKET_H
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h" 
#include "MODBUSconfig.h"
#include "crc.h"
#include "RS485.h"
uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value);
uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value);
void MODS_SendWithCRC(u8 *RS485_send_buffer, int RS485_send_Len);
void MODS_SendAckErr(uint8_t _ucErrCode,u8 *RS485_rec_buffer);
void MODS_SendAckOk(u8 *RS485_rec_buffer);
uint16_t BEBufToUint16(uint8_t *_pBuf);

#endif





