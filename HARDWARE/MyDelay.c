#include "MyDelay.h"

void MY_System_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

//可以延时1864.135ms
void MY_Delay_ms(uint16_t nms)
{
	SysTick->CTRL &= ~(1<<0); 			// 关闭定时器
	SysTick->LOAD = (9*1000*nms) - 1; 	// 设置重装载值 value-1
	SysTick->VAL  = 0; 					// 清空当前计数值
	SysTick->CTRL |= (1<<0); 			// 开启定时器  开始倒数
	while ( (SysTick->CTRL & 0x00010000)== 0 );// 等待倒数完成
	SysTick->CTRL = 0;					// 关闭定时器
	SysTick->VAL  = 0; 					// 清空当前计数值
}

void MY_Delay_us(uint16_t nus)
{
	SysTick->CTRL &= ~(1<<0); 			// 关闭定时器
	SysTick->LOAD = (9*nus) - 1; 	// 设置重装载值 value-1
	SysTick->VAL  = 0; 					// 清空当前计数值
	SysTick->CTRL |= (1<<0); 			// 开启定时器  开始倒数
	while ( (SysTick->CTRL & 0x00010000)== 0 );// 等待倒数完成
	SysTick->CTRL = 0;					// 关闭定时器
	SysTick->VAL  = 0; 					// 清空当前计数值
}
