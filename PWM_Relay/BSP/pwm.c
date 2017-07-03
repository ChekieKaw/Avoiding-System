
/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��PWM.c
 * ����    ����ʼ��TIM2��ͨ��PWM�˿� 
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-04-20
 * Ӳ������: beep->PA1
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "PWM.h"

 /**
  * @file   TIM2_PWM_Init
  * @brief  ��ʼ��TIM2ͨ����PWM�˿�
  * @param  ��
  * @retval ��
  */
void TIM2_PWM_Init(u16 arr,u16 psc)//��������������pwm
{	
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//ʹ��TIM2ʱ�ӣ�GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�Ӻ͸��ù���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0; //TIM2��ͨ��PWM��������˿�PA1��ͨ��1PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(3600*10/72M)s=0.0005s����2KƵ��
	TIM_TimeBaseStructure.TIM_Period = arr;//3600-1;//�Զ���װ��ֵ��ȡֵ������0x0000~0xFFFF֮��
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//10-1;//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ����ȡֵ������0x0000~0xFFFF֮��							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ	 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
		
	//����������  
	//TIM_OCInitStructure.TIM_Pulse =900;               //���ô�װ�벶��ȽϼĴ���������ֵ,ȡֵ������0x0000~0xFFFF֮�䣬ռ�ձ�900/3600
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);          //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);               //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���                     
	//TIM_Cmd(TIM2, DISABLE);	//��ֹTIM2ʹ��
  TIM_Cmd(TIM2, ENABLE);//ʹ�ܶ�ʱ��	
}
/**
  * @file   TIM3_PWM_Init
  * @brief  TIM3�����ã�ͨ��12����ң��������
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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //TIM3 һͨ��PA6����ͨ��PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//TIM3 ��ͨ��PB0/��ͨ��PB1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;//3600-1;//�Զ���װ��ֵ��ȡֵ������0x0000~0xFFFF֮��
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//10-1;//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ����ȡֵ������0x0000~0xFFFF֮��							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ	 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 
	
//	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;//TIM3 һͨ��PA6����ͨ��PA7
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//ʹ��һͨ��
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;//TIM3 һͨ��PA6����ͨ��PA7
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//ʹ�ܶ�ͨ��
	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;//TIM3 ��ͨ��PB0/��ͨ��PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//ʹ����ͨ��
//	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;//TIM3 ��ͨ��PB0/��ͨ��PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//ʹ����ͨ��


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�

	TIM_Cmd(TIM1,ENABLE );
}



/**
  * @file   TIM1_PWM_Init
  * @brief  TIM1�����ã�ͨ��12����ģ��ң�����ź�ң��������
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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //TIM1 һͨ��PA8����ͨ��PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//TIM1 ��ͨ��P10/��ͨ��PA11
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;//3600-1;//�Զ���װ��ֵ��ȡֵ������0x0000~0xFFFF֮��
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//10-1;//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ����ȡֵ������0x0000~0xFFFF֮��							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ	 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 
	
//	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;//TIM1 һͨ��PA8����ͨ��PA9
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);//ʹ��һͨ��
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;//TIM3 һͨ��PA6����ͨ��PA7
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);//ʹ�ܶ�ͨ��
	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;//TIM3 ��ͨ��PB0/��ͨ��PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//ʹ����ͨ��
//	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;//TIM3 ��ͨ��PB0/��ͨ��PB1
//	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);//ʹ����ͨ��


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�
	
	TIM_Cmd(TIM1,ENABLE );
}


/**
  * @file   TIM3_IRQHandler
  * @brief  TIM3���жϷ�����
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
  * @brief  TIM1���жϷ�����
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

//u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
//u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
// 
////��ʱ��5�жϷ������	 
//void TIM5_IRQHandler(void)
//{ 

// 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
//	{	  
//		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
//		 
//		{	    
//			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//				{
//					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
//					TIM5CH1_CAPTURE_VAL=0XFFFF;
//				}else TIM5CH1_CAPTURE_STA++;
//			}	 
//		}
//	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
//		{	
//			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
//			{	  			
//				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
//				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
//		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//			}else  								//��δ��ʼ,��һ�β���������
//			{
//				TIM5CH1_CAPTURE_STA=0;			//���
//				TIM5CH1_CAPTURE_VAL=0;
//	 			TIM_SetCounter(TIM5,0);
//				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
//		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//			}		    
//		}			     	    					   
// 	}
// 
//    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
// 
//}

