#include "gy-30.h"


void IIC_Init(void)
{
		//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	//初始化外设
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	
}

void IIC_SDA_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	//初始化外设
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void IIC_SDA_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	//初始化外设
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void IIC_Start(void)
{
	//输出模式
	IIC_SDA_Output();
	//拉高引脚
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	//延时等待稳定
	Delay_us(5);
	
	IIC_SDA_WRITE(0);
	Delay_us(5);
	
	IIC_SCL_WRITE(0);
	Delay_us(5);
	
}

void IIC_Stop(void)
{
	//输出模式
	IIC_SDA_Output();
	//拉低引脚
	IIC_SDA_WRITE(0);
	IIC_SCL_WRITE(0);
	//延时等待稳定
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
		//拉高写入数据
		IIC_SCL_WRITE(1);
		Delay_us(5);
		//写完数据后拉低
		IIC_SCL_WRITE(0);
		Delay_us(5);
	}
	
}

//主机等待应答信号
uint8_t IIC_WaitAck(void)
{
	uint8_t ack;
	IIC_SDA_Input();
	
	IIC_SCL_WRITE(0);
	Delay_us(5);
	
	//拉高开始接收数据
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

//主机发出应答
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

//读取字节
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
	//开始信号
	IIC_Start();
	IIC_SendBytes(0x46);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
		printf("1无应答\n");
	}
	//上电
	IIC_SendBytes(0x01);
	if(IIC_WaitAck())
	{
		IIC_Stop();
		printf("2无应答\n");
	}
	IIC_Stop();
	
	//每一次写时都需要寻址
	IIC_Start();
	IIC_SendBytes(0x46);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
		printf("1无应答\n");
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
		printf("3无应答\n");
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
