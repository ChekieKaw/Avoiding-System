
/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：PWM.c
 * 描述    ：初始化TIM2四通道PWM端口 
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-04-20
 * 硬件连接: beep->PA1
 * 调试方式：J-Link-OB
**********************************************************************************/	

//头文件
#include "PWM.h"

 /**
  * @file   TIM2_PWM_Init
  * @brief  初始化TIM2通道的PWM端口
  * @param  无
  * @retval 无
  */
void TIM2_PWM_Init(u16 arr,u16 psc)//这个函数用来输出pwm
{	
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//使能TIM2时钟，GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟和复用功能时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0; //TIM2二通道PWM波形输出端口PA1和通道1PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(3600*10/72M)s=0.0005s，即2K频率
	TIM_TimeBaseStructure.TIM_Period = arr;//3600-1;//自动重装载值，取值必须在0x0000~0xFFFF之间
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//10-1;//预分频值，+1为分频系数，取值必须在0x0000~0xFFFF之间							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式	 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
		
	//蜂鸣器控制  
	//TIM_OCInitStructure.TIM_Pulse =900;               //设置待装入捕获比较寄存器的脉冲值,取值必须在0x0000~0xFFFF之间，占空比900/3600
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);          //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能TIMx在CCR2上的预装载寄存器
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);               //使能TIMx在ARR上的预装载寄存器                     
	//TIM_Cmd(TIM2, DISABLE);	//禁止TIM2使能
  TIM_Cmd(TIM2, ENABLE);//使能定时器	
}
/**
  * @file   TIM3_PWM_Init
  * @brief  TIM3的配置，通道12用于遥控器输入
  * @param  u16 arr, u16 psc
  * @retval None
  */
//TIM_ICInitTypeDef  TIM5_ICInitStructure;
TIM_ICInitTypeDef	TIM3_ICInitStructure;

void TIM3_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef        TIM_OCInitStructure;
	NVIC_InitTypeDef		 NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //TIM3 一通道PA6、二通道PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上升沿输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//TIM3 三通道PB0/四通道PB1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;//3600-1;//自动重装载值，取值必须在0x0000~0xFFFF之间
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//10-1;//预分频值，+1为分频系数，取值必须在0x0000~0xFFFF之间							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式	 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
	
//	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
//  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;//TIM3 一通道PA6、二通道PA7
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//使能一通道
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;//TIM3 一通道PA6、二通道PA7
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//使能二通道
	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;//TIM3 三通道PB0/四通道PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//使能三通道
//	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;//TIM3 三通道PB0/四通道PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//使能四通道


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断

	TIM_Cmd(TIM1,ENABLE );
}



/**
  * @file   TIM1_PWM_Init
  * @brief  TIM1的配置，通道12用于模拟遥控器信号遥控器输入
  * @param  u16 arr, u16 psc
  * @retval None
  */
TIM_ICInitTypeDef	TIM1_ICInitStructure;

void TIM1_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef        TIM_OCInitStructure;
	NVIC_InitTypeDef		 NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //TIM1 一通道PA8、二通道PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上升沿输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//TIM1 三通道P10/四通道PA11
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;//3600-1;//自动重装载值，取值必须在0x0000~0xFFFF之间
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//10-1;//预分频值，+1为分频系数，取值必须在0x0000~0xFFFF之间							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式	 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
	
//	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
//  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;//TIM1 一通道PA8、二通道PA9
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);//使能一通道
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;//TIM3 一通道PA6、二通道PA7
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);//使能二通道
	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;//TIM3 三通道PB0/四通道PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//使能三通道
//	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;//TIM3 三通道PB0/四通道PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//使能四通道


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断
	
	TIM_Cmd(TIM1,ENABLE );
}


/**
  * @file   TIM3_IRQHandler
  * @brief  TIM3的中断服务函数
  * @param  None
  * @retval None
  */

u8 TIM3CH1_CAPTURE_STA=0;
u8 TIM3CH2_CAPTURE_STA=0;
u16 TIM3CH1_CAPTURE_VAL=0;
u16 TIM3CH2_CAPTURE_VAL=0;
void TIM3_IRQHandler(void)
{
	//CH1
	if((TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET))
	{
		if(TIM3CH1_CAPTURE_STA&0x40)
		{
			if((TIM3CH1_CAPTURE_STA&0x3F)==0X3F)
			{
				TIM3CH1_CAPTURE_STA |= 0x80;
				TIM3CH1_CAPTURE_VAL=0xFFFF;
			}
			else TIM3CH1_CAPTURE_STA++;
		}
		if(TIM_GetITStatus(TIM3,TIM_IT_CC1)!=RESET)
		{
			if(TIM3CH1_CAPTURE_STA&0x40)
			{
				TIM3CH1_CAPTURE_STA |= 0x80;
				TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM3);
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising);	
			}
			else
			{
				TIM3CH1_CAPTURE_STA=0;
				TIM3CH1_CAPTURE_VAL=0;
				TIM_SetCounter(TIM3,0);
				TIM3CH1_CAPTURE_STA |=0x40;
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);	
			}
			
		}
	}
	
	
	//CH2
	if((TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET))
	{
		if(TIM3CH2_CAPTURE_STA&0x40)
		{
			if((TIM3CH2_CAPTURE_STA&0x3F)==0X3F)
			{
				TIM3CH2_CAPTURE_STA |= 0x80;
				TIM3CH2_CAPTURE_VAL=0xFFFF;
			}
			else TIM3CH2_CAPTURE_STA++;
		}
		if(TIM_GetITStatus(TIM3,TIM_IT_CC2)!=RESET)
		{
			if(TIM3CH2_CAPTURE_STA&0x40)
			{
				TIM3CH2_CAPTURE_STA |= 0x80;
				TIM3CH2_CAPTURE_VAL=TIM_GetCapture2(TIM3);
				TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising);	
			}
			else
			{
				TIM3CH2_CAPTURE_STA=0;
				TIM3CH2_CAPTURE_VAL=0;
				TIM_SetCounter(TIM3,0);
				TIM3CH2_CAPTURE_STA |=0x40;
				TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);	
			}
			
		}
	}
}

/**
  * @file   TIM1_IRQHandler
  * @brief  TIM1的中断服务函数
  * @param  None
  * @retval None
  */
u8 TIM1CH1_CAPTURE_STA=0;
u8 TIM1CH2_CAPTURE_STA=0;
u16 TIM1CH1_CAPTURE_VAL=0;
u16 TIM1CH2_CAPTURE_VAL=0;
void TIM1_IRQHandler(void)
{
	//CH1
	if((TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET))
	{
		if(TIM1CH1_CAPTURE_STA&0x40)
		{
			if((TIM1CH1_CAPTURE_STA&0x3F)==0X3F)
			{
				TIM1CH1_CAPTURE_STA |= 0x80;
				TIM1CH1_CAPTURE_VAL=0xFFFF;
			}
			else TIM1CH1_CAPTURE_STA++;
		}
		if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET)
		{
			if(TIM1CH1_CAPTURE_STA&0x40)
			{
				TIM1CH1_CAPTURE_STA |= 0x80;
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);	
			}
			else
			{
				TIM1CH1_CAPTURE_STA=0;
				TIM1CH1_CAPTURE_VAL=0;
				TIM_SetCounter(TIM1,0);
				TIM1CH1_CAPTURE_STA |=0x40;
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);	
			}
			
		}
	}
	
	
	//CH2
	if((TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET))
	{
		if(TIM1CH2_CAPTURE_STA&0x40)
		{
			if((TIM1CH2_CAPTURE_STA&0x3F)==0X3F)
			{
				TIM1CH2_CAPTURE_STA |= 0x80;
				TIM1CH2_CAPTURE_VAL=0xFFFF;
			}
			else TIM1CH2_CAPTURE_STA++;
		}
		if(TIM_GetITStatus(TIM1,TIM_IT_CC2)!=RESET)
		{
			if(TIM3CH2_CAPTURE_STA&0x40)
			{
				TIM1CH2_CAPTURE_STA |= 0x80;
				TIM1CH2_CAPTURE_VAL=TIM_GetCapture2(TIM1);
				TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Rising);	
			}
			else
			{
				TIM1CH2_CAPTURE_STA=0;
				TIM1CH2_CAPTURE_VAL=0;
				TIM_SetCounter(TIM1,0);
				TIM1CH2_CAPTURE_STA |=0x40;
				TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Falling);	
			}
			
		}
	}
	
}

//u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
//u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
// 
////定时器5中断服务程序	 
//void TIM5_IRQHandler(void)
//{ 

// 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
//	{	  
//		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
//		 
//		{	    
//			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
//			{
//				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//				{
//					TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
//					TIM5CH1_CAPTURE_VAL=0XFFFF;
//				}else TIM5CH1_CAPTURE_STA++;
//			}	 
//		}
//	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
//		{	
//			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
//			{	  			
//				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
//				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
//		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//			}else  								//还未开始,第一次捕获上升沿
//			{
//				TIM5CH1_CAPTURE_STA=0;			//清空
//				TIM5CH1_CAPTURE_VAL=0;
//	 			TIM_SetCounter(TIM5,0);
//				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
//		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//			}		    
//		}			     	    					   
// 	}
// 
//    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
// 
//}

