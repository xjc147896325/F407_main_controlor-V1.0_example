/**
  ******************************************************************************
  * @file    init.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-03
  * @brief   System_Init(void);
						 

  ******************************************************************************
  */

#include "init.h"




/**
  * @brief  system init
  * @param  none
  * @retval нч
  */
void System_Init(void)
{
	char s[] = "heiheihei";
	
	SystemInit();
	NVIC_Priority_Init();
	SysTick_Init();
	
	USART1_Init(115200);
	
	
	USART2_Init(115200);
	USART6_Init(115200);
	
	
	TIM2_Init(8399,9);
	TIM3_Init(8399,499);
	TIM4_PWM_Init(499, 83, GPIOB, GPIO_Pin_7);
	TIM14_PWM_Init(499, 83, GPIOA, GPIO_Pin_7);
	
	
//	drv_spi_init();
//	Laser_Init();
	LED_Init();
	
	Serial_Send(Serial1,"ready %s \r\n",&s);
	
	delay_ms(500);
	
}


