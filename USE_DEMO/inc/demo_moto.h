


#ifndef __DEMO_MOTE_H
#define __DEMO_MOTE_H




#include "stm32f4xx.h"

extern int32_t g_set_position;
void Moto_Init(void);
void Moto_1ms_task(void);
void Moto_Set_PID(float kp, float ki, float kd);


#endif 





























