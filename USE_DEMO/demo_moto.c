


#include "demo_moto.h"
#include "moto_control.h"
#include "bsp_esc.h"
#include "user_math.h"
#include "drv_uart.h"
#include "bsp_joystick.h"
#include "drv_systick.h"

uint8_t flag = 1;
TYPE_Moto_t moto_1;
int32_t g_set_position = 0;

// C620_MotorInfomation_t g_MotorInfo_t[4];

void Moto_Init(void)
{
    Moto_API_PID_Init(&moto_1);
    // Get_C620_MotorInfo(g_MotorInfo_t);
}


void Moto_1ms_task(void)
{
    static int i = 0;
    i ++;
    int dstPosition = C620_MotorInfo[0].Position;
    int dstvelocity = C620_MotorInfo[0].Velocity;
    int16_t I1;
	
    Moto_API_SetMotoInfo(&moto_1, dstvelocity, dstPosition); 
    
    I1 = Moto_API_Cal_GetCurrentVelocityMode(&moto_1, g_set_position);
    absLimit_int16(&I1, 3000);
    if(flag)
        C620_SendCurrentVal(I1, I1, 0, 0);
    else
        C620_SendCurrentVal(0, 0, 0, 0);
    
    if(i > 500)
    {
        i = 0;
        my_printf("%d    %d\r\n", dstPosition, dstvelocity);
    }
}

void Moto_Set_PID(float kp, float ki, float kd)
{
    Moto_API_SetVelocityPID(&moto_1, kp, ki, kd);
}



void Moto_Test_Task(void *p)
{
    int i;
    float kp, ki, kd;
    while(1)
	{
        if(i > 1000)
        {
            i = 0;
        }
        if( ! (i / 20))
        {
            kp = Remote_GetV1() / 100.0;
            ki = Remote_GetV2() / 1000.0;
            kd = Remote_GetV3() / 100.0;
            Moto_Set_PID(kp, ki, kd);
        }            
        i ++ ;
        g_set_position = Remote_GetLX();
        Moto_1ms_task();
        delay_ms(1);
	}
}




