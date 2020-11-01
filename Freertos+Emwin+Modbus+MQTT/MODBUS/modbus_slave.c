#include "modbus_slave.h"








//Modbus返回报文状态
ModbusSlaveSolve_T ModbusResult;

VAR_T g_tVar;






/*
*********************************************************************************************************
*	函 数 名: MODS_Poll
*	功能说明: 解析数据包.进行CRC校验，并确认从站地址
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int MODS_Poll(u8 *RS485_rec_buffer,int RS485_rec_Len)
{
	uint16_t addr;
	uint16_t crc1;
	int i;
	int ret=0;

	if (RS485_rec_Len < 4)				/* 接收到的数据小于4个字节就认为错误 */
	{
		goto err_ret;
	}

	/* 计算CRC校验和 */
	crc1 = CRC16_Modbus(RS485_rec_buffer, RS485_rec_Len);
	if (crc1 != 0)
	{
		goto err_ret;
	}

	/* 站地址 (1字节） */
	addr = RS485_rec_buffer[0];				/* 第1字节 站号 */
	if (addr != SADDR485)		 			/* 判断主机发送的命令地址是否符合 */
	{
		goto err_ret;
	}

	
	
	//数据包从站地址，CRC校验正常
	ret=1;
	return  ret;
	
err_ret:
#if 1										/* 此部分为了串口打印结果,实际运用中可不要 */
	printf("Modbus_Poll解析MODBUS数据失败,原因为非主机传呼地址或CRC校验失败");
	for (i = 0; i < RS485_rec_Len; i++)
	{
		printf(" %02X", RS485_rec_buffer[i]);
	}

#endif
	
	ret=0;
	
	return ret;
}





/*
*********************************************************************************************************
*	函 数 名: MODS_03H
*	功能说明: 读取保持寄存器 在一个或多个保持寄存器中取得当前的二进制值
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
ModbusSlaveSolve_T MODS_03H(u8 *RxBuf,int Rxlen)
{
	/*
		从机地址为11H。保持寄存器的起始地址为006BH，结束地址为006DH。该次查询总共访问3个保持寄存器。

		主机发送:
			11 从机地址
			03 功能码
			00 寄存器地址高字节
			6B 寄存器地址低字节
			00 寄存器数量高字节
			03 寄存器数量低字节
			76 CRC高字节
			87 CRC低字节

		从机应答: 	保持寄存器的长度为2个字节。对于单个保持寄存器而言，寄存器高字节数据先被传输，
					低字节数据后被传输。保持寄存器之间，低地址寄存器先被传输，高地址寄存器后被传输。
			11 从机地址
			03 功能码
			06 字节数
			00 数据1高字节(006BH)
			6B 数据1低字节(006BH)
			00 数据2高字节(006CH)
			13 数据2 低字节(006CH)
			00 数据3高字节(006DH)
			00 数据3低字节(006DH)
			38 CRC高字节
			B9 CRC低字节

		例子:
			01 03 30 06 00 01  6B0B      ---- 读 3006H, 触发电流
			01 03 4000 0010 51C6         ---- 读 4000H 倒数第1条浪涌记录 32字节
			01 03 4001 0010 0006         ---- 读 4001H 倒数第1条浪涌记录 32字节

			01 03 F000 0008 770C         ---- 读 F000H 倒数第1条告警记录 16字节
			01 03 F001 0008 26CC         ---- 读 F001H 倒数第2条告警记录 16字节

			01 03 7000 0020 5ED2         ---- 读 7000H 倒数第1条波形记录第1段 64字节
			01 03 7001 0020 0F12         ---- 读 7001H 倒数第1条波形记录第2段 64字节

			01 03 7040 0020 5F06         ---- 读 7040H 倒数第2条波形记录第1段 64字节
	*/
	uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint8_t reg_value[64];

	ModbusResult.RspCode = RSP_OK;

	if (Rxlen != 8)								/* 03H命令必须是8个字节 */
	{
		ModbusResult.RspCode = RSP_ERR_VALUE;					/* 数据值域错误 */
		goto err_ret;
	}

	reg = BEBufToUint16(&RxBuf[2]); 				/* 寄存器号 */
	num = BEBufToUint16(&RxBuf[4]);					/* 寄存器个数 */
	if (num > sizeof(reg_value) / 2)
	{
		ModbusResult.RspCode = RSP_ERR_VALUE;					/* 数据值域错误 */
		goto err_ret;
	}

	for (i = 0; i < num; i++)
	{
		if (MODS_ReadRegValue(reg, &reg_value[2 * i]) == 0)	/* 读出寄存器值放入reg_value */
		{
			ModbusResult.RspCode = RSP_ERR_REG_ADDR;				/* 寄存器地址错误 */
			break;
		}
		reg++;
	}
	
	if (ModbusResult.RspCode == RSP_OK)							/* 正确应答 */
	{
		ModbusResult.RS485_send_Len = 0;
		ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = RxBuf[0];
		ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = RxBuf[1];
		ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = num * 2;			/* 返回字节数 */

		for (i = 0; i < num; i++)
		{
			ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = reg_value[2*i];
			ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = reg_value[2*i+1];
		}
//		MODS_SendWithCRC(ModbusResult.RS485_send_Buf, g_tModS.TxCount);	/* 发送正确应答 */
		return ModbusResult;
	}
	else
	{
//		MODS_SendAckErr(ModbusResult.RspCode);					/* 发送错误应答 */
		ModbusResult.RspCode=BSP_ERR ;
		return ModbusResult;
	}
err_ret:

	return ModbusResult;
}


