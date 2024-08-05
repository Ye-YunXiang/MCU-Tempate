/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	˯��ģʽ���Գ���
��д�ˣ�	����	
��дʱ�䣺	2018��7��14��
Ӳ��֧�֣�	����1�ſ����� STM32F103C8 �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��������������1�ſ������Ӳ�������ϱ�д�ģ���ֲ���˽�Ӳ���ӿڲ��졣
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "led.h"
#include "key.h"

#include "NVIC.h"


int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RELAY_Init();//�̵�����ʼ��
	LED_Init();//LED 
	KEY_Init();//KEY

	I2C_Configuration();//I2C��ʼ��

	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   SLEEP TEST   "); //��ʾ�ַ���

	INT_MARK=0;//��־λ��0
	NVIC_Configuration();//�����ж����ȼ�
	KEY_INT_INIT();//�����жϳ�ʼ����PA0�ǰ����ж����룩

	NVIC_SystemLPConfig(NVIC_LP_SEVONPEND,DISABLE);	//SEVONPEND: 0��ֻ��ʹ�ܵ��жϻ��¼����ܻ����ںˡ�1���κ��жϺ��¼������Ի����ںˡ���0=DISABLE��1=ENABLE) 
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,DISABLE);	//SLEEPDEEP: 0���͹���ģʽΪ˯��ģʽ��1������͹���ʱΪ���˯��ģʽ��
    NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,DISABLE); //SLEEPONEXIT: 0: �����ѽ����߳�ģʽ���ٽ���˯��ģʽ��1�������Ѻ�ִ������Ӧ���жϴ����������˯��ģʽ��

	while(1){

		GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����
		OLED_DISPLAY_8x16_BUFFER(4,"  CPU SLEEP!    "); //��ʾ�ַ���
		delay_ms(500); //

		__WFI(); //����˯��ģʽ���ȴ��жϻ���
//		__WFE(); //����˯��ģʽ���ȴ��¼�����

		GPIO_WriteBit(LEDPORT,LED1,(BitAction)(0)); //LED����
		OLED_DISPLAY_8x16_BUFFER(4,"  CPU WAKE UP!  "); //��ʾ�ַ���
		delay_ms(500); //
	}

}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����

*/



