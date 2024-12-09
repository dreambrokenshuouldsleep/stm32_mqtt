#include "sg90.h"

//��ʱ��4��ʼ������ʱ��2-5����ͨ�ö�ʱ����3��4��16λ�Ĵ�����2��5��32λ�Ĵ���
void TIM4_init(u16 prc , u16 arr)//ÿ��100us��ʱ
{
	//��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

   /* TIM3 clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 
   /* GPIOA and GPIOB clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	//���ýṹ��GPIO
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//���ýṹ��TIM

	TIM_TimeBaseStructure.TIM_Prescaler = prc-1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//���ýṹ��
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	//�򿪶�ʱ��
	TIM_Cmd(TIM4, ENABLE);
	
	//��Ϊ�õ���ʱ��������ȽϹ�������û��ʹ�ö�ʱ���ж�
}

void Sg90_Init(void)
{
	//���ö�ʱ��4ͨ��1��ʼ��,�������ⲿ����
	TIM4_init(720 , 2000);
	
}

void sg90_forward(void)
{
	//����pwm��ռ�ձ������ڶ��
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
