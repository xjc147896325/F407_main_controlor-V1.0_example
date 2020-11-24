/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 if(E_COUNTER!=0)
	{
		E_COUNTER--;
	}
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/*****************************USART**************************/
/**
  * @brief  This function handles USART1 Handler.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET);
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

/**
  * @brief  This function handles USART2 Handler.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	RemoteAnalyzeTask();
}

/**
  * @brief  This function handles USART3 Handler.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_IT_RXNE) != RESET);
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

/**
  * @brief  This function handles USART6 Handler.
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void)
{
	uint8_t RxData;	

	if(USART_GetFlagStatus(USART6, USART_IT_RXNE) != RESET);
	{
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		RxData = USART_ReceiveData(USART6);

		Get_Gyro_Info(RxData);
	}
}

/*****************************TIMER**************************/

/**
* @brief 定时器2中断函数，1ms
* @param none
* @return none
*/	
void TIM2_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)   //每隔1ms进入一次更新中断
	{	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);      //清除标志位

//		Serial_Send(Serial1,"time:%d\r\n",2);
	
		LED_Task(LED1,500);
		
		//ADC_Queue_Updata(Get_Laser_Val()); //会死
	}
}

/**
* @brief 定时器3中断函数，5ms
* @param none
* @return none
*/	
void TIM3_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   //每隔5ms进入一次更新中断
	{	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //清除标志位
		
	}
}


/**************************CAN**************************/

/**
* @brief CAN1中断函数
* @param none
* @return none
*/	


void CAN1_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1,CAN_IT_TME) != RESET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
}

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMsg;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
		CAN_ClearFlag (CAN1, CAN_IT_FF0);
		CAN_Receive (CAN1, CAN_FIFO0 ,&RxMsg);
	}
}



/*****************************DMA**************************/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
