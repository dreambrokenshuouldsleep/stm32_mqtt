#include "menu.h"
extern unsigned char BMP_pic1[];
volatile u8 Menu1_flag = 0;
volatile u8 Menu2_flag = 1;

volatile u8 Menu1_key = 1;//设置1为初始位置
volatile u8 Menu2_key = 1;
volatile u8 key = 2;

volatile u8 clean = 0;
extern u8 host;
extern u8 ukey ;
extern u8 Confirm_key ;
extern uint32_t sensor_value[];
extern u8 gy_buf[128];//数据存储缓存区，用来储存拼接的字符串
extern u8 ppm_buf[128];
extern u8 temp_buf[128];
extern u8 humi_buf[128];
extern u8 time_buf[128];

u8 i = 0;
u8 show_s = 0;
u8 door = 0;

volatile u8 show_flag = 0;

void get_key(void)
{
	key = ukey;
	Delay_ms(140);
	ukey = 2;
}

//清一次屏幕
void Clean_Menu(void)
{
	if(clean ==1)
	{
		OLED_Clear();
		clean = 0;
	}
}
//主界面一显示
void Menu1_Show(void)
{
	
	if(key == 0)
	{
		Menu1_key++;
		if(Menu1_key > 2)
			Menu1_key = 1;
		//printf("%d\n" ,Menu1_key);
	}
	else if(key == 1)
	{
		Menu1_key--;
		if(Menu1_key < 1)
			Menu1_key = 2;
	}
	key = 2;
	switch(Menu1_key){
		case 1:
		OLED_ShowString(16,2,(u8 *)">>",16);
		OLED_ShowString(16,4,(u8 *)"  ",16);
		break;
		case 2:
		OLED_ShowString(16,4,(u8 *)">>",16);
		OLED_ShowString(16,2,(u8 *)"  ",16);
		break;
	}
	if(show_flag ==0)
	{
		//系统设置
		OLED_ShowCHinese(32,2,0);
		OLED_ShowCHinese(48 ,2,1);
		OLED_ShowCHinese(64,2,2);
		OLED_ShowCHinese(80,2,3);
		//返回
		OLED_ShowCHinese(32,4,4);
		OLED_ShowCHinese(48,4,5);
		show_flag = 1;
	}
	
			
}

//界面二显示
void Menu2_Show(void)
{
	if(key == 0)
	{
		Menu2_key++;
		if(Menu2_key > 7)
		{
			Menu2_key = 1;
			show_flag = 0;
		}
	}
	else if(key == 1)
	{
		Menu2_key--;
		if(Menu2_key < 1)
		{
			Menu2_key = 7;
			show_flag = 0;
		}
	}
	key = 2;
	
	if(Menu2_key < 5)
	{
		Clean_Menu();
		
		switch(Menu2_key){
			case 1:
			OLED_ShowString(16,0,(u8 *)">>",16);
			OLED_ShowString(16,2,(u8 *)"  ",16);
			break;
			case 2:
			OLED_ShowString(16,0,(u8 *)"  ",16);
			OLED_ShowString(16,2,(u8 *)">>",16);
			OLED_ShowString(16,4,(u8 *)"  ",16);
			break;
			case 3:
			OLED_ShowString(16,2,(u8 *)"  ",16);
			OLED_ShowString(16,4,(u8 *)">>",16);
			OLED_ShowString(16,6,(u8 *)"  ",16);
			break;
			case 4:
			OLED_ShowString(16,4,(u8 *)"  ",16);
			OLED_ShowString(16,6,(u8 *)">>",16);
			break;
		}
		if(show_flag == 0)
		{
			//烟雾
			OLED_ShowCHinese(32,0,8);
			OLED_ShowCHinese(48 ,0,9);
			OLED_ShowCHinese(64,0,6);
			OLED_ShowCHinese(80,0,7);
			//光照
			OLED_ShowCHinese(32,2,10);
			OLED_ShowCHinese(48 ,2,11);
			OLED_ShowCHinese(64,2,6);
			OLED_ShowCHinese(80,2,7);
			//温度
			OLED_ShowCHinese(32,4,12);
			OLED_ShowCHinese(48 ,4,13);
			OLED_ShowCHinese(64,4,6);
			OLED_ShowCHinese(80,4,7);
			//湿度
			OLED_ShowCHinese(32,6,14);
			OLED_ShowCHinese(48 ,6,15);
			OLED_ShowCHinese(64,6,6);
			OLED_ShowCHinese(80,6,7);
			show_flag = 1;
		}
		
	}
	else
	{
		
		Clean_Menu();
		//用户输入
		
		switch(Menu2_key){
			case 5:
			OLED_ShowString(16,0,(u8 *)">>",16);
			OLED_ShowString(16,2,(u8 *)"  ",16);
			break;
			case 6:
			OLED_ShowString(16,0,(u8 *)"  ",16);
			OLED_ShowString(16,2,(u8 *)">>",16);
			OLED_ShowString(16,4,(u8 *)"  ",16);
			break;
			case 7:
			OLED_ShowString(16,2,(u8 *)"  ",16);
			OLED_ShowString(16,4,(u8 *)">>",16);
			break;
		}
		//printf("%d\n" , Menu2_key);
		if(show_flag == 0)
		{
			//增加卡
			OLED_ShowCHinese(32,0,16);
			OLED_ShowCHinese(48 ,0,17);
			OLED_ShowCHinese(64,0,21);
			
			OLED_ShowCHinese(32,2,18);
			OLED_ShowCHinese(48,2,19);
			OLED_ShowCHinese(64,2,21);
			
			//返回
			OLED_ShowCHinese(32,4,4);
			OLED_ShowCHinese(48,4,5);
				show_flag = 1;
		}
	}
}

void Menu3_Show(void)
{
	if(key == 0)
	{
		sensor_value[Menu2_key-1]++;
	}
	else if(key == 1)
	{
		sensor_value[Menu2_key - 1]--;
	}
	key = 2;
	OLED_ShowCHinese(32,3,6 + 2*Menu2_key);
	OLED_ShowCHinese(48 ,3,7+ 2*Menu2_key);
	OLED_ShowCHinese(64,3,6);
	OLED_ShowCHinese(80,3,7);
	OLED_ShowNum(48,5,sensor_value[Menu2_key-1],4,16);
	if(Confirm_key == 1)
	{
		OLED_Clear();
		Menu2_flag = 0;
		Confirm_key = 0;
	}
		
}

void Menu4_Show(void)
{
	if(i == 0)
	{
		OLED_ShowCHinese(0,0,29);
		OLED_ShowCHinese(16,0,30);
		OLED_DrawBMP(0, 2,127,6,BMP_pic1);
		OLED_ShowCHinese(0,6,12);
		OLED_ShowCHinese(16,6,13);
		OLED_ShowCHinese(64,6,14);
		OLED_ShowCHinese(80,6,15);
		i++;
	}
	else if(i == 1)
	{
		OLED_ShowCHinese(0,0,29);
		OLED_ShowCHinese(16,0,30);
		OLED_ShowCHinese(0,6,12);
		OLED_ShowCHinese(16,6,13);
		OLED_ShowCHinese(64,6,14);
		OLED_ShowCHinese(80,6,15);
		i++;	
	}
	OLED_ShowString(32,6,temp_buf,16);
	OLED_ShowString(96,6,humi_buf,16);
	OLED_ShowString(32,0,time_buf,16);
	if(sensor_value[4] != 2)
	{
		Menu5_Show();
	}
}

//刷卡成功失败菜单
void Menu5_Show(void)
{
	Clean_Menu();
	if((sensor_value[4] == 1)&&(door == 0))
	{
		sg90_forward();
		door = 1;
	}
	if(sensor_value[4] == 1)
	{
		OLED_ShowCHinese(32,3,20);
		OLED_ShowCHinese(48,3,21);
		OLED_ShowCHinese(64,3,22);
		OLED_ShowCHinese(80,3,23);
		LEDE5_Open;
		LEDB5_Close;
		if(Confirm_key == 1)
		{
			Confirm_key = 0;
			sensor_value[4] = 2;
			host = 1;
			OLED_Clear();
			i = 0;
			show_s = 0;
			LEDE5_Close;
		}
	}
	else if(sensor_value[4] == 0)
	{
		OLED_ShowCHinese(48,3,24);
		OLED_ShowCHinese(64,3,25);
		OLED_ShowCHinese(80,3,26);
		LEDB5_Open;
		LEDE5_Close;
	}
	show_s++;
	if(show_s > 10)
	{
		show_s = 0;
		sensor_value[4] = 2;
		OLED_Clear();
		i = 0;
		LEDB5_Close;
		LEDE5_Close;
		sg90_reversal();
	}
}

//用户主菜单
void Menu_Show(void)
{
	Clean_Menu();
	get_key();
	if(Menu1_flag == 0)
	{
		Menu1_Show();
		if((Confirm_key == 1)&& (Menu1_key == 1))
		{
			Menu1_flag = 1;
			Confirm_key = 0;
			Menu2_flag = 0;
			OLED_Clear();
			show_flag = 0;
			i = 0;
		}
		else if((Confirm_key == 1)&& (Menu1_key == 2))
		{
			Confirm_key = 0;
			host = 0;
			OLED_Clear();
			show_flag = 0;
		}
	}
	else if((Menu1_flag != 0) && (Menu2_flag == 0))
	{
		
		Menu2_Show();
		//返回
		if((Confirm_key == 1)&& (Menu2_key == 7))
		{
			Menu1_flag = 0;
			Confirm_key = 0;
			Menu2_flag = 1;
			Menu2_key = 1;
			show_flag = 0;
			OLED_Clear();
		}
		//增减卡
		else if((Confirm_key == 1)&&(0< Menu2_key)&&(Menu2_key < 7))
		{
			Confirm_key = 0;
			Menu2_flag = 1;
			show_flag = 0;
			OLED_Clear();
		}
	}
	else if((Menu1_flag != 0) && (Menu2_flag != 0) && (0< Menu2_key) && (Menu2_key < 5))
	{
		Menu3_Show();
	}
	else if((Menu1_flag != 0) && (Menu2_flag != 0)&&(4< Menu2_key) && (Menu2_key < 7))
	{
		if(Menu2_key == 5)
		{
			sensor_value[5] = 1;
			if(show_flag == 0)
			{
				OLED_ShowCHinese(32,3,16);
				OLED_ShowCHinese(48,3,17);
				OLED_ShowString(64,3,(u8 *)"......",16);
				show_flag = 1;
			}
			if(Confirm_key == 1)
			{
				Confirm_key = 0;
				Menu2_flag = 0;
				sensor_value[5] = 2;
				show_flag = 0;
				OLED_Clear();
			}
		}
		else if(Menu2_key == 6)
		{
			sensor_value[5] = 0;
			if(show_flag == 0)
			{
				OLED_ShowCHinese(32,3,18);
				OLED_ShowCHinese(48,3,19);
				OLED_ShowString(64,3,(u8 *)"......",16);
				show_flag = 1;
			}
			if(Confirm_key == 1)
			{
				Confirm_key = 0;
				Menu2_flag = 0;
				sensor_value[5] = 2;
				show_flag = 0;
				OLED_Clear();
			}
		}
	}
}




