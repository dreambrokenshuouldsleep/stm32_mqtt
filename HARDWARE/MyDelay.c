#include "MyDelay.h"

void MY_System_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

//������ʱ1864.135ms
void MY_Delay_ms(uint16_t nms)
{
	SysTick->CTRL &= ~(1<<0); 			// �رն�ʱ��
	SysTick->LOAD = (9*1000*nms) - 1; 	// ������װ��ֵ value-1
	SysTick->VAL  = 0; 					// ��յ�ǰ����ֵ
	SysTick->CTRL |= (1<<0); 			// ������ʱ��  ��ʼ����
	while ( (SysTick->CTRL & 0x00010000)== 0 );// �ȴ��������
	SysTick->CTRL = 0;					// �رն�ʱ��
	SysTick->VAL  = 0; 					// ��յ�ǰ����ֵ
}

void MY_Delay_us(uint16_t nus)
{
	SysTick->CTRL &= ~(1<<0); 			// �رն�ʱ��
	SysTick->LOAD = (9*nus) - 1; 	// ������װ��ֵ value-1
	SysTick->VAL  = 0; 					// ��յ�ǰ����ֵ
	SysTick->CTRL |= (1<<0); 			// ������ʱ��  ��ʼ����
	while ( (SysTick->CTRL & 0x00010000)== 0 );// �ȴ��������
	SysTick->CTRL = 0;					// �رն�ʱ��
	SysTick->VAL  = 0; 					// ��յ�ǰ����ֵ
}
