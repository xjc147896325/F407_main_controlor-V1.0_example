
/**
  ******************************************************************************
  * @file    data_send.c
  * @author  Shengnan Wang
  * @version v1.0.0
  * @date    2019.12.3
  * @brief   发送数据到上位机
  ******************************************************************************
  */

#include "data_send.h"


TYPE_Data_t g_Data_t = {0};

void Data_Send_Task(void)
{
    static uint8_t buff[50];
    static TYPE_Pack_Info_t Data_Info = {
        16, 0x0D, 0x0B, 0x0B, 0x0D, buff};
    
    Pack_Deal_API_Data_Pack(&Data_Info,(uint8_t *) &g_Data_t);
    while(Data_Info.i < Data_Info.count)
    {
       while(USART_GetFlagStatus(DATA_SEND_USART, USART_FLAG_TC) != SET);
       USART_SendData(DATA_SEND_USART, Data_Info.pack[Data_Info.i ++]);
    } 
}


void Data_Send_API_SetData( float data1,
                            float data2,
                            float data3,
                            float data4)           
{
    
    g_Data_t.data1 = data1;
    g_Data_t.data2 = data2;
    g_Data_t.data3 = data3;
    g_Data_t.data4 = data4;
}
    
    
    
    
    
    
    
    
    
























