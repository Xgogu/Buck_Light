#include "pwm.h"
#include "led.h"
#include "usart.h"
#include "adc.h"

#define KP 0.10
#define KI 0.01
float voltage_ref = 12;  
float voltage_fb = 0; 
float duty_cycle = 0.0; 
float error1 = 0.0; 
float integral = 0.0; 
float control_signal = 0.0; 
float trueVoltage = 0.0; 
float light=0;
u8 light_test=0;
float jian=6;
float vef=0;
float fanwei=100;
int pwm=0;
int check=1;


//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM14_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 

	
	TIM_Cmd(TIM14, ENABLE);  //使能TIM14

 
										  
}  





void TIM4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_InternalClockConfig(TIM4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4,ENABLE); //使能定时器3
	
}

//定时器3中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
		if(check==1)
		{
		voltage_fb =(float)ADC_ConvertedValue[2]*3/4096;
		light = 100-(ADC_ConvertedValue[0]/40);	
		light_test=(int)light;
		vef=jian*light/fanwei;
		voltage_ref=12-vef;
       error1 = voltage_ref*2500*120/1000/20000/2*10 - voltage_fb;
       integral += error1;
       control_signal = KP * error1 + KI * integral;
       if (control_signal > 1)
       {
           control_signal = 1;
       }
       else if (control_signal < 0)
       {
           control_signal = 0;
       }
       duty_cycle = control_signal * 100;
	   TIM3->CCR2 = duty_cycle;
	   TIM3->CCR1 = duty_cycle;
   }
	else
	{
	
	TIM3->CCR1 = pwm;
	}
	}
	
}


void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	//????????IO???
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3????    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//??PORTC??	
	

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GF9 ??? TIM14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //GF9 ??? TIM14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); //GF9 ??? TIM14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); //GF9 ??? TIM14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //???
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//??????14
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	
	//???TIM3 Channel1 PWM??	 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIM14?CCR1????????

	//???TIM3 Channel2 PWM??	 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3OC2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIM14?CCR2????????

	//???TIM3 Channel3 PWM??	 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3OC3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIM14?CCR3????????

	//???TIM3 Channel4 PWM??	 
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3OC4
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIM14?CCR4???????
	
    TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE?? 
	TIM_Cmd(TIM3, ENABLE);  //??TIM3
}		
