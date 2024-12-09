#include "wwdg.h"


//���ڿ��Ź�һ��������ֹ�����ܷ����ݶ�ʧ�������ж�����������
void Wwdg_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	//1.�ж�ϵͳ��λ�Ƿ���WWDG���Ź�����
	if( RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET )
	{
		printf("System Reset Is By WWDG\n");
		RCC_ClearFlag();
	}
	else
	{
		printf("System Reset Is By User\n");
	}
	
	//ʹ�ܴ��ڿ��Ź�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	//��Ʒ�Ƶϵ��36/4096/8 = 1098hz      910us
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	//���ô��ڿ��Ź�������64~127֮��
	WWDG_SetWindowValue(80);
	/*ι��ʱ�����
	���ޣ�910 * (127 - 80) = 42.77ms    
	���ޣ�910 * (127 - 63) = 58.24ms
	*/
	//ʹ�ܴ��ڿ��Ź�
	WWDG_Enable(128 - 1);
	
	//�����жϷ�����
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;				//�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);	
	
	//����жϱ�־
	 WWDG_ClearFlag();
	//ʹ���ж�
	 WWDG_EnableIT();
	 
}


//���ô��ڿ��Ź��жϷ�����

void  WWDG_IRQHandler(void)
{
	if( WWDG_GetFlagStatus() != RESET )
	{ 	
		WWDG_ClearFlag();		//����жϻ��ѱ�־
	}
}
