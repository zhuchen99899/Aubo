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
								
								printf("KEY1抬起");
											
							
							MODBUS_reg->P01=1;
							MODBUS_reg->P02=1;
							xQueueOverwrite(MODBUS_Var_Queue,(void *)&MODBUS_reg);
							
							break;
							
							
              case KEY_DOWN_K1:       
									printf("KEY1按下");
					
                    break;

              case KEY_UP_K2:       
									
								printf("KEY2抬起");
							
							MODBUS_reg->P01=0;
							MODBUS_reg->P02=0;
							xQueueOverwrite(MODBUS_Var_Queue,(void *)&MODBUS_reg);
                    break;                    

							case KEY_DOWN_K2:       
						
								printf("KEY2按下");
                    break;                    

							case KEY_UP_K3:       
									
								printf("KEY3抬起");
                    break;                    

							case KEY_DOWN_K3: 			
									printf("KEY3按下");		
										break;


                default:
                    break;

							}
						}
				
						
		
		vTaskDelay(10);                           //延时10ms，也就是10个时钟节拍	

     

	}




}





