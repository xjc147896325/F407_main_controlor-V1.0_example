
#ifndef __CONTROL_PLATE_CONNECT_H
#define __CONTROL_PLATE_CONNECT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "drv_uart.h"
#include "drv_systick.h" 
#include "control_plate.h"

#include "drv_can.h"


#define PLATE_BASE_ID          0x100  //0x100-0xF00
#define PLATE_CMD_GET          0x00F
#define PLATE_CMD_TO_WHERE     0x0F0
#define PLATE_ALL_CMD          0x0FF

#define PLATE_CMD_NUM          0x07  //命令数量
#define PLATE_CMD_NONE         0x00  //空命令
#define PLATE_CMD_CONTROL      0x01  //控制命令 详见control_ptate_control

#define PLATE_CMD_SPEED        0x02
#define PLATE_CMD_CURRENT      0x03
#define PLATE_CMD_POSITION12   0x04
#define PLATE_CMD_POSITION34   0x05
#define PLATE_CMD_ANGLE        0x06


#define PLATE_ID_GET        0xF0
#define PLATE_ID_ALL        0xF0

#define PLATE_ID_MASTER     0x10
#define PLATE_ID_C620       0x20
#define PLATE_ID_C610       0x30
#define PALTE_ID_MECHANISM  0x40
#define PLATE_ID_TEST       0xA0


typedef struct TYPE_PlateRxInfo_t
{
    
    uint8_t    cmd;
    uint8_t    val1[4];
    uint8_t    val2[4];
    uint8_t    val3[4];
    uint8_t    val4[4];  
}TYPE_PlateRxInfo_t, *pTYPE_PlateRxInfo_t;

//外部可直接使用, 也可使用API获得指针
extern TYPE_PlateRxInfo_t g_ControlPlateRx_t;
extern uint8_t          g_PLATE_ID;


uint8_t ControlPlate_SendData(uint16_t toWhere, uint8_t cmd, uint8_t *txData);
uint8_t ControlPlate_GetFeedbackInfo(CanRxMsg* RxMsg);

void ControlPlate_Info_Clear(void);


//获取数据
void ControlPlate_API_GetPosition   (int32_t* p1, int32_t* p2, int32_t* p3, int32_t* p4);
void ControlPlate_API_GetCurrent    (int16_t* I1, int16_t* I2, int16_t* I3, int16_t* I4);
void ControlPlate_API_GetSpeed      (int16_t* v1, int16_t* v2, int16_t* v3, int16_t* v4);
void ControlPlate_API_GetAngle      (int16_t* a1, int16_t* a2, int16_t* a3, int16_t* a4);

void ControlPlate_API_GetControlInfo (uint8_t* controlInfo);

//通用int16_t发送获取
uint8_t ControlPlate_API_Send_int16(uint16_t toWhere, uint8_t cmd, int16_t data1, int16_t data2, int16_t data3, int16_t data4);
void    ControlPlate_API_Get_int16 (int16_t* data1, int16_t* data2, int16_t* data3, int16_t* data4);

//用处不大类
uint16_t ControlPlate_API_GetAllCMD  (void);
uint8_t  ControlPlate_API_IsCmdToSelf(void);
uint8_t  ControlPlate_API_GetPlateID (void);
pTYPE_PlateRxInfo_t ControlPlate_API_GetPlateRxInfo(void);
void ControlPlate_None_Fun(void);

//实时改变控制板设置
void ControlPlate_API_Change_ConnectCan(uint16_t i);
void ControlPlate_API_Change_PlateID   (uint16_t id);

#endif



















