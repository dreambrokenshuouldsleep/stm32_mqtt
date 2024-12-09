#include "MyUsart.h"

static volatile char USART1_ReceBuf[128] = {0};
static volatile char USART2_ReceBuf[128] = {0};

static volatile uint32_t USART1_Count = 0;
static volatile uint32_t USART2_Count = 0;
//�ض���printf����
//int fputc(int ch, FILE *f) 
//{
	//USART_SendData(USART1, ch);
	//while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ����ݷ������
	
	//return ch;
//}


//�򿪴���1
void MY_USART1_INIT(u32 BaudRate)
{
	//����ṹ��
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//��ʼ���ṹ��GPIO9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//��ʼ���ṹ��GPIO10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���ô���1�ṹ��
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	//�������ڽ����ж�
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	
	//�����жϽṹ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//��������
	USART_Cmd(USART1, ENABLE);	
}


//����2��ʼ��
void MY_USART2_INIT(u32 BaudRate)
{
	//����ṹ��
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//��ʼ���ṹ��GPIO9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//��ʼ���ṹ��GPIO10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���ô���1�ṹ��
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	//�������ڽ����ж�
	USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);
	
	//�����жϽṹ��
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//��������
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

//����һ�жϷ�����
//void USART1_IRQHandler(void)
//{
	//uint32_t rece_data = 0;
	//if(USART_GetITStatus(USART1,  USART_IT_RXNE) != RESET)
	//{
		//�����ж��ź�
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//���ڽ�����Ϣ
		//rece_data = USART_ReceiveData(USART1);
		//���ڷ�����Ϣ
		//USART_SendData(USART2, rece_data);
		//�����־
		//while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	//}
//}

//����2�жϷ�����
//void USART2_IRQHandler(void)
//{
	//uint32_t rece_data = 0;
	//if(USART_GetITStatus(USART2,  USART_IT_RXNE) != RESET)
	//{
		//�����ж��ź�
		//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//���ڽ�����Ϣ
		//rece_data = USART_ReceiveData(USART2);
		//���ڷ�����Ϣ
		//USART_SendData(USART1, rece_data);
		//�����־
		//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	//}
//}
