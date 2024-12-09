#ifndef __HC_SRO4_H_
#define __HC_SRO4_H_





#include "delay.h"
#include "stm32f10x.h"

#define  TRIG_SET  	  GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define  TRIG_RESET   GPIO_ResetBits(GPIOC,GPIO_Pin_11)
#define  ECHO_READ    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)

void Hc_Sro4_Init(void);
uint32_t SR04_GetDistance(void);
#endif
