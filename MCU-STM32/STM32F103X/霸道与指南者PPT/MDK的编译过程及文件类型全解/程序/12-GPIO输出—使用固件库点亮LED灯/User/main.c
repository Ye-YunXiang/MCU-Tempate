/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
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
#include "bsp_led.h"
#include <stdlib.h>

#define SOFT_DELAY Delay(0x0FFFFF);

 uint8_t test[1024]={0,1,2,3,4,56,5};
// uint8_t test[1024]={0};

// 
// uint8_t test2=10;

void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	uint8_t testttt[50];
	
	
	uint8_t * p;
	/* LED 端口初始化 */
	LED_GPIO_Config();	 

	
	p = (uint8_t *) malloc(50);
	
	p[0] += 10;
	
	testttt[40] = 10;
//	test[5] +=90;
//	
	if(p[0] ==0 && testttt[40] == 11 &&test[10]==100)
	{
		LED1_ON;			  // 亮		
	}
	
	free(p);
	
	while (1)
	{
		LED1_ON;			  // 亮
		SOFT_DELAY;
		LED1_OFF;		   // 灭

		LED2_ON;			 // 亮
		SOFT_DELAY;
		LED2_OFF;		   // 灭

		LED3_ON;			 // 亮
		SOFT_DELAY;
		LED3_OFF;		   // 灭	 

		/*轮流显示 红绿蓝黄紫青白 颜色*/
		LED_RED;
		SOFT_DELAY;
		
		LED_GREEN;
		SOFT_DELAY;
		
		LED_BLUE;
		SOFT_DELAY;
		
		LED_YELLOW;
		SOFT_DELAY;
		
		LED_PURPLE;
		SOFT_DELAY;
				
		LED_CYAN;
		SOFT_DELAY;
		
		LED_WHITE;
		SOFT_DELAY;
		
		LED_RGBOFF;
		SOFT_DELAY;		
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
