/**
  ******************************************************************************
  * @file    joystick
  * @author  Shengnan Wang
  * @version 1.0.0
  * @date    2019.11.28
  * @brief   手柄接收
  ******************************************************************************
  */
    
#include "joystick.h"



TYPE_Joystick_t g_Joystick_t;
uint8_t g_Joystick_Online_Flag = 0x00;
uint8_t g_Joystick_Frame_Num   = 0x00;

void Joystick_Uart_Recive_Task(uint8_t RxData)
{
    static uint8_t rxBuff[22];

    static TYPE_Pack_Info_t joystick_Pack_Info = 
    {16, 0xAA, 0x55, 0x55, 0xAA, rxBuff};

    if (Pack_Deal_API_Read_Pack(&joystick_Pack_Info,RxData))
    {
        //接收到一个数据包
        //强制转换 赋值
        g_Joystick_t = *((pTYPE_Joystick_t) rxBuff);
        g_Joystick_Frame_Num ++;
    }
}
void Joystick_Init(void)
{
   // g_Joystick_Online_Flag = 0;
    
    g_Joystick_Frame_Num   = 0;
    
    g_Joystick_t.Rx      = 0;
    g_Joystick_t.Ry      = 0;
    g_Joystick_t.Lx      = 0;
    g_Joystick_t.Ly      = 0;
                        
    g_Joystick_t.P_LUp   = 0;
    g_Joystick_t.P_R     = 0;
    g_Joystick_t.P_LDown = 0;
                       
    g_Joystick_t.Key1    = 0;
    g_Joystick_t.Key2    = 0;   
}

void Joystick_Frame_Count_Task_1s(void)
{
    g_Joystick_Online_Flag = Online_State_Deal(g_Joystick_Online_Flag, 
                                                g_Joystick_Frame_Num, 15);
	
	if(g_Joystick_Online_Flag == 0x80) 
	{
  
        LED_Control_API(1, LED_BRIGHT, 0, 0, 0);
       
	}
    else if(g_Joystick_Online_Flag == 0x00 || g_Joystick_Online_Flag == 0x01)
    {
        LED_Control_API(1, LED_OUT, 0, 0, 0);
        Joystick_Init();   
	}
    else if(g_Joystick_Online_Flag == 0x40)
    {
        LED_Control_API(1, LED_LIGHT, 2, 200, 0);
    }
    else
    {
        LED_Control_API(1, LED_OUT, 0, 0, 0);
        Joystick_Init();   
        
    }
    
    joystick_printf("    joyStick frame_num:%d\r\n", g_Joystick_Frame_Num);
	g_Joystick_Frame_Num = 0;
}
void Analog_Joystick(pTYPE_Joystick_t pJoystic_t)
{
    static uint8_t txBuff[30];
    static TYPE_Pack_Info_t joystick_Pack_Info = 
    {16, 0xAA, 0x55, 0x55, 0xAA, txBuff};
    // TYPE_Joystick_t my_joystic={0,0,0,0, 0,0,0, 0xB0, 0x00};
    Pack_Deal_API_Data_Pack(&joystick_Pack_Info,(uint8_t *) pJoystic_t);
    
    while(joystick_Pack_Info.i < joystick_Pack_Info.count)
    {
       while(USART_GetFlagStatus(JOYSTICK_USART, USART_FLAG_TC) != SET);
       USART_SendData(JOYSTICK_USART, 
       joystick_Pack_Info.pack[joystick_Pack_Info.i ++]);
    }
}




void Joystick_Test(void)
{
    
     
     joystick_printf("Rx;   %5d  Ry     :%5d,  Lx: %5d,  Ly:%5d\r\n",
                         g_Joystick_t.Rx, g_Joystick_t.Ry,
                         g_Joystick_t.Lx, g_Joystick_t.Ly);
     joystick_printf("P_LUp;%5d  P_LDown:%5d,  p_R:%5d\r\n",
                         g_Joystick_t.P_LUp, g_Joystick_t.P_LDown,
                         g_Joystick_t.P_R);
     joystick_printf("          %x              %x\r\n",
                         g_Joystick_t.Key1, g_Joystick_t.Key2);
     joystick_printf("\r\n");
     
    /*
    joystick_printf("KEY1_R_LOCK_DOWN : %d\r\n", g_Joystick_t.Key1 & KEY1_R_LOCK_DOWN);
    joystick_printf("KEY1_R_LOCK_UP   : %d\r\n", g_Joystick_t.Key1 & KEY1_R_LOCK_UP  );
    joystick_printf("KEY1_L_LOCK_DOWN : %d\r\n", g_Joystick_t.Key1 & KEY1_L_LOCK_DOWN);
    joystick_printf("KEY1_L_LOCK_UP   : %d\r\n", g_Joystick_t.Key1 & KEY1_L_LOCK_UP  );
                                                                                
    joystick_printf("KEY1_R_DIR_DOWN  : %d\r\n", g_Joystick_t.Key1 & KEY1_R_DIR_DOWN );
    joystick_printf("KEY1_R_DIR_RIGHT : %d\r\n", g_Joystick_t.Key1 & KEY1_R_DIR_RIGHT);
    joystick_printf("KEY1_R_DIR_UP    : %d\r\n", g_Joystick_t.Key1 & KEY1_R_DIR_UP   );
    joystick_printf("KEY1_R_DIR_LEFT  : %d\r\n", g_Joystick_t.Key1 & KEY1_R_DIR_LEFT );
    
    joystick_printf("KEY2_L_DIR_DOWN  : %d\r\n", g_Joystick_t.Key2 & KEY2_L_DIR_DOWN );
    joystick_printf("KEY2_L_DIR_RIGHT : %d\r\n", g_Joystick_t.Key2 & KEY2_L_DIR_RIGHT);
    joystick_printf("KEY2_L_DIR_UP    : %d\r\n", g_Joystick_t.Key2 & KEY2_L_DIR_UP   );
    joystick_printf("KEY2_L_DIR_LEFT  : %d\r\n", g_Joystick_t.Key2 & KEY2_L_DIR_LEFT );
                                                                               
    joystick_printf("KEY2_R_WHITE_UP  : %d\r\n", g_Joystick_t.Key2 & KEY2_R_WHITE_UP );
    joystick_printf("KEY2_L_WHITE_UP  : %d\r\n", g_Joystick_t.Key2 & KEY2_L_WHITE_UP );
    joystick_printf("KEY2_R_AXIS      : %d\r\n", g_Joystick_t.Key2 & KEY2_R_AXIS     );
    joystick_printf("KEY2_L_AXIS      : %d\r\n", g_Joystick_t.Key2 & KEY2_L_AXIS     );
    joystick_printf("\r\n");*/
    
}

















