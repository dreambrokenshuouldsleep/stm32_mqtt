#include "yl-38.h"

void YL_Adc1_Init(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//��ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//����GPIO����Ϊģ��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����ADC�ṹ��
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);		//��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) != RESET);		//�ж�У׼�Ƿ���ɣ�׼���´�У׼��
	ADC_StartCalibration(ADC1);		//����У׼
	while (ADC_GetCalibrationStatus(ADC1) != RESET);	
}

void YL_ADC_GetValue(void)
{
	
	uint16_t ADC_Val = 0;
	uint16_t ADC_Vol = 0;

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//�����������ת��
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	//��������ת��Ϊģ���ѹ
	ADC_Val = ADC_GetConversionValue(ADC1);
	ADC_Vol =ADC_Val * 3300 / 4096;
	printf("ADC_Vol Is %d mV\n",ADC_Vol);	
}
