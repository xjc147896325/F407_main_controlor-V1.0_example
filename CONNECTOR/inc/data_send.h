#ifndef _DATA_SEND_H_
#define _DATA_SEND_H_


#include "sys.h"
#include "stm32f4xx.h"
#include "pack_deal.h"
#include "led.h"
#include "uart.h"

#define DATA_SEND_USART USART1

typedef struct TYPE_Data_t 
{
    float data1;
    float data2;
    float data3;
    float data4;
}TYPE_Data_t, *pTYPE_Data_t;


extern TYPE_Data_t g_Data_t;


void Data_Send_Task(void);
void Data_Send_API_SetData( float data1,
                            float data2,
                            float data3,
                            float data4);
            



#endif
                            
                            
                            

