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
u8  card_pydebuf[2];                                            //保存卡的类型
u8  card_numberbuf[5];                                          //保存卡的序列号
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};			//
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};  //保存写入的卡内容    
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
//烟雾阈值、光照阈值、温度阈值、湿度阈值、开关门标志、增减卡标志,靠近阈值
volatile uint32_t sensor_value[7] = {2000 , 1400 , 40 ,100,2,2,20};

extern u8  USART2_RX_BUF[USART2_REC_LEN];
extern volatile u8 key;
	
uint8_t temp_value[5] = {0};

static float ppm = 0;
static float gy = 0;
u8 gy_buf[128] = {0};//数据存储缓存区，用来储存拼接的字符串
u8 ppm_buf[128] = {0};
u8 temp_buf[128] = {0};
u8 humi_buf[128] = {0};
u8 time_buf[128] = {0};
uint32_t time[3] = {0};
char total_buf[256] = {0};
char warn_buf[256] = {0};


volatile u8 len = 1;
//记录停留时间
volatile u8 dis = 0;

volatile u8 flag_sensor = 0;
void value_deal(float gy , float ppm , uint8_t temp , uint8_t humi)//OLED处理字符串函数
{
	sprintf((char *)gy_buf , "%.2flux" , gy);//拼接字符串
	sprintf((char *)ppm_buf , "%.2fppm" , ppm);
	sprintf((char *)temp_buf , "%dC" ,temp);
	sprintf((char *)humi_buf , "%dRH" , humi);
	sprintf((char *)time_buf , "%0.2d:%0.2d:%0.2d" , time[0], time[1], time[2]);
	sprintf(total_buf , "AT+MQTTPUB=0,\"ccc\",\":gy=%s\\,ppm=%s\\,temp=%s\\,humi=%s\\,time=%s\",1,0\r\n",gy_buf,ppm_buf,temp_buf,humi_buf,time_buf);
}

void Get_Value(void)//获取传感器的值
{
	gy = GY_Value();//光照传感器
	ppm = MQ_Get_PPMData();//烟雾传感器
	DHT_GetData(temp_value);//温湿度数据
	Time_Display(RTC_GetCounter(),time);//RTC时钟获得计数
	value_deal( gy ,  ppm ,  26 ,  65);//OLED处理字符串函
}

void send_warn(char * state)
{
	sprintf(warn_buf,"AT+MQTTPUB=0,\"ccc\",\"Warn:gy=%s\\,ppm=%s\\,temp=%s\\,humi=%s\\,usr=%d\\,time=%s\\,stay=%d\\,type=%s\",1,0\r\n",gy_buf,ppm_buf,temp_buf,humi_buf,card_numberbuf[0],time_buf,1 ,state);
	USART_SendString(USART2,warn_buf);
}
//检测异常

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

//任务一
OS_TCB Task1_TCB;

void task1(void *parg);

CPU_STK task1_stk[128];			//任务1的任务堆栈，大小为128字，也就是512字节
//任务二
OS_TCB Task2_TCB;

void task2(void *parg);

CPU_STK task2_stk[128];			//任务1的任务堆栈，大小为128字，也就是512字节

//任务三
OS_TCB Task3_TCB;

void task3(void *parg);

CPU_STK task3_stk[128];			//任务1的任务堆栈，大小为128字，也就是512字节

OS_TCB Task4_TCB;

void task4(void *parg);

CPU_STK task4_stk[128];	

OS_TCB Task5_TCB;

void task5(void *parg);

CPU_STK task5_stk[128];	

int main()
{
OS_ERR err;

	
	delay_init();								//时钟初始化   利用嘀嗒定时器来为操作系统产生周期性的节拍
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组配置 
	uart1_init(115200);
	uart2_init(115200);
	esp8266_Init();
	Iwdg_Init();		
	KEY_Init();
	LED_Init();
	Sg90_Init();
	//OS初始化，它是第一个运行的函数,初始化各种的全局变量，例如中断嵌套计数器、优先级、存储器
	OSInit(&err);		   	 						//初始化UCOSIII
			 
	//创建开始任务
	//创建任务1，着重填写前8个参数
	//rfid任务
	OSTaskCreate(	(OS_TCB *)&Task1_TCB,									//任务控制块  任务控制块的地址
					(CPU_CHAR *)"Task1",									//任务的名字，自定义
					(OS_TASK_PTR)task1,										//任务函数
                    (void *)0,												//传递参数  NULL  不需要传参
					(OS_PRIO)2,											 	//任务的优先级	   数值越小，优先级越高	
					(CPU_STK *)task1_stk,									//任务堆栈基地址   本质上栈空间就是一个数组   如果任务较多，记得修改汇编文件中栈空间的大小
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
			
				);
	//喂狗任务			
	OSTaskCreate(	(OS_TCB *)&Task2_TCB,									//任务控制块  任务控制块的地址
					(CPU_CHAR *)"Task2",									//任务的名字，自定义
					(OS_TASK_PTR)task2,										//任务函数
                    (void *)0,												//传递参数  NULL  不需要传参
					(OS_PRIO)1,											 	//任务的优先级	   数值越小，优先级越高	
					(CPU_STK *)task2_stk,									//任务堆栈基地址   本质上栈空间就是一个数组   如果任务较多，记得修改汇编文件中栈空间的大小
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
	
					
					
				);
	//菜单任务
	OSTaskCreate(	(OS_TCB *)&Task3_TCB,									//任务控制块  任务控制块的地址
				(CPU_CHAR *)"Task3",									//任务的名字，自定义
				(OS_TASK_PTR)task3,										//任务函数
				(void *)0,												//传递参数  NULL  不需要传参
				(OS_PRIO)4,											 	//任务的优先级	   数值越小，优先级越高	
				(CPU_STK *)task3_stk,									//任务堆栈基地址   本质上栈空间就是一个数组   如果任务较多，记得修改汇编文件中栈空间的大小
				(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
				(CPU_STK_SIZE)128,										//任务堆栈大小			
				(OS_MSG_QTY)0,											//禁止任务消息队列
				(OS_TICK)0,												//默认时间片长度																
				(void  *)0,												//不需要补充用户存储区
				(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
				&err													//返回的错误码

				
				
			);
				
	//测量任务
	OSTaskCreate(	(OS_TCB *)&Task4_TCB,									//任务控制块  任务控制块的地址
				(CPU_CHAR *)"Task4",									//任务的名字，自定义
				(OS_TASK_PTR)task4,										//任务函数
				(void *)0,												//传递参数  NULL  不需要传参
				(OS_PRIO)3,											 	//任务的优先级	   数值越小，优先级越高	
				(CPU_STK *)task4_stk,									//任务堆栈基地址   本质上栈空间就是一个数组   如果任务较多，记得修改汇编文件中栈空间的大小
				(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
				(CPU_STK_SIZE)128,										//任务堆栈大小			
				(OS_MSG_QTY)0,											//禁止任务消息队列
				(OS_TICK)0,												//默认时间片长度																
				(void  *)0,												//不需要补充用户存储区
				(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
				&err													//返回的错误码

				
				
			);
	OSTaskCreate(	(OS_TCB *)&Task5_TCB,									//任务控制块  任务控制块的地址
				(CPU_CHAR *)"Task5",									//任务的名字，自定义
				(OS_TASK_PTR)task5,										//任务函数
				(void *)0,												//传递参数  NULL  不需要传参
				(OS_PRIO)5,											 	//任务的优先级	   数值越小，优先级越高	
				(CPU_STK *)task5_stk,									//任务堆栈基地址   本质上栈空间就是一个数组   如果任务较多，记得修改汇编文件中栈空间的大小
				(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
				(CPU_STK_SIZE)128,										//任务堆栈大小			
				(OS_MSG_QTY)0,											//禁止任务消息队列
				(OS_TICK)0,												//默认时间片长度																
				(void  *)0,												//不需要补充用户存储区
				(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
				&err													//返回的错误码

				
				
			);
				
					
  
	OSStart(&err);     //启动OS，进行任务调度
}

void task1(void *parg)
{
	OS_ERR err;
	printf("task1 is create ok\r\n");
	RC522_Init();
	host_numberbuf[0] = 0xd9;
	while(1)
	{
		//验卡
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
		}//增加卡
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
		}//减少卡
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
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //设置时分秒毫秒
	}
}

void task2(void *parg)
{
	OS_ERR err;
	printf("task2 is create ok\r\n");
	
	while(1)
	{
		IWDG_ReloadCounter();
		OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err); //由设置频率定，现在是延时2ms
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
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //由设置频率定，现在是延时2ms
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
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //由设置频率定，现在是延时2ms
	}
}

void task5(void *parg)
{
	OS_ERR err;
	printf("task5 is create ok\r\n");
	while(1)
	{
		USART_SendString(USART2,total_buf);
		OSTimeDlyHMSM(0,0,15,0,OS_OPT_TIME_HMSM_STRICT,&err); //由设置频率定，现在是延时2ms
	}
}
