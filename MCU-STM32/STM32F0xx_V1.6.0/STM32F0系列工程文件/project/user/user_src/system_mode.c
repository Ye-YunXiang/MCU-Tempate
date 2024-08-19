#include "system_mode.h"

struct SLEEP_MODE_STRUCT __sleep_mode = {1, _SYSTEM_BOOT_SCREEN, 0, 0, 0};

// ϵͳ���������ʼ��
void system_mode_peripherals_init(void)
{			
    //swd as gpio
    //Sysctrl_SetFunc(SysctrlSWDUseIOEn, TRUE);

    op_manage_load_activation_variables();              // �����ڴ漤�����
            
    rtc_alarm_irq_close();                              // �ر�RTC����
	
    system_run_lptimer_configuration();                 // �͹��Ķ�ʱ����ʼ��

    system_timer0_open();                               // ��timer0�ж�

	//system_lpuart0_port_init();                         // lpuart���ų�ʼ��

	system_lpuart0_config_init();                       // lpuart���ܳ�ʼ�� 19200

    system_gsm_port_init();                             // GSM���ų�ʼ��

	system_adc_bat_control_port_init();					// �����ADC�������ų�ʼ��

	system_adc_port_init();								// ADC �����˿ڳ�ʼ��

	system_adc_run_init();								// �����ADC��ʼ��

    system_refrigerator_control_init();                 // ��ʼ������ʹ������
	
	system_lcd1602_display_port_init();	                // LCD1602���ų�ʼ��
	
    system_lcd1602_dispaly_config_init();				// LCD��ʾ��ʼ��
	
	system_matrix_key_scan_init();                      // ���󰴼����ų�ʼ��

  	system_led_init();                                  // LED��ʾ���ų�ʼ��
	
	open_paygo_display_struct();						// ��ʼ��ʾ����ṹ��

    system_mode_ID_uart_select_open();                  // id��¼���Ƿ��
	
    __NOP();
}

// id��¼���Ƿ��
void system_mode_ID_uart_select_open(void)
{
    // û����¼ID��������ʱ���ж�UART�Ƿ����
    if(_system_paygo_state.paygo_id != TRUE)
    {
        system_uart0_port_init();                           // uart���ų�ʼ��
		
        system_uart0_config_init();                         // uart���ܳ�ʼ��   9600

        TIMER_5ms_ON();     // ��5ms��ʱ�� Ϊ���Ŀ���
    }
}


// ���Ѻ�m25�ϱ���ʼ��
void system_mode_rouse_reported_init(void)
{	
    op_manage_load_activation_variables();              // �����ڴ漤�����

    rtc_alarm_irq_close();                               // �ر�RTC����

    system_run_lptimer_configuration();                 // �͹��Ķ�ʱ����ʼ��

    system_timer0_open();                               // ��timer0�ж�

	system_lpuart0_port_init();                         // lpuart���ų�ʼ��

	system_lpuart0_config_init();                       // lpuart���ܳ�ʼ�� 19200

    system_gsm_port_init();                             // GSM���ų�ʼ��

	system_adc_bat_control_port_init();					// �����ADC�������ų�ʼ��

	system_adc_port_init();								// ADC �����˿ڳ�ʼ��

	system_adc_run_init();								// �����ADC��ʼ��
	
	open_paygo_display_struct();						// ��ʼ��ʾ����ṹ��
	
    __NOP();
}


// ϵͳ���ѻ�˯������,TRUEΪ�������̣�FALSEΪȫ��״̬
void system_mode_wakeUP_or_sleep_prrocess(_Bool state)
{
    if(state == TRUE)
    {
        if(__sleep_mode.device_power_on_state != FALSE)
        {
            __sleep_mode.run_state++;
        }
        else
        {
            __sleep_mode.run_state = _SYSTEM_SLEEP;
        }
    }
    else
    {
        // ��ƽ״̬Ϊ��������������ģʽΪSLEEP
        if(__sleep_mode.device_power_on_state == TRUE \
			&&(__sleep_mode.run_state == _SYSTEM_SLEEP || __sleep_mode.run_state == _SYSTEM_SLEEP_ROUSE))
		{
			__sleep_mode.run_state = _SYSTEM_BOOT_SCREEN;
		}
        // ��ƽ״̬Ϊ�ػ�,���Ҽ������źŴ�
		else if( __sleep_mode.device_power_on_state == FALSE && __sleep_mode.state_flip_5v == TRUE)
		{
			__sleep_mode.run_state = _SYSTEM_SLEEP;
		}
    }
}


//  ������ʼ����������
void system_mode_IO_init(void)
{
	 ///< ��GPIO����ʱ���ſ�
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
    ///< ����Ϊ�˿����루��LED�˿��⣩
    M0P_GPIO->PADIR = 0XFFFF;
    M0P_GPIO->PBDIR = 0XFFFF;
    M0P_GPIO->PCDIR = 0XFFFF;
    M0P_GPIO->PDDIR = 0XFFFF;
    M0P_GPIO->PEDIR = 0XFFFF;
    M0P_GPIO->PFDIR = 0XFFFF;

    ///< ������������ 5V_ADC �˿����⣩
    M0P_GPIO->PAPD = 0xFEFF;
    M0P_GPIO->PBPD = 0xFFFF;
    M0P_GPIO->PCPD = 0xFFFF;
    M0P_GPIO->PDPD = 0xFFFF;
    M0P_GPIO->PEPD = 0xFFFF;
    M0P_GPIO->PFPD = 0xFFFF;
}


//  ����˯����������
void system_mode_sleep_IO_init(void)
{
	///< ��GPIO����ʱ���ſ�
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    ///< ����Ϊ�˿�����
    M0P_GPIO->PADIR = 0XFFFF;
    M0P_GPIO->PBDIR = 0XFFFF;
    M0P_GPIO->PCDIR = 0XFFFF;
    M0P_GPIO->PDDIR = 0XFFFF;
    M0P_GPIO->PEDIR = 0XFFFF;
    M0P_GPIO->PFDIR = 0XFFFF;

    ///< ����������A08,C14,C15��
    M0P_GPIO->PAPD = 0xFEFF;
    M0P_GPIO->PBPD = 0xFFFF;
    M0P_GPIO->PCPD = 0xFFFF;
    M0P_GPIO->PDPD = 0xFFFF;
    M0P_GPIO->PEPD = 0xFFFF;
    M0P_GPIO->PFPD = 0xFFFF;
}


// �ػ�����
void system_mode_power_off_set(void)
{
    System_LCD_Light_Disable();		// �ر���ʾ
    RED_SATE_CLOSE();				// �رպ��
    GREEN_SATE_CLOSE();				// �ر��̵�
    ADC_TAMPER_CLOSE_TEST_IRQ();			// �ر��ƽ��жϼ��

    system_lcd1602_display_clear_dispaly();		// �����Ļ
    
    ADC_TAMPER_CLOSE_TEST_IRQ();       //�ر����к�Ǽ��

    ADC_OPEN_5V_ON_TEST_IRQ();		// �򿪿������
}

// �жϽ������˯��
void system_mode_judge_depp_sleep_open(void)
{
    // �������˯��
    //if(__sleep_mode.state_flip_5v == TRUE && ADV_5V_GET_STATE() == FALSE)
	if(ADV_5V_GET_STATE() == FALSE)
    {
        __sleep_mode.state_flip_5v = FALSE;
		
        // �ر�ADC���������ʱ��
        system_adc_sleep();		
		
		// �ر�LPuart
		if(_system_paygo_state.paygo_id != TRUE)
		{
			system_uart0_config_off();
		}
		
		// �ر�LPuart�˿�
		system_lpuart0_port_close();
        // �ر�LPuart0����
        system_lpuart0_config_close();
        // �ر�timer0
        system_timer0_close();
        // �ȹ�LPTimeʱ��
        system_run_lptimer_close();     

		system_mode_IO_init();
		
		// ��RTC����
        rtc_alarm_irq_open();
		
		delay1ms(20);
	
		Lpm_GotoSleep(FALSE);		
    }
}
