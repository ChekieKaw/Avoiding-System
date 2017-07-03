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
#include "math.h"
#include "string.h"
#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "exti.h"
#include "systick.h"
#include "key.h"
#include "i2c.h"
#include "ks103.h"




//数据声明
#define KS103_Address 0xd0
#define detect 0xb8
#define oldaddress 0xe8
#define ks103_1_address 0xd0 //done
#define ks103_2_address 0xd2 //done
#define ks103_3_address 0xd4 //done
#define ks103_4_address 0xd6 //done
#define distance_y_1 distance[0]//y+
#define distance_x_0 distance[1]//x-
#define distance_y_0 distance[2]//y-
#define distance_x_1 distance[3]//x+
#define Gravity 9.85
#define system_ID 0x00
#define component_ID 0x00
#define PI 3.14159
#define pwm_roll_max 1916
#define pwm_roll_min 1097
#define pwm_roll_middle ((pwm_roll_max+pwm_roll_min)/2)
#define pwm_pitch_max 1088
#define pwm_pitch_min 1918
#define pwm_pitch_middle ((pwm_pitch_max+pwm_pitch_min)/2)

typedef struct sonar_distance
{
	u8 address;
	u16 distance;
	int position;//This is the falg of sonar position (or its direction)
}Sonar_Distance;

typedef struct attitute
{
	double pitch_angel;
	double roll_angel;
	double yaw_angel;
	double thrust;
}ATTITUTE;


typedef struct __pwm_cmp
{
	int thrust;
	int yaw;
	int roll;
	int pitch;
}pwm_cmp;

Sonar_Distance distance[4];
ATTITUTE current_attitute;
ATTITUTE set_attitute;
double distance_x;
double distance_y;
pwm_cmp pwm_out;
#define distance_init() {\
						distance[0].address=ks103_1_address;distance[0].position=1;distance[0].distance=12000;\
						distance[1].address=ks103_2_address;distance[1].position=-1;distance[1].distance=12000;\
						distance[2].address=ks103_3_address;distance[2].position=-1;distance[2].distance=12000;\
						distance[3].address=ks103_4_address;distance[3].position=1;distance[3].distance=12000;\
						}



//函数声明
void System_Run(void);
double distance2angle(double distance);



/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	int i=0,j=0;
	memset(&current_attitute,0,sizeof(current_attitute));
	memset(&set_attitute,0,sizeof(set_attitute));
	LED_GPIO_Config();//初始化LED
	USART1_Config();
	Exti_Config();
	KEY_GPIO_Config();
	SYSTICK_Init();//初始化滴答时钟
	TIM2_PWM_Init(19999,71);//初始化通用定时器TIM2：分频单位：1us
//    printf("*****************************************************************\r\n");
//    printf("*                                                               *\r\n");
//    printf("*  Thank you for using The Development Board Of YuanDi   ! ^_^  *\r\n");
//	  printf("*  按下S1按键后，蜂鸣器开始响   \r\n");
//	  printf("*  按下S2按键后，蜂鸣器停止响   \r\n");
//    printf("*                                                               *\r\n");
//    printf("*****************************************************************\r\n");
	distance_init();
		while(1)
		{
			distance_x=0;
			distance_y=0;
			for(i=0;i<4;i++)
			{
			KS103_WriteOneByte(distance[i].address,0x02,0xb8);
			}
			while(!SCL_read);
			do
			{
				for(j=0;j<4;j++)
				{
					distance[j].distance=KS103_GetData(distance[i].address,0x02);
				}
			}while(distance[0].distance>11000&&
			  distance[1].distance>11000&&
			  distance[2].distance>11000&&
			  distance[3].distance>11000);
			distance_x=(distance_x_1.distance*distance_x_1.position+distance_x_0.distance*distance_x_0.position)*
					(1/sqrt(1+pow(tan(current_attitute.roll_angel/360*2*PI),2)));
		distance_y=(distance_y_1.distance*distance_y_1.position+distance_y_0.distance*distance_y_0.position)*
					(1/sqrt(1+pow(tan(current_attitute.pitch_angel/360*2*PI),2)));
		set_attitute.pitch_angel=(float)atan(-distance2angle(
														((double)fabs((float)distance_x)*(double)fabs((float)distance_y))/
														sqrt(pow(distance_x,2)+pow(distance_y,2))
													)*distance_x/
									(Gravity*sqrt(pow(distance_x,2)+pow(distance_y,2)))
									)/(2*PI)*360;
		set_attitute.roll_angel=(float)atan(-distance2angle(
		((double)fabs((float)distance_x)*(double)fabs((float)distance_y))/
		sqrt(pow(distance_x,2)+pow(distance_y,2)))*distance_y/
		(Gravity*sqrt(pow(distance_x,2)+pow(distance_y,2)))
		)/(2*PI)*360;
		
		pwm_out.roll=(int)(pwm_roll_middle+(set_attitute.roll_angel/45)*(pwm_roll_max-pwm_roll_min)/2);
		pwm_out.pitch=(int)(pwm_pitch_middle+(set_attitute.pitch_angel/45)*(pwm_pitch_max-pwm_pitch_min)/2);
														
		
		if(set_attitute.roll_angel!=0)	TIM_SetCompare2(TIM2,pwm_out.roll);
		else TIM_SetCompare2(TIM2,0);//PA1输出
		if(set_attitute.pitch_angel!=0) TIM_SetCompare1(TIM1,pwm_out.pitch);//PA0输出
		else TIM_SetCompare1(TIM1,0);
														
														
		}
}

/**
  * @file   distance2angle
  * @brief  convert distance data to flying angle
  * @param  double distance
  * @retval double angle
  */
double distance2angle(double distance)
{
	if (distance>=0&&distance <=1) return 5;
	else if (distance >1 && distance <= 6) return (-1+exp(-0.358*distance +2.148));
	else if (distance >6 )return 0;
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
