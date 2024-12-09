#include "led.h"

//LEDE5初始化
void LEDE5_INIT(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	//初始化外设
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	LEDE5_Close;
}

void LEDB5_INIT(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	//初始化外设
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	LEDB5_Close;
}

void LED_Init(void)
{
	LEDE5_INIT();
	LEDB5_INIT();
}

