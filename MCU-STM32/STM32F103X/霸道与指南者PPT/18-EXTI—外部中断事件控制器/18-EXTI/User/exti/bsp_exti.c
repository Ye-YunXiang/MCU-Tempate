#include "bsp_exti.h"

static void EXTI_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	//中断源
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	//使能中断源
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	//调用NVIC初始化
	NVIC_Init(&NVIC_InitStruct);
}

void EXIT_Key_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	
	// 配置中断优先级
	EXTI_NVIC_Config();
	
	// 初始化GPIO
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK, ENABLE);	
	GPIO_InitStruct.GPIO_Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;		
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStruct);	
	
	// 初始化EXTI外设时钟，很重要
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//选择中断源
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	//选择中断线
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	//选择中断模式
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	//选择触发模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	//使能中断
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	//初始化了
	EXTI_Init(&EXTI_InitStruct);	
}














