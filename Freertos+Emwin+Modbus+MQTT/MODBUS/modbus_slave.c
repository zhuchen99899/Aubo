//#include "modbus_slave.h"


////Modbus报文状态
//	uint8_t RspCode;

//static void MODS_SendWithCRC(u8 *RS485_send_buffer, int RS485_send_Len);
//static void MODS_SendAckOk(u8 *RS485_rec_buffer);
//static void MODS_SendAckErr(uint8_t _ucErrCode,u8 *RS485_rec_buffer);


////static void MODS_AnalyzeApp(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len);



////static void MODS_01H(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len);



///*
//*********************************************************************************************************
//*	函 数 名: GetTregState
//*	功能说明: 获取输入寄存器状态
//*	形    参: _pBuf : 数组
//*	返 回 值: 16位整数值
//*
//*   大端(Big Endian)与小端(Little Endian)
//*********************************************************************************************************
//*/
//void bsp_GetTregState()
//{


//}


///*
//*********************************************************************************************************
//*	函 数 名: BEBufToUint16
//*	功能说明: 将2字节数组(大端Big Endian次序，高字节在前)转换为16位整数
//*	形    参: _pBuf : 数组
//*	返 回 值: 16位整数值
//*
//*   大端(Big Endian)与小端(Little Endian)
//*********************************************************************************************************
//*/
//uint16_t BEBufToUint16(uint8_t *_pBuf)
//{
//    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
//}

///*
//*********************************************************************************************************
//*	函 数 名: MODS_SendWithCRC
//*	功能说明: 发送一串数据, 自动追加2字节CRC
//*	形    参: _pBuf 数据；
//*			  _ucLen 数据长度（不带CRC）
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void MODS_SendWithCRC(u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	uint16_t crc;
//	uint8_t buf[S_TX_BUF_SIZE];

//	memcpy(buf, RS485_send_buffer, RS485_send_Len);
//	crc = CRC16_Modbus(RS485_send_buffer, RS485_send_Len);
//	buf[RS485_send_Len++] = crc >> 8;
//	buf[RS485_send_Len++] = crc;

//	RS485_send(buf, RS485_send_Len);
//	
//#if 1									/* 此部分为了串口打印结果,实际运用中可不要 */
//	printf("MODBUS从站发送附带CRC数据:%s\r\n",buf);
//#endif
//}


///*
//*********************************************************************************************************
//*	函 数 名: MODS_SendAckErr
//*	功能说明: 发送错误应答
//*	形    参: _ucErrCode : 错误代码
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void MODS_SendAckErr(uint8_t _ucErrCode,u8 *RS485_rec_buffer)
//{
//	uint8_t txbuf[3];

//	txbuf[0] = SADDR485;	/* 485地址 */
//	txbuf[1] = RS485_rec_buffer[1] | 0x80;				/* 异常的功能码 */
//	txbuf[2] = _ucErrCode;							/* 错误代码(01,02,03,04) */
//	
//	MODS_SendWithCRC(txbuf, 3);
//}

///*
//*********************************************************************************************************
//*	函 数 名: MODS_SendAckOk
//*	功能说明: 发送正确的应答.
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void MODS_SendAckOk(u8 *RS485_rec_buffer)
//{
//	uint8_t txbuf[6];
//	uint8_t i;

//	for (i = 0; i < 6; i++)
//	{
//		txbuf[i] = RS485_rec_buffer[i];
//	}
//	MODS_SendWithCRC(txbuf, 6);
//}


///*
//*********************************************************************************************************
//*	函 数 名: MODS_Poll
//*	功能说明: 解析数据包.进行CRC校验，并确认从站地址
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void MODS_Poll(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	uint16_t addr;
//	uint16_t crc1;


//	if (RS485_rec_Len < 4)				/* 接收到的数据小于4个字节就认为错误 */
//	{
//		goto err_ret;
//	}

//	/* 计算CRC校验和 */
//	crc1 = CRC16_Modbus(RS485_rec_buffer, RS485_rec_Len);
//	if (crc1 != 0)
//	{
//		goto err_ret;
//	}

//	/* 站地址 (1字节） */
//	addr = RS485_rec_buffer[0];				/* 第1字节 站号 */
//	if (addr != SADDR485)		 			/* 判断主机发送的命令地址是否符合 */
//	{
//		goto err_ret;
//	}

//	/* 分析应用层协议 */
//	MODS_AnalyzeApp(RS485_rec_buffer,RS485_rec_Len,RS485_send_buffer, RS485_send_Len);						
//	
//err_ret:
//#if 1										/* 此部分为了串口打印结果,实际运用中可不要 */
//	printf("Modbus_Poll解析MODBUS数据失败,485数据缓冲区数据为:%s",RS485_rec_buffer);
//#endif
//	

//}




///*
//*********************************************************************************************************
//*	函 数 名: MODS_AnalyzeApp
//*	功能说明: 分析应用层协议
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void MODS_AnalyzeApp(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	switch (RS485_rec_buffer[1])				/* 第2个字节 功能码 */
//	{
//		case 0x01:							/* 读取线圈状态（此例程用led代替）*/
//			MODS_01H(RS485_rec_buffer,RS485_rec_Len,RS485_send_buffer,RS485_send_Len);
//			printf("01功能处理");
//			break;

////		case 0x02:							/* 读取输入状态（按键状态）*/
////			MODS_02H();
////			bsp_PutMsg(MSG_MODS_02H, 0);
////			break;
////		
////		case 0x03:							/* 读取保持寄存器（此例程存在g_tVar中）*/
////			MODS_03H();
////			bsp_PutMsg(MSG_MODS_03H, 0);
////			break;
////		
////		case 0x04:							/* 读取输入寄存器（ADC的值）*/
////			MODS_04H();
////			bsp_PutMsg(MSG_MODS_04H, 0);
////			break;
////		
////		case 0x05:							/* 强制单线圈（设置led）*/
////			MODS_05H();
////			bsp_PutMsg(MSG_MODS_05H, 0);
////			break;
////		
////		case 0x06:							/* 写单个保存寄存器（此例程改写g_tVar中的参数）*/
////			MODS_06H();	
////			bsp_PutMsg(MSG_MODS_06H, 0);
////			break;
////			
////		case 0x10:							/* 写多个保存寄存器（此例程存在g_tVar中的参数）*/
////			MODS_10H();
////			bsp_PutMsg(MSG_MODS_10H, 0);
////			break;
////		
//		default:
//			RspCode = RSP_ERR_CMD;
//			MODS_SendAckErr(RspCode,RS485_rec_buffer);	/* 告诉主机命令错误 */
//			break;
//	}
//}




/*
*********************************************************************************************************
*	函 数 名: MODS_02H
*	功能说明: 读取输入状态（对应K01～K03）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//static void MODS_02H(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	/*
//		主机发送:
//			11 从机地址
//			02 功能码
//			00 寄存器地址高字节
//			C4 寄存器地址低字节
//			00 寄存器数量高字节
//			16 寄存器数量低字节
//			BA CRC校验高字节
//			A9 CRC校验低字节

//		从机应答:  响应各离散输入寄存器状态，分别对应数据区中的每位值，1 代表ON；0 代表OFF。
//		           第一个数据字节的LSB(最低字节)为查询的寻址地址，其他输入口按顺序在该字节中由低字节
//		           向高字节排列，直到填充满8位。下一个字节中的8个输入位也是从低字节到高字节排列。
//		           若返回的输入位数不是8的倍数，则在最后的数据字节中的剩余位至该字节的最高位使用0填充。
//			11 从机地址
//			02 功能码
//			03 返回字节数
//			AC 数据1(00C4H-00CBH)
//			DB 数据2(00CCH-00D3H)
//			35 数据3(00D4H-00D9H)
//			20 CRC校验高字节
//			18 CRC校验低字节

//		例子:
//		01 02 20 01 00 08  23CC  ---- 读取T01-08的状态
//		01 02 20 04 00 02  B3CA  ---- 读取T04-05的状态
//		01 02 20 01 00 12  A207   ---- 读 T01-18
//	*/

//	uint16_t reg;
//	uint16_t num;
//	uint16_t i;
//	uint16_t m;
//	uint8_t status[10];

//	RspCode = RSP_OK;

//	if (RS485_rec_Len != 8)
//	{
//		RspCode = RSP_ERR_VALUE;				/* 数据值域错误 */
//		return;
//	}

//	reg = BEBufToUint16(&RS485_rec_buffer[2]); 			/* 寄存器号 */
//	num = BEBufToUint16(&RS485_rec_buffer[4]);				/* 寄存器个数 */

//	m = (num + 7) / 8;
//	if ((reg >= REG_T01) && (num > 0) && (reg + num <= REG_TXX + 1))
//	{
//		for (i = 0; i < m; i++)
//		{
//			status[i] = 0;
//		}
//		for (i = 0; i < num; i++)
//		{
//			if (bsp_GetKeyState((KEY_ID_E)(KID_K1 + reg - REG_T01 + i)))
//			{
//				status[i / 8] |= (1 << (i % 8));
//			}
//		}
//	}
//	else
//	{
//		RspCode = RSP_ERR_REG_ADDR;				/* 寄存器地址错误 */
//	}

//	if (RspCode == RSP_OK)						/* 正确应答 */
//	{
//		RS485_send_Len = 0;
//		RS485_send_buffer[RS485_send_Len] = RS485_rec_buffer[0];
//		RS485_send_buffer[RS485_send_Len] = RS485_rec_buffer[1];
//		RS485_send_buffer[RS485_send_Len] = m;			/* 返回字节数 */

//		for (i = 0; i < m; i++)
//		{
//			RS485_send_buffer[RS485_send_Len] = status[i];	/* T01-02状态 */
//		}
//		MODS_SendWithCRC(RS485_send_buffer, RS485_send_Len);
//	}
//	else
//	{
//		MODS_SendAckErr(RspCode,RS485_rec_buffer);				/* 告诉主机命令错误 */
//	}
//}





///*
//*********************************************************************************************************
//*	函 数 名: MODS_01H
//*	功能说明: 读取线圈状态（对应远程开关D01/D02/D03）
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
///* 说明:这里用LED代替继电器,便于观察现象 */
//static void MODS_01H(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	/*
//	 举例：
//		主机发送:
//			11 从机地址
//			01 功能码
//			00 寄存器起始地址高字节
//			13 寄存器起始地址低字节
//			00 寄存器数量高字节
//			25 寄存器数量低字节
//			0E CRC校验高字节
//			84 CRC校验低字节

//		从机应答: 	1代表ON，0代表OFF。若返回的线圈数不为8的倍数，则在最后数据字节未尾使用0代替. BIT0对应第1个
//			11 从机地址
//			01 功能码
//			05 返回字节数
//			CD 数据1(线圈0013H-线圈001AH)
//			6B 数据2(线圈001BH-线圈0022H)
//			B2 数据3(线圈0023H-线圈002AH)
//			0E 数据4(线圈0032H-线圈002BH)
//			1B 数据5(线圈0037H-线圈0033H)
//			45 CRC校验高字节
//			E6 CRC校验低字节

//		例子:
//			01 01 10 01 00 03   29 0B	--- 查询D01开始的3个继电器状态
//			01 01 10 03 00 01   09 0A   --- 查询D03继电器的状态
//	*/
//	uint16_t reg;
//	uint16_t num;
//	uint16_t i;
//	uint16_t m;
//	uint8_t status[10];
//	

//		RspCode = RSP_OK;
//	/* 没有外部继电器，直接应答错误 */
//	if (RS485_rec_Len != 8)
//	{
//		RspCode = RSP_ERR_VALUE;				/* 数据值域错误 */
//		return;
//	}

//	reg = BEBufToUint16(&RS485_rec_buffer[2]); 			/* 寄存器号 */
//	num = BEBufToUint16(&RS485_rec_buffer[4]);				/* 寄存器个数 */

//	m = (num + 7) / 8;
//	
//	if ((reg >= REG_D01) && (num > 0) && (reg + num <= REG_DXX + 1))
//	{
//		for (i = 0; i < m; i++)
//		{
//			status[i] = 0;
//		}
//		for (i = 0; i < num; i++)
//		{
//			if (bsp_IsLedOn(i + 1 + reg - REG_D01))		/* 读LED的状态，写入状态寄存器的每一位 */
//			{  
//				status[i / 8] |= (1 << (i % 8));
//			}
//		}
//	}
//	else
//	{
//		RspCode = RSP_ERR_REG_ADDR;				/* 寄存器地址错误 */
//	}

//	if (RspCode == RSP_OK)						/* 正确应答 */
//	{
//		RS485_send_Len = 0;
//		RS485_send_buffer[RS485_send_Len++] = RS485_rec_buffer[0];
//		RS485_send_buffer[RS485_send_Len++] = RS485_rec_buffer[1];
//		RS485_send_buffer[RS485_send_Len++] = m;			/* 返回字节数 */

//		for (i = 0; i < m; i++)
//		{
//			RS485_send_buffer[RS485_send_Len++] = status[i];	/* 继电器状态 */
//		}
//		MODS_SendWithCRC(RS485_send_buffer, RS485_send_Len);
//	}
//	else
//	{
//		MODS_SendAckErr(RspCode,RS485_rec_buffer);				/* 告诉主机命令错误 */
//	}
//}


