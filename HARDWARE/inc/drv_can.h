/**
  ******************************************************************************
  * @file    C620.h
  * @author  jOKERII, WSN
  * @version V1.1
  * @date    20-11-19
  * @brief    void CAN1_Init()
			  void CAN2_Init()
          20.11.19 V1.1 add can2 drv

  ******************************************************************************
  */

#ifndef __DRV_CAN_H
#define __DRV_CAN_H	 

#include "stm32f4xx.h"
#include "drv_gpio.h"
#include "drv_nvic.h"



void CAN1_Init(void);
void CAN2_Init(void);


#endif

















