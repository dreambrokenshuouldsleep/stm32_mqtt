#ifndef __MY_USART_H__
#define __MY_USART_H__

#include "stm32f10x.h"
#include <stdio.h>



int fputc(int ch, FILE *f);
//����1��ʼ��
void MY_USART1_INIT(u32 BaudRate);
//����1�жϷ�����
void MY_USART2_INIT(u32 BaudRate);

#endif
