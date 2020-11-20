#ifndef __DREVE_H__
#define __DREVE_H__

#include "stm32f4xx.h"
#include "drv_systick.h"
#include "drv_uart.h"


#include "bsp_joystick.h"
#include "bsp_led.h"
#include "bsp_esc.h"

#include "pid.h"
#include "moto_control.h"
#include "path_plan.h"
#include "POINTS.h"
#include "chassis.h"

#include "control_plate.h"
#include "control_plate_master.h"

#define drive_printf my_printf

#define LOCATOR_TEST 1
#if LOCATOR_TEST
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
#endif

typedef enum
{
    CHASSIS_TASK_DEBUG  = 1,
    CHASSIS_TASK_REMOTE = 2,
    CHASSIS_TASK_NONE   = 3,
    CHASSIS_TASK_AUTO   = 4,
    CHASSIS_TASK_AUTO2  = 5,
    CHASSIS_TASK_LOCK_1 = 6,
    CHASSIS_TASK_LOCK_2 = 7,
}TYPE_TASK_CHOOSE;


#define AUTO_FINISH_FALG_1    1
#define AUTO_FINISH_FALG_2    2
#define AUTO_FINISH_FLAG_NONE 0


typedef struct TYPE_Control_t
{
    
    int16_t x;
    int16_t y;
    int16_t theta;
    uint8_t mode;
    uint8_t k;
    float debug_data1;
    float debug_data2;
    float debug_data3;
}TYPE_Control_t, *pTYPE_Control_t;


// 四个电机
extern TYPE_Moto_t     g_Moto_t[4];
// 控制器
extern TYPE_Control_t  g_Control_t;

extern   pTYPE_Path_Plan_t pPlan_t ;
extern   pTYPE_Path_Plan_t pPlan_t2 ;
extern   uint8_t        e_Auto_Finish_Flag;

void ChassisRemoteTask(void);
void ChassisDebugTask(void);
void ChassisAutoTask(void);
void ChassisLockTask(void);
// void ChassisTask(void);
void ChassisControl(uint8_t x);

void Chassis_Init(void);

void ChassisTask_1ms_SentData(void);

// void Romote_Data2Control(void);
void Control_Info_Print(void);
void ChassisTask(void);
#endif








