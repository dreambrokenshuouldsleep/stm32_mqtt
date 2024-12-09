#ifndef __SG90_H_
#define __SG90_H_



#include "stm32f10x.h"
#include "delay.h"
#include "tim.h"


void Sg90_Init(void);
void sg90_forward(void);
void sg90_reversal(void);

#endif
