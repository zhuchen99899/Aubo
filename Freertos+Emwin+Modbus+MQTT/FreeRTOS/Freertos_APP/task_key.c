#include "task_key.h"



void key_task(void *pvParameters)
{

u8 ucKeyCode;
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
											break;
							
              case KEY_DOWN_K1:       
									printf("KEY1����");
					
                    break;

              case KEY_UP_K2:       
									
								printf("KEY2̧��");
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





