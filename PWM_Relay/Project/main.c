/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    ：S1按键按下，蜂鸣器开始发声，S2按键按下，蜂鸣器停止发声；
 * 库版本  ：V3.50
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-04-20
 * 调试方式：J-Link-OB
**********************************************************************************/	

//头文件
#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "exti.h"
#include "systick.h"
#include "key.h"
#include "string.h"
#include "pwm.h"

//数据结构
typedef struct __pwm_cmp
{
	int thrust;
	int yaw;
	int roll;
	int pitch;
}pwm_cmp;

pwm_cmp pwm_rc_in;
pwm_cmp pwm_sonar_in;
pwm_cmp pwm_out;

//extern u8 TIM3CH1_CAPTURE_STA;
//extern u8 TIM3CH2_CAPTURE_STA;
//extern u16 TIM3CH1_CAPTURE_VAL;
//extern u16 TIM3CH2_CAPTURE_VAL;

//函数声明
void System_Run(void);


//外部变量声明
extern u8 TIM3CH1_CAPTURE_STA;
extern u8 TIM3CH2_CAPTURE_STA;
extern u16 TIM3CH1_CAPTURE_VAL;
extern u16 TIM3CH2_CAPTURE_VAL;
extern u8 TIM1CH1_CAPTURE_STA;
extern u8 TIM1CH2_CAPTURE_STA;
extern u16 TIM1CH1_CAPTURE_VAL;
extern u16 TIM1CH2_CAPTURE_VAL;

/**
  * @file   main
  * @brief  Main program.说明：所有通道一为pitch,通道2为roll！
  * @param  None
  * @retval None
  */
int main(void)
{
		LED_GPIO_Config();//初始化LED
		USART1_Config();
	  Exti_Config();
  	KEY_GPIO_Config();
		SYSTICK_Init();//初始化滴答时钟
		TIM2_PWM_Init(19999,71);//初始化通用定时器TIM2,输出pwm
	TIM3_PWM_Init(19999,71);//初始化定时器TIM3,从遥控器输入的PWM信号
	TIM1_PWM_Init(19999,71);//初始化定时器TIM1，从模拟器输入的PWM信号
	
//    printf("*****************************************************************\r\n");
//    printf("*                                                               *\r\n");
//    printf("*  Thank you for using The Development Board Of YuanDi   ! ^_^  *\r\n");
//	  printf("*  按下S1按键后，蜂鸣器开始响   \r\n");
//	  printf("*  按下S2按键后，蜂鸣器停止响   \r\n");
//    printf("*                                                               *\r\n");
//    printf("*****************************************************************\r\n");
	
	memset(&pwm_out,0,sizeof(pwm_out));
	memset(&pwm_rc_in,0,sizeof(pwm_rc_in));
	memset(&pwm_sonar_in,0,sizeof(pwm_sonar_in));
		
	
		while(1)
		{
			if(TIM1CH1_CAPTURE_STA&0x80||TIM1CH2_CAPTURE_STA&0x80)
			{
				pwm_sonar_in.pitch=TIM1CH1_CAPTURE_VAL;
				pwm_sonar_in.roll=TIM1CH2_CAPTURE_VAL;
				pwm_out.pitch=pwm_sonar_in.pitch;
				pwm_out.roll=pwm_sonar_in.roll;
			}
			else if(TIM3CH1_CAPTURE_STA&0x80||TIM3CH2_CAPTURE_STA&0x80)
			{
				pwm_rc_in.pitch=TIM3CH1_CAPTURE_VAL;
				pwm_rc_in.roll=TIM3CH2_CAPTURE_VAL;
				pwm_out.pitch=pwm_rc_in.pitch;
				pwm_out.roll=pwm_rc_in.roll;
			}
			TIM_SetCompare1(TIM2,pwm_out.pitch);
			TIM_SetCompare2(TIM2,pwm_out.roll);
		}
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
