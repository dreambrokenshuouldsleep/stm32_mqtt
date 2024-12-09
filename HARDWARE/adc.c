#include "adc.h"

//ADC1_IN1 PA1
void Adc1_Init(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//打开时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置GPIO引脚为模拟模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置ADC结构体
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//采样周期12.5+1.5 = 14；
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);		//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) != RESET);		//判断校准是否完成（准备下次校准）
	ADC_StartCalibration(ADC1);		//启动校准
	while (ADC_GetCalibrationStatus(ADC1) != RESET);	
}

void ADC_GetValue(void)
{
	
	uint16_t ADC_Val = 0;
	uint16_t ADC_Vol = 0;
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//开启软件触发转换
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	//把数字量转换为模拟电压
	ADC_Val = ADC_GetConversionValue(ADC1);
	ADC_Vol = ADC_Val * 3300 / 4096;
	printf("ADC_Vol Is %d mV\n",ADC_Vol);	
}
