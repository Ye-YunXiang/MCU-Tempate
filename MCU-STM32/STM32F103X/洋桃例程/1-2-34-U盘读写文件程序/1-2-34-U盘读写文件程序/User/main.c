/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	U�̶�д�ļ�����
��д�ˣ�	����	
��дʱ�䣺	2018��5��14��
Ӳ��֧�֣�	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "touch_key.h"
#include "relay.h"
#include "oled0561.h"

#include "spi.h"
#include "ch376.h"
#include "filesys.h"

u8 buf[128]; 

int main (void){//������
	u8 s,i;
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	TOUCH_KEY_Init();//����������ʼ��
	RELAY_Init();//�̵�����ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(2,"  U DISK TEST   "); //��ʾ�ַ���
	//CH376��ʼ��	
	SPI2_Init();//SPI�ӿڳ�ʼ��
	if(mInitCH376Host()== USB_INT_SUCCESS){//CH376��ʼ��
		OLED_DISPLAY_8x16_BUFFER(4,"   CH376 OK!    "); //��ʾ�ַ���
	}
	while(1){
		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) delay_ms(100);  // ���U���Ƿ�����,�ȴ�U�̰γ�
		OLED_DISPLAY_8x16_BUFFER(6," U DISK Ready!  "); //��ʾ�ַ���
		delay_ms(200); //ÿ�β������Ҫ����ʱ
		for ( i = 0; i < 100; i ++ ){ 
			delay_ms( 50 );
			s = CH376DiskMount( );  //��ʼ�����̲����Դ����Ƿ����.   
			if ( s == USB_INT_SUCCESS ) /* ׼���� */
			break;                                          
			else if ( s == ERR_DISK_DISCON )/* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			break;  
			if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
			break; 
		}
		OLED_DISPLAY_8x16_BUFFER(6," U DISK INIT!   "); //��ʾ�ַ���
		delay_ms(200); //ÿ�β������Ҫ����ʱ
      	s=CH376FileCreatePath( "/����.TXT" ); // �½��༶Ŀ¼�µ��ļ�,֧�ֶ༶Ŀ¼·��,���뻺����������RAM�� 
		delay_ms(200); //ÿ�β������Ҫ����ʱ
		s = sprintf( (char *)buf , "���ҵ��� www.DoYoung.net/YT");
		s=CH376ByteWrite( buf,s, NULL ); // ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� 
		delay_ms(200); //ÿ�β������Ҫ����ʱ
		s=CH376FileClose( TRUE );   // �ر��ļ�,�����ֽڶ�д�����Զ������ļ����� 
		OLED_DISPLAY_8x16_BUFFER(6," U DISK SUCCESS "); //��ʾ�ַ���
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) delay_ms(500);  // ���U���Ƿ�����,�ȴ�U�̰γ�
		OLED_DISPLAY_8x16_BUFFER(6,"                "); //��ʾ�ַ���
		delay_ms(200); //ÿ�β������Ҫ����ʱ
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



