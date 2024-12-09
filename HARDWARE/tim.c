#include "tim.h"


//�򿪶�ʱ��6,6,7Ϊ������ʱ��
void Tim6_Init(u16 prc, u16 arr)//prcΪ��Ƶϵ����arrΪ����ʱ��
{
	//��ʼ���ṹ��
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	
	TIM_InitStruct.TIM_Prescaler = prc-1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = arr-1;
	
	//�����ж�
	TIM_TimeBaseInit(TIM6 , &TIM_InitStruct);
	
	//����NVIC
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	//�򿪶�ʱ��
	TIM_Cmd(TIM6, ENABLE);
}


//��ʱ��6�жϷ�����
void TIM6_IRQHandler()
{
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
	{
		TIM_ClearFlag(TIM6,TIM_IT_Update);
	}
	
}
