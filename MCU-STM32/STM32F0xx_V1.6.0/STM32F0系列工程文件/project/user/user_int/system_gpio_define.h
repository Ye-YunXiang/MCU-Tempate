#ifndef _SYSTEM_GPIO_DEFINE_H_
#define _SYSTEM_GPIO_DEFINE_H_

#include "gpio.h"


/*-- M26���� ---------------------------------*/
#define GSM_EN_PORT                 (GpioPortA)
#define GSM_EN_PIN                  (GpioPin11)

#define GSM_PWRKEY_PORT             (GpioPortB)
#define GSM_PWRKEY_PIN              (GpioPin12)

/*-- UART���� ---------------------------------*/
#define UART0_MCU_TX_PORT           (GpioPortA)
#define UART0_MCU_TX_PIN            (GpioPin9)

#define UART0_MCU_RX_PORT           (GpioPortA)
#define UART0_MCU_RX_PIN            (GpioPin10)

#define LPUART0_MCU_TX1_PORT        (GpioPortB)
#define LPUART0_MCU_TX1_PIN         (GpioPin10)

#define LPUART0_MCU_RX1_PORT        (GpioPortB)
#define LPUART0_MCU_RX1_PIN         (GpioPin11)

/*-- ���̲��� ---------------------------------*/
#define	KEY_R1_PORT		    	    (GpioPortB)
#define	KEY_R1_PIN		    	    (GpioPin9)

#define	KEY_R2_PORT		    	    (GpioPortB)
#define	KEY_R2_PIN		    	    (GpioPin8)

#define	KEY_R3_PORT		    	    (GpioPortB)
#define	KEY_R3_PIN		    	    (GpioPin7)

#define KEY_R4_PORT		    	    (GpioPortB)
#define	KEY_R4_PIN	    		    (GpioPin6)

#define	KEY_C1_PORT		    	    (GpioPortB)
#define	KEY_C1_PIN		    	    (GpioPin5)

#define	KEY_C2_PORT	    		    (GpioPortB)
#define	KEY_C2_PIN	    		    (GpioPin4)

#define	KEY_C3_PORT		    	    (GpioPortB)
#define	KEY_C3_PIN	    		    (GpioPin3)

/*-- LCD���� ---------------------------------*/
#define LCDRS_PORT                  (GpioPortA)	
#define LCDRS_PIN                   (GpioPin1)

#define LCDRW_PORT	    	        (GpioPortA)		
#define LCDRW_PIN                   (GpioPin2)

#define LCDEN_PORT		            (GpioPortA)		
#define LCDEN_PIN		            (GpioPin3)

#define LCDD4_PORT	    	        (GpioPortA)		
#define LCDD4_PIN	    	        (GpioPin4)

#define LCDD5_PORT	    	        (GpioPortA)		
#define LCDD5_PIN	    	        (GpioPin5)

#define LCDD6_PORT	    	        (GpioPortA)			
#define LCDD6_PIN	    	        (GpioPin6)

#define LCDD7_PORT	    	        (GpioPortA)			
#define LCDD7_PIN   		        (GpioPin7)

#define	LCD_LIGHT_PORT	            (GpioPortB)		
#define LCD_LIGHT_PIN	            (GpioPin1)

/*-- LED�Ʋ��� ---------------------------------*/
#define LED_RED_PORT                (GpioPortB)
#define LED_RED_PIN                 (GpioPin0)

#define LED_GREEN_PORT              (GpioPortB)
#define GREEN_PIN                   (GpioPin2)

/*-- ������Ʋ��� ---------------------------------*/
#define REFRIGERATOR_PORT           (GpioPortF)
#define REFRIGERATOR_PIN            (GpioPin6)

/*-- ADC��ⲿ�� ---------------------------------*/
#define ADC_24V_PORT                (GpioPortA)     
#define ADC_24V_PIN                 (GpioPin0)      // �ⲿ����

#define ADC_BAT_PORT                (GpioPortB)
#define ADC_BAT_PIN                 (GpioPin14)     // С��ص͹��Ĺ���

#define ADC_TEMP_PORT               (GpioPortB)
#define ADC_TEMP_PIN                (GpioPin15)     // �¶ȼ��

#define BAT_ADC_EN_PORT             (GpioPortA)     
#define BAT_ADC_EN_PIN              (GpioPin12)     // ��ADC���

#define ADC_5V_PORT                 (GpioPortA)
#define ADC_5V_PIN                  (GpioPin8)      // �ⲿ��Դ������ < ��ƽ��� >

#define ADC_TAMPER_PORT             (GpioPortA)
#define ADC_TAMPER_PIN              (GpioPin15)     // ��Ǵ򿪼��     < ��ƽ��� >

/*-- �������Ʋ��� ---------------------------------*/
#define BT_STATE_PORT               (GpioPortB)     
#define BT_STATE_PIN                (GpioPin13)      // ����״̬���

#define BT_EN_PORT                  (GpioPortC)     
#define BT_EN_PIN                   (GpioPin13)      // ����������

#define BT_TX_PORT                  (GpioPortF)     
#define BT_TX_PIN                   (GpioPin0)   

#define BT_RX_PORT                  (GpioPortF)     
#define BT_RX_PIN                   (GpioPin1)      



#endif
