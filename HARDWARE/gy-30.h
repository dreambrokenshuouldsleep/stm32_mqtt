#ifndef __GY_30_H
#define __GY_30_H





#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#define IIC_SCL_WRITE(n)    n?GPIO_SetBits(GPIOE,GPIO_Pin_13):GPIO_ResetBits(GPIOE,GPIO_Pin_13)
#define IIC_SDA_WRITE(n)    n?GPIO_SetBits(GPIOE,GPIO_Pin_14):GPIO_ResetBits(GPIOE,GPIO_Pin_14)
#define IIC_SDA_READ    	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)

void GY_Init(void);
uint16_t GY_ReadBytes(void);
float GY_Value(void);

#endif
