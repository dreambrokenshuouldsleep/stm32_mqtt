#include "wwdg.h"


//窗口看门狗一般用来防止程序跑飞数据丢失，配置中断来保存数据
void Wwdg_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	//1.判断系统复位是否由WWDG看门狗导致
	if( RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET )
	{
		printf("System Reset Is By WWDG\n");
		RCC_ClearFlag();
	}
	else
	{
		printf("System Reset Is By User\n");
	}
	
	//使能窗口看门狗时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	//设计分频系数36/4096/8 = 1098hz      910us
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	//设置窗口看门狗的上限64~127之间
	WWDG_SetWindowValue(80);
	/*喂狗时间计算
	上限：910 * (127 - 80) = 42.77ms    
	下限：910 * (127 - 63) = 58.24ms
	*/
	//使能窗口看门狗
	WWDG_Enable(128 - 1);
	
	//配置中断服务函数
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;				//中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能通道
	NVIC_Init(&NVIC_InitStructure);	
	
	//清空中断标志
	 WWDG_ClearFlag();
	//使能中断
	 WWDG_EnableIT();
	 
}


//配置窗口看门狗中断服务函数

void  WWDG_IRQHandler(void)
{
	if( WWDG_GetFlagStatus() != RESET )
	{ 	
		WWDG_ClearFlag();		//清空中断唤醒标志
	}
}
