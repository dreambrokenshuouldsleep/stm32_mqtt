#include "stm32f10x.h"
#include "led.h"
#include "includes.h"
#include "delay.h"
#include "usart.h"
#include "iwdg.h"
#include "oled.h"
#include "menu.h"
#include "key.h"
#include "rfid.h"
#include "gy-30.h"
#include "MQ-2.h"
#include "dht11.h"
#include "rtc.h"
#include "esp8266.h"
#include "hc-sro4.h"
#include "sg90.h"

u8  mfrc552pidbuf[18];											
u8  card_pydebuf[2];                                            //���濨������
u8  card_numberbuf[5];                                          //���濨�����к�
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};			//
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};  //����д��Ŀ�����    
u8  card_readbuf[18];  

u8 host_numberbuf[128] = {0};
volatile u8 host = 0;
extern u8 door;
extern unsigned char BMP_pic1[];
extern unsigned char BMP_pic2[];
extern u8 clean;
volatile u8 rfid_state = 2;
extern u8 Confirm_key ;
extern u8 i;
//volatile uint8_t Data = 0;
//������ֵ��������ֵ���¶���ֵ��ʪ����ֵ�������ű�־����������־,������ֵ
volatile uint32_t sensor_value[7] = {2000 , 1400 , 40 ,100,2,2,20};

extern u8  USART2_RX_BUF[USART2_REC_LEN];
extern volatile u8 key;
	
uint8_t temp_value[5] = {0};

static float ppm = 0;
static float gy = 0;
u8 gy_buf[128] = {0};//���ݴ洢����������������ƴ�ӵ��ַ���
u8 ppm_buf[128] = {0};
u8 temp_buf[128] = {0};
u8 humi_buf[128] = {0};
u8 time_buf[128] = {0};
uint32_t time[3] = {0};
char total_buf[256] = {0};
char warn_buf[256] = {0};


volatile u8 len = 1;
//��¼ͣ��ʱ��
volatile u8 dis = 0;

volatile u8 flag_sensor = 0;
void value_deal(float gy , float ppm , uint8_t temp , uint8_t humi)//OLED�����ַ�������
{
	sprintf((char *)gy_buf , "%.2flux" , gy);//ƴ���ַ���
	sprintf((char *)ppm_buf , "%.2fppm" , ppm);
	sprintf((char *)temp_buf , "%dC" ,temp);
	sprintf((char *)humi_buf , "%dRH" , humi);
	sprintf((char *)time_buf , "%0.2d:%0.2d:%0.2d" , time[0], time[1], time[2]);
	sprintf(total_buf , "AT+MQTTPUB=0,\"ccc\",\":gy=%s\\,ppm=%s\\,temp=%s\\,humi=%s\\,time=%s\",1,0\r\n",gy_buf,ppm_buf,temp_buf,humi_buf,time_buf);
}

void Get_Value(void)//��ȡ��������ֵ
{
	gy = GY_Value();//���մ�����
	ppm = MQ_Get_PPMData();//��������
	DHT_GetData(temp_value);//��ʪ������
	Time_Display(RTC_GetCounter(),time);//RTCʱ�ӻ�ü���
	value_deal( gy ,  ppm ,  26 ,  65);//OLED�����ַ�����
}

void send_warn(char * state)
{
	sprintf(warn_buf,"AT+MQTTPUB=0,\"ccc\",\"Warn:gy=%s\\,ppm=%s\\,temp=%s\\,humi=%s\\,usr=%d\\,time=%s\\,stay=%d\\,type=%s\",1,0\r\n",gy_buf,ppm_buf,temp_buf,humi_buf,card_numberbuf[0],time_buf,1 ,state);
	USART_SendString(USART2,warn_buf);
}
//����쳣

void compara_value(void)
{
	if(flag_sensor == 0)
	{
		if(gy > (float)sensor_value[1])
		{
			send_warn("GY");
			flag_sensor = 1;
		}
		if(ppm > (float)sensor_value[0])
		{
			send_warn("MQ");
			flag_sensor = 1;
		}
		if((temp_value[2] > sensor_value[2]))
		{
			send_warn("TEMP");
			flag_sensor = 1;
		}
		if(temp_value[0] > sensor_value[3])
		{
			send_warn("HUMI");
			flag_sensor = 1;
		}
	}
}
void detec_distance(void)
{
	uint32_t distance = 0;
	distance = SR04_GetDistance();
	//printf("%d\n",distance);
	if(distance < sensor_value[6])
	{
		
		dis++;
		if(dis > 10)
		{
			send_warn("people");
			dis = 0;
		}
	}
	else
	{
		dis = 0;
	}
}

//����һ
OS_TCB Task1_TCB;

void task1(void *parg);

CPU_STK task1_stk[128];			//����1�������ջ����СΪ128�֣�Ҳ����512�ֽ�
//�����
OS_TCB Task2_TCB;

void task2(void *parg);

CPU_STK task2_stk[128];			//����1�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//������
OS_TCB Task3_TCB;

void task3(void *parg);

CPU_STK task3_stk[128];			//����1�������ջ����СΪ128�֣�Ҳ����512�ֽ�

OS_TCB Task4_TCB;

void task4(void *parg);

CPU_STK task4_stk[128];	

OS_TCB Task5_TCB;

void task5(void *parg);

CPU_STK task5_stk[128];	

int main()
{
OS_ERR err;

	
	delay_init();								//ʱ�ӳ�ʼ��   ������શ�ʱ����Ϊ����ϵͳ���������ԵĽ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ������� 
	uart1_init(115200);
	uart2_init(115200);
	esp8266_Init();
	Iwdg_Init();		
	KEY_Init();
	LED_Init();
	Sg90_Init();
	//OS��ʼ�������ǵ�һ�����еĺ���,��ʼ�����ֵ�ȫ�ֱ����������ж�Ƕ�׼����������ȼ����洢��
	OSInit(&err);		   	 						//��ʼ��UCOSIII
			 
	//������ʼ����
	//��������1��������дǰ8������
	//rfid����
	OSTaskCreate(	(OS_TCB *)&Task1_TCB,									//������ƿ�  ������ƿ�ĵ�ַ
					(CPU_CHAR *)"Task1",									//��������֣��Զ���
					(OS_TASK_PTR)task1,										//������
                    (void *)0,												//���ݲ���  NULL  ����Ҫ����
					(OS_PRIO)2,											 	//��������ȼ�	   ��ֵԽС�����ȼ�Խ��	
					(CPU_STK *)task1_stk,									//�����ջ����ַ   ������ջ�ռ����һ������   �������϶࣬�ǵ��޸Ļ���ļ���ջ�ռ�Ĵ�С
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
			
				);
	//ι������			
	OSTaskCreate(	(OS_TCB *)&Task2_TCB,									//������ƿ�  ������ƿ�ĵ�ַ
					(CPU_CHAR *)"Task2",									//��������֣��Զ���
					(OS_TASK_PTR)task2,										//������
                    (void *)0,												//���ݲ���  NULL  ����Ҫ����
					(OS_PRIO)1,											 	//��������ȼ�	   ��ֵԽС�����ȼ�Խ��	
					(CPU_STK *)task2_stk,									//�����ջ����ַ   ������ջ�ռ����һ������   �������϶࣬�ǵ��޸Ļ���ļ���ջ�ռ�Ĵ�С
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
	
					
					
				);
	//�˵�����
	OSTaskCreate(	(OS_TCB *)&Task3_TCB,									//������ƿ�  ������ƿ�ĵ�ַ
				(CPU_CHAR *)"Task3",									//��������֣��Զ���
				(OS_TASK_PTR)task3,										//������
				(void *)0,												//���ݲ���  NULL  ����Ҫ����
				(OS_PRIO)4,											 	//��������ȼ�	   ��ֵԽС�����ȼ�Խ��	
				(CPU_STK *)task3_stk,									//�����ջ����ַ   ������ջ�ռ����һ������   �������϶࣬�ǵ��޸Ļ���ļ���ջ�ռ�Ĵ�С
				(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
				(CPU_STK_SIZE)128,										//�����ջ��С			
				(OS_MSG_QTY)0,											//��ֹ������Ϣ����
				(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
				(void  *)0,												//����Ҫ�����û��洢��
				(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
				&err													//���صĴ�����

				
				
			);
				
	//��������
	OSTaskCreate(	(OS_TCB *)&Task4_TCB,									//������ƿ�  ������ƿ�ĵ�ַ
				(CPU_CHAR *)"Task4",									//��������֣��Զ���
				(OS_TASK_PTR)task4,										//������
				(void *)0,												//���ݲ���  NULL  ����Ҫ����
				(OS_PRIO)3,											 	//��������ȼ�	   ��ֵԽС�����ȼ�Խ��	
				(CPU_STK *)task4_stk,									//�����ջ����ַ   ������ջ�ռ����һ������   �������϶࣬�ǵ��޸Ļ���ļ���ջ�ռ�Ĵ�С
				(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
				(CPU_STK_SIZE)128,										//�����ջ��С			
				(OS_MSG_QTY)0,											//��ֹ������Ϣ����
				(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
				(void  *)0,												//����Ҫ�����û��洢��
				(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
				&err													//���صĴ�����

				
				
			);
	OSTaskCreate(	(OS_TCB *)&Task5_TCB,									//������ƿ�  ������ƿ�ĵ�ַ
				(CPU_CHAR *)"Task5",									//��������֣��Զ���
				(OS_TASK_PTR)task5,										//������
				(void *)0,												//���ݲ���  NULL  ����Ҫ����
				(OS_PRIO)5,											 	//��������ȼ�	   ��ֵԽС�����ȼ�Խ��	
				(CPU_STK *)task5_stk,									//�����ջ����ַ   ������ջ�ռ����һ������   �������϶࣬�ǵ��޸Ļ���ļ���ջ�ռ�Ĵ�С
				(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
				(CPU_STK_SIZE)128,										//�����ջ��С			
				(OS_MSG_QTY)0,											//��ֹ������Ϣ����
				(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
				(void  *)0,												//����Ҫ�����û��洢��
				(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
				&err													//���صĴ�����

				
				
			);
				
					
  
	OSStart(&err);     //����OS�������������
}

void task1(void *parg)
{
	OS_ERR err;
	printf("task1 is create ok\r\n");
	RC522_Init();
	host_numberbuf[0] = 0xd9;
	while(1)
	{
		//�鿨
		if(sensor_value[5] == 2)
		{
			if(Read_card(card_pydebuf ,card_numberbuf ,card_key0Abuf , card_readbuf))
			{
				for(u8 j = 0 ; j < 10 ; j++)
				{
					if(host_numberbuf[j] == card_numberbuf[0])
					{
						sensor_value[4] = 1;
						clean = 1;
						i = 1;
						door = 0;
						send_warn("NO");
						break;
					}
					sensor_value[4] = 0;
					clean = 1;
					i = 1;
				}
				if(sensor_value[4] == 0)
				{
					send_warn("ERR");
				}
				card_numberbuf[0] = 0;
			}
		}//���ӿ�
		else if(sensor_value[5] == 1)
		{
			if(Read_card(card_pydebuf ,card_numberbuf ,card_key0Abuf , card_readbuf))
			{
				host_numberbuf[len] = card_numberbuf[0];
				len++;
				OLED_ShowCHinese(32,3,16);
				OLED_ShowCHinese(48,3,17);
				OLED_ShowCHinese(64,3,22);
				OLED_ShowCHinese(80,3,23);
				clean = 1;
			}
			card_numberbuf[0] = 0;
		}//���ٿ�
		else if(sensor_value[5] == 0)
		{
			if(Read_card(card_pydebuf ,card_numberbuf ,card_key0Abuf , card_readbuf))
			{
				for(u8 j = 0;j<10;j++)
				{
					if(host_numberbuf[j] == card_numberbuf[0])
					{
						host_numberbuf[j] = 0;
						OLED_ShowCHinese(32,3,18);
						OLED_ShowCHinese(48,3,19);
						OLED_ShowCHinese(64,3,22);
						OLED_ShowCHinese(80,3,23);
						break;
					}
				}	
			}
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //����ʱ�������
	}
}

void task2(void *parg)
{
	OS_ERR err;
	printf("task2 is create ok\r\n");
	
	while(1)
	{
		IWDG_ReloadCounter();
		OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err); //������Ƶ�ʶ�����������ʱ2ms
	}
}

void task3(void *parg)
{
	OS_ERR err;
	printf("task3 is create ok\r\n");
	OLED_Init();
	OLED_Clear();
	while(1)
	{
		
		if(host == 1)
		{
			
			Menu_Show();
		}
		else
		{
			Menu4_Show();
		}
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //������Ƶ�ʶ�����������ʱ2ms
	}
}

void task4(void *parg)
{
	OS_ERR err;
	printf("task4 is create ok\r\n");
	GY_Init();
	MQ_Adc1_Init();
	RTC_Init();
	Hc_Sro4_Init();
	while(1)
	{
		Get_Value();
		set_value();
		compara_value();
		detec_distance();
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //������Ƶ�ʶ�����������ʱ2ms
	}
}

void task5(void *parg)
{
	OS_ERR err;
	printf("task5 is create ok\r\n");
	while(1)
	{
		USART_SendString(USART2,total_buf);
		OSTimeDlyHMSM(0,0,15,0,OS_OPT_TIME_HMSM_STRICT,&err); //������Ƶ�ʶ�����������ʱ2ms
	}
}
