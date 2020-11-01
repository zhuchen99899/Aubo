#include "modbus_slave.h"








//Modbus���ر���״̬
ModbusSlaveSolve_T ModbusResult;

VAR_T g_tVar;






/*
*********************************************************************************************************
*	�� �� ��: MODS_Poll
*	����˵��: �������ݰ�.����CRCУ�飬��ȷ�ϴ�վ��ַ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int MODS_Poll(u8 *RS485_rec_buffer,int RS485_rec_Len)
{
	uint16_t addr;
	uint16_t crc1;
	int i;
	int ret=0;

	if (RS485_rec_Len < 4)				/* ���յ�������С��4���ֽھ���Ϊ���� */
	{
		goto err_ret;
	}

	/* ����CRCУ��� */
	crc1 = CRC16_Modbus(RS485_rec_buffer, RS485_rec_Len);
	if (crc1 != 0)
	{
		goto err_ret;
	}

	/* վ��ַ (1�ֽڣ� */
	addr = RS485_rec_buffer[0];				/* ��1�ֽ� վ�� */
	if (addr != SADDR485)		 			/* �ж��������͵������ַ�Ƿ���� */
	{
		goto err_ret;
	}

	
	
	//���ݰ���վ��ַ��CRCУ������
	ret=1;
	return  ret;
	
err_ret:
#if 1										/* �˲���Ϊ�˴��ڴ�ӡ���,ʵ�������пɲ�Ҫ */
	printf("Modbus_Poll����MODBUS����ʧ��,ԭ��Ϊ������������ַ��CRCУ��ʧ��");
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
*	�� �� ��: MODS_03H
*	����˵��: ��ȡ���ּĴ��� ��һ���������ּĴ�����ȡ�õ�ǰ�Ķ�����ֵ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
ModbusSlaveSolve_T MODS_03H(u8 *RxBuf,int Rxlen)
{
	/*
		�ӻ���ַΪ11H�����ּĴ�������ʼ��ַΪ006BH��������ַΪ006DH���ôβ�ѯ�ܹ�����3�����ּĴ�����

		��������:
			11 �ӻ���ַ
			03 ������
			00 �Ĵ�����ַ���ֽ�
			6B �Ĵ�����ַ���ֽ�
			00 �Ĵ����������ֽ�
			03 �Ĵ����������ֽ�
			76 CRC���ֽ�
			87 CRC���ֽ�

		�ӻ�Ӧ��: 	���ּĴ����ĳ���Ϊ2���ֽڡ����ڵ������ּĴ������ԣ��Ĵ������ֽ������ȱ����䣬
					���ֽ����ݺ󱻴��䡣���ּĴ���֮�䣬�͵�ַ�Ĵ����ȱ����䣬�ߵ�ַ�Ĵ����󱻴��䡣
			11 �ӻ���ַ
			03 ������
			06 �ֽ���
			00 ����1���ֽ�(006BH)
			6B ����1���ֽ�(006BH)
			00 ����2���ֽ�(006CH)
			13 ����2 ���ֽ�(006CH)
			00 ����3���ֽ�(006DH)
			00 ����3���ֽ�(006DH)
			38 CRC���ֽ�
			B9 CRC���ֽ�

		����:
			01 03 30 06 00 01  6B0B      ---- �� 3006H, ��������
			01 03 4000 0010 51C6         ---- �� 4000H ������1����ӿ��¼ 32�ֽ�
			01 03 4001 0010 0006         ---- �� 4001H ������1����ӿ��¼ 32�ֽ�

			01 03 F000 0008 770C         ---- �� F000H ������1���澯��¼ 16�ֽ�
			01 03 F001 0008 26CC         ---- �� F001H ������2���澯��¼ 16�ֽ�

			01 03 7000 0020 5ED2         ---- �� 7000H ������1�����μ�¼��1�� 64�ֽ�
			01 03 7001 0020 0F12         ---- �� 7001H ������1�����μ�¼��2�� 64�ֽ�

			01 03 7040 0020 5F06         ---- �� 7040H ������2�����μ�¼��1�� 64�ֽ�
	*/
	uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint8_t reg_value[64];

	ModbusResult.RspCode = RSP_OK;

	if (Rxlen != 8)								/* 03H���������8���ֽ� */
	{
		ModbusResult.RspCode = RSP_ERR_VALUE;					/* ����ֵ����� */
		goto err_ret;
	}

	reg = BEBufToUint16(&RxBuf[2]); 				/* �Ĵ����� */
	num = BEBufToUint16(&RxBuf[4]);					/* �Ĵ������� */
	if (num > sizeof(reg_value) / 2)
	{
		ModbusResult.RspCode = RSP_ERR_VALUE;					/* ����ֵ����� */
		goto err_ret;
	}

	for (i = 0; i < num; i++)
	{
		if (MODS_ReadRegValue(reg, &reg_value[2 * i]) == 0)	/* �����Ĵ���ֵ����reg_value */
		{
			ModbusResult.RspCode = RSP_ERR_REG_ADDR;				/* �Ĵ�����ַ���� */
			break;
		}
		reg++;
	}
	
	if (ModbusResult.RspCode == RSP_OK)							/* ��ȷӦ�� */
	{
		ModbusResult.RS485_send_Len = 0;
		ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = RxBuf[0];
		ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = RxBuf[1];
		ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = num * 2;			/* �����ֽ��� */

		for (i = 0; i < num; i++)
		{
			ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = reg_value[2*i];
			ModbusResult.RS485_send_Buf[ModbusResult.RS485_send_Len++] = reg_value[2*i+1];
		}
//		MODS_SendWithCRC(ModbusResult.RS485_send_Buf, g_tModS.TxCount);	/* ������ȷӦ�� */
		return ModbusResult;
	}
	else
	{
//		MODS_SendAckErr(ModbusResult.RspCode);					/* ���ʹ���Ӧ�� */
		ModbusResult.RspCode=BSP_ERR ;
		return ModbusResult;
	}
err_ret:

	return ModbusResult;
}


