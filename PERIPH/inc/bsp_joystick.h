#ifndef __BSP_JOYSTICK_H
#define __BSP_JOYSTICK_H

#include "stm32f4xx.h"
#include "drv_uart.h"
#include "bsp_led.h"

//extern volatile uint8_t RemoteUpdated;
extern uint8_t Joystick_Connect_Flag ;

typedef struct Remote_t
{
	int16_t   LX;
	int16_t   LY;
	int16_t   RX;
	int16_t   RY;
	int16_t   V1;
	int16_t   V2;
	int16_t	  V3;
	
	int8_t    KEY1;
	int8_t    KEY2;
	int8_t    KEY3;
	int8_t    KEY4;
	int8_t    KEY5;
	int8_t    KEY6;
	int8_t    KEY7;
	int8_t    KEY8;
	int8_t    KEY9;
	int8_t    KEY10;
	int8_t    KEY11;
	int8_t    KEY12;
	int8_t    KEY13;
	int8_t    KEY14;
	int8_t    KEY15;
	int8_t    KEY16;
}Remote_t;

//extern Remote_t Remote;
//extern uint8_t  Remote_Refresh;
//extern uint8_t  Joystick_Connect_Flag;

typedef enum
{
	Remote_SW0   = 0x00,
	Remote_SW1   = 0x01,
	Remote_SW2   = 0x10,
	Remote_SW3   = 0x11,
	Remote_SWErr = 0xFF,
}RemoteSwitch_TypeDef;

//void SX1278_Reciver_Init(void);
void RemoteInit(void);
void UpdateRemoteInfo(void* RxData);
void RemoteAnalyzeTask(void);
void UnPack(void);
int16_t Remote_GetChanalVal(uint8_t Chanel);
int16_t Remote_GetLX(void);
int16_t Remote_GetLY(void);
int16_t Remote_GetRX(void);
int16_t Remote_GetRY(void);
int16_t Remote_GetV1(void);
int16_t Remote_GetV2(void);
int16_t Remote_GetV3(void);
int16_t Remote_GetKEY1(void);
int16_t Remote_GetKEY2(void);
int16_t Remote_GetKEY3(void);
int16_t Remote_GetKEY4(void);
int16_t Remote_GetKEY5(void);
int16_t Remote_GetKEY6(void);
int16_t Remote_GetKEY7(void);
int16_t Remote_GetKEY8(void);
int16_t Remote_GetKEY9(void);
int16_t Remote_GetKEY10(void);
int16_t Remote_GetKEY11(void);
int16_t Remote_GetKEY12(void);
int16_t Remote_GetKEY13(void);
int16_t Remote_GetKEY14(void);
int16_t Remote_GetKEY15(void);
int16_t Remote_GetKEY16(void);

void Reflash_Task(void);

#endif
