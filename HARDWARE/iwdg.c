#include "iwdg.h"

//看门狗一般配合定时器使用，通过定时器中断来喂狗
void Iwdg_Init(void)
{
	//判断狗是由什么复位的
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		printf("System Reset Is By IWDG\n");
		RCC_ClearFlag();
	}
	else
	{
		printf("System Reset Is By User\n");
	}
	
	//解除IWDG的写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//设置独立看门狗的预分频值156.25HZ：40khz/256
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	//设置喂狗时间为4s中
	IWDG_SetReload(625-1);
	//重载计数寄存器,喂狗操作
	IWDG_ReloadCounter();
	//使能看门狗
	IWDG_Enable();
	
}

