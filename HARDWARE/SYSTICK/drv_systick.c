#include "drv_systick.h"

////////////////////////////////////////////////////////////////////////////////// 	 

#if SYSTEM_SUPPORT_FREE_RTOS
#include "FreeRTOS.h"					//支持OS时，使用	  
#include "task.h"
#endif

static uint8_t  fac_us = 0;							//us延时倍乘数			

#if SYSTEM_SUPPORT_FREE_RTOS
static uint16_t fac_ms = 0;							//us延时倍乘数,在os下,代表每个节拍的ms数
#endif

//systick中断服务函数,使用OS时用到
void SysTick_Handler(void)
{	
    #if SYSTEM_SUPPORT_FREE_RTOS
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
    #endif
}

			   
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为AHB时钟的1/8
//SYSCLK:系统时钟频率
void delay_init(u8 SYSCLK)
{
    uint32_t reload;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    fac_us = SYSCLK;
    reload = SYSCLK;
    #if SYSTEM_SUPPORT_FREE_RTOS
    reload *= 1000000/configTICK_RATE_HZ;       //168000
	fac_ms  = 1000   /configTICK_RATE_HZ;	    //代表OS可以延时的最少单位	 1  
    #else
    reload *= 1000000/ 1000;       //168000
    #endif
    
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload - 1; 					//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//开启SYSTICK    
}								    


//延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=21)	    								   
void delay_us(uint32_t nus)
{
    uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks = nus * fac_us; 						//需要的节拍数 
	
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}
//延时nms
//nms:要延时的ms数
//nms:0~65535
void delay_ms(uint16_t nms)
{	
    #if SYSTEM_SUPPORT_FREE_RTOS
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
	{		 
		if(nms>= fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(nms / fac_ms);	        //OS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
    #endif
	delay_us((uint32_t)(nms*1000));				//普通方式延时
}


//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对168M条件下,nms<=798ms 
void delay_xms(uint16_t nms)
{	 		  	  
	uint32_t i;
    for(i = 0; i < nms;i++)
    {
        delay_us(1000);
    }
}


	 





































