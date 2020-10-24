#include "modbus_packet.h"



extern VAR_T g_tVar;
/*
*********************************************************************************************************
*	函 数 名: MODS_ReadRegValue
*	功能说明: 读取保持寄存器的值
*	形    参: reg_addr 寄存器地址
*			  reg_value 存放寄存器结果
*	返 回 值: 1表示OK 0表示错误
*********************************************************************************************************
*/
uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
	uint16_t value;
	
	switch (reg_addr)									/* 判断寄存器地址 */
	{
		case SLAVE_REG_P01:
			value =	g_tVar.P01;	
			break;

		case SLAVE_REG_P02:
			value =	g_tVar.P02;							/* 将寄存器值读出 */
			break;
	
		default:
			return 0;									/* 参数异常，返回 0 */
	}

	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											/* 读取成功 */
}




/*
*********************************************************************************************************
*	函 数 名: MODS_WriteRegValue
*	功能说明: 读取保持寄存器的值
*	形    参: reg_addr 寄存器地址
*			  reg_value 寄存器值
*	返 回 值: 1表示OK 0表示错误
*********************************************************************************************************
*/
uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value)
{
	switch (reg_addr)							/* 判断寄存器地址 */
	{	
		case SLAVE_REG_P01:
			g_tVar.P01 = reg_value;				/* 将值写入保存寄存器 */
			break;
		
		case SLAVE_REG_P02:
			g_tVar.P02 = reg_value;				/* 将值写入保存寄存器 */
			break;
		
		default:
			return 0;		/* 参数异常，返回 0 */
	}

	return 1;		/* 读取成功 */
}






/*
*********************************************************************************************************
*	函 数 名: MODS_SendWithCRC
*	功能说明: 发送一串数据, 自动追加2字节CRC
*	形    参: _pBuf 数据；
*			  _ucLen 数据长度（不带CRC）
*	返 回 值: 无
*********************************************************************************************************
*/
void MODS_SendWithCRC(u8 *RS485_send_buffer, int RS485_send_Len)
{
	uint16_t crc;
	uint8_t buf[S_TX_BUF_SIZE];
  int i;
	memcpy(buf, RS485_send_buffer, RS485_send_Len);
	crc = CRC16_Modbus(RS485_send_buffer, RS485_send_Len);
	buf[RS485_send_Len++] = crc >> 8;
	buf[RS485_send_Len++] = crc;

	RS485_send(buf, RS485_send_Len);
	
#if 1									/* 此部分为了串口打印结果,实际运用中可不要 */
	
	
		for (i = 0; i < RS485_send_Len; i++)
	{
		printf(" %02X", buf[i]);
	}

#endif
}




/*
*********************************************************************************************************
*	函 数 名: MODS_SendAckErr
*	功能说明: 发送错误应答
*	形    参: _ucErrCode : 错误代码
*	返 回 值: 无
*********************************************************************************************************
*/
void MODS_SendAckErr(uint8_t _ucErrCode,u8 *RS485_rec_buffer)
{
	uint8_t txbuf[3];

	txbuf[0] = SADDR485;	/* 485地址 */
	txbuf[1] = RS485_rec_buffer[1] | 0x80;				/* 异常的功能码 */
	txbuf[2] = _ucErrCode;							/* 错误代码(01,02,03,04) */
	
	MODS_SendWithCRC(txbuf, 3);
}

/*
*********************************************************************************************************
*	函 数 名: MODS_SendAckOk
*	功能说明: 发送正确的应答.
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MODS_SendAckOk(u8 *RS485_rec_buffer)
{
	uint8_t txbuf[6];
	uint8_t i;

	for (i = 0; i < 6; i++)
	{
		txbuf[i] = RS485_rec_buffer[i];
	}
	MODS_SendWithCRC(txbuf, 6);
}



/*
*********************************************************************************************************
*	函 数 名: BEBufToUint16
*	功能说明: 将2字节数组(大端Big Endian次序，高字节在前)转换为16位整数
*	形    参: _pBuf : 数组
*	返 回 值: 16位整数值
*
*   大端(Big Endian)与小端(Little Endian)
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}
