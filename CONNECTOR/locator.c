/**
  ******************************************************************************
  * @file    locator
  * @author  Shengnan Wang
  * @version 1.0.0
  * @date    2019.12.2
  * @brief   定位器接收
  ******************************************************************************
  */
#include "locator.h"


TYPE_Locator_t g_Locator_t;
uint8_t g_Locator_Online_Flag = 0;
uint8_t g_Locator_Frame_Num = 0;

void Locator_Uart_Recive_Task(uint8_t RxData)
{
    static uint8_t rxBuff[30];
    static TYPE_Pack_Info_t locator_Pack_Info = 
    {24, 0x0D, 0x0A, 0x0A, 0x0D, rxBuff};

    if (Pack_Deal_API_Read_Pack(&locator_Pack_Info, RxData))
    {
        //接收到一个数据包
        //强制转换 赋值
        g_Locator_t = *((pTYPE_Locator_t) rxBuff);
        g_Locator_Frame_Num ++;
    }
}

void Locator_Frame_Count_Task_1s(void)
{
	if (g_Locator_Frame_Num < 5)
	{
         if(g_Locator_Online_Flag == 0x00)
        {
            g_Locator_Online_Flag = 0x00;
        }
        else if(g_Locator_Online_Flag == 0x01)
        {
            g_Locator_Online_Flag = 0x01;
        }
        else if(g_Locator_Online_Flag == 0x80)
        {
            g_Locator_Online_Flag = 0x01;
        }
        else if(g_Locator_Online_Flag == 0x40)
        {
            g_Locator_Online_Flag = 0x01;
        }
        else 
        {
            g_Locator_Online_Flag = 0x08;
        }
        LED_Control_API(0, LED_OUT, 0, 0, 0);
        
	}
	else
	{
        if(g_Locator_Online_Flag == 0x00)
        {
            g_Locator_Online_Flag = 0x80;
        }
        else if(g_Locator_Online_Flag == 0x01)
        {
            g_Locator_Online_Flag = 0x40;
        }
        else if(g_Locator_Online_Flag == 0x80)
        {
            g_Locator_Online_Flag = 0x80;
        }
        else if(g_Locator_Online_Flag == 0x40)
        {
            g_Locator_Online_Flag = 0x40;
        }
        else 
        {
            g_Locator_Online_Flag = 0x08;
        }  
        LED_Control_API(0, LED_BRIGHT, 0, 0, 0);
       
	}
	locator_printf("    Locator frame_num:  %d\r\n", g_Locator_Frame_Num);
    g_Locator_Frame_Num = 0;
}


void Locator_Reset(void)
{
    Serial_SendString(Serial3, "ACT0");
}
void    Locator_SetXY(float x, float y)
{
    char s[15] = {0};
    uint8_t i;
    s[0] = 'A',s[1] = 'C',s[2] = 'T',s[3] = 'D';
    x = x * 50.f / 39.4477f * 20;
    y = y * 50.f / 39.4477f * 20;
    *((float*)(&s[4])) = x;
    *((float*)(&s[8])) = y;
    
  //locator_printf("%f   %f\r\n", x, *((float*)(&s[4])));
    for(i = 0; i < 12;i++)
    {
        Serial_SendByte(Serial3, s[i]);
    }
  
}

void Locator_Test(void)
{
    
    locator_printf("Yaw;     %f\r\n", g_Locator_t.Yaw);
    locator_printf("Pitch;   %f\r\n", g_Locator_t.Pitch);
    locator_printf("Row;     %f\r\n", g_Locator_t.Row);
    locator_printf("xVal;    %f\r\n", g_Locator_t.xVal);
    locator_printf("yVal;    %f\r\n", g_Locator_t.yVal);
    locator_printf("Omega;   %f\r\n", g_Locator_t.Omega);
    locator_printf("\r\n");
}
     
     
     























