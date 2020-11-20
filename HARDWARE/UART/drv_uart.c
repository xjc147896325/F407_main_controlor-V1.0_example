/**
  ******************************************************************************
  * @file    uart.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-03-31
  * @brief   USART1_Init(uint32_t BaudRate);
						 USART2_Init(uint32_t BaudRate);
						 USART3_Init(uint32_t BaudRate);
						 USART6_Init(uint32_t BaudRate);
						 
						 Serial_Send(Serial_TypeDef *Serialx,const char *Data,...);
  ******************************************************************************
  */


/**
  ******************************************************************************
  * @file    uart.h
  * @author  jOKERII, Shengnan Wang
  * @version v1.2
  * @date    20-11-18
  * @brief   header
  *        20-11-18 v1.2 增加对my_pirntf 的支持
  ******************************************************************************
  */
	

#include "drv_uart.h"

/**
  * @brief  USART init 1 2 3 6
  * @param  BaudRate
  * @retval None
  */

void USART1_Init( uint32_t BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = BaudRate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_Init(USART1,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART1_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = USART1_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
}

void USART2_Init( uint32_t BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = BaudRate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_Init(USART2,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = USART2_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	
}

void USART3_Init( uint32_t BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = BaudRate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_Init(USART3,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART3_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = USART3_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
	
}

void USART6_Init( uint32_t BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = BaudRate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_Init(USART6,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART6_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = USART6_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART6,ENABLE);
	
}

void Serial_SendByte(Serial_TypeDef *Serialx, uint8_t Data)
{
	while(USART_GetFlagStatus(Serialx, USART_FLAG_TC) != SET);
	USART_SendData(Serialx, Data);
}

/**
  * @brief  格式化串口输出函数
  * @param  USARTx:  指定串口
		    	Data：   发送数组
		    	...:     不定参数
  * @retval 无
  */
void Serial_Send(Serial_TypeDef *Serialx,const char *Data,...)
{
	const char *string;
	int  dec, hex;
	double flo;
	char buff[16];
	
	va_list pointer;
	va_start(pointer,Data);
	
	while(0 != *Data)
	{
		if(0x5c == *Data)  //'\'
		{
			switch(*++Data)
			{
				case 'r':
				{
					Serial_SendByte(Serialx, 0x0d);   //回车
					Data++;
					break;
				}
				case 'n':
				{
					Serial_SendByte(Serialx, 0x0a); //换行
					Data++;
					break;
				}
				default:
				{
					Data++;
					break;
				}
			}
		}
		
		else
		{
			if('%' == *Data)
			{
				switch(*++Data)
				{
					case 's':        //string
					{
						string = va_arg(pointer,const char *);
						for(; *string; string++)     //string end of \0(sacii 0)
						{
							Serial_SendByte(Serialx,*string);
							while(USART_GetFlagStatus(Serialx,USART_FLAG_TC == RESET));
						}
						Data++;
						break;
					}
					case 'd':
					{
						dec = va_arg(pointer,int);
						itoa(dec,buff,10);
						for(string = buff; *string; string++)
						{
							Serial_SendByte(Serialx,*string);
							while(USART_GetFlagStatus(Serialx,USART_FLAG_TC == RESET));
						}
						Data++;
						break;
					}
					case 'f':
					{
						flo = va_arg(pointer,double);
						sprintf(buff,"%f",flo);    //未发现更好的x 
						for(string = buff; *string; string++)
						{
							Serial_SendByte(Serialx,*string);
							while(USART_GetFlagStatus(Serialx,USART_FLAG_TC == RESET));
						}
						Data++;
						break;
					}
					case 'X':
					{
						hex = va_arg(pointer,int);
						sprintf(buff,"%X",hex);    //未发现更好的x 
						for(string = buff; *string; string++)
						{
							Serial_SendByte(Serialx,*string);
							while(USART_GetFlagStatus(Serialx,USART_FLAG_TC == RESET));
						}
						Data++;
						break;
					}
					default:
					{
						Data++;
						break;
					}
				}
			}
			else
			{
				Serial_SendByte(Serialx,*Data++);
				while(USART_GetFlagStatus(Serialx,USART_FLAG_TC == RESET));
			}
		}
	}
}

/**
  * @brief  int to string(max 9999999-dec)
  * @param  value  :  value
		    	  *string： string
		    	  radix  :  only 10(dec)
  * @retval 无
  */
char *itoa(int value, char *string, int radix)
{
	char *TS_Buff = string;
	int  d = 0 ,flag = 0;
	
	if(10 != radix)
	{
		*TS_Buff = 0;
		return string;
	}
	
	if(0 == value)
	{
		*TS_Buff++ = 0x30;      //ascii "0"
		*TS_Buff   = 0;
		return TS_Buff;
	}
	
	if(0 > value)
	{
		*TS_Buff++ = '-';
		value *= -1;
	}
	
	for (int i = 1000000; i > 0; i /= 10)
	{
		d = value / i;
		
		if(d || flag)                    //ACTION
		{
			*TS_Buff++ = (char) (d + 0x30);
			value -= d * i;
			flag = 1;
		}
	}
	
	*TS_Buff = 0;
	
	return TS_Buff ;
}

void my_printf(char* fmt,...)  
{
    
 
    u16 i,j;
    uint8_t USART_TX_BUF[100];
    va_list ap;
    va_start(ap,fmt);

    vsprintf((char*)USART_TX_BUF,fmt,ap);
    va_end(ap);
    i=strlen((const char*)USART_TX_BUF);//此次发送数据的长度
    for(j=0;j<i;j++)//循环发送数据
    {
      while(USART_GetFlagStatus(PRINTF_SERIAL,USART_FLAG_TC)!=SET)
          ; //等待上次传输完成 
        USART_SendData(PRINTF_SERIAL,USART_TX_BUF[j]);      //发送数据到串口3 
    }
}











