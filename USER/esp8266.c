#include "esp8266.h"

void esp8266_Init(void)
{
	USART_SendString(USART2,"AT\r\n");
	Delay_ms(50);
	USART_SendString(USART2,"AT+CWJAP=\"ABCD\",\"11223344\"\r\n");
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	USART_SendString(USART2,"AT+MQTTUSERCFG=0,1,\"esp8266\",\"espressif\",\"1234567890\",0,0,\"\"\r\n");
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	USART_SendString(USART2,"AT+MQTTCONN=0,\"192.168.137.1\",1883,1\r\n");
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	Delay_ms(1000);
	USART_SendString(USART2,"AT+MQTTSUB=0,\"aaa\",1\r\n");
}
