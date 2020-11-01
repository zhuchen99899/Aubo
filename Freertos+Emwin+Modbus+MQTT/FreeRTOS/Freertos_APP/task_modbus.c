#include "task_modbus.h"

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
extern QueueHandle_t RS485_buffer_Queue; //rs485��Ϣ���о��



typedef struct RS485buff
{
u32 RS485_lenth;
u8 RS485_buffer[256];
}RS485buff;

//modbus_RTU����
void modbus_task(void *p_arg)
{

	ModbusSlaveSolve_T ModbusResult;
	u8 PollCRC=0;

	/**********485�������ݻ���ṹ��**********/
	RS485buff *RS485receive;
		/***************�ź�������************/
	BaseType_t err=pdFALSE;
	

	//��������


		while(1)
		{
	

			err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//��ȡ����2�����ж϶�ֵ�ź���
				if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
			
				printf("485�����յ�����\r\n");
			  xQueuePeek(RS485_buffer_Queue,(void *)&RS485receive,portMAX_DELAY);
				PollCRC=MODS_Poll(RS485receive->RS485_buffer,RS485receive->RS485_lenth);
				if(PollCRC)
				{
				printf("CRCУ������,������������\r\n");
					switch (RS485receive->RS485_buffer[1])
					{
						case 0x01:							/* ��ȡ��Ȧ״̬����������led���棩*/
							printf("���޹�����01Ӧ�ó���\r\n");
							break;
						case 0x02:							/* ��ȡ����״̬������״̬��*/
							printf("���޹�����02Ӧ�ó���\r\n");
							break;
						case 0x03:							/* ��ȡ���ּĴ����������̴���g_tVar�У�*/
							printf("������Ϊ03���������ģ���׼���ظ�:\r\n");
							//MODS_03H�е�����Ϣ���к���ȡ���Ĵ���ֵ��ȡ
						ModbusResult=MODS_03H(RS485receive->RS485_buffer,RS485receive->RS485_lenth);
						if(ModbusResult.RspCode == RSP_OK)
						{
							printf("���ļ�����󣬴�վ�ظ���");
								
						MODS_SendWithCRC(ModbusResult.RS485_send_Buf,ModbusResult.RS485_send_Len);
						
						}
						else
						{
							printf("��վ���͵Ĺ�����03Modbus���ĳ��ִ���\r\n");
						MODS_SendAckErr(ModbusResult.RspCode,RS485receive->RS485_buffer);
						}
						
						
							break;
						case 0x04:							/* ��ȡ����Ĵ�����ADC��ֵ��*/
						printf("���޹�����04Ӧ�ó���\r\n");
							break;
						case 0x05:							/* ǿ�Ƶ���Ȧ������led��*/
						printf("���޹�����05Ӧ�ó���\r\n");
						break;
						case 0x06:							/* д��������Ĵ����������̸�дg_tVar�еĲ�����*/
						printf("���޹�����06Ӧ�ó���\r\n");
						break;
						case 0x07:							/* д�������Ĵ����������̴���g_tVar�еĲ�����*/
						printf("���޹�����07Ӧ�ó���\r\n");
						break;
						
						default:
							
						ModbusResult.RspCode=RSP_ERR_CMD;
						MODS_SendAckErr(ModbusResult.RspCode,RS485receive->RS485_buffer);	/* ��������������� */
						break;
					}
					
					
					
					
					
					
					
					
					
					
				}//ifPoll
				
				
				
				
				
				//RS485_send(test,sizeof(test));
				
				
				
				
				
				
				
			
			}//����485���ڿ����жϴ����ź���
			
			
			
			vTaskDelay(500);
		}
		
		
}
