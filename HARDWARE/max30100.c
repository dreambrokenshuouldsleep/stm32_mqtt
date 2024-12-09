#include "max30100.h"



void IIC_Init(void)
{
		//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	//��ʼ������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	
}

void IIC_SDA_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	//��ʼ������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void IIC_SDA_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	//��ʼ������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void IIC_Start(void)
{
	//���ģʽ
	IIC_SDA_Output();
	//��������
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	//��ʱ�ȴ��ȶ�
	MY_Delay_us(5);
	
	IIC_SDA_WRITE(0);
	MY_Delay_us(5);
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
	
}

void IIC_Stop(void)
{
	//���ģʽ
	IIC_SDA_Output();
	//��������
	IIC_SDA_WRITE(0);
	IIC_SCL_WRITE(0);
	//��ʱ�ȴ��ȶ�
	MY_Delay_us(5);
	
	IIC_SCL_WRITE(1);
	MY_Delay_us(5);
	
	IIC_SDA_WRITE(1);
	MY_Delay_us(5);
	
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
		MY_Delay_us(5);
		//����д������
		IIC_SCL_WRITE(1);
		MY_Delay_us(5);
		//д�����ݺ�����
		IIC_SCL_WRITE(0);
		MY_Delay_us(5);
	}
	
}

//�����ȴ�Ӧ���ź�
uint8_t IIC_WaitAck(void)
{
	uint8_t ack;
	IIC_SDA_Input();
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
	
	//���߿�ʼ��������
	IIC_SCL_WRITE(1);
	MY_Delay_us(5);
	
	if(IIC_SDA_READ == 1)
		ack = 1;
	else
		ack = 0;
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
	
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
	
	MY_Delay_us(5);
	
	IIC_SCL_WRITE(1);
	MY_Delay_us(5);
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
}

//��ȡ�ֽ�
uint8_t IIC_ReadBytes(void)
{
	uint8_t i = 0;
	uint8_t Data = 0;
	
	IIC_SDA_Input();
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
	
	for(;i<8;i++)
	{
		IIC_SCL_WRITE(1);
		MY_Delay_us(5);
		
		if(IIC_SDA_READ == 1)
		{
			Data |= 1<<(7-i);
		}
		
		IIC_SCL_WRITE(0);
		MY_Delay_us(5);
	}
	return Data;
}

void MAX30100_Init(void)
{
	 IIC_Init();
}

void MAX30100_WriteBytes(uint32_t Address , uint32_t address)
{
	uint8_t status = 0;
	//��ʼ�ź�
	IIC_Start();
	IIC_SendBytes(0xAE);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
		status = 1;
	}
	//����ģʽ
	IIC_SendBytes(Address);
	if(IIC_WaitAck())
	{
		IIC_Stop();
		status = 2;
	}
	//����ģʽ
	IIC_SendBytes(address);
	if(IIC_WaitAck())
	{
		IIC_Stop();
		status = 3;
	}
	IIC_Stop();
	printf("Address:%d, address:%d ,status:%d\n",Address,address,status);
}

uint16_t MAX30100_ReadBytes(uint32_t Address)
{
	uint16_t Data = 0;
	//��ʼ�ź�
	IIC_Start();
	IIC_SendBytes(0xAE);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
	}
	//����ģʽ
	IIC_SendBytes(Address);
	if(IIC_WaitAck())
	{
		IIC_Stop();
	}
	IIC_Stop();
	
	IIC_Start();
	IIC_SendBytes(0xAF);
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
	printf("�����ϣ���\n");
	MY_Delay_ms(1000);
	return Data;

}
