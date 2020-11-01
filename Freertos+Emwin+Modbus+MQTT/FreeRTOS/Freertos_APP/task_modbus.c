#include "task_modbus.h"

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
extern QueueHandle_t RS485_buffer_Queue; //rs485消息队列句柄



typedef struct RS485buff
{
u32 RS485_lenth;
u8 RS485_buffer[256];
}RS485buff;

//modbus_RTU任务
void modbus_task(void *p_arg)
{

	ModbusSlaveSolve_T ModbusResult;
	u8 PollCRC=0;

	/**********485总线数据缓冲结构体**********/
	RS485buff *RS485receive;
		/***************信号量参数************/
	BaseType_t err=pdFALSE;
	

	//测试数组


		while(1)
		{
	

			err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//获取串口2空闲中断二值信号量
				if(err==pdTRUE)										//获取信号量成功
			{
			
				printf("485总线收到数据\r\n");
			  xQueuePeek(RS485_buffer_Queue,(void *)&RS485receive,portMAX_DELAY);
				PollCRC=MODS_Poll(RS485receive->RS485_buffer,RS485receive->RS485_lenth);
				if(PollCRC)
				{
				printf("CRC校验正常,分析功能码中\r\n");
					switch (RS485receive->RS485_buffer[1])
					{
						case 0x01:							/* 读取线圈状态（此例程用led代替）*/
							printf("暂无功能码01应用程序\r\n");
							break;
						case 0x02:							/* 读取输入状态（按键状态）*/
							printf("暂无功能码02应用程序\r\n");
							break;
						case 0x03:							/* 读取保持寄存器（此例程存在g_tVar中）*/
							printf("功能码为03，分析报文，并准备回复:\r\n");
							//MODS_03H中调用消息队列函数取出寄存器值读取
						ModbusResult=MODS_03H(RS485receive->RS485_buffer,RS485receive->RS485_lenth);
						if(ModbusResult.RspCode == RSP_OK)
						{
							printf("报文检查无误，从站回复：");
								
						MODS_SendWithCRC(ModbusResult.RS485_send_Buf,ModbusResult.RS485_send_Len);
						
						}
						else
						{
							printf("主站发送的功能码03Modbus报文出现错误\r\n");
						MODS_SendAckErr(ModbusResult.RspCode,RS485receive->RS485_buffer);
						}
						
						
							break;
						case 0x04:							/* 读取输入寄存器（ADC的值）*/
						printf("暂无功能码04应用程序\r\n");
							break;
						case 0x05:							/* 强制单线圈（设置led）*/
						printf("暂无功能码05应用程序\r\n");
						break;
						case 0x06:							/* 写单个保存寄存器（此例程改写g_tVar中的参数）*/
						printf("暂无功能码06应用程序\r\n");
						break;
						case 0x07:							/* 写多个保存寄存器（此例程存在g_tVar中的参数）*/
						printf("暂无功能码07应用程序\r\n");
						break;
						
						default:
							
						ModbusResult.RspCode=RSP_ERR_CMD;
						MODS_SendAckErr(ModbusResult.RspCode,RS485receive->RS485_buffer);	/* 告诉主机命令错误 */
						break;
					}
					
					
					
					
					
					
					
					
					
					
				}//ifPoll
				
				
				
				
				
				//RS485_send(test,sizeof(test));
				
				
				
				
				
				
				
			
			}//接收485串口空闲中断传来信号量
			
			
			
			vTaskDelay(500);
		}
		
		
}
