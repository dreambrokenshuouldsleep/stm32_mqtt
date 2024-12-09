#include "hc-sro4.h"

//PE7 PE8
void Hc_Sro4_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	//������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	//Echo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//Trig
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TRIG_RESET;
}

//��ȡ����
uint32_t SR04_GetDistance(void)
{
	uint32_t cnt=0;
	
	//1.TRIG�������10us�ĸߵ�ƽ
	TRIG_SET;
	Delay_us(20);
	TRIG_RESET;
	
	//2.�ȴ�ECHO���ų��ָߵ�ƽ
	while( ECHO_READ == 0 );
	
	//3.����ߵ�ƽ������ʱ��
	while( ECHO_READ == 1 )
	{
		Delay_us(9);
		cnt++;
	}
	cnt = cnt/2;
	
	return	cnt*3; //��λ��mm

}
