/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	U盘插拔测试程序
编写人：	杜洋	
编写时间：	2018年4月27日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "touch_key.h"
#include "relay.h"
#include "oled0561.h"

#include "spi.h"
#include "ch376.h"
#include "filesys.h"

int main (void){//主程序
	u8 s;
	delay_ms(500); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	TOUCH_KEY_Init();//触摸按键初始化
	RELAY_Init();//继电器初始化

	I2C_Configuration();//I2C初始化
	OLED0561_Init(); //OLED初始化
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(2,"  U DISK TEST   "); //显示字符串
	//CH376初始化	
	SPI2_Init();//SPI接口初始化
	if(mInitCH376Host()== USB_INT_SUCCESS){//CH376初始化
		OLED_DISPLAY_8x16_BUFFER(4,"   CH376 OK!    "); //显示字符串
	}
	while(1){
		s = CH376DiskConnect();	 //读出U盘的状态 
		if(s == USB_INT_SUCCESS){ //检查U盘是否连接//等待U盘插入
			OLED_DISPLAY_8x16_BUFFER(6," U DISK Ready!  "); //显示字符串
		}else{
			OLED_DISPLAY_8x16_BUFFER(6,"                "); //显示字符串		
		} 
		delay_ms(500); //刷新的间隔
	}
}

/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED控制

*/



