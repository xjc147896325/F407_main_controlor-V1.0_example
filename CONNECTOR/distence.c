#include "distence.h"
uint8_t RX_Buffer[22] = {0};

uint32_t Distance[3] = {0};


union FloatType
{
	float FloatNum; 
    int IntNum;
};

uint32_t * Get_Dis(void)
{
    return Distance;
}

uint8_t unpack_deal(char * buff,uint32_t *distance, float* beta)
{
	uint8_t i = 0;
	union FloatType Number;
	
	if(buff[0] == (char)0xAA && buff[1] == (char)0x55 )
	{

		for(i = 0; i < 3; i++)
		{
			distance[i] = 0;
			distance[i] |= buff[5 + 4*i];
			distance[i] <<= 8;
			distance[i] |= buff[4 + 4*i];
			distance[i] <<= 8;
			distance[i] |= buff[3 + 4*i];
			distance[i] <<= 8;
			distance[i] |= buff[2 + 4*i];
		}
		if(buff[14] == (char)0xc3 && buff[15] == (char)0x3c)
		{
			Number.IntNum  = 0;
			Number.IntNum  = buff[19];
			Number.IntNum  = (Number.IntNum << 8) | buff[18];
			Number.IntNum  = (Number.IntNum << 8) | buff[17];
			Number.IntNum  = (Number.IntNum << 8) | buff[16];
			*beta = Number.FloatNum;
			
			if(buff[20] == (char)0x55 && buff[21] == (char)0xaa)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}


void Get_Buff(void)
{
	const uint8_t DataLen = 18;
	static uint8_t RxData;	
	static uint8_t Counter = 0, i = 0;
	if(USART_GetFlagStatus(USART6, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		RxData = USART_ReceiveData(USART6);
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
					RX_Buffer[0] = 0xAA;
					RX_Buffer[1] = 0x55;
				}
				else
					if(RxData == 0xAA);
					else
						Counter = 0;
				break;
			case 2:		//Data Recieving
				RX_Buffer[i+2] = RxData;
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
					RX_Buffer[20] = 0x55;
					RX_Buffer[21] = 0xAA;
					unpack_deal(RX_Buffer,Get_Dis(),0);		
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














