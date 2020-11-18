/**
  ******************************************************************************
  * @file    bsp_esc.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-08
  * @brief   Í·ÎÄ¼þ
						 

  ******************************************************************************
  */
	
#ifndef __BSP_ESC_H
#define __BSP_ESC_H


#include "stm32f4xx.h"
#include "drv_can.h"
#include "drv_uart.h"


#define ESC_BaseID	0x201

#define	ID_ESC1	0x201
#define	ID_ESC2	0x202
#define	ID_ESC3	0x203
#define	ID_ESC4	0x204

#define IS_ESCID(id, min, max) \
						(((int)id <= max) && ((int)id >= min))

typedef struct C620_MotorInfomation_t
{
	uint16_t	AnglePulse;
	int16_t		Velocity;
	int16_t		Current;
	uint8_t		Temprature;
	int32_t		Position;
}C620_MotorInfomation_t;

typedef struct C610_MotorInfomation_t
{
	uint16_t	AnglePulse;
	int16_t		Velocity;
	int16_t		Current;
	int32_t		Position;
}C610_MotorInfomation_t;


extern C620_MotorInfomation_t     C620_MotorInfo[4];
extern C610_MotorInfomation_t     C610_MotorInfo[4];


/*  Function used to get the feedback information from ESC on CAN bus *********/
void C620_GetMotorInfo(CanRxMsg* RxMsg);
void C610_GetMotorInfo(CanRxMsg* RxMsg);

/*  Function used to send the current value to ESC to control the motor *******/
uint8_t C620_SendCurrentVal(int16_t I1, int16_t I2, int16_t I3, int16_t I4);
uint8_t C610_SendCurrentVal(int16_t I5, int16_t I6, int16_t I7, int16_t I8);

void Get_C620_MotorInfo(C620_MotorInfomation_t **Info);
void Get_C610_MotorInfo(C610_MotorInfomation_t **Info);

#endif

