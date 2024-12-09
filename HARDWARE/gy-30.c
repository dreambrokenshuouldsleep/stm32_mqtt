#include "gy-30.h"


void IIC_Init(void)
{
		//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	//��ʼ������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	
}

void IIC_SDA_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	//��ʼ������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void IIC_SDA_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	//��ʼ������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void IIC_Start(void)
{
	//���ģʽ
	IIC_SDA_Output();
	//��������
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	//��ʱ�ȴ��ȶ�
	Delay_us(5);
	
	IIC_SDA_WRITE(0);
	Delay_us(5);
	
	IIC_SCL_WRITE(0);
	Delay_us(5);
	
}

void IIC_Stop(void)
{
	//���ģʽ
	IIC_SDA_Output();
	//��������
	IIC_SDA_WRITE(0);
	IIC_SCL_WRITE(0);
	//��ʱ�ȴ��ȶ�
	Delay_us(5);
	
	IIC_SCL_WRITE(1);
	Delay_us(5);
	
	IIC_SDA_WRITE(1);
	Delay_us(5);
	
}

void IIC_SendBytes(uint8_t Data)
{
	uint8_t i = 0;
	IIC_SDA_Output();
	
	IIC_SDA_WRITE(0);
	IIC_SCL_WRITE(0);
	
	for(;i<8;i++)
	{
		if(Data & 1 << (7-i))
			IIC_SDA_WRITE(1);
		else
			IIC_SDA_WRITE(0);
		Delay_us(5);
		//����д������
		IIC_SCL_WRITE(1);
		Delay_us(5);
		//д�����ݺ�����
		IIC_SCL_WRITE(0);
		Delay_us(5);
	}
	
}

//�����ȴ�Ӧ���ź�
uint8_t IIC_WaitAck(void)
{
	uint8_t ack;
	IIC_SDA_Input();
	
	IIC_SCL_WRITE(0);
	Delay_us(5);
	
	//���߿�ʼ��������
	IIC_SCL_WRITE(1);
	Delay_us(5);
	
	if(IIC_SDA_READ == 1)
		ack = 1;
	else
		ack = 0;
	
	IIC_SCL_WRITE(0);
	Delay_us(5);
	
	return ack;
	
}

//��������Ӧ��
void IIC_HostAck(uint8_t ack)
{
	IIC_SDA_Output();
	
	IIC_SCL_WRITE(0);
	IIC_SDA_WRITE(0);
	
	if(ack)
		IIC_SDA_WRITE(1);
	else
		IIC_SDA_WRITE(0);
	
	Delay_us(5);
	
	IIC_SCL_WRITE(1);
	Delay_us(5);
	
	IIC_SCL_WRITE(0);
	Delay_us(5);
}

//��ȡ�ֽ�
uint8_t IIC_ReadBytes(void)
{
	uint8_t i = 0;
	uint8_t Data = 0;
	
	IIC_SDA_Input();
	
	IIC_SCL_WRITE(0);
	Delay_us(5);
	
	for(;i<8;i++)
	{
		IIC_SCL_WRITE(1);
		Delay_us(5);
		
		if(IIC_SDA_READ == 1)
		{
			Data |= 1<<(7-i);
		}
		
		IIC_SCL_WRITE(0);
		Delay_us(5);
	}
	return Data;
}

void GY_Init(void)
{
	 IIC_Init();
}

uint16_t GY_ReadBytes(void)
{
	uint16_t Data = 0;
	//��ʼ�ź�
	IIC_Start();
	IIC_SendBytes(0x46);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
		printf("1��Ӧ��\n");
	}
	//�ϵ�
	IIC_SendBytes(0x01);
	if(IIC_WaitAck())
	{
		IIC_Stop();
		printf("2��Ӧ��\n");
	}
	IIC_Stop();
	
	//ÿһ��дʱ����ҪѰַ
	IIC_Start();
	IIC_SendBytes(0x46);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
		printf("1��Ӧ��\n");
	}
	IIC_SendBytes(0x10);
	if(IIC_WaitAck())
	{
		IIC_Stop();
	}
	IIC_Stop();
	Delay_ms(180);
	IIC_Start();
	IIC_SendBytes(0x47);
	if(IIC_WaitAck())
	{
		IIC_Stop();
		printf("3��Ӧ��\n");
	}
	Data = IIC_ReadBytes();
	IIC_HostAck(0);
	Data = Data << 8;
	Data |= IIC_ReadBytes();
	IIC_HostAck(1);
	IIC_Stop();
	return Data;
}

float GY_Value(void)
{
	float Data = 0;
	Data = GY_ReadBytes();
	Data = (float)Data/1.2;
	return Data;
	
}
