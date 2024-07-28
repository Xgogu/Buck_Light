#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern float voltage_ref;
extern int pwm;
extern int check;
extern u8 light_test;
void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM1_PWM_Init(u32 arr,u32 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u32 arr,u32 psc);

#endif
