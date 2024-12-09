#ifndef __BEEP_H_
#define __BEEP_H_


#include "stm32f10x.h"

#define Beep_State(n)  n?GPIO_SetBits(GPIOB,GPIO_Pin_8):GPIO_ResetBits(GPIOB,GPIO_Pin_8)

void Beep_Init(void);



#endif
