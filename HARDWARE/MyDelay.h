#ifndef __MY_DELAY_H_
#define __MY_DELAY_H_


#include "stm32f10x.h"

void MY_System_Init(void);
//��ʱms,���1864.35ms
void MY_Delay_ms(uint16_t nms);
//��ʱus
void MY_Delay_us(uint16_t nus);

#endif
