/**
  ******************************************************************************
  * @file    drv_dac.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-30
  * @brief   void Dac1_Init(void)

			 void Dac1_Set_Vol(u16 vol)

  ******************************************************************************
  */


#include "drv_dac.h"


void Dac1_Init(void)
{  
    DAC_InitTypeDef DAC_InitType;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
       
	GPIO_Format_Init(GPIOA,GPIO_Pin_4,GPIO_Mode_AN,GPIO_OType_PP,\
					GPIO_PuPd_DOWN,GPIO_Speed_100MHz);

    DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

    DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
}
//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp  /=  1000;
	temp=temp*4095/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)temp);//12位右对齐数据格式设置DAC值
}





















































