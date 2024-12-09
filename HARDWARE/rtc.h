#ifndef __RTC_H_
#define __RTC_H_

#include "stm32f10x.h"

#include "usart.h"
void RTC_Init(void);
void Time_Display(uint32_t TimeVar , uint32_t * buf);
void Time_Adjust(uint32_t THH,uint32_t TMM,uint32_t TSS);
#endif
