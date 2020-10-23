//#include "modbus_slave.h"


////Modbus����״̬
//	uint8_t RspCode;

//static void MODS_SendWithCRC(u8 *RS485_send_buffer, int RS485_send_Len);
//static void MODS_SendAckOk(u8 *RS485_rec_buffer);
//static void MODS_SendAckErr(uint8_t _ucErrCode,u8 *RS485_rec_buffer);


////static void MODS_AnalyzeApp(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len);



////static void MODS_01H(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len);



///*
//*********************************************************************************************************
//*	�� �� ��: GetTregState
//*	����˵��: ��ȡ����Ĵ���״̬
//*	��    ��: _pBuf : ����
//*	�� �� ֵ: 16λ����ֵ
//*
//*   ���(Big Endian)��С��(Little Endian)
//*********************************************************************************************************
//*/
//void bsp_GetTregState()
//{


//}


///*
//*********************************************************************************************************
//*	�� �� ��: BEBufToUint16
//*	����˵��: ��2�ֽ�����(���Big Endian���򣬸��ֽ���ǰ)ת��Ϊ16λ����
//*	��    ��: _pBuf : ����
//*	�� �� ֵ: 16λ����ֵ
//*
//*   ���(Big Endian)��С��(Little Endian)
//*********************************************************************************************************
//*/
//uint16_t BEBufToUint16(uint8_t *_pBuf)
//{
//    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
//}

///*
//*********************************************************************************************************
//*	�� �� ��: MODS_SendWithCRC
//*	����˵��: ����һ������, �Զ�׷��2�ֽ�CRC
//*	��    ��: _pBuf ���ݣ�
//*			  _ucLen ���ݳ��ȣ�����CRC��
//*	�� �� ֵ: ��
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
//#if 1									/* �˲���Ϊ�˴��ڴ�ӡ���,ʵ�������пɲ�Ҫ */
//	printf("MODBUS��վ���͸���CRC����:%s\r\n",buf);
//#endif
//}


///*
//*********************************************************************************************************
//*	�� �� ��: MODS_SendAckErr
//*	����˵��: ���ʹ���Ӧ��
//*	��    ��: _ucErrCode : �������
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//static void MODS_SendAckErr(uint8_t _ucErrCode,u8 *RS485_rec_buffer)
//{
//	uint8_t txbuf[3];

//	txbuf[0] = SADDR485;	/* 485��ַ */
//	txbuf[1] = RS485_rec_buffer[1] | 0x80;				/* �쳣�Ĺ����� */
//	txbuf[2] = _ucErrCode;							/* �������(01,02,03,04) */
//	
//	MODS_SendWithCRC(txbuf, 3);
//}

///*
//*********************************************************************************************************
//*	�� �� ��: MODS_SendAckOk
//*	����˵��: ������ȷ��Ӧ��.
//*	��    ��: ��
//*	�� �� ֵ: ��
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
//*	�� �� ��: MODS_Poll
//*	����˵��: �������ݰ�.����CRCУ�飬��ȷ�ϴ�վ��ַ
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void MODS_Poll(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	uint16_t addr;
//	uint16_t crc1;


//	if (RS485_rec_Len < 4)				/* ���յ�������С��4���ֽھ���Ϊ���� */
//	{
//		goto err_ret;
//	}

//	/* ����CRCУ��� */
//	crc1 = CRC16_Modbus(RS485_rec_buffer, RS485_rec_Len);
//	if (crc1 != 0)
//	{
//		goto err_ret;
//	}

//	/* վ��ַ (1�ֽڣ� */
//	addr = RS485_rec_buffer[0];				/* ��1�ֽ� վ�� */
//	if (addr != SADDR485)		 			/* �ж��������͵������ַ�Ƿ���� */
//	{
//		goto err_ret;
//	}

//	/* ����Ӧ�ò�Э�� */
//	MODS_AnalyzeApp(RS485_rec_buffer,RS485_rec_Len,RS485_send_buffer, RS485_send_Len);						
//	
//err_ret:
//#if 1										/* �˲���Ϊ�˴��ڴ�ӡ���,ʵ�������пɲ�Ҫ */
//	printf("Modbus_Poll����MODBUS����ʧ��,485���ݻ���������Ϊ:%s",RS485_rec_buffer);
//#endif
//	

//}




///*
//*********************************************************************************************************
//*	�� �� ��: MODS_AnalyzeApp
//*	����˵��: ����Ӧ�ò�Э��
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//static void MODS_AnalyzeApp(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	switch (RS485_rec_buffer[1])				/* ��2���ֽ� ������ */
//	{
//		case 0x01:							/* ��ȡ��Ȧ״̬����������led���棩*/
//			MODS_01H(RS485_rec_buffer,RS485_rec_Len,RS485_send_buffer,RS485_send_Len);
//			printf("01���ܴ���");
//			break;

////		case 0x02:							/* ��ȡ����״̬������״̬��*/
////			MODS_02H();
////			bsp_PutMsg(MSG_MODS_02H, 0);
////			break;
////		
////		case 0x03:							/* ��ȡ���ּĴ����������̴���g_tVar�У�*/
////			MODS_03H();
////			bsp_PutMsg(MSG_MODS_03H, 0);
////			break;
////		
////		case 0x04:							/* ��ȡ����Ĵ�����ADC��ֵ��*/
////			MODS_04H();
////			bsp_PutMsg(MSG_MODS_04H, 0);
////			break;
////		
////		case 0x05:							/* ǿ�Ƶ���Ȧ������led��*/
////			MODS_05H();
////			bsp_PutMsg(MSG_MODS_05H, 0);
////			break;
////		
////		case 0x06:							/* д��������Ĵ����������̸�дg_tVar�еĲ�����*/
////			MODS_06H();	
////			bsp_PutMsg(MSG_MODS_06H, 0);
////			break;
////			
////		case 0x10:							/* д�������Ĵ����������̴���g_tVar�еĲ�����*/
////			MODS_10H();
////			bsp_PutMsg(MSG_MODS_10H, 0);
////			break;
////		
//		default:
//			RspCode = RSP_ERR_CMD;
//			MODS_SendAckErr(RspCode,RS485_rec_buffer);	/* ��������������� */
//			break;
//	}
//}




/*
*********************************************************************************************************
*	�� �� ��: MODS_02H
*	����˵��: ��ȡ����״̬����ӦK01��K03��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//static void MODS_02H(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	/*
//		��������:
//			11 �ӻ���ַ
//			02 ������
//			00 �Ĵ�����ַ���ֽ�
//			C4 �Ĵ�����ַ���ֽ�
//			00 �Ĵ����������ֽ�
//			16 �Ĵ����������ֽ�
//			BA CRCУ����ֽ�
//			A9 CRCУ����ֽ�

//		�ӻ�Ӧ��:  ��Ӧ����ɢ����Ĵ���״̬���ֱ��Ӧ�������е�ÿλֵ��1 ����ON��0 ����OFF��
//		           ��һ�������ֽڵ�LSB(����ֽ�)Ϊ��ѯ��Ѱַ��ַ����������ڰ�˳���ڸ��ֽ����ɵ��ֽ�
//		           ����ֽ����У�ֱ�������8λ����һ���ֽ��е�8������λҲ�Ǵӵ��ֽڵ����ֽ����С�
//		           �����ص�����λ������8�ı������������������ֽ��е�ʣ��λ�����ֽڵ����λʹ��0��䡣
//			11 �ӻ���ַ
//			02 ������
//			03 �����ֽ���
//			AC ����1(00C4H-00CBH)
//			DB ����2(00CCH-00D3H)
//			35 ����3(00D4H-00D9H)
//			20 CRCУ����ֽ�
//			18 CRCУ����ֽ�

//		����:
//		01 02 20 01 00 08  23CC  ---- ��ȡT01-08��״̬
//		01 02 20 04 00 02  B3CA  ---- ��ȡT04-05��״̬
//		01 02 20 01 00 12  A207   ---- �� T01-18
//	*/

//	uint16_t reg;
//	uint16_t num;
//	uint16_t i;
//	uint16_t m;
//	uint8_t status[10];

//	RspCode = RSP_OK;

//	if (RS485_rec_Len != 8)
//	{
//		RspCode = RSP_ERR_VALUE;				/* ����ֵ����� */
//		return;
//	}

//	reg = BEBufToUint16(&RS485_rec_buffer[2]); 			/* �Ĵ����� */
//	num = BEBufToUint16(&RS485_rec_buffer[4]);				/* �Ĵ������� */

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
//		RspCode = RSP_ERR_REG_ADDR;				/* �Ĵ�����ַ���� */
//	}

//	if (RspCode == RSP_OK)						/* ��ȷӦ�� */
//	{
//		RS485_send_Len = 0;
//		RS485_send_buffer[RS485_send_Len] = RS485_rec_buffer[0];
//		RS485_send_buffer[RS485_send_Len] = RS485_rec_buffer[1];
//		RS485_send_buffer[RS485_send_Len] = m;			/* �����ֽ��� */

//		for (i = 0; i < m; i++)
//		{
//			RS485_send_buffer[RS485_send_Len] = status[i];	/* T01-02״̬ */
//		}
//		MODS_SendWithCRC(RS485_send_buffer, RS485_send_Len);
//	}
//	else
//	{
//		MODS_SendAckErr(RspCode,RS485_rec_buffer);				/* ��������������� */
//	}
//}





///*
//*********************************************************************************************************
//*	�� �� ��: MODS_01H
//*	����˵��: ��ȡ��Ȧ״̬����ӦԶ�̿���D01/D02/D03��
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
///* ˵��:������LED����̵���,���ڹ۲����� */
//static void MODS_01H(u8 *RS485_rec_buffer,int RS485_rec_Len,u8 *RS485_send_buffer, int RS485_send_Len)
//{
//	/*
//	 ������
//		��������:
//			11 �ӻ���ַ
//			01 ������
//			00 �Ĵ�����ʼ��ַ���ֽ�
//			13 �Ĵ�����ʼ��ַ���ֽ�
//			00 �Ĵ����������ֽ�
//			25 �Ĵ����������ֽ�
//			0E CRCУ����ֽ�
//			84 CRCУ����ֽ�

//		�ӻ�Ӧ��: 	1����ON��0����OFF�������ص���Ȧ����Ϊ8�ı�����������������ֽ�δβʹ��0����. BIT0��Ӧ��1��
//			11 �ӻ���ַ
//			01 ������
//			05 �����ֽ���
//			CD ����1(��Ȧ0013H-��Ȧ001AH)
//			6B ����2(��Ȧ001BH-��Ȧ0022H)
//			B2 ����3(��Ȧ0023H-��Ȧ002AH)
//			0E ����4(��Ȧ0032H-��Ȧ002BH)
//			1B ����5(��Ȧ0037H-��Ȧ0033H)
//			45 CRCУ����ֽ�
//			E6 CRCУ����ֽ�

//		����:
//			01 01 10 01 00 03   29 0B	--- ��ѯD01��ʼ��3���̵���״̬
//			01 01 10 03 00 01   09 0A   --- ��ѯD03�̵�����״̬
//	*/
//	uint16_t reg;
//	uint16_t num;
//	uint16_t i;
//	uint16_t m;
//	uint8_t status[10];
//	

//		RspCode = RSP_OK;
//	/* û���ⲿ�̵�����ֱ��Ӧ����� */
//	if (RS485_rec_Len != 8)
//	{
//		RspCode = RSP_ERR_VALUE;				/* ����ֵ����� */
//		return;
//	}

//	reg = BEBufToUint16(&RS485_rec_buffer[2]); 			/* �Ĵ����� */
//	num = BEBufToUint16(&RS485_rec_buffer[4]);				/* �Ĵ������� */

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
//			if (bsp_IsLedOn(i + 1 + reg - REG_D01))		/* ��LED��״̬��д��״̬�Ĵ�����ÿһλ */
//			{  
//				status[i / 8] |= (1 << (i % 8));
//			}
//		}
//	}
//	else
//	{
//		RspCode = RSP_ERR_REG_ADDR;				/* �Ĵ�����ַ���� */
//	}

//	if (RspCode == RSP_OK)						/* ��ȷӦ�� */
//	{
//		RS485_send_Len = 0;
//		RS485_send_buffer[RS485_send_Len++] = RS485_rec_buffer[0];
//		RS485_send_buffer[RS485_send_Len++] = RS485_rec_buffer[1];
//		RS485_send_buffer[RS485_send_Len++] = m;			/* �����ֽ��� */

//		for (i = 0; i < m; i++)
//		{
//			RS485_send_buffer[RS485_send_Len++] = status[i];	/* �̵���״̬ */
//		}
//		MODS_SendWithCRC(RS485_send_buffer, RS485_send_Len);
//	}
//	else
//	{
//		MODS_SendAckErr(RspCode,RS485_rec_buffer);				/* ��������������� */
//	}
//}


