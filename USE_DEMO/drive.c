
#include "drive.h"


#if LOCATOR_TEST

TYPE_Locator_t g_Locator_t;
#endif
uint8_t        e_Auto_Finish_Flag;

//电机
TYPE_Moto_t    g_Moto_t[4] = {0};
TYPE_Moto_t    g_Moto610_t[4] = {0};
//控制接口
TYPE_Control_t g_Control_t={0};
/*
TYPE_Path_Plan_t plan_t;
TYPE_Path_Plan_t plan_t2;
pTYPE_Path_Plan_t pPlan_t = &plan_t;
pTYPE_Path_Plan_t pPlan_t2 = &plan_t2;
*/
C620_MotorInfomation_t* g_MotorInfo_t;
C610_MotorInfomation_t* g_MotorInfo610_t;

void Control_Info_Print(void)
{
	drive_printf("mode:%d\r\n", g_Control_t.mode);
	drive_printf("PID: %f, %f, %f\r\n", 
			g_Control_t.debug_data1, 
			g_Control_t.debug_data2,
			g_Control_t.debug_data3);
	
	drive_printf("  V, %5d, %5d, %5d\r\n", 
				g_Control_t.x,        
	            g_Control_t.y,  
	            g_Control_t.theta );
}


/**
  * @ brief Chassis需要的所有固件初始化
  *
  *
  */

void Chassis_Init(void)
{
    Chassis_API_Init();
    Moto_API_PID_Init_C620(g_Moto_t + 0);
    Moto_API_PID_Init_C620(g_Moto_t + 1);
    Moto_API_PID_Init_C620(g_Moto_t + 2);
    Moto_API_PID_Init_C620(g_Moto_t + 3);
    
    Moto_API_PID_Init_C610(g_Moto610_t + 0);
    Moto_API_PID_Init_C610(g_Moto610_t + 1);
    Moto_API_PID_Init_C610(g_Moto610_t + 2);
    Moto_API_PID_Init_C610(g_Moto610_t + 3);
    
    g_MotorInfo_t = C620_MotorInfo;
    g_MotorInfo610_t = C610_MotorInfo;
    
    
    PathPlan_Init(pPlan_t, &g_Path3_Info_t, PATH_PLAN_MODE_2);
    PathPlan_Init(pPlan_t2, &g_Path4_Info_t, PATH_PLAN_MODE_2);
}


 
void Romote_Data2Control(void)
{
    static uint8_t i = 0;
    i ++;
    g_Control_t.x     =   Remote_GetRX() ;
    g_Control_t.y     =   Remote_GetRY() ;
    g_Control_t.theta =   Remote_GetLX() / 50.0;
    
    g_Control_t.debug_data1 = Remote_GetV1();
    g_Control_t.debug_data2 = Remote_GetV2() ;
    g_Control_t.debug_data3 = Remote_GetV3();
    
    if (Joystick_Connect_Flag == 0x00)
    {
         g_Control_t.mode = CHASSIS_TASK_NONE;
    }

    else if(Remote_GetKEY16())
    {
        g_Control_t.mode = CHASSIS_TASK_REMOTE;   
    }
    
   
    if (i >= 99)
    {
        drive_printf("    Romote_Data2Control\r\n");
        i = 0;
    }
}


//遥控器底盘调PID

void ChassisDebugTask(void)
{
    float kp, ki, kd;
    kp = g_Control_t.debug_data1 / 100.0f;
    ki = g_Control_t.debug_data2 / 1000.0f;
    kd = g_Control_t.debug_data3 / 100.0f;
    
    {
        g_Moto_t[0].pid_Velocity_t.Kp = kp;
        g_Moto_t[1].pid_Velocity_t.Kp = kp;
        g_Moto_t[2].pid_Velocity_t.Kp = kp;
        g_Moto_t[3].pid_Velocity_t.Kp = kp;
        
    }
    {
        g_Moto_t[0].pid_Velocity_t.Ki = ki;
        g_Moto_t[1].pid_Velocity_t.Ki = ki;
        g_Moto_t[2].pid_Velocity_t.Ki = ki;
        g_Moto_t[3].pid_Velocity_t.Ki = ki;
    }
    {
        g_Moto_t[0].pid_Velocity_t.Kd = kd;
        g_Moto_t[1].pid_Velocity_t.Kd = kd;
        g_Moto_t[2].pid_Velocity_t.Kd = kd;
        g_Moto_t[3].pid_Velocity_t.Kd = kd;
    }
    Chassis_API_setSta_XYA(g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
    Chassis_API_setV(g_Control_t.x, g_Control_t.y, g_Control_t.theta, 0);
}


/**
  * @brief 在定时器中每ms调用
  *        获取柔化后的电流值并发送
  *
  * @param 
  * @retval
  *        
  *        
  */

void ChassisTask_1ms_SentData(void)
{
    float v[4] =   {0, 0, 0, 0};
    #if USE_STREER
    float p[4] =   {0, 0, 0, 0};
    #endif
    int16_t i[4] = {0, 0, 0, 0};
    
    static int16_t num = 0;
    num ++;
    if ( g_ChassisConfig_t.chassisMode == CHASSIS_MODE_ROMOTE ||
      //   g_ChassisConfig_t.chassisMode == CHASSIS_MODE_AUTO   ||
      //   g_ChassisConfig_t.chassisMode == CHASSIS_MODE_LOCK_1 ||
         g_ChassisConfig_t.chassisMode == CHASSIS_MODE_DEBUG
         )
    {
       
        // 获取每个轮的速度
        Chassis_API_CalcWheelSpeed();
        Chassis_API_GetWheelV(v + 0, v + 1, v + 2, v + 3);
        //当主控用
        //ControlPlate_API_SendMotoSpeed(PLATE_ID_C620, v[0],v[1],v[2],v[3]);
		        
        Moto_API_SetMotoInfo(g_Moto_t + 0, g_MotorInfo_t[0].Velocity, 0);
        Moto_API_SetMotoInfo(g_Moto_t + 1, g_MotorInfo_t[1].Velocity, 0);
        Moto_API_SetMotoInfo(g_Moto_t + 2, g_MotorInfo_t[2].Velocity, 0);
        Moto_API_SetMotoInfo(g_Moto_t + 3, g_MotorInfo_t[3].Velocity, 0);
        
        //  PID计算应该发送的电流, 获取要发送的电流
        i[0] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_t + 0, v[0]);
        i[1] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_t + 1, v[1]);
        i[2] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_t + 2, v[2]);
        i[3] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_t + 3, v[3]);
        C620_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        
#if  USE_STREER
        Chassis_API_getPosition(p+0, p+1, p+2, p+3);
        //当主控用
        //ControlPlate_API_SendMotoPosition(PLATE_ID_C610, p[0], p[1], p[2], p[3]);
        Moto_API_SetMotoInfo(g_Moto_t + 0, g_MotorInfo610_t[0].Velocity, g_MotorInfo610_t[0].Positon);
        Moto_API_SetMotoInfo(g_Moto_t + 1, g_MotorInfo610_t[1].Velocity, g_MotorInfo610_t[1].Positon);
        Moto_API_SetMotoInfo(g_Moto_t + 2, g_MotorInfo610_t[2].Velocity, g_MotorInfo610_t[2].Positon);
        Moto_API_SetMotoInfo(g_Moto_t + 3, g_MotorInfo610_t[3].Velocity, g_MotorInfo610_t[3].Positon);
        
        i[0] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_t + 0, p[0], 10000);
        i[1] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_t + 1, p[1], 10000);
        i[2] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_t + 2, p[2], 10000);
        i[3] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_t + 3, p[3], 10000);
        
        C610_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
#endif
    }
    else if (g_ChassisConfig_t.chassisMode == CHASSIS_MODE_DISNABEL)
    {
        C620_API_SendCurrentVal(0, 0, 0, 0);
#if  USE_STREER       
        C610_API_SendCurrentVal(0, 0, 0, 0);
#endif
    }
    else
    {
        C620_API_SendCurrentVal(0, 0, 0, 0);
#if  USE_STREER       
        C610_API_SendCurrentVal(0, 0, 0, 0);
#endif
    }
    
    if (num > 999)
    {			
        drive_printf("   wheel V :%f,%f,%f,%f\r\n", v[0], v[1], v[2], v[3]);
        drive_printf("   moto  i :%d,%d,%d,%d\r\n", i[0], i[1], i[2], i[3]);
        drive_printf("     omega :%f  X:%f Y:%f \r\n" ,g_Locator_t.Yaw,g_Locator_t.xVal, g_Locator_t.yVal);
        num = 0;
    }
}

void ChassisRemoteTask(void)
{   
    Chassis_API_setSta_XYA(g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
    Chassis_API_setV(g_Control_t.x, g_Control_t.y, g_Control_t.theta, 0);
}
/*
void ChassisAutoTask(void)
{
    float x, y, a;
    static int  i = 0;
    i ++ ;
    PathPlan_SetChassisInfo(pPlan_t, 
            g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
    if(PathPlan_Run_One_Time(pPlan_t))
    {
        PathPlan_GetV_XYA(pPlan_t, &x, &y, &a);
        Chassis_API_setSta_XYA(g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
        Chassis_API_setV(x, y, 0.0f, 1);
        e_Auto_Finish_Flag = AUTO_FINISH_FALG_1;
    }
    else
    {
        PathPlan_GetV_XYA(pPlan_t, &x, &y, &a);
        Chassis_API_setSta_XYA(g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
        Chassis_API_setV(x, y, 0.0f, 1);
        e_Auto_Finish_Flag = AUTO_FINISH_FLAG_NONE;
    }
    if ( i > 100)
    {
        drive_printf("%d  , %f,  %f\r\n",pPlan_t->prePointNum, x, y);
        PathPlan_Point_Print(pPlan_t, pPlan_t->prePointNum);
        i = 0;
    }
}
void ChassisAutoTask2(void)
{
    float x, y, a;
    static int  i = 0;
    i ++ ;
    PathPlan_SetChassisInfo(pPlan_t2, 
            g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
    if(PathPlan_Run_One_Time(pPlan_t2))
    {
        PathPlan_GetV_XYA(pPlan_t2, &x, &y, &a);
     //   Chassis_API_setSta_XYA(g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
        Chassis_API_setV(x, y, 0.0f, 1); 
        e_Auto_Finish_Flag = AUTO_FINISH_FALG_2;
    }
    else
    {
        PathPlan_GetV_XYA(pPlan_t2, &x, &y, &a);
        Chassis_API_setSta_XYA(g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
        Chassis_API_setV(x, y, 0.0f, 1);
        e_Auto_Finish_Flag = AUTO_FINISH_FLAG_NONE;
    }
        
    
    if ( i > 100)
    {
        drive_printf("%d  , %f,  %f\r\n",pPlan_t2->prePointNum, x, y);
        PathPlan_Point_Print(pPlan_t2, pPlan_t2->prePointNum);
        i = 0;
    }
}
void ChassisLockTask(void)
{
    // 双锁
   // Chassis_API_setSta_XYA(g_Locator_t.xVal, g_Locator_t.yVal, g_Locator_t.Yaw);
    Chassis_API_setV(0, 0, 0, 2);
}
*/


void ChassisLoseForce(void)
{
    Chassis_API_StatusReset();
}


//将 g_Control_t 中的mode 变为 实

void ChassisTask(void)
   
{
    static uint16_t i = 0;
    i ++;
   //  LED_Control_API(2, LED_LIGHT, g_Control_t.mode * 2, 200, 1000);
    if (i >= 199)
    {
        
        drive_printf("  mode :%d \r\n", g_Control_t.mode);
        i = 0;
    }
    if (g_Control_t.mode == CHASSIS_TASK_DEBUG)
    {
       g_ChassisConfig_t.chassisMode = CHASSIS_MODE_DEBUG;
      // ChassisDebugTask();
       ChassisLoseForce();
    }
    else if(g_Control_t.mode == CHASSIS_TASK_REMOTE)
    {
        g_ChassisConfig_t.chassisMode = CHASSIS_MODE_ROMOTE;
        ChassisRemoteTask();
    }
    else if(g_Control_t.mode == CHASSIS_TASK_NONE)
    {
        g_ChassisConfig_t.chassisMode = CHASSIS_MODE_DISNABEL;
        ChassisLoseForce();
    }
    
    else if(g_Control_t.mode == CHASSIS_TASK_AUTO)
    {
       g_ChassisConfig_t.chassisMode = CHASSIS_MODE_AUTO;
     //  ChassisAutoTask();
        ChassisLoseForce();
    }
   else if(g_Control_t.mode == CHASSIS_TASK_AUTO2)
    {
        g_ChassisConfig_t.chassisMode = CHASSIS_MODE_AUTO;
      //  ChassisAutoTask2();
        ChassisLoseForce();
    }
    else if(g_Control_t.mode == CHASSIS_TASK_LOCK_1)
    {
        g_ChassisConfig_t.chassisMode = CHASSIS_MODE_LOCK_1;
       ChassisLoseForce();
     //   ChassisLockTask();        
    }
    else
    {
         ChassisLoseForce();
    }
}








