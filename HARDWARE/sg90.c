#include "sg90.h"

//定时器4初始化，定时器2-5都是通用定时器，3，4是16位寄存器，2，5是32位寄存器
void TIM4_init(u16 prc , u16 arr)//每次100us定时
{
	//初始化结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

   /* TIM3 clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 
   /* GPIOA and GPIOB clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	//配置结构体GPIO
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//配置结构体TIM

	TIM_TimeBaseStructure.TIM_Prescaler = prc-1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//配置结构体
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	//打开定时器
	TIM_Cmd(TIM4, ENABLE);
	
	//因为用到定时器的输出比较功能所以没有使用定时器中断
}

void Sg90_Init(void)
{
	//配置定时器4通道1初始化,建议在外部配置
	TIM4_init(720 , 2000);
	
}

void sg90_forward(void)
{
	//设置pwm的占空比来调节舵机
	TIM_SetCompare1(TIM4,50);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,100);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,150);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,200);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,250);
	Delay_ms(100);
}

void sg90_reversal(void)
{
	TIM_SetCompare1(TIM4,250);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,200);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,150);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,100);
	Delay_ms(100);
	TIM_SetCompare1(TIM4,50);
	Delay_ms(100);
}
