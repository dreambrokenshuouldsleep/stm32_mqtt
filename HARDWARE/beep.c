#include "beep.h"

//GPIO����PB8
void Beep_Init(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	//��ʼ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	Beep_State(0);
	
}
