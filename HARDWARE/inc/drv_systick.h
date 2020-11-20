#ifndef __DRV_SYSTICK_H
#define __DRV_SYSTICK_H 			   
#include "stm32f4xx.h"	  


#define delay delay_ms
#define SysTick_Init delay_init

#define SYSTEM_SUPPORT_FREE_RTOS 0


void delay_init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
void delay_xms(uint16_t nms); //无任务调度 延时ms

#if SYSTEM_SUPPORT_FREE_RTOS
extern void xPortSysTickHandler(void);
#endif

#endif





























