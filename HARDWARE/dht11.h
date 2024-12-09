#ifndef __DHT11_H_
#define __DHT11_H_

#include "stm32f10x.h"
#include "delay.h"
#define DHT11_Out_High GPIO_SetBits(GPIOG,GPIO_Pin_1)
#define DHT11_Out_Low  GPIO_ResetBits(GPIOG,GPIO_Pin_1)
#define DHT11_Read     GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1)

void DHT11_Output(void);
void DHT11_Input(void);
void DHT11_Start(void);
uint32_t DHT11_Check(void);
uint32_t DHT_ReadBytes(void);
uint8_t DHT_ReadData(void);
uint32_t DHT_GetData(uint8_t * buf);

#endif
