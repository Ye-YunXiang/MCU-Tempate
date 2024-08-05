/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	MP3������������
��д�ˣ�	����	
��дʱ�䣺	2018��4��20��
Ӳ��֧�֣�	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "touch_key.h"
#include "relay.h"
#include "oled0561.h"
#include "rtc.h"

#include "encoder.h"
#include "usart.h"
#include "my1690.h"

int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RTC_Config();//ʵʱʱ�ӳ�ʼ��
	TOUCH_KEY_Init();//����������ʼ��
	RELAY_Init();//�̵�����ʼ��

	ENCODER_Init(); //��ת��������ʼ��
	MY1690_Init(); //MP3оƬ��ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(2," MP3 TIME READ  "); //��ʾ�ַ���

	while(1){

		if(RTC_Get()==0){
			OLED_DISPLAY_8x16(4,8*3,rhour/10+0x30);//��ʾʱ��
			OLED_DISPLAY_8x16(4,8*4,rhour%10+0x30);//��ʾʱ��
			OLED_DISPLAY_8x16(4,8*5,':');//
			OLED_DISPLAY_8x16(4,8*6,rmin/10+0x30);//��ʾʱ��
			OLED_DISPLAY_8x16(4,8*7,rmin%10+0x30);//��ʾʱ��
			OLED_DISPLAY_8x16(4,8*8,':');//
			OLED_DISPLAY_8x16(4,8*9,rsec/10+0x30);//��ʾʱ��
			OLED_DISPLAY_8x16(4,8*10,rsec%10+0x30);//��ʾʱ��
			delay_ms(200); //��ʱ
		}
		if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0    //�ж�4�������Ƿ���
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0){
			delay_ms(20); //��ʱ
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0){	//4��������A��һ����B��һ����
				OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //��ʾ�ַ���
				//���﷨
//				MY1690_CMD3(0x41,12); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,rhour/10+1); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,11); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,rhour%10+1); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,13); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,rmin/10+1); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,11); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,rmin%10+1); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,14); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,rsec/10+1); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,11); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,rsec%10+1); //ֱ�Ӳ���
//				MY1690_CMD3(0x41,15); //ֱ�Ӳ���

				//���﷨
				MY1690_CMD3(0x41,12); //ֱ�Ӳ���
				if(rhour/10==0){
					//������
				}else if(rhour/10==1){
					MY1690_CMD3(0x41,11); //ֱ�Ӳ���
				}else{
					MY1690_CMD3(0x41,rhour/10+1); //ֱ�Ӳ���
					MY1690_CMD3(0x41,11); //ֱ�Ӳ���
				}
				if(rhour%10!=0 || rhour==0){
					MY1690_CMD3(0x41,rhour%10+1); //ֱ�Ӳ���
				}
				MY1690_CMD3(0x41,13); //ֱ�Ӳ���
				if(rmin/10==0){
					MY1690_CMD3(0x41,rmin/10+1); //ֱ�Ӳ���
				}else if(rmin/10==1){
					MY1690_CMD3(0x41,11); //ֱ�Ӳ���
				}else{
					MY1690_CMD3(0x41,rmin/10+1); //ֱ�Ӳ���
					MY1690_CMD3(0x41,11); //ֱ�Ӳ���
				}
				if(rmin%10!=0 || rmin==0){
					MY1690_CMD3(0x41,rmin%10+1); //ֱ�Ӳ���
				}
				MY1690_CMD3(0x41,14); //ֱ�Ӳ���
				if(rsec/10==0){
					MY1690_CMD3(0x41,rsec/10+1); //ֱ�Ӳ���
				}else if(rsec/10==1){
					MY1690_CMD3(0x41,11); //ֱ�Ӳ���
				}else{
					MY1690_CMD3(0x41,rsec/10+1); //ֱ�Ӳ���
					MY1690_CMD3(0x41,11); //ֱ�Ӳ���
				}
				if(rsec%10!=0 || rsec==0){
					MY1690_CMD3(0x41,rsec%10+1); //ֱ�Ӳ���
				}
				MY1690_CMD3(0x41,15); //ֱ�Ӳ���
			}
			while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0	//�ȴ������ſ�
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0);
		}
		//���ڽ��մ���
        if(USART3_RX_STA==1){ //�����־λ��1 ��ʾ�յ�STOP
            OLED_DISPLAY_8x16_BUFFER(6,"  -- STOP --    "); //��ʾ�ַ���
            USART3_RX_STA=0; //���������ݱ�־λ��0
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



