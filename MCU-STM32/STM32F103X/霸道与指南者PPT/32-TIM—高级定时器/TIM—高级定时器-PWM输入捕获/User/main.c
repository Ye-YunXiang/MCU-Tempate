
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_GeneralTim.h"  
#include "bsp_AdvanceTim.h" 
#include "bsp_usart.h"

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	USART_Config();
	
	/* 通用定时器初始化 */
	GENERAL_TIM_Init();
	
	/* 高级定时器初始化 */
	ADVANCE_TIM_Init();
	
  while(1)
  {      
  }
}
/*********************************************END OF FILE**********************/
