/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ap3216c���ԣ�������Ϣͨ��USART1��ӡ�ڵ��Եĳ����ն�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "./ap3216c/ap3216c.h"
#include "bsp_SysTick.h"  
#include "stm32f10x.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include <string.h>


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  /**/
  float ALS;
  uint16_t PS;
  uint16_t IR;
  uint8_t IntStatus;
  
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();
  
  printf("\r\n ��ӭʹ��Ұ�𿪷��塣\r\n");	
  printf("\r\n ����һ������һ���մ������������� \r\n");
  
  printf(" оƬ��ʼ����.....\n");
  /* ��ʼ�� ���մ����� */
  ap3216c_init();

  while(1)    
  {
    IntStatus = ap3216c_get_IntStatus();    // �ȶ�״̬λ����ADC����λ�����״̬λ��Ĭ�����ã�
    ALS = ap3216c_read_ambient_light();
    PS = ap3216c_read_ps_data();
    IR = ap3216c_read_ir_data();

    printf("\n����ǿ���ǣ�%.2fLux\n����ǿ���ǣ�%d\n", ALS, IR);

    if (PS == 55555)    // IR ̫ǿ PS ������Ч
      printf("IR ̫ǿ PS ������Ч\n");
    else
    {
      printf("�ӽ������ǣ�%d\n", PS & 0x3FF);
    }
    
    if (AP_INT_Read() == 0)
      printf("���жϲ���\n");
    
    if ((PS >> 15) & 1)
      printf("����ӽ�\n");
    else
      printf("����Զ��\n");
    
    if (IntStatus & 0x1)
      printf("ALS �����ж�\n");
    
    if (IntStatus >> 1 & 0x1)
      printf("PS �����ж�\n");
    
    SysTick_Delay_Ms(400);
  }
}


/*********************************************END OF FILE**********************/
