/**
  ******************************************************************************
  * @file    control_plate_master.c
  * @author  
  * @version V1.0.0
  * @date    2020.2.2
  * @brief   主控 辅控 交流相关
  *          主要定义发送电机的相关信息,以控制电机运动,
  *
  ******************************************************************************
  *
  *
  *
  *
  *
  ******************************************************************************/



#include "control_plate_master.h"
#include "control_plate.h"
#define pass ;

enum {PLATE_MASTER_MODE_NONE      ,
      PLATE_MASTER_MODE_CONTROL   ,
      PLATE_MASTER_MODE_LOOPBACK  , 
     }PLATE_MASTER_MODE = DEFAULT_PLATE_MASTER_MODE;


/**
  * @brief  发送速度到其它板子
  * @param  toWhere: 接收的板子的ID
  *         moto1-moto4:四个电机的速度
  *         
  *         
  * @retval CAN_TxStatus_Ok, CAN_TxStatus_Failed
  */


uint8_t ControlPlate_API_SendMotoSpeed    (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4)
{
    int16_t txBuff[4];

    txBuff[0] = moto1;
    txBuff[1] = moto2;
    txBuff[2] = moto3;
    txBuff[3] = moto4;
    return ControlPlate_SendData(toWhere, PLATE_CMD_SPEED, (uint8_t*)txBuff);
    
}
/**
  * @brief  发送电流到其它板子
  * @param  toWhere: 接收的板子的ID
  *         moto1-moto4:四个电机信息
  *         
  *         
  * @retval CAN_TxStatus_Ok, CAN_TxStatus_Failed
  */

uint8_t ControlPlate_API_SendMotoCurrent  (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4)
{
    int16_t txBuff[4];

    txBuff[0] = moto1;
    txBuff[1] = moto2;
    txBuff[2] = moto3;
    txBuff[3] = moto4;

    return ControlPlate_SendData(toWhere, PLATE_CMD_CURRENT, (uint8_t*)txBuff);
    

}

/**
  * @brief  发送位置到其它板子
  * @param  toWhere: 接收的板子的ID
  *         moto1-moto4:四个电机信息
  *         
  *         
  * @retval CAN_TxStatus_Ok, CAN_TxStatus_Failed
  */
uint8_t ControlPlate_API_SendMotoPosition (uint16_t toWhere, int32_t moto1, int32_t moto2, int32_t moto3, int32_t moto4)
{
    int32_t txBuff[4];

    txBuff[0] = moto1;
    txBuff[1] = moto2;
    txBuff[2] = moto3;
    txBuff[3] = moto4;
    return ( ControlPlate_SendData(toWhere, PLATE_CMD_POSITION12, (uint8_t*)(txBuff + 0)) &&
             ControlPlate_SendData(toWhere, PLATE_CMD_POSITION34, (uint8_t*)(txBuff + 2)));
}
/**
  * @brief  发送角度到其它板子
  * @param  toWhere: 接收的板子的ID
  *         moto1-moto4:四个电机信息
  *         
  *         
  * @retval CAN_TxStatus_Ok, CAN_TxStatus_Failed
  */

uint8_t ControlPlate_API_SendMotoAngle    (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4)
{
    int16_t txBuff[4];
    
    txBuff[0] = moto1;
    txBuff[1] = moto2;
    txBuff[2] = moto3;
    txBuff[3] = moto4;
    return ControlPlate_SendData(toWhere, PLATE_CMD_ANGLE, (uint8_t*)txBuff);
}



void Plate_Master_Mode_Change(uint8_t i)
{
    if( i < 3)
        PLATE_MASTER_MODE = i;
}

uint8_t Plate_Master_Mode_Get(void)
{
    return (uint8_t) PLATE_MASTER_MODE;
}

void Plate_Master_Init(void)
{
    pass;
}
void Plate_Master_Task(void)
{
    pass;
}

void Plate_Master_Can_Control_Deal(void)
{
    
    //uint8_t controlInfo[8];
    //ControlPlate_API_GetControlInfo(controlInfo);
    pass;

}

void Plate_Master_Test(void)
{
    pass;
}

























