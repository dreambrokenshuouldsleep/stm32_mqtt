#include "led.h"

//LEDE5��ʼ��
void LEDE5_INIT(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	//��ʼ������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	LEDE5_Close;
}

void LEDB5_INIT(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	//��ʼ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	LEDB5_Close;
}

void LED_Init(void)
{
	LEDE5_INIT();
	LEDB5_INIT();
}

