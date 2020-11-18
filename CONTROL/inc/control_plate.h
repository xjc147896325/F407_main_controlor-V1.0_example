
#ifndef __CONTROL_PLATE_H
#define __CONTROL_PLATE_H


#include "control_plate_connect.h"
#include "control_plate_slave.h"
#include "control_plate_master.h"
#include "control_plate_control.h"

#define COULD_SALVE_CONTROL_MOTO 1         //能否控制电机

#if COULD_SALVE_CONTROL_MOTO
    #define IS_USE_ORIGINAL_ESC  0         //使用原来的电调文件
#else
    #define IS_USE_ORIGINAL_ESC  0         
    //不能控制电机时电调控制屏蔽，无需更改
#endif

#define IS_USE_ORIGINAL_JOYSTICK   0        //是还使用原来的手柄结构体
#define COULD_RUN_SELF_FUNCTION    0        //能否接收运行自定义函数命令
#define COULD_PLATE_ID_CHANGE      0        //能否动态更改控制版ID
#define COULD_CONNECT_CAN_CHANGE   0        //能否动态更改连接的CAN

#define PLATE_MASTER               0        //作为主机
#define PLATE_SLAVE                0        //作为从机
#define DEFAULT_PLATE_SLAVE_MODE   PLATE_SLAVE_MODE_LOOPBACK   //作为主机默认模式
#define DEFAULT_PLATE_MASTER_MODE  PLATE_MASTER_MODE_LOOPBACK  //作为从机默认模式
#define DEFAULT_MOTO_USE           C610                        //作为从机默认控制的电机
 

#define DEFAULT_CONNECT_CAN  USE_CAN1       //使用的发CAN   USE_CAN1 USE_CAN2  None     
#define DEFAULT_PLATE_ID     PLATE_ID_TEST  //板子ID        PALTE_ID_... A) - F0
#define plate_printf         my_printf      //printf重定义  必须要有一个void fun(char*, ...)的函数实现
                                            //例: void none_printf(char* p, ...) {;};//一个空函数

#define ANGLE_RATIO          1146880        //14 * 36 * 8192 / 360 / 100
                                            //角度模式下的电机转0.01度的position
                       
                       
                       
//相头的外部函数变量声明，用来减少warning, 不用动
#if IS_USE_ORIGINAL_ESC
extern void Plate_Set_Zore_Position(uint8_t mode, int32_t *pPosition);
#endif

#if COULD_SALVE_CONTROL_MOTO
extern void Plate_Slave_Moto_Use_Change(uint8_t moto_use);
extern void Plate_Set_Zore_Position(uint8_t mode, int32_t *pPosition);
#endif

#if COULD_RUN_SELF_FUNCTION
extern void *  g_p_Self_Funs[];
extern void    ControlPlate_Deal_Run_Self_Fun(uint8_t* controlInfo);
extern uint8_t ControlPlate_API_Plate_Run_Self_Function(uint8_t toWhere, uint8_t funNum, uint8_t* parameter);
#endif

#if PLATE_MASTER
extern void    Plate_Master_Mode_Change(uint8_t i) ;
extern uint8_t Plate_Master_Mode_Get(void)         ;
#endif

#if PLATE_SLAVE
void    Plate_Slave_Mode_Change(uint8_t mode);
uint8_t Plate_Slave_Mode_Get(void);
#endif



#endif











