/**
  *****************************************************************
  * @file    pid.c
  * @author  ShengNan Wang
  * @version V1.2.2
  * @date    2020.2.2
  * @brief   PID控制相关
  *****************************************************************    
  * @更新说明:
  *     1.1.0: 
  *          1.对PID进行了简单重构 封装
  *     1.2.1:
  *          1.分离两种PID的计算, 以减少计算资源
  *     1.2.2  2.11:
  *          1.去掉简短的函数调用,应该能提极少的速
  *          2.注释掉同时计算两种PID的函数
  *
  *****************************************************************
  */


# include "pid.h"


/**
  * @brief  设置PID为默认参数
  * @param  
  * @retval 
  */
void PID_t_init(pTYPE_PID_t pPID_t)
{
    
    pPID_t -> Kp = PID_DEFAULT_P_I_D;
    pPID_t -> Ki = PID_DEFAULT_P_I_D;
    pPID_t -> Kd = PID_DEFAULT_P_I_D;
    
    pPID_t -> deadBand  = PID_DEFAULT_DEAD_BAND;
    pPID_t -> maxOutput = PID_DEFAULT_MAX_OUT_PUT;
    pPID_t -> maxIntegral = PID_DEFAULT_MAX_INTEGRAL;
    pPID_t -> maxIncrease = PID_DEFAULT_MAX_INCREASE;
    
    pPID_t -> err       = 0;
    pPID_t -> preErr    = 0;
    pPID_t -> lastErr   = 0;
    
    pPID_t -> feedBack        = 0;
    pPID_t -> reference       = 0;
    pPID_t -> outPutPositon   = 0;
    pPID_t -> outPutIncrement = 0;
    
}

/**
  * @brief  设置PID计算产生的数据为0
  * @param  
  * @retval 
  */

void PID_t_data_clear(pTYPE_PID_t pPID_t)
{    
    pPID_t -> err      = 0;
    pPID_t -> preErr   = 0;
    pPID_t -> lastErr  = 0;
    
    pPID_t -> feedBack        = 0;
    pPID_t -> reference       = 0;
    pPID_t -> outPutPositon   = 0;
    pPID_t -> outPutIncrement = 0;
 
}
/**
  * @brief  设置PID所有值为0
  * @param  
  * @retval 
  */


void PID_t_all_clear(pTYPE_PID_t pPID_t)
{
    pPID_t -> Kp = 0;
    pPID_t -> Ki = 0;
    pPID_t -> Kd = 0;
    
    pPID_t -> deadBand    = 0;
    pPID_t -> maxOutput   = 0;
    pPID_t -> maxIntegral = 0;
    pPID_t -> maxIncrease = 0;
    
    pPID_t -> err            = 0;
    pPID_t -> preErr         = 0;
    pPID_t -> lastErr        = 0;
    
    pPID_t -> feedBack        = 0;
    pPID_t -> reference       = 0;
    pPID_t -> outPutPositon   = 0;
    pPID_t -> outPutIncrement = 0;
              
}             


/**
  * @brief  设置PID
  * @param  P I D 
  *         当参数小于0时保持原值不变
  * @retval 
  */

void PID_set_pid_parameter( pTYPE_PID_t   pPID_t,
                            TYPE_PID_DATA Kp,
                            TYPE_PID_DATA Ki, 
                            TYPE_PID_DATA Kd)
{
    if ( Kp >= 0) pPID_t -> Kp = Kp;
    if ( Ki >= 0) pPID_t -> Ki = Ki;
    if ( Kd >= 0) pPID_t -> Kd = Ki;
}




/**
  * @brief      设置参考值
  * @param      
  * @retval     
  *            
  *            
  */
void PID_API_set_reference( pTYPE_PID_t   pPID_t,
                            TYPE_PID_DATA reference)
{
    pPID_t -> reference = reference;
}


/**
  * @brief   设置当前值
  * @param   
  * @retval  
  *          
  *          
  */
void PID_set_feed_back( pTYPE_PID_t   pPID_t,
                        TYPE_PID_DATA feedBack)
{
    pPID_t -> feedBack = feedBack;
}


/**
  * @brief   计算位置PID偏差相关数据
  * @param   
  * @retval  
  *          
  *          
  */

void PID_cal_positon_err(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA current_err =  pPID_t -> reference - pPID_t -> feedBack ;
    pPID_t -> err = current_err;
    // 积分计算               
    pPID_t -> errIntegral += current_err;
    if (pPID_t -> errIntegral > pPID_t -> maxIntegral )
        pPID_t -> errIntegral = pPID_t -> maxIntegral;
    else if (pPID_t -> errIntegral < -pPID_t -> maxIntegral )
        pPID_t -> errIntegral = -pPID_t -> maxIntegral;
    
    // 微分计算 
    // 是否微分滤波
    pPID_t -> errDifferential = current_err - pPID_t -> lastErr;
   
    #if PID_DIFFERENTIAL_WAVE_FILTERING
    {
        ;
    }
    #endif
}
/**
  * @brief   计算增量PID偏差相关数据
  * @param   
  * @retval  
  *          
  *          
  */

void PID_cal_increment_err(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA current_err =  pPID_t -> reference - pPID_t -> feedBack ;
    pPID_t -> preErr  = pPID_t -> lastErr;
    pPID_t -> lastErr = pPID_t -> err    ;
    pPID_t -> err     = current_err      ;
}



/**
  * @brief   计算位置PID输出值
  * @param   
  * @retval  
  *          
  *          
  */
TYPE_PID_DATA PID_cal_positon_output(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA outPutPositon = 0.0f;
    // 死区
    if (pPID_t -> feedBack > pPID_t -> reference - pPID_t -> deadBand &&
        pPID_t -> feedBack < pPID_t -> reference + pPID_t -> deadBand)
    {
            ;
    }
    //非死区
    else
    {
        outPutPositon =    pPID_t -> Kp * pPID_t -> err                        +
                           pPID_t -> Ki * pPID_t -> errIntegral                -
                           pPID_t -> Kd * pPID_t -> errDifferential            ;
        // limite outPutPositon
        if (outPutPositon > pPID_t -> maxOutput)
            outPutPositon = pPID_t -> maxOutput;
        else if (outPutPositon < -pPID_t -> maxOutput)
            outPutPositon = -pPID_t -> maxOutput;
        pPID_t -> outPutPositon = outPutPositon;
    }
    return pPID_t -> outPutPositon;
}

/**
  * @brief   计算增量PID输出值
  * @param   
  * @retval  
  *          
  *          
  */
TYPE_PID_DATA PID_cal_increment_output(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA outPutIncrement = 0.0f;    
    
    // 死区
    if (pPID_t -> feedBack > pPID_t -> reference - pPID_t -> deadBand &&
        pPID_t -> feedBack < pPID_t -> reference + pPID_t -> deadBand)
    {
            ;
    }
    //非死区
    else
    {
        outPutIncrement =   pPID_t -> Kp * (pPID_t -> err - pPID_t -> lastErr)
                        +   pPID_t -> Ki *  pPID_t -> err
                        +   pPID_t -> Kd * (pPID_t -> err - 2 * pPID_t -> lastErr + pPID_t -> preErr);
        // limite outPutIntegral
        if (outPutIncrement > pPID_t -> maxIncrease)
            outPutIncrement = pPID_t -> maxIncrease;
        else if (outPutIncrement < -pPID_t -> maxIncrease)
            outPutIncrement = -pPID_t -> maxIncrease;
        pPID_t -> outPutIncrement = outPutIncrement;
    }
    return pPID_t -> outPutIncrement;
}

TYPE_PID_DATA PID_get_outPutPosition(pTYPE_PID_t pPID_t)
{
    return pPID_t -> outPutPositon;
}
TYPE_PID_DATA PID_get_outPutIncrement(pTYPE_PID_t pPID_t)
{
    return pPID_t -> outPutIncrement;
}

/**
  * @brief   当前值 得到 增量输出值 的API
             refence  设定的参考值  
  * @param   pPID_t   PID结构体指针
             feedBack 当前值
  * @retval  增量式PID值
  *        
  *        
  */
TYPE_PID_DATA PID_API_inputData_getOutputIncrement( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA feedBack)
{
    
    pPID_t -> feedBack           = feedBack;    
    PID_cal_increment_err          (pPID_t);
    return PID_cal_increment_output(pPID_t);
   // return PID_get_outPutIncrement (pPID_t);
}



/**
  * @brief   当前值 得到 位置输出值 的API
             refence  设定的参考值  
  * @param   pPID_t   PID结构体指针
             feedBack 当前值
  * @retval  位置式PID值
  *        
  *        
  */
TYPE_PID_DATA PID_API_inputData_getOutputPosition( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA feedBack)
{
    
    pPID_t -> feedBack           = feedBack;    
    PID_cal_positon_err(pPID_t);
    return PID_cal_positon_output (pPID_t);
    //return PID_get_outPutPosition (pPID_t);
}

/**
  * @brief   设置PID所有参数的API
  * @param   pPID_t   PID结构体指针
              ...  对应PID参数值

                  小于0时对应的参数值保持不变

  * @retval  None
  *        
  *        
  */
void          PID_API_set_parameter               ( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA Kp,
                                                    TYPE_PID_DATA Ki, 
                                                    TYPE_PID_DATA Kd,
                                                    TYPE_PID_DATA deadBand,        
                                                    TYPE_PID_DATA maxOutput,
                                                    TYPE_PID_DATA maxIntegral,
                                                    TYPE_PID_DATA maxIncrease)
{
    
    if ( Kp >= 0) pPID_t -> Kp = Kp;
    if ( Ki >= 0) pPID_t -> Ki = Ki;
    if ( Kd >= 0) pPID_t -> Kd = Ki;
    if (deadBand    >= 0) pPID_t -> deadBand    = deadBand   ;
    if (maxOutput   >= 0) pPID_t -> maxOutput   = maxOutput  ;
    if (maxIntegral >= 0) pPID_t -> maxIntegral = maxIntegral;
    if (maxIncrease >= 0) pPID_t -> maxIncrease = maxIncrease;
  
}





/**
  * @brief   一个使用实例
  * @param   
              
  * @retval  
  *        
  *        
  */

/*

void pid_test(void)
{
    float feedBack1 = 100, feedBack2 = 100 ;
    TYPE_PID_t  pid_t1, pid_t2;
    pTYPE_PID_t ppid_t1 = &pid_t1, ppid_t2 = &pid_t2;
    
    PID_t_init(ppid_t1);
    PID_API_set_parameter(ppid_t1, 0.2, 0.2, 0, -1, -1, -1, -1);
    
    PID_t_init(ppid_t2);    
    PID_API_set_parameter(ppid_t2, 0.02, 0.11, 0, -1, -1, -1, -1);
    PID_API_set_reference(ppid_t1, 19);
    PID_API_set_reference(ppid_t2, 1000);
    
    while(1)
    {
        feedBack1 = PID_API_inputData_getOutputPosition  (ppid_t1, feedBack1);
        feedBack2 += PID_API_inputData_getOutputIncrement(ppid_t2, feedBack2);
        
        printf("position  :%f  \t increment  :%f\r\n", feedBack1, feedBack2);
        Sleep(100);
    }   
}

int main()
{
    
    pid_test();
    return 0;   
    
}
*/

