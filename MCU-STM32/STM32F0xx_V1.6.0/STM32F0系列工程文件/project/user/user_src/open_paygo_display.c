/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_display.c 
* Description:	Manage OpenPAYGO device display
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#include "open_paygo_display.h"

display_state_t display_state_manage;

#define  Power_Up_String0            "    koolboks    "
#define  Power_Up_String1            "  life is cool  "

#define  Power_Up_String2            " PLEASE WAIT... "
#define  Power_Up_String3            "STARTING  DEVICE"

#define  Power_Up_String4            " FACTORY SETUP  "
#define  Power_Up_String5            "      MODE      "

#define  Power_Up_String6            " PRESS '*' KEY  "
#define  Power_Up_String7            "AND VALID TOKENS"

#define  Power_Up_String8            "   RTC ERROR    "        // RTC������ʾ
#define  Power_Up_String9            " CONTACT STAFF  "

#define  Rtc_Error_String1           "#  RTC ERROR    "        // RTC������ʾ1
#define  Rtc_Error_String2           "# CONTACT STAFF "        // RTC������ʾ2
#define  Rtc_Error_String3           "# NO POWER OFF  "        // RTC������ʾ3

#define  lcd_dispaly_string8         " ACTIVATED FOR  "
#define  lcd_dispaly_string9         "    DAYS    HOUR"

#define  lcd_dispaly_string11        "NOT TOKENS..."

#define  payg_credit_unlocked1       "  PAYG CREDIT"
#define  payg_credit_unlocked2       "*   unlocked   *"

#define  payg_credit_disable         "-   disable   -"

#define  payg_credit_count_close     "- CLOSE COUNT -"

#define  lcd_disable_lock1           "TOO MANY ERRORS"
#define  lcd_disable_lock2           "TRY AGAIN LATER"


// ��ʼ��ʾ����ṹ��
void open_paygo_display_struct(void)
{
	memset(&display_state_manage, 0, sizeof(display_state_t));
}


/********************************************************************
 ** @brief  ��Ļ��ʾָ������
 ** @param  enum DISPLAY_STATE ָ����ͬ״̬
 ** @retval ��
********************************************************************/
void open_paygo_display_startup(enum DISPLAY_STATE state)
{
    char temp_disable[16] = {};
    
    irq_time.lcd_light_up_time = 0;
    
    System_LCD_Light_Enable();

    switch (state)
    {
        case DISPLAY_SLOGAN :       // �������

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(Power_Up_String0,strlen(Power_Up_String0),0x80); 
            system_lcd1602_string_dispaly(Power_Up_String1,strlen(Power_Up_String1),0xC0);

            break;

        case DISPLAY_UP_LOADING :   // �����ȴ�����
            
            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(Power_Up_String2,strlen(Power_Up_String2),0x80); 
            system_lcd1602_string_dispaly(Power_Up_String3,strlen(Power_Up_String3),0xC0);

            break;

        case DISPLAY_NO_ID :        // û��ID����
            
            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(Power_Up_String4,strlen(Power_Up_String4),0x80); 
            system_lcd1602_string_dispaly(Power_Up_String5,strlen(Power_Up_String5),0xC0);

            break;

        case DISPLAY_NO_PAYGO :     // ��IDû��PAYGO����
            
            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(Power_Up_String6,strlen(Power_Up_String6),0x80); 
            system_lcd1602_string_dispaly(Power_Up_String7,strlen(Power_Up_String7),0xC0);

            break;

        case DISPLAY_RTC_ERROR :    // RTC�������
            
            //system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(Power_Up_String8,strlen(Power_Up_String8),0x80); 
            system_lcd1602_string_dispaly(Power_Up_String9,strlen(Power_Up_String9),0xC0);

            break;

        case DISPLAY_NO_TOKEN :    // �����Tokenû��
            
            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(lcd_dispaly_string11,strlen(lcd_dispaly_string11),0x80); 

            system_led_blink_red_one();

            system_led_blink_red_one();

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(Power_Up_String2,strlen(Power_Up_String2),0x80); 

            delay1ms(1000);

            break;

        case DISPLAY_ACTIVATED_TIME :    // ���ʱ
            
            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(lcd_dispaly_string8,strlen(lcd_dispaly_string8),0x80); 
            system_lcd1602_string_dispaly(lcd_dispaly_string9,strlen(lcd_dispaly_string9),0xC0);

            break;

        case DISPLAY_WAIT :    // �ȴ�
            
            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(Power_Up_String2,strlen(Power_Up_String2),0x80); 

            break;

        case DISPLAY_UNLOCK:    // unlock

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(payg_credit_unlocked1,strlen(payg_credit_unlocked1),0x80); 
            system_lcd1602_string_dispaly(payg_credit_unlocked2,strlen(payg_credit_unlocked2),0xC0);

            break;

        case DISPLAY_TOKEN_DISABLE:    // disable

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(payg_credit_disable,strlen(payg_credit_disable),0x80); 

            system_led_blink_green_one();

            system_led_blink_green_one();

            break;

        case DISPLAY_TOKEN_ID:    // ��id

            system_led_blink_green_one();

            system_led_blink_green_one();
            
            
            sprintf(temp_disable,"ID: %d", _this_pointer._this_product_id_pointer->unit_number_code);

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(temp_disable, strlen(temp_disable),0x80); 

            delay1ms(5000);
            
            break;

        case DISPLAY_TOKEN_COUNT:    // ��count

            system_led_blink_green_one();

            system_led_blink_green_one();

            sprintf(temp_disable,"COUNT: %d", _this_pointer._this_product_time_pointer->token_count);

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(temp_disable, strlen(temp_disable),0x80); 

            delay1ms(5000);

            break;
            
        case DISPLAY_TOKEN_COUNT_CLOSE:    // ���count

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(payg_credit_count_close, strlen(payg_credit_count_close),0x80); 

            system_led_blink_green_one();

            system_led_blink_green_one();
        
            break;

        case DISPLAY_TOKEN_EFFICACIOUS:

            system_led_blink_green_one();

            system_led_blink_green_one();

            break;

        case DISPLAY_KEYBOARD_LOCK:

            system_lcd1602_display_clear_dispaly();	
            system_lcd1602_string_dispaly(lcd_disable_lock1, strlen(lcd_disable_lock1),0x80);
            system_lcd1602_string_dispaly(lcd_disable_lock2, strlen(lcd_disable_lock2),0xC0);

            system_led_blink_red_one();

            system_led_blink_red_one();

            delay1ms(2000);

            break;

        default:
            break;
    }
}


/********************************************************************
 ** @brief  �������������й�����ʾ������
 ** @param  ��
 ** @retval ��
********************************************************************/
void open_paygo_display_running_manage(void)
{
    if(display_state_manage.display_state_token_enum != DISPLAY_FALSE)
    {
        open_paygo_display_startup(display_state_manage.display_state_token_enum);

        display_state_manage.display_state_token_enum = DISPLAY_FALSE;
    }

    // ���´���
    if(display_state_manage.display_state_flag)
    {
        open_paygo_display_startup(display_state_manage.display_state_enum);

        display_state_manage.display_state_flag = 0;
    }

    // ���»ʱ��
    if(display_state_manage.display_state_activity)
    {
        char temp_str[5];

        if(display_state_manage.display_day >=100)
        {
            sprintf(temp_str,"%d",display_state_manage.display_day);
        }
        else if(display_state_manage.display_day >=10)
        {
            sprintf(temp_str,"0%d",display_state_manage.display_day);
        }
        else
        {
            sprintf(temp_str,"00%d",display_state_manage.display_day);
        }
        system_lcd1602_string_dispaly(temp_str,strlen(temp_str),0xC0);


        if(display_state_manage.display_hour >=10)
        {
            sprintf(temp_str,"%d",display_state_manage.display_hour);
        }
        else
        {
            sprintf(temp_str,"0%d",display_state_manage.display_hour);
        }
        system_lcd1602_string_dispaly(temp_str,strlen(temp_str),0xC9);

        display_state_manage.display_state_activity = 0;
    }

    //RTC �� ERROR ״̬����
    op_manage_rtc_error_manage();   

    // ��������
    open_paygo_display_light_time_manage();

    // LED�ƹ���
    // open_paygo_display_led_activity_blink_manage();

}


/********************************************************************
 ** @brief  ��ʾ�ڼ�ʱ�����У�RTC������ʾ
 ** @param  ��
 ** @retval ��
********************************************************************/
uint8_t rtc_error_display_count = 0;
void open_paygo_display_rtc_error_run_manage(void)
{
    rtc_error_display_count++;

    switch (rtc_error_display_count)
    {
    case 1 :
        system_lcd1602_string_dispaly(Rtc_Error_String1,strlen(Rtc_Error_String1),0x80);
        break;
    
    case 2 :
        system_lcd1602_string_dispaly(Rtc_Error_String2,strlen(Rtc_Error_String2),0x80);
        break;

    case 3 :
        system_lcd1602_string_dispaly(Rtc_Error_String3,strlen(Rtc_Error_String3),0x80);
        break;

    default:
        break;
    }

    if(rtc_error_display_count == 3)
    {
        
        rtc_error_display_count = 0;
    }

}


/********************************************************************
 ** @brief  LCD����������
 ** @param  ��
 ** @retval ��
********************************************************************/
void open_paygo_display_light_time_manage(void)
{
	if(__sleep_mode.device_power_on_state != FALSE)
	{
		if(irq_time.lcd_light_up_time == UI_DISPLAY_TIMEE)
		{
			System_LCD_Light_Disable();

			irq_time.lcd_light_up_time += 2;
		}
		else if(irq_time.lcd_light_up_time < UI_DISPLAY_TIMEE )
		{
			System_LCD_Light_Enable();
		}
	}
	else
	{
		System_LCD_Light_Disable();
		irq_time.lcd_light_up_time = UI_DISPLAY_TIMEE + 2;
	}
}


/********************************************************************
 ** @brief  led�Ƽ�ʱ��˸����
 ** @param  ��
 ** @retval ��
********************************************************************/
void open_paygo_display_led_activity_blink_manage(void)
{
	if(!_system_paygo_state.paygo_unlock && _system_paygo_state.paygo_enabled \
            && __sleep_mode.device_power_on_state == TRUE && __sleep_mode.run_state == _SYSTEM_RUN)
	{
		if(irq_time.lptimer_led_1s_flag && rtc_state.rtc_run_state != RTC_ERROR)
		{
			if(display_state_manage.display_day >= 2)	// �����48Сʱ
			{
				if(Gpio_ReadOutputIO(LED_GREEN_PORT, GREEN_PIN))
				{
					RED_SATE_CLOSE();
					GREEN_SATE_CLOSE();
				}
				else
				{
					GREEN_SATE_OPEN();
				}
			}
			else if(display_state_manage.display_day < 2)	// �С��48Сʱ
			{
				if(Gpio_ReadOutputIO(LED_RED_PORT, LED_RED_PIN))
				{
					RED_SATE_CLOSE();
				}
				else
				{
					GREEN_SATE_CLOSE();
					RED_SATE_OPEN();
				}
			}

			irq_time.lptimer_led_1s_flag = 0;
		}
		else if(rtc_state.rtc_run_state == RTC_ERROR)
		{
			GREEN_SATE_OPEN();

			RED_SATE_OPEN();
		}
	}
	else if(_system_paygo_state.paygo_unlock)	// unlock״̬
	{
		RED_SATE_CLOSE();

		GREEN_SATE_OPEN();
	}
	
}


/********************************************************************
 ** @brief  ����Token����Ļ��ʾ
 ** @param  ��
 ** @retval ��
********************************************************************/
void open_paygo_token_data_display(char temp_last_key,uint8_t temp_num)
{
    
    switch (temp_last_key)
    {
        case '*':
            System_LCD_Light_Enable();
            system_lcd1602_display_clear_dispaly();
            system_lcd1602_token_data_display(&temp_last_key, &temp_num, 0x2A);

            break;
        case '#':
            system_lcd1602_token_data_display(&temp_last_key, &temp_num, 0x23);

            break;
        default:
            system_lcd1602_token_data_display(&temp_last_key, &temp_num, 0x00);

            break;
    }
}



