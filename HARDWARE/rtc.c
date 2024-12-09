#include "rtc.h"


void RTC_Init(void)
{
	//打开外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	//解除写保护
	PWR_BackupAccessCmd(ENABLE);
	//不初始化BKP
	//BKP_DeInit();
	//打开外部低速时钟
	RCC_LSEConfig(RCC_LSE_ON);
	//等待外部时钟稳定
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	//rtc使用外部时钟
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	//使能rtc
	RCC_RTCCLKCmd(ENABLE);
	//等待寄存器同步
	RTC_WaitForSynchro();
	//rtc分频
	RTC_SetPrescaler(32767);
	//等待rtc最后一次写任务
	RTC_WaitForLastTask();
	
}


void Time_Adjust(uint32_t THH,uint32_t TMM,uint32_t TSS)
{
  /* Wait until last write operation on RTC registers has finished */
   RTC_WaitForLastTask();
  /* Change the current time */
	
  RTC_SetCounter(THH*3600 + TMM*60 + TSS);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

void Time_Display(uint32_t TimeVar , uint32_t * buf)
{
	uint32_t THH = 0, TMM = 0, TSS = 0;
	
	/* Reset RTC Counter when Time is 23:59:59 */
	if (RTC_GetCounter() == 0x0001517F)
	{
	  RTC_SetCounter(0x0);
	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
	}

	/* Compute  hours */
	THH = TimeVar / 3600;
	/* Compute minutes */
	TMM = (TimeVar % 3600) / 60;
	/* Compute seconds */
	TSS = (TimeVar % 3600) % 60;
	
	buf[0] = THH;
	buf[1] = TMM;
	buf[2] = TSS;
	//printf("Time: %0.2d:%0.2d:%0.2d\n", THH, TMM, TSS);
}

