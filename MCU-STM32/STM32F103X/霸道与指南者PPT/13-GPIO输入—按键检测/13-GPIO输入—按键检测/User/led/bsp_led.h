#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_G_GPIO_PIN              GPIO_Pin_2
#define LED_G_GPIO_PORT             GPIOA
#define LED_G_GPIO_CLK              RCC_APB2Periph_GPIOA


#define    ON        1
#define    OFF       0

// \  C语言里面叫续行符，后面不能有任何的东西

#define   LED_G(a)   if(a) \
	                       GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN); \
                     else  GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);

// ^ 异或，C语言的一个二进制的运算符
// 与1异或改变，与0异或不变

#define LED_G_TOGGLE        {LED_G_GPIO_PORT->ODR ^= LED_G_GPIO_PIN;}

void LED_GPIO_Config(void);

#endif /* __BSP_LED_H */

