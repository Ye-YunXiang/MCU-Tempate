/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	main.c 
* Description:	
* 
* Author:		Poetry Cloud
* Date: 	 	2023/09/15
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/09/15, Cloud create this file
* CPU:HC32L170JATA  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

/*-------------------------------------------------------------------------------
 * �����Ϊ����ΪKB_GSM_DEMO_ v1.0д�� ���� 2023-12-12 ֹͣ���¡�
 * ��������ûд��GSM��Ҫ��ʵ���˷������ݣ��������ܶ������Ƶġ�
--------------------------------------------------------------------------------*/


#include "main_struct_typedef.h"
#include "main_file_define.h"


int credit_data=0;

int32_t main(void)
{	
	delay1ms(1000);
	
	system_clock_configuration_init();                  // ʱ�ӳ�ʼ��
	
	system_timer0_configuration();                      // ��ʱ����ʼ��

	system_run_lptimer_configuration();                 // �͹��Ķ�ʱ����ʼ��

	system_internal_rtc_init();		                    // RTC��ʼ��
	
	system_uart0_port_init();                           // uart���ų�ʼ��

	system_uart0_config_init();                         // uart���ܳ�ʼ��

    system_lpuart0_port_init();                         // lpuart���ų�ʼ��

	system_lpuart0_config_init();                       // lpuart���ܳ�ʼ��

//	system_lcd1602_display_port_init();	                // LCD1602���ų�ʼ��
//	
//	system_lcd1602_dispaly_config_init();               // ��ʾ��ʼ��
//	
//	system_matrix_key_scan_init();                      // ���󰴼����ų�ʼ��

//    system_led_init();                                  // LED��ʾ���ų�ʼ��

//    system_gsm_port_init();                             // GSM���ų�ʼ��

//    op_manage_load_activation_variables();              // �����ڴ漤�����

//    op_manage_rouse_calibration_time_and_display();     // ��֤RTC���Լ���������

//    //open_paygo_display_startup(DISPLAY_NO_ID);
//	
//    __NOP();
//	
//	uint32_t time_RTC;

	while(1)
	{
//        system_console_run_manage();    //���й���

//        open_paygo_display_running_manage();    // ������ʾ���������������ơ���Ļ��ʾ

//	    // __NOP();

//        // // �͹���ģʽ����
//        system_gsm_mqtt_manage();
//		
//		starting_run_examine_get_sec(&time_RTC, FALSE);

    }
}


/*******************************************************************************
 * LPTIMER0�жϷ����������ڸ��ֱ�־λ
 ******************************************************************************/
void LpTim0_IRQHandler(void)
{
    // led special 1s flag bit
    irq_time.lptimer_led_1s_flag = 1;

    // global availability 1s flag bit
    irq_time.lptimer_1s_flag = 1;
    
    // timer synchronizes the time of the RTC
    irq_time.lptimer_time_count ++;

    // proof time AND storage time flag bit
    if( irq_time.lptimer_time_count >= irq_time.irq_10_min)
    {
        irq_time.lptimer_test_flag = 1;     // 10���Ӽ���־

        if( irq_time.lptimer_time_count >= irq_time.irq_30_min \
                    && _system_paygo_state.paygo_enabled == TRUE)
        {
            irq_time.lptimer_flash_flag = 1;    // 30���Ӵ洢��־
        }
    }

    // GSM������Ϣʱ���־
    if(irq_time.lptimer_time_count == gsm_power_sending_time_count)
    {
        mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_ON;
    }

    // LCD light screen timing
    if(irq_time.lcd_light_up_time < UI_DISPLAY_TIMEE)
    {
        irq_time.lcd_light_up_time ++;
    }

    // 2s flag bit, mainly for lock
    if(irq_time.lptimer_2s_flag < 2)
    {
        irq_time.lptimer_2s_flag ++;
    }
    
    Lptim_ClrItStatus(M0P_LPTIMER0);//���LPTIMER0���жϱ�־λ      
}


/*******************************************************************************
 * TIM0�жϷ��������ø�����ʱ250ms��־λ
 ******************************************************************************/
void Tim0_IRQHandler(void)
{
    if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq))
    {
        if(irq_time.timer_5_ms_count <= 100)
        {
            irq_time.timer_5_ms_count++;
        }

        if(irq_time.timer_5_ms_count == 50)
        {
            irq_time.timer_250ms_flag = 1;
        }
        else
        {
            irq_time.timer_250ms_flag = 0;
        }


        if(irq_time.timer_5_ms_count == 2)
        {
            strM25_Fram_Record.InfBit.FramFinishFlag = 1;
            strM25_Fram_Record .Data_RX_BUF [ strM25_Fram_Record .InfBit .FramLength ] = '\0';
        }
        else if(mqtt_state.mqtt_at_type == MQTT_TYPE_WAIT_RX)  //MQTT��ʱ�ظ�ʱ��
        {
            mqtt_state.mqtt_wait_receive_count++;

            if(mqtt_state.mqtt_wait_receive_count == 12000)
            {
                strM25_Fram_Record.InfBit.FramFinishFlag = 1;
                strM25_Fram_Record .Data_RX_BUF [ strM25_Fram_Record .InfBit .FramLength ] = '\0';
            }
        }

        if(mqtt_state.mqtt_task_state_count_down != 0)
        {
            mqtt_state.mqtt_task_state_count_down--;
        }

        gsm_power_restart_time = GSM_POWER_RESTART_TIME;

        // ��������ʱ
        if(gsm_power_restart_time != 0)
        {
            gsm_power_restart_time--;
            if(gsm_power_restart_time == 1)
            {
                mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_RESTART;
                gsm_power_restart_time = 0;
            }
        }

        Bt_ClearIntFlag(TIM0,BtUevIrq); //�жϱ�־����
    }
}


/********************************************************************
 ** \brief  ���¸�uart�жϽ��ձ�־λ��ʼ��
 ** @param  ��
 ** \retval ��
********************************************************************/
static void uart0_receive_flag_close(void)
{
	uart_tx_t.Uart_Receive_SN_Bits = UARTRECEIVEBITSCLEARNULL;
    uart_tx_t.Uart_Receive_SC_Bits = UARTRECEIVEBITSCLEARNULL;
    uart_tx_t.Uart_Receive_SU_Bits = UARTRECEIVEBITSCLEARNULL;
    uart_tx_t.Uart_Receive_SK_Bits = UARTRECEIVEBITSCLEARNULL;
    uart_tx_t.Uart_Receive_PK_Bits = UARTRECEIVEBITSCLEARNULL;
    uart_tx_t.Uart_Receive_DS_Bits = UARTRECEIVEBITSCLEARNULL;

    uart_tx_t.Uart_Receive_Data_Type = Uart_Receive_Type0;
}

/*******************************************************************************
 * UART0 �жϷ�����
 ******************************************************************************/
void Uart0_IRQHandler(void)
{
	Uart_ClrStatus(M0P_UART0, UartRC);        //���ж�״̬λ
	UartReceive_ID_Data = Uart_ReceiveData(M0P_UART0);   //���������ֽ�
	
	if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type0)
	{
		if(uart_tx_t.Uart_Receive_SN_Bits < SNIDRECEIVEBITLENTH)	// ���յ����к�bit����С�� 9
		{
			openpaygo_id_t.SNID_Receive_Temp[uart_tx_t.Uart_Receive_SN_Bits] = UartReceive_ID_Data;
		   
			switch (UartReceive_ID_Data)
			{
				case 0x3B:          //receive ";"
					uart_tx_t.Uart_Receive_Data_Type = Uart_Receive_Type1;
					break;
				case 0x40:          //receive "@"
					uart0_receive_flag_close();
					break;
				default:
					uart_tx_t.Uart_Receive_SN_Bits++;
					break;
			}
			
		}

		openpaygo_id_receive_successfully = FALSE;		// ID ��δ���ճɹ���־λ��
	}
	else if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type1)
	{
		if(uart_tx_t.Uart_Receive_SC_Bits < SCIDRECEIVEBITLENTH)			// ���յ����к�bit����С�� 10
		{
			if(UartReceive_ID_Data != 0x3B)
			{
				openpaygo_id_t.SCID_Receive_Temp[uart_tx_t.Uart_Receive_SC_Bits] = UartReceive_ID_Data;
			}

			switch (UartReceive_ID_Data)
			{
				case 0x3B:          //receive ";"
					uart_tx_t.Uart_Receive_Data_Type = Uart_Receive_Type2;
					break;
				case 0x40:          //receive "@"
					uart0_receive_flag_close();
					break;
				default:
					uart_tx_t.Uart_Receive_SC_Bits++;
					break;
			}
		}

		openpaygo_id_receive_successfully = FALSE;
	}
	else if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type2)
	{
		if(uart_tx_t.Uart_Receive_SU_Bits < SUIDRECEIVEBITLENTH)		// ���յ����к�bit����С�� 10
		{ 
			if(UartReceive_ID_Data != 0x3B)
			{
				openpaygo_id_t.SUID_Receive_Temp[uart_tx_t.Uart_Receive_SU_Bits] = UartReceive_ID_Data;
			}

			switch (UartReceive_ID_Data)
			{
				case 0x3B:          //receive ";"
					uart_tx_t.Uart_Receive_Data_Type = Uart_Receive_Type3;
					break;
				case 0x40:          //receive "@"
					uart0_receive_flag_close();
					break;
				default:
					uart_tx_t.Uart_Receive_SU_Bits++;
					break;
			}
		}

		openpaygo_id_receive_successfully = FALSE;
	}
	else if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type3)
	{
		if(uart_tx_t.Uart_Receive_SK_Bits < SKIDRECEIVEBITLENTH)		// ���յ����к�bit����С�� 10
		{ 
			if(UartReceive_ID_Data != 0x3B)
			{
				openpaygo_id_t.SKID_Receive_Temp[uart_tx_t.Uart_Receive_SK_Bits] = UartReceive_ID_Data;
			}

			switch (UartReceive_ID_Data)
			{
				case 0x3B:          //receive ";"
					uart_tx_t.Uart_Receive_Data_Type = Uart_Receive_Type4;
					break;
				case 0x40:          //receive "@"
					uart0_receive_flag_close();
					break;
				default:
					uart_tx_t.Uart_Receive_SK_Bits++;
					break;
			}
		}

		openpaygo_id_receive_successfully = FALSE;
	}
	else if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type4)
	{
		if(uart_tx_t.Uart_Receive_PK_Bits < PKIDRECEIVEBITLENTH)		// ���յ����к�bit����С�� 10
		{ 
			if(UartReceive_ID_Data != 0x3B)
			{
				openpaygo_id_t.PKID_Receive_Temp[uart_tx_t.Uart_Receive_PK_Bits] = UartReceive_ID_Data;
			}

			switch (UartReceive_ID_Data)
			{
				case 0x3B:          //receive ";"
					uart_tx_t.Uart_Receive_Data_Type = Uart_Receive_Type5;
					break;
				case 0x40:          //receive "@"
					uart0_receive_flag_close();
					break;
				default:
					uart_tx_t.Uart_Receive_PK_Bits++;
					break;
			}
		}

		openpaygo_id_receive_successfully = FALSE;
	}
	else if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type5)
	{
		if(uart_tx_t.Uart_Receive_DS_Bits < DSIDRECEIVEBITLENTH)  	// ���յ����к�bit����С�� 32
		{
			if(UartReceive_ID_Data != 0x3B)
			{
				openpaygo_id_t.DSID_Receive_Temp[uart_tx_t.Uart_Receive_DS_Bits] = UartReceive_ID_Data;
			}
		   
			if(uart_tx_t.Uart_Receive_DS_Bits >= (DSIDRECEIVEBITLENTH - 1))
			{
				openpaygo_id_receive_successfully = TRUE;

				// uart_tx_t.Uart_Receive_Data_Type = Uart_Receive_Type0;
			}
			else 
			{
				uart_tx_t.Uart_Receive_DS_Bits++;
			}
		}
	}
	else 
	{;}
	
//		Uart_SendDataIt(M0P_UART0, UartReceive_ID_Data);

    
    if(Uart_GetStatus(M0P_UART0, UartTC))         //UART1���ݷ���
    {
        Uart_ClrStatus(M0P_UART0, UartTC);        //���ж�״̬λ
    }
}


/*******************************************************************************
 * LPUART0 �жϷ�����
 ******************************************************************************/

///<LPUART0 �жϷ�����
void LpUart0_IRQHandler(void)
{	
    uint8_t ucCh;
	
//	 if(LPUart_GetStatus(M0P_LPUART1, LPUartTC))
//    {
//        LPUart_ClrStatus(M0P_LPUART1, LPUartTC);   ///<�巢���ж�����
//        
////        LPUart_DisableIrq(M0P_LPUART1,LPUartTxIrq);///<��ֹ�����ж�
////        LPUart_EnableIrq(M0P_LPUART1,LPUartRxIrq); ///<ʹ�ܽ����ж�
//    }

	
	irq_time.timer_5_ms_count = 0;
	
//        u8RxData = LPUart_ReceiveData(M0P_LPUART0);///��ȡ����
	ucCh = LPUart_ReceiveData(M0P_LPUART0);  

	if ( strM25_Fram_Record.InfBit.FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //Ԥ��1���ֽ�д������
	{	
		strM25_Fram_Record.Data_RX_BUF [ strM25_Fram_Record.InfBit.FramLength ++ ]  = ucCh;
	}


//        LPUart_DisableIrq(M0P_LPUART0,LPUartRxIrq);///<��ֹ�����ж�
//        LPUart_EnableIrq(M0P_LPUART0,LPUartTxIrq); ///<ʹ�ܷ����ж�

	
	LPUart_ClrStatus(M0P_LPUART0, LPUartRC);   ///<������ж�����
	
	
}
