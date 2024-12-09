#ifndef __MY_DELAY_H_
#define __MY_DELAY_H_


#include "stm32f10x.h"

void MY_System_Init(void);
//延时ms,最高1864.35ms
void MY_Delay_ms(uint16_t nms);
//延时us
void MY_Delay_us(uint16_t nus);

#endif
