#ifndef __MQ_2_H_
#define __MQ_2_H_



#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include <stdio.h>
#include <Math.h>
#define PPMTime 10
void MQ_Adc1_Init(void);
float MQ_Get_PPMData(void);

#endif
