#ifndef __PLATE_MASTER_H
#define __PLATE_MASTER_H



#include "control_plate.h"

#include "drv_uart.h"


#if PLATE_MASTER
void Plate_Master_Init(void);
void Plate_Master_Task(void);
void Plate_Master_Test(void);
void Plate_Master_Can_Control_Deal(void);

void    Plate_Master_Mode_Change(uint8_t i) ;
uint8_t Plate_Master_Mode_Get(void)         ;

//∑¢ÀÕ¿‡
uint8_t ControlPlate_API_SendMotoSpeed   (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4);
uint8_t ControlPlate_API_SendMotoCurrent (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4);
uint8_t ControlPlate_API_SendMotoPosition(uint16_t toWhere, int32_t moto1, int32_t moto2, int32_t moto3, int32_t moto4);
uint8_t ControlPlate_API_SendMotoAngle   (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4);
#endif

#endif

























