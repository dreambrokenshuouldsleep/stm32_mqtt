#include "tim.h"


//打开定时器6,6,7为基本定时器
void Tim6_Init(u16 prc, u16 arr)//prc为分频系数，arr为计数时间
{
	//初始化结构体
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	
	TIM_InitStruct.TIM_Prescaler = prc-1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = arr-1;
	
	//开启中断
	TIM_TimeBaseInit(TIM6 , &TIM_InitStruct);
	
	//开启NVIC
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	//打开定时器
	TIM_Cmd(TIM6, ENABLE);
}


//定时器6中断服务函数
void TIM6_IRQHandler()
{
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
	{
		TIM_ClearFlag(TIM6,TIM_IT_Update);
	}
	
}
