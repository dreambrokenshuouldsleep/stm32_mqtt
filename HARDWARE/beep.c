#include "beep.h"

//GPIO引脚PB8
void Beep_Init(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	//初始化外设
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	Beep_State(0);
	
}
