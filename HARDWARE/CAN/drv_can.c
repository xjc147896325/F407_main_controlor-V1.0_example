/**
  ******************************************************************************
  * @file    C620.h
  * @author  jOKERII, WSN
  * @version V1.1
  * @date    20-11-19
  * @brief    void CAN1_Init()
			  void CAN2_Init()
          20.11.19 V1.1 add can2 drv

  ******************************************************************************
  */
#include "drv_can.h"

//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//则波特率为:42M/((6+7+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 


void CAN1_Init(void)
{

	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    NVIC_InitTypeDef       NVIC_InitStructure;
	
	//使能相关时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

	//初始化GPIO
	GPIO_Format_Init(GPIOA, GPIO_Pin_11|GPIO_Pin_12, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);

	//引脚复用映射配置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM 							 = DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM 							 = DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM 							 = DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART 							 = DISABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM 							 = DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP 							 = ENABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode 							 = CAN_Mode_Normal;	 //模式设置 
  	CAN_InitStructure.CAN_SJW  							 = CAN_SJW_1tq;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1  							 = CAN_BS1_9tq; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2  							 = CAN_BS2_4tq;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler 					 = 3;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	CAN_FilterInitStructure.CAN_FilterNumber         	 = 0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode           	 = CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale          	 = CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh         	 = 0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow          	 = 0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     	 = 0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow      	 = 0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment 	 = 0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation     	 = ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
	NVIC_InitStructure.NVIC_IRQChannel 				     = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN1_RX_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority	     = CAN1_RX_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 					 = CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN1_TX_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = CAN1_TX_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);   

	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_TME ,ENABLE);
}




void CAN2_Init(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    /*NVIC CONFIG*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


    //使能相关时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA时钟                                                                                                                    

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);//使能CAN1时钟       

    //初始化GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PB5,PB6

    //引脚复用映射配置
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2); //GPIOB5复用为CAN2
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2); //GPIOB5复用为CAN2

    //CAN单元设置
    CAN_InitStructure.CAN_TTCM=DISABLE;                      //非时间触发通信模式   
    CAN_InitStructure.CAN_ABOM=DISABLE;                      //软件自动离线管理          
    CAN_InitStructure.CAN_AWUM=DISABLE;                      //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART=ENABLE;                      //禁止报文自动传送
    CAN_InitStructure.CAN_RFLM=DISABLE;                      //报文不锁定,新的覆盖旧的  
    CAN_InitStructure.CAN_TXFP=DISABLE;                      //优先级由报文标识符决定
    CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;        //模式设置
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;                        //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;                         //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;                        //Tbs2范围CAN_BS2_1tq ~        CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler=3;                          //分频系数(Fdiv)为brp+1       
    CAN_Init(CAN2, &CAN_InitStructure);                           // 初始化CAN2
                                                                                                //500K-----> CAN_SJW_1tq+CAN_BS1_7tq+CAN_BS2_6tq   +6

    //配置过滤器
    CAN_FilterInitStructure.CAN_FilterNumber=14;          //过滤器14
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0

    CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN2_RX_PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = CAN2_RX_SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN2_TX_PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = CAN2_TX_SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);   


    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN2,CAN_IT_TME ,ENABLE);

}   







