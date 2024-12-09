#include "key.h"
#include "includes.h"
#include "menu.h"
volatile u8 ukey = 2;
volatile u8 Confirm_key = 0;
extern u8 Menu2_key;
extern u8 clean;
extern u8 Menu2_flag;
extern u8 show_flag;
void KEY_Init(void)
{		
		
		EXTI_InitTypeDef   EXTI_InitStructure;
		GPIO_InitTypeDef   GPIO_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;

		/* Enable GPIOA clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		/* Configure PA.00 pin as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		EXTI_DeInit();
	
		/* Connect EXTI0 Line to PA.00 pin */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
		/* Configure EXTI0 line */
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		EXTI_InitStructure.EXTI_Line = EXTI_Line3;
		EXTI_Init(&EXTI_InitStructure);
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
		EXTI_Init(&EXTI_InitStructure);
		
		EXTI_ClearITPendingBit(EXTI_Line4);
		EXTI_ClearITPendingBit(EXTI_Line3);
		EXTI_ClearITPendingBit(EXTI_Line0);
		/* Enable and set EXTI0 Interrupt to the lowest priority */
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
		NVIC_Init(&NVIC_InitStructure);
}

void GPIO_Init_PF(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}


void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		ukey = 0;
		if(Menu2_key == 4 && Menu2_flag == 0)
		{
			clean = 1;
			show_flag = 0;
		}
		if(Menu2_key == 7 && Menu2_flag == 0)
		{
			clean = 1;
			show_flag = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}

}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		ukey = 1;
		if(Menu2_key == 1 && Menu2_flag == 0)
		{
			clean = 1;
			show_flag = 0;
		}
		if(Menu2_key == 5 && Menu2_flag == 0)
		{
			clean = 1;
			show_flag = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}

}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		Confirm_key = 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}
