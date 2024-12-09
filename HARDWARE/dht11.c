#include "dht11.h"


//LEDE5��ʼ��
void DHT11_Output(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//��ʼ������
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	DHT11_Out_High;
}

void DHT11_Input(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//��ʼ������
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void DHT11_Start(void)
{
	DHT11_Output();
	DHT11_Out_Low;
	Delay_ms(20);
	DHT11_Out_High;
	Delay_us(30);
}

uint32_t DHT11_Check(void)
{
	DHT11_Input();
	uint32_t i = 0 ;
	while( DHT11_Read == 1 && i < 100)
	{
		Delay_us(1);
		i++;
	}
	if(i >= 100)
	{
		return 1;
	}
	i = 0;
	while( DHT11_Read == 0 && i < 100)
	{
		Delay_us(1);
		i++;
	}
	if(i >= 100)
	{
		return 1;
	}
	return 0;
}

//��ȡ�ֽ�
uint32_t DHT_ReadBytes(void)
{
	uint32_t i = 0 ;
	//�ȴ��͵�ƽ����
	while( DHT11_Read == 1 && i < 100 )
	{
		Delay_us(1);
		i++;
	}
	i = 0;
	//�ȴ��ߵ�ƽ����
	while( DHT11_Read == 0 && i < 100)
	{
		Delay_us(1);
		i++;
	}
	Delay_us(40);
	if(DHT11_Read)
		return 1;
	else
		return 0;
}

//��ȡ����
uint8_t DHT_ReadData(void)
{
	uint32_t data = 0;
	uint8_t i = 0;
	for( ; i < 8 ; i ++ )
	{	
		data  <<= 1;
		data |= DHT_ReadBytes();
	}
	return data;
	
}

//�������
uint32_t DHT_GetData(uint8_t * buf)
{
	uint32_t i = 0;
	DHT11_Start();
	
	if(DHT11_Check() == 0)
	{
		for(;i<5;i++)
		{
			buf[i] = DHT_ReadData();
		}
		if(buf[0] + buf[1] + buf[2] + buf[3] == buf[5])
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 1;
	
	
}

