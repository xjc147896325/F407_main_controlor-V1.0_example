/**
  ******************************************************************************
  * @file    init.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-03-30
  * @brief   头文件
						 

  ******************************************************************************
  */
	
#ifndef __INIT_H
#define __INIT_H


#define ZERO_MODE               0 // 1：零点电压测试模式   0：正常电压采集模式
                                  // 零点电压测试用于获取得到0V（即短路）输入时的偏置电压
                                  // AD转换值。根据电路设计把运放输出端电压偏置0.2V左右
                                  // （即模拟量经过运放后实现了“放大倍数+电压偏置”功能）
                                  // 这样可以解决运放零点漂移问题。

// 放大倍数=R2/R1=2000/6800倍
#define OPA_RES_R1              6800  // 6.8k 运放输入端电阻
#define OPA_RES_R2              2000  // 2k 运放反馈电阻
#define REFERENCE_VOLTAGE       3297  // 参考电压（放大1000倍）
#define BIAS_VOLTAGE_IN1        0xFAB3E  // 输入1偏置电压，即把IN1和GND短接时AD7190转换结果
#define BIAS_VOLTAGE_IN2        0xF9DCA  // 输入2偏置电压，即把IN2和GND短接时AD7190转换结果
#define BIAS_VOLTAGE_IN3        0xFA8A4  // 输入3偏置电压，即把IN3和GND短接时AD7190转换结果
#define BIAS_VOLTAGE_IN4        0xFA9EB  // 输入4偏置电压，即把IN4和GND短接时AD7190转换结果


#include "stm32f4xx.h"
#include "drv_systick.h"
#include "drv_uart.h"
#include "drv_dac.h"
#include "drv_adc.h"
#include "drv_can.h"
#include "drv_gpio.h"
#include "drv_timer.h"
#include "drv_adc.h"
#include "drv_spi.h"
#include "bsp_oled.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_ad7190.h"
#include "bsp_joystick.h"

void System_Init(void);

#endif
