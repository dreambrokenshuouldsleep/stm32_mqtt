#ifndef __LED_H_
#define __LED_H_


#include "stm32f10x.h"

#define  LEDE5_Close GPIO_SetBits(GPIOE,GPIO_Pin_5)
#define	 LEDE5_Open  GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define  LEDB5_Close GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define	 LEDB5_Open  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
//≥ı ºªØLED_E5
void LEDE5_INIT(void);
void LEDB5_INIT(void);
void LED_Init(void);
#endif 
