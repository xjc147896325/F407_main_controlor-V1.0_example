#ifndef __PLATE_CONTROL_H
#define __PLATE_CONTROL_H


#include "bsp_joystick.h"
#include "control_plate.h"
#include "control_plate_slave.h"
#include "bsp_led.h"
#include "drv_uart.h"
#include "drv_systick.h"


#if IS_USE_ORIGINAL_ESC
#include "bsp_esc.h"
#include "drv_can.h"
#else
/*移植时用*/

#endif


#define CONTROL_CMD_GET           0x0F
#define CONTROL_ONLINE            0x01
#define CONTROL_REQ_DATA          0x02
#define CONTROL_RESP_DATA         0x03
#define CONTROL_SET_610_P_0       0x04
#define CONTROL_CHANGE_CONFIG     0x05
#define CONTROL_RUN_SELF_FUNCTION 0x06
#define CONTROL_STOP_MOTO         0x07

#define REQ_DATA_MOTO_P_C610    0x10 
   #define MOTO_C610_DATA_1             0x01
    //...
/* 有待完善
#define REQ_DATA_MOTO_P_C620    0x20
#define REQ_DATA_MOTO_RPM_C610  0x30
#define REQ_DATA_MOTO_RPM_C620  0x40
#define REQ_DATA_MOTO_I_C610    0x50
#define REQ_DATA_MOTO_I_C620    0x60
 */
 
 //请求遥控器数据
#define REQ_DATA_JOYSTICK       0xA0
    #define JOYSTICK_DATA_NONE      0x01
    #define JOYSTICK_DATA_LXY_RX    0x02
    #define JOYSTICK_DATA_RXY_LX    0x03
    #define JOYSTICK_DATA_LXY_KEY   0x04
    #define JOYSTICK_DATA_RXY_KEY   0x05
#define REQ_DATA_PLATE_ID       0xB0
    #define PLATE_ID_DATA_TO_SELF     0x01
    #define PLATE_ID_DATA_TO_ALL      0x02
    //.......
//
#define CONTROL_CHANGE_PLATE_ID       0x01
#define CONTROL_CHANGE_CONNECT_CAN    0x02
#define CONTROL_CHANGE_MASTER_MODE    0x03
#define CONTROL_CHANGE_SLAVE_MODE     0x04
#define CONTROL_CHANGE_SLAVE_MOTO_USE 0x05
#define CONTROL_CHANGE_SLAVE_RUN_FLAG 0x06
#define CONTROL_CHANGE_CAN1_USE       0x07
#define CONTROL_CHANGE_CAN2_USE       0x08
#define CONTROL_CHANGE_C610_CAN_USE   0x09
#define CONTROL_CHANGE_C620_CAN_USE   0x0A

// s[0]     self id | 控制命令   
// s[1-...]  附加命令
//控制类
uint8_t ControlPlate_API_OnlineSend   (uint16_t toWhere);
uint8_t ControlPlate_API_Request_Data(uint16_t toWhere, uint8_t dataType, uint8_t num);
uint8_t ControlPlate_API_Plate_Config(uint8_t toWhere, uint8_t changeWhat, uint16_t changeResult);
void    ControlPlate_API_Control_Task (void);

uint8_t ControlPlate_Deal_ReqData(uint8_t reqId, uint8_t dataType, uint8_t num);
void    ControlPlate_Deal_Online(uint8_t onlineID);
void    ControlPlate_Deal_RespData(uint8_t* controlInfo);
void    ControlPlate_Deal_Plate_Config(uint8_t changeWhat, uint16_t changeResult);
void    Plate_Control_Test(void);

#if IS_USE_ORIGINAL_ESC && PLATE_SLAVE
uint8_t ControlPlate_API_Set_610_P_0(uint16_t toWhere, uint8_t flag, int32_t* position);
uint8_t ControlPlate_API_Stop_Moto(uint8_t toWhere, uint8_t num);

void    ControlPlate_Deal_Set_610_P_0(uint8_t* controlInfo);
void    ControlPlate_Deal_Stop_Moto(uint8_t num);
#endif


#if COULD_RUN_SELF_FUNCTION
extern void * g_p_Self_Funs[];
void    ControlPlate_Deal_Run_Self_Fun(uint8_t* controlInfo);
uint8_t ControlPlate_API_Plate_Run_Self_Function(uint8_t toWhere, uint8_t funNum, uint8_t* parameter);
#endif

#endif

























