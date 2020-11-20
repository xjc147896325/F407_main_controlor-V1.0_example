/**
  ******************************************************************************
  * @file    uart.h
  * @author  jOKERII, Shengnan Wang
  * @version v1.2
  * @date    20-11-18
  * @brief   header
  *        20-11-18 v1.2 增加对my_pirntf 的支持
  ******************************************************************************
  */
	
#ifndef __DRV_UART_H
#define __DRV_UART_H

#include "stm32f4xx.h"
#include "stdarg.h"
#include "stdio.h"
#include "drv_nvic.h"
#include "string.h"

#define Serial1	USART1
#define Serial2	USART2
#define Serial3	USART3
#define Serial6	USART6
#define ERR_SERIAL Serial1 // 错误输出串口 默认串口1

// 打印串口，直接用 printf
#define PRINTF_SERIAL Serial2

typedef USART_TypeDef Serial_TypeDef;



void USART1_Init(uint32_t BaudRate);
void USART2_Init(uint32_t BaudRate);
void USART3_Init(uint32_t BaudRate);
void USART6_Init(uint32_t BaudRate);

void my_printf(char* fmt,...);
void Serial_SendByte(Serial_TypeDef *Serialx, uint8_t Data);
void Serial_Send(Serial_TypeDef *Serialx,const char *Data,...);

char *itoa(int value, char *string, int radix);

#endif
