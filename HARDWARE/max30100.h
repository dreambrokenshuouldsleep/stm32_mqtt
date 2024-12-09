#ifndef __MAX_30100_
#define __MAX_30100_






#include "stm32f10x.h"
#include "MyDelay.h"
#include "MyUsart.h"
#define IIC_SCL_WRITE(n)    n?GPIO_SetBits(GPIOD,GPIO_Pin_8):GPIO_ResetBits(GPIOD,GPIO_Pin_8)
#define IIC_SDA_WRITE(n)    n?GPIO_SetBits(GPIOD,GPIO_Pin_9):GPIO_ResetBits(GPIOD,GPIO_Pin_9)
#define IIC_SDA_READ    	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)

void MAX30100_Init(void);
void MAX30100_WriteBytes(uint32_t Address , uint32_t address);
uint16_t MAX30100_ReadBytes(uint32_t Address);


#endif
