/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	��ʱ���������������
��д�ˣ�	����	
��дʱ�䣺	2018��6��4��
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
#include "SG90.h"
#include "touch_key.h"


int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RELAY_Init();//�̵�����ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(3,"   SG90 TEST    "); //��ʾ�ַ���

	TOUCH_KEY_Init();//������ʼ��
	SG90_Init();//SG90�����ʼ��
	SG90_angle(0);//�������Ϊ0����Сֵ��

	while(1){
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 0       "); //��ʾ�ַ���
			SG90_angle(0);//�������Ϊ0����Сֵ��
		}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 45      "); //��ʾ�ַ���
			SG90_angle(45);//�������Ϊ0����Сֵ��
		}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 90     "); //��ʾ�ַ���
			SG90_angle(90);//�������Ϊ0����Сֵ��
		}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 180     "); //��ʾ�ַ���
			SG90_angle(180);//�������Ϊ0����Сֵ��
		}
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



