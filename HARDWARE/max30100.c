#include "max30100.h"



void IIC_Init(void)
{
		//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	//初始化外设
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	
}

void IIC_SDA_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	//初始化外设
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void IIC_SDA_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	//初始化外设
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void IIC_Start(void)
{
	//输出模式
	IIC_SDA_Output();
	//拉高引脚
	IIC_SDA_WRITE(1);
	IIC_SCL_WRITE(1);
	//延时等待稳定
	MY_Delay_us(5);
	
	IIC_SDA_WRITE(0);
	MY_Delay_us(5);
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
	
}

void IIC_Stop(void)
{
	//输出模式
	IIC_SDA_Output();
	//拉低引脚
	IIC_SDA_WRITE(0);
	IIC_SCL_WRITE(0);
	//延时等待稳定
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
		//拉高写入数据
		IIC_SCL_WRITE(1);
		MY_Delay_us(5);
		//写完数据后拉低
		IIC_SCL_WRITE(0);
		MY_Delay_us(5);
	}
	
}

//主机等待应答信号
uint8_t IIC_WaitAck(void)
{
	uint8_t ack;
	IIC_SDA_Input();
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
	
	//拉高开始接收数据
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
	
	MY_Delay_us(5);
	
	IIC_SCL_WRITE(1);
	MY_Delay_us(5);
	
	IIC_SCL_WRITE(0);
	MY_Delay_us(5);
}

//读取字节
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
	//开始信号
	IIC_Start();
	IIC_SendBytes(0xAE);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
		status = 1;
	}
	//配置模式
	IIC_SendBytes(Address);
	if(IIC_WaitAck())
	{
		IIC_Stop();
		status = 2;
	}
	//心率模式
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
	//开始信号
	IIC_Start();
	IIC_SendBytes(0xAE);
	
	if(IIC_WaitAck())
	{
		IIC_Stop();
	}
	//配置模式
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
		printf("3无应答\n");
	}
	Data = IIC_ReadBytes();
	IIC_HostAck(0);
	Data = Data << 8;
	Data |= IIC_ReadBytes();
	IIC_HostAck(1);
	IIC_Stop();
	printf("检测完毕！！\n");
	MY_Delay_ms(1000);
	return Data;

}
