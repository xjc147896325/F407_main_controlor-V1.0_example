/**
  ******************************************************************************
  * @file    main.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-03-31
  * @brief   
  ******************************************************************************
  */
#include "main.h"
#include "free_rtos_test.h"
#include "bsp_esc.h"
#include "demo_moto.h"

int main()
{	
//	double i = 0;
//	double Voltage_Data[4];
	int i = 0;
	System_Init();
    // free_rtos();
	// GPIO_Format_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_100MHz);
	Moto_Init();
    g_set_position = 200;
    delay_ms(1000);
	while(1)
	{
        /*
		GPIO_SetBits(GPIOC,GPIO_Pin_0); 
        delay_xms(500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_0); 
        delay_xms(500);*/
        // C620_SendCurrentVal(i, i, i, i);
        if(i > 1000)
        {
            i = 0;
        }
        i ++ ;
        Moto_1ms_task();
        delay_ms(1);
	}
}
