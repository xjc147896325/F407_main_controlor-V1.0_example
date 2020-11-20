/**
  ***********************************************************
  * @file    moto_control.c
  * @author  Shengnan Wang
  * @version V1.2.3
  * @date    2020.1.9
  * @brief   使用控制电机的两种模式
  *          速度模式  位置模式  速度位置模式 及其PID, 
  *
  ***********************************************************
  * @更新说明:
  *     1.0.1 更名,C620 为Moto 表示这是控制电机的, 与电调关系不大
  *     1.1.0 改为moto_control 使文件独立, 相关调用全通过传参完成
  *           需要使用时用结构体定义一个电机，并初始化
  *     1.1.1 修正位置信息记录的BUG,并完善了速度位置模式,
  *              使其能顺利运行,并在3508上完成测试
  *     1.1.2 1.增加C610的电机初始化,
  *           2.增加Moto_API_PID_Init_C620函数,即原Moto_API_PID_Init
  *           3.增加舵轮上调的PID
  *     1.2.1 1.增加电机的MAX_RPM属性, 使不同电机MAX_RPM可以不同
  *           2.完成了控制C610的测试
  *           3.增加设置MAX_RPM的API
  *     1.2.2 1.修正了少数函数命名错误
  *     1.2.3 1.增加速度模式下,每次计算电流前对速度进行柔化
  *
  *
  **********************************************************
  * 版权所有
****/
#include "moto_control.h"


/**
  * @brief PID初始化C620
  * @param 
  * @retval
  */
void Moto_API_PID_Init(pTYPE_Moto_t pMoto_t)
{   
    PID_t_data_clear(&pMoto_t ->pid_Position_t);
    PID_t_data_clear(&pMoto_t ->pid_Velocity_t);
    /*全向轮用*/
    PID_API_set_parameter(& pMoto_t -> pid_Velocity_t, 
                    11.7000f,0.5000f, 8.0000f, //Kp,      Ki,    Kd, 
                    5.0f, 16383 / 20 * 10,5000.0f, 10.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    
    PID_API_set_parameter(& pMoto_t -> pid_Position_t,
                    0.0200f, 0.0100f, 0.0000f, //Kp,      Ki,    Kd, 
                    2000.0f, 8000,      3000.0f, 16.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    
    /*舵轮用       
    PID_API_set_parameter( & pMoto_t ->pid_Velocity_t,
                    5.0f, 0.5f, 0.0f, //Kp,      Ki,    Kd, 
                    50.0f, 0.8192 * 4000,3000.0f, 600.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    
    */
    pMoto_t -> max_rpm = MOTO_MAX_RPM_C620;
    pMoto_t -> soft_value = MOTO_SOFT_C620;
}
/**
  * @brief PID初始化C620
  * @param 
  * @retval
  */
void Moto_API_PID_Init_C620(pTYPE_Moto_t pMoto_t)
{
    Moto_API_PID_Init(pMoto_t);
}


/**
  * @brief PID初始化C610
  * @param 
  * @retval
  */

void Moto_API_PID_Init_C610(pTYPE_Moto_t pMoto_t)
{
    PID_t_data_clear(&pMoto_t ->pid_Position_t);
    PID_t_data_clear(&pMoto_t ->pid_Velocity_t);
           
    PID_API_set_parameter(& pMoto_t -> pid_Velocity_t, 
                    5.0f, 0.05f, 16.0f, //Kp,      Ki,    Kd, 
                    0.0f, 0.8192 * 6000,3000.0f, 16.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
                    
    PID_API_set_parameter(& pMoto_t -> pid_Position_t, 
                    0.1f, 0.0f, 0.0f, //Kp,      Ki,    Kd, 
                    100.0f, 20000.0f, 5000.0f, 16.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    pMoto_t -> max_rpm = MOTO_MAX_RPM_C610;
    pMoto_t -> soft_value = MOTO_SOFT_C610;
}


/**
  * @brief  设置电机信息
  *             速度模式可仅设置速度,位置模式可仅设置位置
  *               速度_位置模式下两个都要设置
  * @param  pMoto_t 电机的一个结构体
  *         present_velocity, present_position
  *         
  * @retval 
  */

void Moto_API_SetMotoInfo(pTYPE_Moto_t pMoto_t, int16_t present_velocity, int32_t present_position)
{
    pMoto_t -> present_velocity = present_velocity;
    pMoto_t -> present_position = present_position;
}

/**
  * @brief  设置速度环PID
  * @param  pMoto_t 电机的一个结构体
            Kp, Ki, Kd
  * @retval 
  */
void Moto_API_SetVelocityPID(
pTYPE_Moto_t pMoto_t, float Kp, float Ki, float Kd)
{
    PID_API_set_parameter(& pMoto_t -> pid_Velocity_t, Kp, Ki, Kd, -1, -1,-1 -1,-1);
}

/**
  * @brief  计算 速度模式下 设定速度下应该发送的电流
  *         
  * @param  dstVelocity : 设定的RPM
  * @retval 应该发送的电流
  */

int16_t Moto_API_Cal_GetCurrentVelocityMode(pTYPE_Moto_t pMoto_t, float dstVelocity)
{

    //与底盘的最大速度共存
#if MOTO_SOFT
    if(pMoto_t -> soft_value != 0.0f)
       dstVelocity = FlexibelValue_int16(dstVelocity,
                pMoto_t -> src_rpm, pMoto_t -> soft_value);
#endif
    absLimit(&dstVelocity, pMoto_t -> max_rpm);
    PID_API_set_reference(& pMoto_t -> pid_Velocity_t, dstVelocity);
    return (int16_t) PID_API_inputData_getOutputPosition(& pMoto_t -> pid_Velocity_t, pMoto_t -> present_velocity);
}

/**
  * @brief  设置位置环PID
  * @param  Kp, Ki, Kd
  * @retval 
  */
void Moto_API_SetPositionPID(pTYPE_Moto_t pMoto_t, float Kp, float Ki, float Kd)
{
    PID_API_set_parameter(& pMoto_t -> pid_Position_t, Kp, Ki, Kd, -1, -1,-1 -1,-1);
}

/**
  * @brief  计算 位置模式下 设定位置下应该发送的电流
  * @param  dstPosition : 设定的
  *         
  * @retval 应该发送的电流
  */

int16_t Moto_API_Cal_GetCurrentPositionMode(pTYPE_Moto_t pMoto_t, int32_t dstPosition)
{
    PID_API_set_reference(& pMoto_t -> pid_Position_t,(float) dstPosition);
    return (int16_t) PID_API_inputData_getOutputPosition(& pMoto_t -> pid_Position_t, pMoto_t -> present_position);
}



/**
  * @brief  计算 速度_位置模式下 设定位置_速度下应该发送的电流
  * @param  dstPosition : 设定的位置
  *         dstVelocity    : 设定速度
  * @retval 应该发送的电流
  * 
  */
int16_t Moto_API_Cal_GetCurrentVelocityPositionMode(pTYPE_Moto_t pMoto_t, int32_t dstPosition, float dstVelocity)
{
    int16_t v = Moto_API_Cal_GetCurrentPositionMode(pMoto_t, dstPosition);
    absLimit_int16(&v, dstVelocity);    
    return Moto_API_Cal_GetCurrentVelocityMode(pMoto_t, v);
}


/**
  * @brief  设定电机最大速度的API
  * @param  pMoto_t     : 电机的结构体指针
  *         max_rpm     : 设定的最大速度
  * @retval 应该发送的电流
  * 
  */
void Moto_API_Set_Max_RPM(pTYPE_Moto_t pMoto_t, float max_rpm)
{
    
    pMoto_t -> max_rpm = max_rpm;
}


/**
  * @brief  一个使用实例 
            参考使用，不允许运行
  * @param  
  *         
  * @retval 
  */
/*


uint8_t flag = 0;
TYPE_Moto_t moto_1;
int32_t g_set_position = 0;

C620_MotorInfomation_t** pp_MotorInfo_t;

void Moto_Init(void)
{
    Moto_API_PID_Init(&moto_1);
    Get_C620_MotorInfo(pp_MotorInfo_t);
}



void Moto_1ms_task(void)
{
    int dstPosition = pp_MotorInfo_t[1]->Position;
    int dstvelocity = pp_MotorInfo_t[1]->Velocity;
    int16_t I1;
	
    Moto_API_SetMotoInfo(&moto_1, dstvelocity, dstPosition); 
    
    I1 = Moto_API_Cal_GetCurrentVelocityPositionMode(&moto_1, g_set_position, 1000);
    absLimit_int16(&I1, 5000);
    if(flag)
        C620_SendCurrentVal(0, I1, 0, 0);
    else
        C620_SendCurrentVal(0, 0, 0, 0);
}
void Moto_Set_PID(float kp, float ki, float kd)
{
    Moto_API_SetVelocityPID(&moto_1, kp, ki, kd);
}




*/





































