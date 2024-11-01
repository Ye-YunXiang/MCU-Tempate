/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"


//1.unlock FLASH
//2.擦除指定的页（查看工程空间分布，以防擦除程序本身）
//3.写入数据
//4.读出数据校验

//作业：尝试擦除程序本身的空间，看看实验现象

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	
	uint32_t *p;
	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	
	/* 发送一个字符串 */
	printf("欢迎使用秉火STM32开发板\n\n\n\n");
	
	
	FLASH_Unlock(); 
	
	
	//擦除第5页
	FLASH_ErasePage(0x08000000+2*1024*5);
	
	//可以向地址 0x08000000+2*1024*5  至  0x08000000+2*1024*6 地址写入数据
	FLASH_ProgramWord(0x08000000+2*1024*5,0x01234567);
	
	FLASH_Lock();
	
	printf("\r\n已经向内部FLASH写入完成");
	
	p = (uint32_t *)(0x08000000+2*1024*5);
	
	printf("\r\n读取内部FLASH该地址存储的内容为：0x%x",*p);
	
	
	 
  while(1)
	{	
		
	}	
}
/*********************************************END OF FILE**********************/
