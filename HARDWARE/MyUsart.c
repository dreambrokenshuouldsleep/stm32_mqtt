#include "MyUsart.h"

static volatile char USART1_ReceBuf[128] = {0};
static volatile char USART2_ReceBuf[128] = {0};

static volatile uint32_t USART1_Count = 0;
static volatile uint32_t USART2_Count = 0;
//重定向printf函数
//int fputc(int ch, FILE *f) 
//{
	//USART_SendData(USART1, ch);
	//while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待数据发送完成
	
	//return ch;
//}


//打开串口1
void MY_USART1_INIT(u32 BaudRate)
{
	//定义结构体
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//初始化结构体GPIO9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//初始化结构体GPIO10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置串口1结构体
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	//开启串口接收中断
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	
	//设置中断结构体
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//开启串口
	USART_Cmd(USART1, ENABLE);	
}


//串口2初始化
void MY_USART2_INIT(u32 BaudRate)
{
	//定义结构体
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//初始化结构体GPIO9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//初始化结构体GPIO10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置串口1结构体
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	//开启串口接收中断
	USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);
	
	//设置中断结构体
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//开启串口
	USART_Cmd(USART2, ENABLE);	
}

void MY_Usart2_SendString(const char * str)
{
   const char *p = str;
	while(p && *p)
	{
		USART_SendData(USART2, *p);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET );
	}
}

//串口一中断服务函数
//void USART1_IRQHandler(void)
//{
	//uint32_t rece_data = 0;
	//if(USART_GetITStatus(USART1,  USART_IT_RXNE) != RESET)
	//{
		//消除中断信号
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//串口接收信息
		//rece_data = USART_ReceiveData(USART1);
		//串口发送信息
		//USART_SendData(USART2, rece_data);
		//清楚标志
		//while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	//}
//}

//串口2中断服务函数
//void USART2_IRQHandler(void)
//{
	//uint32_t rece_data = 0;
	//if(USART_GetITStatus(USART2,  USART_IT_RXNE) != RESET)
	//{
		//消除中断信号
		//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//串口接收信息
		//rece_data = USART_ReceiveData(USART2);
		//串口发送信息
		//USART_SendData(USART1, rece_data);
		//清楚标志
		//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	//}
//}
