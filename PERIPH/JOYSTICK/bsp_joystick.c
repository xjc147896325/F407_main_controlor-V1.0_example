/**
  ******************************************************************************
  * @file    joystick.h
  * @author  xjc
  * @version V1.0.0
  * @date    13-11-2018
  * @brief     
  ******************************************************************************
**/

#include "bsp_joystick.h"

volatile uint8_t RemoteUpdated = 0;
Remote_t Remote;

int8_t  KEY_G[16]             = {0};
int16_t ADC_Value_G[7]        = {0};
uint8_t RX_Buffer[16]         = {0};
uint8_t Remote_Refresh        =  0 ;
uint8_t Joystick_Connect_Flag =  0 ;

//void SX1278_Reciver_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;          //G2,4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOG, &GPIO_InitStructure);
//	
//	GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_4);
//	
//	UART2_Init();
//}

void RemoteInit(void)
{
	Remote.LX = 0;
	Remote.LY = 0;
	Remote.RX = 0;
	Remote.RY = 0;
	Remote.V1 = 0;
	Remote.V2 = 0;
	Remote.V3 = 0;

	Remote.KEY1  = 0;
	Remote.KEY2  = 0;
	Remote.KEY3  = 0;
	Remote.KEY4  = 0;
	Remote.KEY5  = 0;
	Remote.KEY6  = 0;
	Remote.KEY7  = 0;
	Remote.KEY8  = 0;
	Remote.KEY9  = 0;
	Remote.KEY10 = 0;
	Remote.KEY11 = 0;
	Remote.KEY12 = 0;
	Remote.KEY13 = 0;
	Remote.KEY14 = 0;
	Remote.KEY15 = 0;
	Remote.KEY16 = 0;
}

int16_t Remote_GetChanalVal(uint8_t Chanel)
{
	if((Chanel > 0) && (Chanel < 8))
	{
		return ADC_Value_G[Chanel - 1];
	}
	else
	{
		if((Chanel > 8) && (Chanel < 16))
		{
			return KEY_G[Chanel - 7];
		}
	}
	return 0;
}


void UpdateRemoteInfo(void* RxData)
{
	UnPack();
	
	Remote.RX = ADC_Value_G[0];
	Remote.RY = ADC_Value_G[1];
	Remote.LX = ADC_Value_G[2];
	Remote.LY = ADC_Value_G[3];
	Remote.V1 = ADC_Value_G[4];
	Remote.V2 = ADC_Value_G[5];
	Remote.V3 = ADC_Value_G[6];

	Remote.KEY1  = KEY_G[0];
	Remote.KEY2  = KEY_G[1];
	Remote.KEY3  = KEY_G[2];
	Remote.KEY4  = KEY_G[3];
	Remote.KEY5  = KEY_G[4];
	Remote.KEY6  = KEY_G[5];
	Remote.KEY7  = KEY_G[6];
	Remote.KEY8  = KEY_G[7];
	Remote.KEY9  = KEY_G[8];
	Remote.KEY10 = KEY_G[9];
	Remote.KEY11 = KEY_G[10];
	Remote.KEY12 = KEY_G[11];
	Remote.KEY13 = KEY_G[12];
	Remote.KEY14 = KEY_G[13];
	Remote.KEY15 = KEY_G[14];
	Remote.KEY16 = KEY_G[15]; 
}

void RemoteAnalyzeTask(void)
{
	const uint8_t DataLen = 16;
	static uint8_t RxData;	
	static uint8_t Counter = 0, i = 0;
	if(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		RxData = USART_ReceiveData(USART2);
		switch(Counter)
		{
			case 0:		//Wait Start Signal[1]
				if(RxData == 0xAA)
					Counter++;
				else
					Counter = 0;
				break;
			case 1:		//Wait Start Signal[2]
				if(RxData == 0x55)
				{
					i = 0;
					Counter++;
				}
				else
					if(RxData == 0xAA);
					else
						Counter = 0;
				break;
			case 2:		//Data Recieving
				RX_Buffer[i] = RxData;
				i++;
				if(i >= DataLen)
				{
					i = 0;
					Counter ++;
				}
				break;
			case 3:		//Wait End Signal[1]
				if(RxData == 0x55)
					Counter++;
				else
					Counter = 0;
				break;
			case 4:		//Wait End Signal[2]
				if(RxData == 0xAA)		//End Signal[2] Matched
				{
					UpdateRemoteInfo((void*)&RX_Buffer[0]);					
					RemoteUpdated = 1;
				}
				else
				{
					for(i = 0;i < 16;i++)
					{
						RX_Buffer[i] = 0;
					}
				}
				Counter = 0;
				break;
			default:
				Counter = 0;
				break;
		}
	}
}

int16_t Remote_GetLX(void)
{
	return Remote.LX;
}

int16_t Remote_GetRX(void)
{
	return Remote.RX;
}

int16_t Remote_GetLY(void)
{
	return Remote.LY;
}

int16_t Remote_GetRY(void)
{
	return Remote.RY;
}

int16_t Remote_GetV1(void)
{
	return Remote.V1;
}

int16_t Remote_GetV2(void)
{
	return Remote.V2;
}

int16_t Remote_GetV3(void)
{
	return Remote.V3;
}

int16_t Remote_GetKEY1(void)
{
	return Remote.KEY1;
}

int16_t Remote_GetKEY2(void)
{
	return Remote.KEY2;
}

int16_t Remote_GetKEY3(void)
{
	return Remote.KEY3;
}

int16_t Remote_GetKEY4(void)
{
	return Remote.KEY4;
}

int16_t Remote_GetKEY5(void)
{
	return Remote.KEY5;
}

int16_t Remote_GetKEY6(void)
{
	return Remote.KEY6;
}

int16_t Remote_GetKEY7(void)
{
	return Remote.KEY7;
}

int16_t Remote_GetKEY8(void)
{
	return Remote.KEY8;
}

int16_t Remote_GetKEY9(void)
{
	return Remote.KEY9;
}

int16_t Remote_GetKEY10(void)
{
	return Remote.KEY10;
}

int16_t Remote_GetKEY11(void)
{
	return Remote.KEY11;
}

int16_t Remote_GetKEY12(void)
{
	return Remote.KEY12;
}

int16_t Remote_GetKEY13(void)
{
	return Remote.KEY13;
}

int16_t Remote_GetKEY14(void)
{
	return Remote.KEY14;
}

int16_t Remote_GetKEY15(void)
{
	return Remote.KEY15;
}

int16_t Remote_GetKEY16(void)
{
	return Remote.KEY16;
}

void UnPack(void)
{
	static uint8_t i;
	
	for(i = 0;i < 7;i++)
	{
		ADC_Value_G[i] = (int16_t)((RX_Buffer[i*2])|RX_Buffer[i*2+1] << 8);
	}
	for(i = 0;i < 8;i++)
	{
		KEY_G[15-i]   = ((RX_Buffer[14]&(0x01<<(7-i)))>>(7-i));
	}
	for(i = 0;i < 8;i++)
	{
		KEY_G[7-i] = ((RX_Buffer[15]&(0x01<<(7-i)))>>(7-i));
	}
}

