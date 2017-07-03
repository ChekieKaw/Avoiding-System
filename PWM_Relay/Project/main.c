/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��main.c
 * ����    ��S1�������£���������ʼ������S2�������£�������ֹͣ������
 * ��汾  ��V3.50
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-04-20
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "exti.h"
#include "systick.h"
#include "key.h"
#include "string.h"
#include "pwm.h"

//���ݽṹ
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

//��������
void System_Run(void);


//�ⲿ��������
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
  * @brief  Main program.˵��������ͨ��һΪpitch,ͨ��2Ϊroll��
  * @param  None
  * @retval None
  */
int main(void)
{
		LED_GPIO_Config();//��ʼ��LED
		USART1_Config();
	  Exti_Config();
  	KEY_GPIO_Config();
		SYSTICK_Init();//��ʼ���δ�ʱ��
		TIM2_PWM_Init(19999,71);//��ʼ��ͨ�ö�ʱ��TIM2,���pwm
	TIM3_PWM_Init(19999,71);//��ʼ����ʱ��TIM3,��ң���������PWM�ź�
	TIM1_PWM_Init(19999,71);//��ʼ����ʱ��TIM1����ģ���������PWM�ź�
	
//    printf("*****************************************************************\r\n");
//    printf("*                                                               *\r\n");
//    printf("*  Thank you for using The Development Board Of YuanDi   ! ^_^  *\r\n");
//	  printf("*  ����S1�����󣬷�������ʼ��   \r\n");
//	  printf("*  ����S2�����󣬷�����ֹͣ��   \r\n");
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
