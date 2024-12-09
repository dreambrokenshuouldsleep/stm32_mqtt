#include "iwdg.h"

//���Ź�һ����϶�ʱ��ʹ�ã�ͨ����ʱ���ж���ι��
void Iwdg_Init(void)
{
	//�жϹ�����ʲô��λ��
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		printf("System Reset Is By IWDG\n");
		RCC_ClearFlag();
	}
	else
	{
		printf("System Reset Is By User\n");
	}
	
	//���IWDG��д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//���ö������Ź���Ԥ��Ƶֵ156.25HZ��40khz/256
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	//����ι��ʱ��Ϊ4s��
	IWDG_SetReload(625-1);
	//���ؼ����Ĵ���,ι������
	IWDG_ReloadCounter();
	//ʹ�ܿ��Ź�
	IWDG_Enable();
	
}

