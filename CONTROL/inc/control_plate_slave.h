#ifndef __PLATE_FEEDBAKS_H
#define __PLATE_FEEDBAKS_H


#include "control_plate.h"
#include "control_plate_control.h"
#include "control_plate_master.h"
#include "bsp_led.h"
#include "drv_uart.h"



#if IS_USE_ORIGINAL_ESC
#include "bsp_esc.h"
#include "moto_control.h"
#else
/*“∆÷≤ ±”√*/
#endif



#if PLATE_SLAVE

#if COULD_SALVE_CONTROL_MOTO
void Plate_LoseFoce(void);
void Plate_SpeedMode(void);
void Plate_PositionMode(void);
void Plate_CurrentMode(void);
void Plate_AngleMode(void);
void Plate_SCPA_Mode(void);
void Plate_Set_Zore_Position(uint8_t mode, int32_t *pPosition);
#endif

void Plate_Moto_Init(void);
void Plate_Slave_Init(void);
void Plate_Slave_LoopBackMode(void);
void Plate_Slave_Task(void);
void Plate_Slave_Test(void);

void    Plate_Slave_SetRunFlag(uint8_t flag);
void    Plate_Slave_Moto_Use_Change(uint8_t moto_use);
void    Plate_Slave_Mode_Change(uint8_t mode);
uint8_t Plate_Slave_Mode_Get(void);
uint8_t Plate_Slave_Moto_Use_Get(void);

#endif



#endif

























