#ifndef __KEY_H_
#define __KEY_H_



#include "stm32f10x.h"
#include "oled.h"
#include "usart.h"



void KEY_Init(void);
uint8_t Read_Key(void);
void GPIO_Init_PF(void);
#endif
