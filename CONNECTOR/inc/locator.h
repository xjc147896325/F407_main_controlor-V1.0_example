#ifndef _LOCATOR_H_
#define _LOCATOR_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "pack_deal.h"
#include "uart.h"

#define locator_printf my_printf

typedef struct TYPE_Locator_t
{
		
    float Yaw;
    float Pitch;
    float Row;
    float xVal;
    float yVal;
    float Omega;

}TYPE_Locator_t, *pTYPE_Locator_t;

extern TYPE_Locator_t g_Locator_t;
extern uint8_t         g_Locator_Frame_Count;
// flag :0x00 从未上线
//      :0x01 上线后掉线
//      :0x80 第一次上线
//      :0x40 掉线后上线
//      :other err
extern uint8_t         g_Locator_Online_Flag;

void    Locator_Uart_Recive_Task(uint8_t RxData);
void    Locator_Test(void);
void    Locator_Frame_Count_Task_1s(void);
void    Locator_Reset(void);
void    Locator_SetXY(float x, float y);

#endif

