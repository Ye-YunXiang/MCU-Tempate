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
* CPU:HC32L170JATA  FLASH:64K,128页每页512字节
******************************************************************************/

/*-------------------------------------------------------------------------------
 * 本软件为的是为KB_GSM_DEMO_ v1.0写的 截至 2023-12-12 停止更新。
 * 除了蓝牙没写，GSM简要的实现了发送数据，其他功能都是完善的。
--------------------------------------------------------------------------------*/


#include "main_struct_typedef.h"
#include "main_file_define.h"


int credit_data=0;

int32_t main(void)
{	
	delay1ms(1000);
	
	system_clock_configuration_init();                  // 时钟初始化
	
	system_timer0_configuration();                      // 定时器初始化

	system_run_lptimer_configuration();                 // 低功耗定时器初始化

	system_internal_rtc_init();		                    // RTC初始化
	
	system_uart0_port_init();                           // uart引脚初始化

	system_uart0_config_init();                         // uart功能初始化

    system_lpuart0_port_init();                         // lpuart引脚初始化

	system_lpuart0_config_init();                       // lpuart功能初始化

//	system_lcd1602_display_port_init();	                // LCD1602引脚初始化
//	
//	system_lcd1602_dispaly_config_init();               // 显示初始化
//	
//	system_matrix_key_scan_init();                      // 矩阵按键引脚初始化

//    system_led_init();                                  // LED显示引脚初始化

//    system_gsm_port_init();                             // GSM引脚初始化

//    op_manage_load_activation_variables();              // 加载内存激活变量

//    op_manage_rouse_calibration_time_and_display();     // 验证RTC，以及开机加载

//    //open_paygo_display_startup(DISPLAY_NO_ID);
//	
//    __NOP();
//	
//	uint32_t time_RTC;

	while(1)
	{
//        system_console_run_manage();    //运行管理

//        open_paygo_display_running_manage();    // 各种显示管理，如亮屏、亮灯、屏幕显示

//	    // __NOP();

//        // // 低功耗模式管理
//        system_gsm_mqtt_manage();
//		
//		starting_run_examine_get_sec(&time_RTC, FALSE);

    }
}


/*******************************************************************************
 * LPTIMER0中断服务函数：用于各种标志位
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
        irq_time.lptimer_test_flag = 1;     // 10分钟检测标志

        if( irq_time.lptimer_time_count >= irq_time.irq_30_min \
                    && _system_paygo_state.paygo_enabled == TRUE)
        {
            irq_time.lptimer_flash_flag = 1;    // 30分钟存储标志
        }
    }

    // GSM发送信息时间标志
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
    
    Lptim_ClrItStatus(M0P_LPTIMER0);//清除LPTIMER0的中断标志位      
}


/*******************************************************************************
 * TIM0中断服务函数：用给出计时250ms标志位
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
        else if(mqtt_state.mqtt_at_type == MQTT_TYPE_WAIT_RX)  //MQTT超时回复时间
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

        // 重启倒计时
        if(gsm_power_restart_time != 0)
        {
            gsm_power_restart_time--;
            if(gsm_power_restart_time == 1)
            {
                mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_RESTART;
                gsm_power_restart_time = 0;
            }
        }

        Bt_ClearIntFlag(TIM0,BtUevIrq); //中断标志清零
    }
}


/********************************************************************
 ** \brief  重新给uart中断接收标志位初始化
 ** @param  无
 ** \retval 无
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
 * UART0 中断服务函数
 ******************************************************************************/
void Uart0_IRQHandler(void)
{
	Uart_ClrStatus(M0P_UART0, UartRC);        //清中断状态位
	UartReceive_ID_Data = Uart_ReceiveData(M0P_UART0);   //接收数据字节
	
	if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type0)
	{
		if(uart_tx_t.Uart_Receive_SN_Bits < SNIDRECEIVEBITLENTH)	// 接收的序列号bit长度小于 9
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

		openpaygo_id_receive_successfully = FALSE;		// ID 还未接收成功标志位。
	}
	else if(uart_tx_t.Uart_Receive_Data_Type == Uart_Receive_Type1)
	{
		if(uart_tx_t.Uart_Receive_SC_Bits < SCIDRECEIVEBITLENTH)			// 接收的序列号bit长度小于 10
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
		if(uart_tx_t.Uart_Receive_SU_Bits < SUIDRECEIVEBITLENTH)		// 接收的序列号bit长度小于 10
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
		if(uart_tx_t.Uart_Receive_SK_Bits < SKIDRECEIVEBITLENTH)		// 接收的序列号bit长度小于 10
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
		if(uart_tx_t.Uart_Receive_PK_Bits < PKIDRECEIVEBITLENTH)		// 接收的序列号bit长度小于 10
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
		if(uart_tx_t.Uart_Receive_DS_Bits < DSIDRECEIVEBITLENTH)  	// 接收的序列号bit长度小于 32
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

    
    if(Uart_GetStatus(M0P_UART0, UartTC))         //UART1数据发送
    {
        Uart_ClrStatus(M0P_UART0, UartTC);        //清中断状态位
    }
}


/*******************************************************************************
 * LPUART0 中断服务函数
 ******************************************************************************/

///<LPUART0 中断服务函数
void LpUart0_IRQHandler(void)
{	
    uint8_t ucCh;
	
//	 if(LPUart_GetStatus(M0P_LPUART1, LPUartTC))
//    {
//        LPUart_ClrStatus(M0P_LPUART1, LPUartTC);   ///<清发送中断请求
//        
////        LPUart_DisableIrq(M0P_LPUART1,LPUartTxIrq);///<禁止发送中断
////        LPUart_EnableIrq(M0P_LPUART1,LPUartRxIrq); ///<使能接收中断
//    }

	
	irq_time.timer_5_ms_count = 0;
	
//        u8RxData = LPUart_ReceiveData(M0P_LPUART0);///读取数据
	ucCh = LPUart_ReceiveData(M0P_LPUART0);  

	if ( strM25_Fram_Record.InfBit.FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //预留1个字节写结束符
	{	
		strM25_Fram_Record.Data_RX_BUF [ strM25_Fram_Record.InfBit.FramLength ++ ]  = ucCh;
	}


//        LPUart_DisableIrq(M0P_LPUART0,LPUartRxIrq);///<禁止接收中断
//        LPUart_EnableIrq(M0P_LPUART0,LPUartTxIrq); ///<使能发送中断

	
	LPUart_ClrStatus(M0P_LPUART0, LPUartRC);   ///<清接收中断请求
	
	
}
