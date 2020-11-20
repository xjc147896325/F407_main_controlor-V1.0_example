/**
  ******************************************************************************
  * @file    esc.c
  * @author  xjc
  * @version V1.0.0
  * @date    2019-09-18
  * @brief   
  ******************************************************************************
  */
/**
  ******************************************************************************
  * API:
	      
void C620_VelocityControl(uint8_t ESC_Id, float DstVelocity);
void C620_SetVelocityPIDRatio(uint8_t ESC_Id, float Kp, float Ki, float Kd);
void C620_SetPositionPIDRatio(uint8_t ESC_Id, float Kp, float Ki, float Kd);
void C620_VelocityControl(uint8_t ESC_Id, float DstVelocity);
void C620_PositionControl(uint8_t ESC_Id, int32_t DstPosition);

void C620_GetMotorInfo(CanRxMsg* RxMsg);

uint8_t C620_SendCurrentVal(int16_t I1, int16_t I2, int16_t I3, int16_t I4);

  ******************************************************************************
  */


#include "bsp_esc.h"
#include "drv_uart.h"

#define CAN1_C620 1
#define CAN1_C610 0


C620_MotorInfomation_t     C620_MotorInfo[4] = {0};
C610_MotorInfomation_t     C610_MotorInfo[4] = {0};

//C620
void C620_GetMotorInfo(CanRxMsg* RxMsg)
{
	int8_t ESC_Id = RxMsg -> StdId - ESC_BaseID ;//²âÊÔ
	uint16_t LastPulse;
	int16_t DeltaPulse;
	
	if(IS_ESCID(ESC_Id, 0, 3))
	{
		LastPulse = C620_MotorInfo[ESC_Id].AnglePulse;
		C620_MotorInfo[ESC_Id].AnglePulse = (uint16_t)(RxMsg -> Data[0] << 8 | RxMsg -> Data[1]);
		C620_MotorInfo[ESC_Id].Velocity	 = (int16_t)(RxMsg -> Data[2] << 8 | RxMsg -> Data[3]);
		C620_MotorInfo[ESC_Id].Current	 = (int16_t)(RxMsg -> Data[4] << 8 | RxMsg -> Data[5]);
		C620_MotorInfo[ESC_Id].Temprature = (uint8_t)(RxMsg -> Data[6]);
		DeltaPulse = C620_MotorInfo[ESC_Id].AnglePulse - LastPulse;
		if(DeltaPulse > 4095)
		{
			DeltaPulse -= 8192;
		}
		else 
		{
			if(DeltaPulse < -4096)
			{
				DeltaPulse += 8192;
			}
		}
		C620_MotorInfo[ESC_Id].Position += DeltaPulse;
	}
	else
	{
		Serial_Send(ERR_SERIAL,"ESC recive err, please connect xjc\r\n");
	}

}
//C610
void C610_GetMotorInfo(CanRxMsg* RxMsg)
{
	int8_t ESC_Id = RxMsg -> StdId - ESC_BaseID ;//²âÊÔ
	uint16_t LastPulse;
	int16_t DeltaPulse;
	
	if(IS_ESCID(ESC_Id, 0, 3))
	{
		LastPulse = C610_MotorInfo[ESC_Id].AnglePulse;
		C610_MotorInfo[ESC_Id].AnglePulse = (uint16_t)(RxMsg -> Data[0] << 8 | RxMsg -> Data[1]);
		C610_MotorInfo[ESC_Id].Velocity	 = (int16_t)(RxMsg -> Data[2] << 8 | RxMsg -> Data[3]);
		C610_MotorInfo[ESC_Id].Current	 = (int16_t)(RxMsg -> Data[4] << 8 | RxMsg -> Data[5]);
		DeltaPulse = C610_MotorInfo[ESC_Id].AnglePulse - LastPulse;
		if(DeltaPulse > 4095)
		{
			DeltaPulse -= 8192;
		}
		else 
		{
			if(DeltaPulse < -4096)
			{
				DeltaPulse += 8192;
			}
		}
		C610_MotorInfo[ESC_Id].Position += DeltaPulse;
	}
	else
	{
		Serial_Send(ERR_SERIAL,"ESC recive err, please connect xjc\r\n");
	}

}

uint8_t C620_SendCurrentVal(int16_t I1, int16_t I2, int16_t I3, int16_t I4)
{
	uint8_t MailBox = 0;
	int16_t Current_1 = I1;
	int16_t Current_2 = I2;
	int16_t Current_3 = I3;
	int16_t Current_4 = I4;
	CanTxMsg TxMsg;
	TxMsg.StdId	= 0x200;
	TxMsg.DLC	= 0x08;
	TxMsg.IDE	= CAN_Id_Standard;
	TxMsg.RTR	= CAN_RTR_Data;
	
	TxMsg.Data[0] = Current_1 >> 8;
	TxMsg.Data[1] = Current_1;
	TxMsg.Data[2] = Current_2 >> 8;
	TxMsg.Data[3] = Current_2;
	TxMsg.Data[4] = Current_3 >> 8;
	TxMsg.Data[5] = Current_3;
	TxMsg.Data[6] = Current_4 >> 8;
	TxMsg.Data[7] = Current_4;
	
	MailBox = CAN_Transmit(CAN1, &TxMsg);
	if(MailBox == CAN_TxStatus_NoMailBox)
		return CAN_TxStatus_Failed;
	return CAN_TxStatus_Ok;
}

uint8_t C610_SendCurrentVal(int16_t I1, int16_t I2, int16_t I3, int16_t I4)
{
	uint8_t MailBox = 0;
	int16_t Current_1 = I1;
	int16_t Current_2 = I2;
	int16_t Current_3 = I3;
	int16_t Current_4 = I4;
	CanTxMsg TxMsg;
	TxMsg.StdId	= 0x200;
	TxMsg.DLC	= 0x08;
	TxMsg.IDE	= CAN_Id_Standard;
	TxMsg.RTR	= CAN_RTR_Data;
	
	TxMsg.Data[0] = Current_1 >> 8;
	TxMsg.Data[1] = Current_1;
	TxMsg.Data[2] = Current_2 >> 8;
	TxMsg.Data[3] = Current_2;
	TxMsg.Data[4] = Current_3 >> 8;
	TxMsg.Data[5] = Current_3;
	TxMsg.Data[6] = Current_4 >> 8;
	TxMsg.Data[7] = Current_4;
	
	MailBox = CAN_Transmit(CAN1, &TxMsg);
	if(MailBox == CAN_TxStatus_NoMailBox)
		return CAN_TxStatus_Failed;
	return CAN_TxStatus_Ok;
}

void Get_C620_MotorInfo(C620_MotorInfomation_t **Info)
{
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
	{
		Info[i]->AnglePulse = C620_MotorInfo[i].AnglePulse;
		Info[i]->Velocity   = C620_MotorInfo[i].Velocity;
		Info[i]->Current    = C620_MotorInfo[i].Current;
		Info[i]->Temprature = C620_MotorInfo[i].Temprature;
		Info[i]->Position   = C620_MotorInfo[i].Position;
	}
}

void Get_C610_MotorInfo(C610_MotorInfomation_t **Info)
{
	uint8_t i = 0;
	for(i = 0; i < 4; i++)
	{
		Info[i]->AnglePulse = C610_MotorInfo[i].AnglePulse;
		Info[i]->Velocity   = C610_MotorInfo[i].Velocity;
		Info[i]->Current    = C610_MotorInfo[i].Current;
		Info[i]->Position   = C610_MotorInfo[i].Position;
	}
}

