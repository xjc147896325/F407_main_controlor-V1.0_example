#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "stm32f4xx.h"
#include "sys.h"
#include "pack_deal.h"
#include "uart.h"
#include "led.h"




#define KEY1_R_LOCK_DOWN  0x80 
#define KEY1_R_LOCK_UP    0x40 
#define KEY1_L_LOCK_DOWN  0x20 
#define KEY1_L_LOCK_UP    0x10

#define KEY1_LOCK_MODE    0xF0 
#define KEY1_R_DIR        0x0F
#define KEY1_R_DIR_DOWN   0x08 
#define KEY1_R_DIR_RIGHT  0x04 
#define KEY1_R_DIR_UP     0x02 
#define KEY1_R_DIR_LEFT   0x01 
                               
                               
#define KEY2_L_DIR_DOWN   0x80 
#define KEY2_L_DIR_RIGHT  0x40 
#define KEY2_L_DIR_UP     0x20 
#define KEY2_L_DIR_LEFT   0x10 
                               
#define KEY2_R_WHITE_UP   0x08 
#define KEY2_L_WHITE_UP   0x04 
#define KEY2_R_AXIS       0x02 
#define KEY2_L_AXIS       0x01 

#define joystick_printf   my_printf
#define JOYSTICK_USART  USART2


typedef struct TYPE_Joystick_t
{
		//uint8_t frame_num;
    int16_t Rx; // 01
    int16_t Ry; // 23
    int16_t Lx; // 45
    int16_t Ly; // 67
    
    int16_t P_LUp;   // 
    int16_t P_R;     //
    int16_t P_LDown; //
    
    uint8_t Key1;    //
    uint8_t Key2;    //
}TYPE_Joystick_t, *pTYPE_Joystick_t;

extern TYPE_Joystick_t g_Joystick_t;
extern uint8_t         g_Joystick_Frame_Num;
extern uint8_t         g_Joystick_Online_Flag;
                    // flag :0x00 从未上线
                    //      :0x01 上线后掉线
                    //      :0x80 第一次上线
                    //      :0x40 掉线后上线
                    //      :other err
void    Joystick_Init(void);
void    Joystick_Uart_Recive_Task(uint8_t RxData);
void    Joystick_Test(void);
void    Joystick_Frame_Count_Task_1s(void);
void    Analog_Joystick(pTYPE_Joystick_t pJoystic_t);

#endif

