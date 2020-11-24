/**
  ******************************************************************************
  * @file    gyro.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-24
  * @brief   
  ******************************************************************************
  */
  
#include "gyro.h" 

Gyro_Init_Info Gyro_Info;

void Get_Gyro_Info(uint8_t Rxbuf)
{
	static uint8_t flag    =  0, counter = 0;
	static uint8_t Temp[5] = {0};
	uint8_t        i       =  0;
	int32_t       data     =  0;
	
	switch(flag)
	{
		case(0):
		{
			if(Rxbuf == 0x80)
			{
				flag = 1;
				counter = 0;
			}
			else
			{
				flag = 0;
			}
			break;
		}
		case(1):
		{
			if((Rxbuf & 0x80) == 0x00)
			{
				if(counter < 5)
				{
					Temp[counter] = Rxbuf;
					counter++;
				}
				else
				{
					if(XOR_Check(Temp, Message) == Rxbuf)
					{
						data = (Temp[4] << 28) | (Temp[3] << 21) | (Temp[2] << 14) | (Temp[1] << 7) | Temp[0];
						Gyro_Info.Gyro_Message = data;
						
						Gyro_Info.Angular_velocity_z = Gyro_Info.Gyro_Message / NOMINAL_FACTOR ; 
						counter = 0;
						flag++;
						for(i = 0; i < 5; i++)
						{
							Temp[i] = 0;
						}
						//Serial_Send(Serial1,"Gyro_Info.Gyro_Message = %d \r\n", Gyro_Info.Gyro_Message);    //OK
						Serial_Send(Serial1,"Gyro_Info.Angular_velocity_z = %f \r\n", Gyro_Info.Angular_velocity_z);    //OK

					}
					else
					{
						flag    = 0;
						for(i = 0; i < 5; i++)
						{
							Temp[i] = 0;
						}
					}
				}
			}
			else
			{
				flag    = 0;
				for(i = 0; i < 5; i++)
				{
					Temp[i] = 0;
				}
			}
			
			break;
		}
		case(2):
		{
			if((Rxbuf & 0x80) == 0x00)
			{
				if(counter < 2)
				{
					Temp[counter] = Rxbuf;
					counter++;
				}
				else
				{
					if(XOR_Check(Temp, Temprature) == Rxbuf)
					{
						data = (Temp[1] << 7) | Temp[0];
						Gyro_Info.Gyro_Temperature = data/16;
						counter = 0;
						flag = 0;
						//Serial_Send(Serial1,"Gyro_Info.Gyro_Temperature = %f \r\n", Gyro_Info.Gyro_Temperature);   //OK
					}
					else
					{
						flag    = 0;
						for(i = 0; i < 5; i++)
						{
							Temp[i] = 0;
						}
					}
				}
			}
			else
			{
				flag    = 0;
				for(i = 0; i < 5; i++)
				{
					Temp[i] = 0;
				}
			}
			
			break;
		}
		default:
		{
			flag    = 0;
			for(i = 0; i < 5; i++)
			{
				Temp[i] = 0;
			}
		}
	}
	
}

uint8_t XOR_Check(uint8_t *Temp, Gyro_Mode_Typedef Gyro_Mode)
{
	uint8_t ans = 0;
	
	switch(Gyro_Mode)
	{
		case(Message):
		{
			ans = Temp[0] ^ Temp[1] ^ Temp[2] ^ Temp[3] ^ Temp[4];
			break;
		}
		case(Temprature):
		{
			ans = Temp[0] ^ Temp[1];
			break;
		}
	}
	return ans;
}


