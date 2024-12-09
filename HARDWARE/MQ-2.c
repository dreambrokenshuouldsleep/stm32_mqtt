#include "MQ-2.h"
/*
*烟雾传感器
*/
void MQ_Adc1_Init(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//打开时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置GPIO引脚为模拟模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//配置ADC结构体
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
	
	//ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);	//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) != RESET);		//判断校准是否完成（准备下次校准）
	ADC_StartCalibration(ADC1);		//启动校准
	while (ADC_GetCalibrationStatus(ADC1) != RESET);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);	
}

float MQ_Get_PPMData(void)
{
	float TempData = 0;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//开启软件触发转换
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	for(u8 i = 0; i < PPMTime ; i++)
	{
		TempData += ADC_GetConversionValue(ADC1);
		Delay_ms(5);
	}
	
	TempData = TempData / PPMTime;
	float Vol =(TempData*5/4096);
	float RS = (5-Vol)/(Vol*0.5);
	float R0 = 6.64;
	float PpmValue = pow(11.5428*R0/RS , 0.6549f);
	return PpmValue;
}
