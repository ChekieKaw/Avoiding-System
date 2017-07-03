#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"


//u8 TIM3CH1_CAPTURE_STA=0;
//u8 TIM3CH2_CAPTURE_STA=0;
//u16 TIM3CH1_CAPTURE_VAL=0;
//u16 TIM3CH2_CAPTURE_VAL=0;
//TIM_ICInitTypeDef	TIM1_ICInitStructure;
//TIM_ICInitTypeDef	TIM3_ICInitStructure;

void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr, u16 psc);

//void TIM3_IRQHandler(void);
//void TIM1_IRQHandler(void);

#endif
