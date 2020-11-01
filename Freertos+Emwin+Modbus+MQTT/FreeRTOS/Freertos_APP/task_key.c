#include "task_key.h"



void key_task(void *pvParameters)
{
extern QueueHandle_t MODBUS_Var_Queue;
VAR_T *MODBUS_reg;
VAR_T init_modbus_reg;
u8 ucKeyCode;
	MODBUS_reg=&init_modbus_reg;
	
	while(1)
	{
				bsp_KeyScan();

        ucKeyCode = bsp_GetKey();
        if (ucKeyCode > 0)
        {
            /* ???? */
            switch (ucKeyCode)
            {
							case KEY_UP_K1:
								
								printf("KEY1̧��");
											
							
							MODBUS_reg->P01=1;
							MODBUS_reg->P02=1;
							xQueueOverwrite(MODBUS_Var_Queue,(void *)&MODBUS_reg);
							
							break;
							
							
              case KEY_DOWN_K1:       
									printf("KEY1����");
					
                    break;

              case KEY_UP_K2:       
									
								printf("KEY2̧��");
							
							MODBUS_reg->P01=0;
							MODBUS_reg->P02=0;
							xQueueOverwrite(MODBUS_Var_Queue,(void *)&MODBUS_reg);
                    break;                    

							case KEY_DOWN_K2:       
						
								printf("KEY2����");
                    break;                    

							case KEY_UP_K3:       
									
								printf("KEY3̧��");
                    break;                    

							case KEY_DOWN_K3: 			
									printf("KEY3����");		
										break;


                default:
                    break;

							}
						}
				
						
		
		vTaskDelay(10);                           //��ʱ10ms��Ҳ����10��ʱ�ӽ���	

     

	}




}





