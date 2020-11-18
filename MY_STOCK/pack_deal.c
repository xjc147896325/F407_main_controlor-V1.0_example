#include "pack_deal.h"
/********************************************************************************
* @File name:   readpack.c
* @Author:      ShengNan Wang
* @Version:     1.3.1
* @Date:        2019.12.24
* @Description: 通用打包解包解包
*   版权所有，盗版也没什么用
*               
*********************************************************************************
* @解包移植指南:
*   在对应串口初始化头文件中 定义 解包的数据包结构体 
*       声明一个对应的结构体变量为全局变量,方便外部访问
*       或直接访问 数据包结构体 全局变量   
*
*   在串口中断服务函数中
*            定义一个uint8_t的静态数据接收缓冲区, 至少比数据包字节大小 加4 大
*            定义 TYPE_Pack_Info_t 数据包结构体为静态变量
*               并完成对应数据的初始化(设置信息), 详情见头文件
*   在串口中断服务函数判断中断是否为接收中断
*       是则, 清除中断标志
*             接收当前字节数据ch, 并调用 uint8_t Pack_Deal_API_Read_Pack(pTYPE_Pack_Info_t , ch)
*                           根据返回值判断是还处理数据等后序操作
*                               返回值为0时还未完成一个数据包接收, 一般不做任何操作
*                               返回值为1时完成一个数据接收, 
*                                    强制转换 接收缓冲区指针 为 解包的数据包结构体 的指针
*                                    并包取值赋给 解包的数据包结构体
*
* @打包移植指南:
*     在对应打包发送的头文件中 定义打包的数据包结构体 _t
*         声明一个对应的结构体变量为全局变量,方便外部访问
*         或直接访问 数据包结构体 全局变量
*
*     在发闼函数中
*          定义一个uint8_t的静态数据发送缓冲区, 至少比数据包字节大小 加4 大
*          定义 TYPE_Pack_Info_t 数据包结构体为静态变量
*              并完成对应数据的初始化, 详情见头文件
*
                                        制转换数据包结构体指针为uint8_t*
*     调用void Pack_Deal_API_Data_Pack(pTYPE_Pack_Info_t, (uint8_t*) p_t(&_t)),   把数据打包到数据发送缓冲区
*          从数据发送缓冲区读取并串口发送 pTYPE_Pack_Info_t -> count 个数据
*
*  @更新说明
*     1.2.0    2019,10,15
*                 1.修正可能收不到数据的BUG
*                 2.增加一个设置数据包信息的API
*                 3.重命名函数
*                 4.添加了一个移植示例
*     1.2.1  2019.12.18
*                 1.修正接收一个错误包后接收不到数据的BUG
*     1.3.1  2019.12.24
*                 1.增加无帖头帖尾的数据打包, bitcopy
*     1.3.2  2020.2.13
*                 1.增加在线状态处理函数 Online_State_Deal,
*******************************************************************************/


/*******************************************************
*
* Function name ：Read_Pack
* Description   : 通用解包 串口接收数据
*                 在串口中断服务函数调用
* Parameter     ：
*        @pPackInfo_t      数据包信息结构体指针
*                               详情见头文件
*        @ch               收到的一个字节数据
*       
* Return        ：1 接收完成
                  0 接收未完成
**********************************************************/



uint8_t Pack_Deal_API_Read_Pack(pTYPE_Pack_Info_t pPackInfo_t, uint8_t ch)
{
    switch (pPackInfo_t -> count)
    {
        case 0:
            // 帧头1
            if (ch == pPackInfo_t -> frame_head1)
            {
                // 正确
                if(pPackInfo_t -> frame_head2 == FRAME_NONE)
                    // 不存在帧头2
                    pPackInfo_t -> count = 2; // 转到接收数据
                else
                    // 存在帧头2
                    pPackInfo_t -> count = 1;  //转到帧头2判断
            }
            else
            {
                // 错误
                pPackInfo_t -> count = 0;     // 转到帧头1判断
            }
            
            //19.12.18 add的语句
            pPackInfo_t -> i = 0;
            break;
            
        case 1:
            // 帧头2
            
            // 正确
            if (ch == pPackInfo_t -> frame_head2)
                pPackInfo_t -> count = 2; // 转到接收数据
            
            // 数据还是帧头1的,(当把帧尾2判断为帧头1)
            else if (ch == pPackInfo_t -> frame_head1)
                pPackInfo_t -> count = 1; // 继续帧头2判断
            // 错误
            else
                pPackInfo_t -> count = 0; // 转到帧头1判断

            
            break;
            
            
        case 2:
            // 接收数据
            pPackInfo_t -> pack[pPackInfo_t -> i] = ch;
            pPackInfo_t -> i ++;
            if (pPackInfo_t -> i >= pPackInfo_t -> data_size)
            {
                // 接收数据完成
                pPackInfo_t -> count = 3; // 转到帧尾1判断
            }
            break;
            
        case 3:
            // 帧尾1判断
            if (ch == pPackInfo_t -> frame_end1)
            {
                // 帧尾1正确
                if(pPackInfo_t -> frame_end2 == FRAME_NONE)
                {
                    // 不存在帧尾2
                    pPackInfo_t -> count = 0;        // 接收下一帧数据
                    
                    return 1;
                }
                else
                {
                    // 存在帧尾2
                    pPackInfo_t -> count = 4;
                }                
            }
            else
            {
                // 帧尾1错误
                pPackInfo_t -> count = 0;   //重新接收数据
            }
            break;
        case 4:
            // 帧尾2判断
            if (ch == pPackInfo_t -> frame_end2)
            {
                // 帧尾2正确
                pPackInfo_t -> count = 0;         // 接收下一帧数据
                return 1;
            }
            else
            {
                // 帧尾2错误
                pPackInfo_t -> count = 0; //重新接收数据
            }
            break;
        
        default:
            //出错重置
            pPackInfo_t -> count = 0;  //出错重置
    }
    return 0;
}

/*******************************************************
*
* Function name ：Data_Pack
* Description   : 根据打包信息,打包对应的结构体 到 数据包发送缓冲区
*                 
* Parameter     ：
*        @pPackInfo_t      数据包信息结构体指针
*                               详情见头文件
*        @p_t              数据包信息结构体指针 强制转换 为uint8_t*后的指针
*       
* Return        : None
                
**********************************************************/



void Pack_Deal_API_Data_Pack(pTYPE_Pack_Info_t pPackInfo_t, uint8_t* p_t)
{
    uint8_t i = 0;
    //加帧头1
    uint8_t offset = 1;
    pPackInfo_t -> pack[0] = pPackInfo_t -> frame_head1;
    
    if (pPackInfo_t -> frame_head2 != FRAME_NONE)
    {   
        // 存在帧头2 并添加
        offset = 2;
        pPackInfo_t -> pack[1] = pPackInfo_t -> frame_head2;
    }
    
    // 数据打包
    for(i = 0;i < pPackInfo_t -> data_size; i++)
    {
        pPackInfo_t -> pack [i + offset] = p_t[i];
    }
    
    // 加帧尾1
    pPackInfo_t -> pack[i + offset] = pPackInfo_t -> frame_end1;
    i ++;
    if (pPackInfo_t -> frame_end2 == FRAME_NONE)
    {
        // 不存在帧尾2, 未尾加0
        pPackInfo_t -> pack[i + offset] = 0;
    }
    else
    {
        // 存在帧尾2, 加帧尾2, 并未尾加0
        pPackInfo_t -> pack[i + offset] = pPackInfo_t -> frame_end2;
        i ++;
        pPackInfo_t -> pack[i + offset] = 0;
    }
    pPackInfo_t -> count = pPackInfo_t -> data_size + offset * 2;
    pPackInfo_t -> i = 0;
}

/*******************************************************
*
* Function name ：Pack_Deal_API_SetPackInfo
* Description   : 设置数据包结构体的信息
*                 
* Parameter     ：
*        @pPackInfo_t      数据包信息结构体指针
*                               详情见头文件
*        @frame_head1
*        @frame_head2
*        @frame_end1
*        @frame_end2
*       
* Return        : None
                
**********************************************************/


void    Pack_Deal_API_SetPackInfo(pTYPE_Pack_Info_t pPackInfo_t, uint8_t data_size,
                                  uint8_t frame_head1, uint8_t frame_head2,
                                  uint8_t frame_end1,  uint8_t frame_end2,
                                  uint8_t* buff)
{                                 
    pPackInfo_t -> data_size   = data_size;    
    pPackInfo_t -> frame_head1 = frame_head1; 
    pPackInfo_t -> frame_head2 = frame_head2; 
    pPackInfo_t -> frame_end1  = frame_end1;  
    pPackInfo_t -> frame_end2  = frame_end2;  
    pPackInfo_t -> pack        =  buff;       
    
    pPackInfo_t -> i = 0;
    pPackInfo_t -> count = 0;
    
}

/*
******************************************************
*
* Function name Pack_Deal_API_Bit_Copy
* Description   : Bit数据拷贝
*                 
* Parameter     ：
*        @pNew      :拷贝数据指针                               
*        @pOld      :原数据指针
*        @dataSize  :Bit大小

*       
* Return        : None
                
**********************************************************/


void Pack_Deal_API_Bit_Copy(uint8_t *pNew, uint8_t *pOld, uint8_t dataSize)
{
    int i;
    for(i = 0 ; i < dataSize;i++)
    {
        pNew[i] = pOld[i];
    }
}

/*
******************************************************
*
* Function name Online_State_Deal
* Description   : 在线状态处理
*                 一般每秒调用,根据
*           旧状态, 当前帖数, 最小帖数, 得到一个新状态
*                 
* Parameter     ：
*        @oldSta     :旧状态                               
*        @frame_num  :当前帖数
*        @min_frame  :最小帖数

*       
* Return        : 新状态 0x00, 从未上线
*                        0x01, 上线后掉线
*                        0x80, 第一次上线
*                        0x40, 掉线后上线
*                        other, err
**********************************************************/




uint8_t Online_State_Deal(uint8_t oldSta, uint8_t frame_num, uint8_t min_frame)
{
    
    if (frame_num < min_frame)
	{
         switch (oldSta) 
             {
                 case 0x00:        
                    return 0x00;
                 
                 case 0x01:        
                    return 0x01; 
                 
                 case 0x80:              
                    return 0x01;   
                 
                 case 0x40:        
                    return 0x01;      
                 
                 default:
                    return 0x08;
             }
	}
	else
	{        
        switch (oldSta) 
             {
                 case 0x00:        
                    return 0x80;
                 
                 case 0x01:        
                    return 0x40;  
                 
                 case 0x80:              
                    return 0x80;    
                 
                 case 0x40:        
                    return 0x40;
                 
                 default:
                    return 0x08;
             }
      
       
	}    
}






/*
一个示例

typedef struct TYPE_Rx_PID_t
{
    int num;
    float x;
    float y;
    float z;
}TYPE_Rx_PID_t, *pTYPE_Rx_PID_t;

TYPE_Rx_PID_t g_PID_1;

void UART3_Config(void)
{
    ....;
}

void USART3_IRQHandler(void)
{
    static uint8_t RxData;
    static uint8_t buff[50];
    static uint8_t buff2[50];
    
    static TYPE_Pack_Info_t pid_Pack_Info = {
        16, 0x0D, 0x0A, 0x0A, 0x0D, buff};
    
    static TYPE_Pack_Info_t pid_Pack_Info2;
    Pack_Deal_API_SetPackInfo(pid_Pack_Info2, 16, 0x0D, 0x0A, 0x0A, 0x0D, buff);
    
    if(USART_GetFlagStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        
        RxData = USART_ReceiveData(USART3);
        if (Pack_Deal_API_Read_Pack(&pid_Pack_Info, RxData))
        {
            //接收到一个数据包
            //强制转换 赋值
            g_PID_1 = *((pTYPE_Rx_PID_t) buff);
            
            // 重新打包并发送
            Pack_Deal_API_Data_Pack(&pid_Pack_Info2,(uint8_t *) &g_PID_1);
            while(pid_Pack_Info2.i < pid_Pack_Info2.count)
            {
               while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
               USART_SendData(USART3, pid_Pack_Info2.pack[pid_Pack_Info2.i ++]);
            }
        }
    }
}

*/
                                      













