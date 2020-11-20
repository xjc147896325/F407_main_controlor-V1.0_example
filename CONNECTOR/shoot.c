


#include "shoot.h"



void Shoot_task(void)
{
    uint16_t dacval=0;
    static uint16_t i;
    
    dacval  = g_Joystick_t.P_LUp * 33 * 10 / 409 / 100 * 100;
    dacval += g_Joystick_t.P_R   * 33 / 409;
    if(dacval > 3300)
    {
        dacval = 3300; 
    }
    if(dacval < 1560)
    {
        dacval = 1560;
    }
    Dac1_Set_Vol(1650);
    
    i ++;
    if(i > 99)
    {
        shoot_printf("  deacval :%d \r\n", dacval);
        i = 0;
    }
    if(e_Control_En && e_Flag_update == 1) // 自动
    {   
        switch(e_Control_Flag)
        {
            case FLAG_SHOOT:
                GPIO_set_time(3, SHOOT_LIST_TIME, 1);
                e_Control_Time_Count[INDEX_SHOOT] = DURATION_SHOOT / CONTROL_TASK_TIME;
               // g_Beep_Flag = Beep_Task_On; //打  
                e_Flag_update = 2;
                break;
            case FLAG_LAY:
                GPIO_set_time(2, SHOOT_LIST_TIME, 1);
                e_Control_Time_Count[INDEX_LAY] = DURATION_LAY / CONTROL_TASK_TIME;
                //g_Beep_Flag = Beep_Task_On; //放
                e_Flag_update = 2;
                break;
            case FLAG_GRAB:
                GPIO_set_time(5, 0, 0);
                e_Control_Time_Count[INDEX_GRAB] = DURATION_GRAB / CONTROL_TASK_TIME;
                //g_Beep_Flag = Beep_Task_On;    // 抓
                e_Flag_update = 2;
                break;
            case FLAG_LOOSEN:
                GPIO_set_time(5, 0, 1);        // 松
                e_Control_Time_Count[INDEX_LOOSEN] = DURATION_LOOSEN / CONTROL_TASK_TIME;
                //g_Beep_Flag = Beep_Task_On;      
                e_Flag_update = 2;
                break;
            case FLAG_START:
                break;
            /*
            case FLAG_FINISH:
                g_Beep_Flag = Beep_Task_On;      
                e_Flag_update = 2;*/
        }
        
    }
    
    else
    {
        if(g_Joystick_t.Key1 & KEY1_R_DIR_DOWN)
        {
            GPIO_set_time(2, SHOOT_LIST_TIME, 1);
            g_Beep_Flag = Beep_Task_On;    //放
        }
       
        else if(g_Joystick_t.Key1 & KEY1_R_DIR_UP)
        {
            GPIO_set_time(3, SHOOT_LIST_TIME, 1);
            g_Beep_Flag = Beep_Task_On;    // 打
        }
        else if(g_Joystick_t.Key2 & KEY2_L_DIR_DOWN)
        { 
            GPIO_set_time(5, 0, 1);        // 松
            g_Beep_Flag = Beep_Task_On;
        }
        else if(g_Joystick_t.Key2 & KEY2_L_DIR_UP)
        {
            GPIO_set_time(5, 0, 0);
            g_Beep_Flag = Beep_Task_On;    // 抓
        }
    }
}





