
/**
  ***********************************************************
  * @file    chassis.c
  * @author  Shengnan Wang
  * @version V1.3.3
  * @date    2020.10.10
  * @brief   底盘的一些解算
  *          , 
  *
  ***********************************************************
  * @更新说明:
  *           1.1.0 第一版成型
  *                 后面进行了多次修正，调试
  *           
  *           1.2.0    1.增加可自行更改的坐想转换函数.
  *                    2.增加角度锁定函数
  *    20,1,8  1.3.0   1.增加舵轮的控制 
  *
  *    20,1,10 1.3.1   1.增加坐标锁定    
  *
  *    20,1,19 1.3.2   1.去轮速度柔化，增加轴速度柔化
  *                    2.进行简单名称修正
  *                    3.增加舵轮的八字模式， 自锁、自旋，未测试
  *    20.10.10 1.3.3  1.改锁定模式的参数为宏定义
  *
  **********************************************************
  * 版权所有,但翻版怎么究?
  ****/


#include "chassis.h"


TYPE_ChassisConfig_t g_ChassisConfig_t;
TYPE_ChassisStatus_t g_ChassisStatus_t;


/**
  * @brief 底盘状态初重置
  *        设置底盘为初始状态
  *        设定所有底盘数据重新为0
  *        重置后底盘处失能模式
  *        
  * @param 
  * @retval 
  * 
  */

void Chassis_API_StatusReset(void)
{    
    
    g_ChassisStatus_t.speed_x = 0;
    g_ChassisStatus_t.speed_y = 0;
    g_ChassisStatus_t.speed_a = 0;
   
    g_ChassisStatus_t.refSpeed_x = 0;
    g_ChassisStatus_t.refSpeed_y = 0;
    g_ChassisStatus_t.refSpeed_a = 0;
 
    g_ChassisStatus_t.speed_Wheel_1 = 0;
    g_ChassisStatus_t.speed_Wheel_2 = 0;
    g_ChassisStatus_t.speed_Wheel_3 = 0;   
    g_ChassisStatus_t.speed_Wheel_4 = 0;
    
    g_ChassisStatus_t.x = 0;
    g_ChassisStatus_t.y = 0;
    g_ChassisStatus_t.theta = 0;
    
    #if USE_STREER
    g_ChassisStatus_t.position_Wheel_1 = 0;
    g_ChassisStatus_t.position_Wheel_2 = 0;
    g_ChassisStatus_t.position_Wheel_3 = 0;
    g_ChassisStatus_t.position_Wheel_4 = 0;
        
    g_ChassisStatus_t.zero_Position_Wheel_1 = 0;
    g_ChassisStatus_t.zero_Position_Wheel_2 = 0;
    g_ChassisStatus_t.zero_Position_Wheel_3 = 0;
    g_ChassisStatus_t.zero_Position_Wheel_4 = 0;
    #endif
    
    g_ChassisConfig_t.chassisMode   = CHASSIS_MODE_DISNABEL;
}

/**
  * @brief                     底盘初始化
  *        设置底盘的基本参数         及设定        底盘的初始状态
  *        直接修改此函数来修                     底盘处于失力状态
  *        改设定数据 
  * @param 
  * @retval 
  * 
  */

void Chassis_API_Init(void)
{
    
    g_ChassisConfig_t.wit  = 0    ;
    g_ChassisConfig_t.len  = 0    ;
    g_ChassisConfig_t.r    = 0    ;
    
    g_ChassisConfig_t.maxAxisSpeed = MAX_AXIS_SPEED;
    g_ChassisConfig_t.maxOmagaSpeed= MAX_OMAGA_SPEED;
    
    g_ChassisConfig_t.axisRatio    = RPM_RATION / 1000.0f;  
    g_ChassisConfig_t.omegaRatio   = RPM_RATION / CHASSIS_R;
    
    g_ChassisConfig_t.dirRatio     = 1000 ;
    g_ChassisConfig_t.turnRatio    = 1000 ;
    
    g_ChassisConfig_t.accLimit      = ACC_AXIS  / 1000;
    g_ChassisConfig_t.omagaAccLimit = ACC_OMAGA / 1000;
    
    g_ChassisConfig_t.accLimitStatu = ACC_LIMIT;
    g_ChassisConfig_t.chassisMode   = CHASSIS_MODE_DISNABEL;
    g_ChassisConfig_t.wheelNum      = DEFAULT_CHASSIS_WHEEL; //底盘轮数
    
    Chassis_API_StatusReset();
    
}

/**
  * @brief 轴速度柔化及加速限制
  *        此函数在定时器中每ms或更长调用一次
  *
  * @param 
  * @retval 
  * 
  */

 void Chassis_API_AxisSpeedSoften_Acclimit_Task_1ms(void)
{
 
    if (g_ChassisConfig_t.accLimitStatu == AccLimit_Axis)
    {
        g_ChassisStatus_t.speed_a = FlexibelValue(g_ChassisStatus_t.refSpeed_a, g_ChassisStatus_t.speed_a, g_ChassisConfig_t.omagaAccLimit);
        g_ChassisStatus_t.speed_x = FlexibelValue(g_ChassisStatus_t.refSpeed_x, g_ChassisStatus_t.speed_x, g_ChassisConfig_t.accLimit);
        g_ChassisStatus_t.speed_y = FlexibelValue(g_ChassisStatus_t.refSpeed_y, g_ChassisStatus_t.speed_y, g_ChassisConfig_t.accLimit);
    }
    else
    {
        g_ChassisStatus_t.speed_a = g_ChassisStatus_t.refSpeed_a;
        g_ChassisStatus_t.speed_x = g_ChassisStatus_t.refSpeed_x;
        g_ChassisStatus_t.speed_y = g_ChassisStatus_t.refSpeed_y;
    }
}

/**
  * @brief 由平动速度和角速度计算轮上速度
  * @param 
  * @retval 
  * 
  */

void Chassis_API_CalcWheelSpeed(void)
{
   #if USE_STREER
    int32_t position;
    #endif
    
    if(g_ChassisConfig_t.wheelNum == CHASSIS_WHEEL_3)
    {
        ;
        //三轮的, 用不到,也未验证正确性 就注释了
        /*
            g_ChassisStatus_t.Speed_Wheel_1 = +g_ChassisStatus_t.Speed_x + 1.7320508f * g_ChassisStatus_t.Speed_y - g_ChassisStatus_t.OmegaRatio * g_ChassisStatus_t.Speed_a;
            g_ChassisStatus_t.Speed_Wheel_2 = +g_ChassisStatus_t.Speed_x - 1.7320508f * g_ChassisStatus_t.Speed_y - g_ChassisStatus_t.OmegaRatio * g_ChassisStatus_t.Speed_a;
            wser 
            g_ChassisStatus_t.Speed_Wheel_3 = -2 * g_ChassisStatus_t.Speed_x - g_ChassisStatus_t.OmegaRatio * g_ChassisStatus_t.Speed_a;
            //g_ChassisStatus_t.Speed_Wheel_4 = 0;
        }*/
    }
    else if(g_ChassisConfig_t.wheelNum == CHASSIS_WHEEL_4)
    {
     /*                                   
                         2 ---                                          
                            |                                         
                            |                                         
                            |                                         
                   1 |------+-------|3                                         
                            |                                         
                            |                                         
                            |         
                         4 ---         
     */
        //全向轮
        g_ChassisStatus_t.speed_Wheel_1 = - g_ChassisStatus_t.speed_x * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
        g_ChassisStatus_t.speed_Wheel_2 = - g_ChassisStatus_t.speed_y * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
        g_ChassisStatus_t.speed_Wheel_3 = + g_ChassisStatus_t.speed_x * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
        g_ChassisStatus_t.speed_Wheel_4 = + g_ChassisStatus_t.speed_y * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
    }
#if USE_STREER
    else if(g_ChassisConfig_t.wheelNum == CHASSIS_WHEEL_8)
    {
        //舵轮
        
    
        g_ChassisStatus_t.speed_Wheel_1 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        g_ChassisStatus_t.speed_Wheel_2 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        g_ChassisStatus_t.speed_Wheel_3 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        g_ChassisStatus_t.speed_Wheel_4 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        
        position = g_ChassisStatus_t.speed_x * g_ChassisConfig_t.dirRatio;
        absLimit_int32(&position, MAX_C610_POSITON);
        g_ChassisStatus_t.position_Wheel_1 = position;
        g_ChassisStatus_t.position_Wheel_2 = position;
        g_ChassisStatus_t.position_Wheel_3 = position;
        g_ChassisStatus_t.position_Wheel_4 = position;
        
        position = g_ChassisStatus_t.speed_a * g_ChassisConfig_t.turnRatio;
        g_ChassisStatus_t.position_Wheel_1 += position;
        g_ChassisStatus_t.position_Wheel_2 += position;     
    }
#endif
    else
    {
        
    }
}
/**
  * @brief 控制坐标转换函数,根据情况自行更改
  *        
  * @param 
  * @retval 
  * 
  */
void Chassis_API_Coordinate_Tranform(float *x, float *y)
{

 //全向轮大底盘
   float x_ = *x, y_ = *y;
    *x =  -x_ *  0.707f + y_ * 0.707f;
    *y =   x_ *  0.707f + y_ * 0.707f;  
    
}
/**
  * @brief 角度锁定
  *        
  * @param 
  * @retval 
  * 
  */
void Chassis_API_Angle_Lock(float kp, float deadBand)
{   
    float delta = g_ChassisStatus_t.theta - g_ChassisStatus_t.lock_a;
    if(delta <-deadBand || delta >deadBand)
        g_ChassisStatus_t.refSpeed_a = -delta * kp;
    else
        g_ChassisStatus_t.refSpeed_a = 0;
}
/**
  * @brief 坐标锁定
  *        
  * @param 
  * @retval 
  * 
  */
 void  Chassis_API_Coordinate_Lock(float kp, float deadBand)
{
    float delta_x = g_ChassisStatus_t.x - g_ChassisStatus_t.lock_x;
    float delta_y = g_ChassisStatus_t.y - g_ChassisStatus_t.lock_y;
    float x = 0;
    float y = 0;
    
    if(delta_x <-deadBand || delta_x >deadBand)
        x = -delta_x * kp;
    else
        x = 0.0f;
   
    if(delta_y <-deadBand || delta_y >deadBand)
        y = -delta_y * kp;
    else
        y = 0.0f;
    
    Chassis_API_Coordinate_Tranform(&x, &y);
    
    g_ChassisStatus_t.refSpeed_x = x; 
    g_ChassisStatus_t.refSpeed_y = y;
}

/**
  * @brief 设置速度速度, x y 的单位为 mm/s 
  *        并计算轮应该达到的速度
  * @param x, y, a:
  *        lock_flag : 0普通遥控
  *                  : 1角度锁定
  *                  : 2角度、位置都锁定
  *                  : 3位置锁定
  * @retval 
  * 
  */


void Chassis_API_setV(float x, float y,float a, uint8_t lock_flag)
{
    
    
    absLimit(&x, g_ChassisConfig_t.maxAxisSpeed);
    absLimit(&y, g_ChassisConfig_t.maxAxisSpeed);
    absLimit(&a, g_ChassisConfig_t.maxOmagaSpeed);
    
    
    if(lock_flag==1)
    {
         Chassis_API_Coordinate_Tranform(&x, &y);
        g_ChassisStatus_t.refSpeed_x = x; 
        g_ChassisStatus_t.refSpeed_y = y;
        
        g_ChassisStatus_t.lock_a +=  a;
        g_ChassisStatus_t.lock_x = g_ChassisStatus_t.x;
        g_ChassisStatus_t.lock_y = g_ChassisStatus_t.y;
        
        Chassis_API_Angle_Lock(CHASSIS_LOCK_A_KP, CHASSIS_LOCK_A_DEADBAND);
    }

    else if(lock_flag == 0)
    {
        Chassis_API_Coordinate_Tranform(&x, &y); 
        g_ChassisStatus_t.refSpeed_x = x;
        g_ChassisStatus_t.refSpeed_y = y;
        g_ChassisStatus_t.refSpeed_a = a;
        
        g_ChassisStatus_t.lock_a = g_ChassisStatus_t.theta;
        g_ChassisStatus_t.lock_x = g_ChassisStatus_t.x;
        g_ChassisStatus_t.lock_y = g_ChassisStatus_t.y;   
    }
    else if(lock_flag == 2)
    {
       a *= 0.01f ;
       x *= 0.01f;
       y *= 0.01f;
       g_ChassisStatus_t.lock_a += a;
       g_ChassisStatus_t.lock_x += x;
       g_ChassisStatus_t.lock_y += y;
        
        Chassis_API_Angle_Lock(CHASSIS_LOCK_A_KP, CHASSIS_LOCK_A_DEADBAND);
        Chassis_API_Coordinate_Lock(CHASSIS_LOCK_X_KP, CHASSIS_LOCK_X_DEADBAND);
    }
}

/**
  * @brief 设置底盘当前信息 
  * @param 
  * @retval 
  * 
  */
void Chassis_API_setSta_XYA(float x, float y, float a)
{
    g_ChassisStatus_t.x     = x;   
    g_ChassisStatus_t.y     = y;
    g_ChassisStatus_t.theta = a;
}

/**
  * @brief 获取速度, x y 的单位为 mm/s 
  * @param 
  * @retval 
  * 
  */
void Chassis_API_getSta_XYA(float *x, float *y,float *a)
{
    *x = g_ChassisStatus_t.x;   
    *y = g_ChassisStatus_t.y;
    *a = g_ChassisStatus_t.theta;
}

/**
  * @brief 获取达到设定速度时 轮的速度 的API(单位RPM)
  * @param 
  * @retval
  * 
  */
void Chassis_API_GetWheelV(float *v1,float *v2,float *v3,float *v4)
{
    *v1 = g_ChassisStatus_t.speed_Wheel_1;
    *v2 = g_ChassisStatus_t.speed_Wheel_2;
    *v3 = g_ChassisStatus_t.speed_Wheel_3;
    *v4 = g_ChassisStatus_t.speed_Wheel_4;
}
/**
  * @brief 获取发送的速度API(单位RPM)
  * @param 外部存每个轮的速度的指针
  * @retval 
  * 
  */

#if USE_STREER  //舵轮用
/**
  * @brief 获取达到设定位置 的API(单位RPM)
  * @param 
  * @retval
  * 
  */
void Chassis_API_getPosition(float *p1,float *p2,float *p3,float *p4)
{
    *p1 = g_ChassisStatus_t.position_Wheel_1;
    *p2 = g_ChassisStatus_t.position_Wheel_2;
    *p3 = g_ChassisStatus_t.position_Wheel_3;
    *p4 = g_ChassisStatus_t.position_Wheel_4;
}
// 2自旋
void  Chassis_API_Streer_To_Rotate(void)
{
    g_ChassisStatus_t.position_Wheel_1 = STREER_DIR_RATION * 90;
    g_ChassisStatus_t.position_Wheel_2 = STREER_DIR_RATION * 180;
    g_ChassisStatus_t.position_Wheel_3 = STREER_DIR_RATION * -90;
    g_ChassisStatus_t.position_Wheel_4 = STREER_DIR_RATION * 180;
    
    g_ChassisConfig_t.chassisMode = CHASSIS_MODE_STREER_ROTATE;
}
// 旋转
void  Chassis_API_Streer_Rotate(void)
{
    
    g_ChassisStatus_t.speed_Wheel_1 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
    g_ChassisStatus_t.speed_Wheel_2 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
    g_ChassisStatus_t.speed_Wheel_3 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
    g_ChassisStatus_t.speed_Wheel_4 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
}

//  2遥控
void  Chassis_API_Streer_To_Romate(void)
{
    g_ChassisStatus_t.position_Wheel_1 = 0;
    g_ChassisStatus_t.position_Wheel_2 = 0;
    g_ChassisStatus_t.position_Wheel_3 = 0;
    g_ChassisStatus_t.position_Wheel_4 = 0;
    
    g_ChassisConfig_t.chassisMode = CHASSIS_MODE_ROMOTE;
}

void  Chassis_API_Streer_Mark_Zero(int32_t* p)
{
    ;
}

#endif

float Chassis_API_getMaxSpeed(void)
    
{
    return g_ChassisConfig_t.maxAxisSpeed;   
}

/**
  * @brief 获取底盘配置的API
  * @param None
  * @retval 底盘的配置的结构体指针
  * 
  */


pTYPE_ChassisConfig_t Chassis_API_GetConfig(void)
{ 
    return &g_ChassisConfig_t;
}
/**
  * @brief 获取底盘状态的API
  * @param None
  * @retval 底盘的状态的结构体指针
  * 
  */

pTYPE_ChassisStatus_t Chassis_API_GetStatus(void)
{    
    return &g_ChassisStatus_t;
}

/*
a demo,

void Chassis_Test(void)
{
    Chassis_API_Init();

    tim_1ms_task:
        Chassis_API_AxisSpeedSoften_Acclimit_Task_1ms();

    Chassis_API_setV(0, 0, 0); 
    Chassis_API_CalcWheelSpeed();
    Chassis_API_GetWheelV(*v1, *v2, *v3, *v4);
    
    
    
}

*/



















