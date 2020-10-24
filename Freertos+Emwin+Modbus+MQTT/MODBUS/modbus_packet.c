#include "modbus_packet.h"



extern VAR_T g_tVar;
/*
*********************************************************************************************************
*	�� �� ��: MODS_ReadRegValue
*	����˵��: ��ȡ���ּĴ�����ֵ
*	��    ��: reg_addr �Ĵ�����ַ
*			  reg_value ��żĴ������
*	�� �� ֵ: 1��ʾOK 0��ʾ����
*********************************************************************************************************
*/
uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
	uint16_t value;
	
	switch (reg_addr)									/* �жϼĴ�����ַ */
	{
		case SLAVE_REG_P01:
			value =	g_tVar.P01;	
			break;

		case SLAVE_REG_P02:
			value =	g_tVar.P02;							/* ���Ĵ���ֵ���� */
			break;
	
		default:
			return 0;									/* �����쳣������ 0 */
	}

	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											/* ��ȡ�ɹ� */
}




/*
*********************************************************************************************************
*	�� �� ��: MODS_WriteRegValue
*	����˵��: ��ȡ���ּĴ�����ֵ
*	��    ��: reg_addr �Ĵ�����ַ
*			  reg_value �Ĵ���ֵ
*	�� �� ֵ: 1��ʾOK 0��ʾ����
*********************************************************************************************************
*/
uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value)
{
	switch (reg_addr)							/* �жϼĴ�����ַ */
	{	
		case SLAVE_REG_P01:
			g_tVar.P01 = reg_value;				/* ��ֵд�뱣��Ĵ��� */
			break;
		
		case SLAVE_REG_P02:
			g_tVar.P02 = reg_value;				/* ��ֵд�뱣��Ĵ��� */
			break;
		
		default:
			return 0;		/* �����쳣������ 0 */
	}

	return 1;		/* ��ȡ�ɹ� */
}






/*
*********************************************************************************************************
*	�� �� ��: MODS_SendWithCRC
*	����˵��: ����һ������, �Զ�׷��2�ֽ�CRC
*	��    ��: _pBuf ���ݣ�
*			  _ucLen ���ݳ��ȣ�����CRC��
*	�� �� ֵ: ��
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
	
#if 1									/* �˲���Ϊ�˴��ڴ�ӡ���,ʵ�������пɲ�Ҫ */
	
	
		for (i = 0; i < RS485_send_Len; i++)
	{
		printf(" %02X", buf[i]);
	}

#endif
}




/*
*********************************************************************************************************
*	�� �� ��: MODS_SendAckErr
*	����˵��: ���ʹ���Ӧ��
*	��    ��: _ucErrCode : �������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MODS_SendAckErr(uint8_t _ucErrCode,u8 *RS485_rec_buffer)
{
	uint8_t txbuf[3];

	txbuf[0] = SADDR485;	/* 485��ַ */
	txbuf[1] = RS485_rec_buffer[1] | 0x80;				/* �쳣�Ĺ����� */
	txbuf[2] = _ucErrCode;							/* �������(01,02,03,04) */
	
	MODS_SendWithCRC(txbuf, 3);
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_SendAckOk
*	����˵��: ������ȷ��Ӧ��.
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: BEBufToUint16
*	����˵��: ��2�ֽ�����(���Big Endian���򣬸��ֽ���ǰ)ת��Ϊ16λ����
*	��    ��: _pBuf : ����
*	�� �� ֵ: 16λ����ֵ
*
*   ���(Big Endian)��С��(Little Endian)
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}