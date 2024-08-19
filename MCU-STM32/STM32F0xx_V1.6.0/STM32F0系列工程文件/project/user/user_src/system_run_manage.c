/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_run_manage.c
* Description: 	Managing system running status
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#include "system_run_manage.h"
#include "system_ota.h"

// �жϺͽ���UART��ID�Ƿ��гɹ�
_Bool judge_uart_receive_id(void * data_t)
{
	char log[50] = {0};
	//-------------------------------------------------------------------------------------
	//- У����� 
	//-------------------------------------------------------------------------------------
	int i = 0;
	uint16_t CRC = 0;
	char array_crc[2] = {0};
	// �����0��ʼ������Ҫ��4λCRCҪ����1
	char * source_data = &UART0_Fram_Record.Data_RX_BUF[UART0_Fram_Record.InfBit.FramLength - sizeof(array_crc)*2];
	// CRC��ȡ������
    array_to_hex_bit(&array_crc[0], sizeof(array_crc), source_data);
	// �õ�CRC
	for(i = 0; i < sizeof(array_crc); i++)
	{
		CRC = (CRC * 0x100) + (array_crc[i] & 0xff);
	}
	uint16_t CRC_File = CRC_16_XMODEM(0,0x1021,0,false,false, (uint8_t *)&UART0_Fram_Record.Data_RX_BUF[0], (UART0_Fram_Record.InfBit.FramLength - CRCID));
	
	sprintf(log,"CRC: %X    CRC_File: %X ", CRC, CRC_File);
	system_ble_uart0_send_string(&log[0], strlen(log));
	
	// ���У���벻�ԣ�ֱ���˳�
	if(CRC != CRC_File)
	{
		return FALSE;
	}


	//-------------------------------------------------------------------------------------
	//- ��ʼ���� 
	//-------------------------------------------------------------------------------------
	struct OpenpayGo_ID * op_id_t = (struct OpenpayGo_ID *)data_t;
	uint8_t Uart_Receive_Type = Uart_Receive_Type0;
	uint8_t Uart_Receive_Count = 0;		// ���ռ���
	uint8_t Uart_Receive_All_Count = 0;	// һ�����ռ���

	// ��������
	while(Uart_Receive_Type != Uart_Receive_Type_Succeed \
			&& Uart_Receive_All_Count <= UART0_Fram_Record.InfBit.FramLength \
			&& Uart_Receive_Type != Uart_Receive_Type_Failure)
	{
		if(Uart_Receive_Type == Uart_Receive_Type0)
		{
			if(Uart_Receive_Count <= SNID)	// ���յ����к�bit����С�� 9
			{
				// op_id_t.SNID_Receive_Temp[uart_tx_t.Uart_Receive_SN_Bits] = UartReceive_ID_Data;
			
				switch ( UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count] )
				{
					case 0x3B:          //receive ";"
						Uart_Receive_Type = Uart_Receive_Type1;
						Uart_Receive_Count = 0;
						break;
					default:
						Uart_Receive_Count++;
						break;
				}
			}
			else
			{
				Uart_Receive_Type = Uart_Receive_Type_Failure;
			}
		}
		else if(Uart_Receive_Type == Uart_Receive_Type1)
		{
			if(Uart_Receive_Count <= SCID)			// ���յ����к�bit����С�� 10
			{
				if(UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count] != 0x3B)
				{
					op_id_t->SCID_Receive_Temp[Uart_Receive_Count] = UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count];
				}

				switch (UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count])
				{
					case 0x3B:          //receive ";"
						Uart_Receive_Type = Uart_Receive_Type2;
						Uart_Receive_Count = 0;
						break;
					default:
						Uart_Receive_Count++;
						break;
				}
			}
			else
			{
				Uart_Receive_Type = Uart_Receive_Type_Failure;
			}
		}
		else if(Uart_Receive_Type == Uart_Receive_Type2)
		{
			if(Uart_Receive_Count <= SUID)		// ���յ����к�bit����С�� 10
			{ 
				if(UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count] != 0x3B)
				{
					op_id_t->SUID_Receive_Temp[Uart_Receive_Count] = UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count];
				}

				switch (UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count])
				{
					case 0x3B:          //receive ";"
						Uart_Receive_Type = Uart_Receive_Type3;
						Uart_Receive_Count = 0;
						break;
					default:
						Uart_Receive_Count++;
						break;
				}
			}
			else
			{
				Uart_Receive_Type = Uart_Receive_Type_Failure;
			}
		}
		else if(Uart_Receive_Type == Uart_Receive_Type3)
		{
			if(Uart_Receive_Count <= SKID)		// ���յ����к�bit����С�� 10
			{ 
				if(UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count] != 0x3B)
				{
					op_id_t->SKID_Receive_Temp[Uart_Receive_Count] = UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count];
				}

				switch (UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count])
				{
					case 0x3B:          //receive ";"
						Uart_Receive_Type = Uart_Receive_Type4;
						Uart_Receive_Count = 0;
						break;
					default:
						Uart_Receive_Count++;
						break;
				}
			}
			else
			{
				Uart_Receive_Type = Uart_Receive_Type_Failure;
			}
		}
		else if(Uart_Receive_Type == Uart_Receive_Type4)
		{
			if(Uart_Receive_Count <= PKID)		// ���յ����к�bit����С�� 10
			{ 
				if(UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count] != 0x3B)
				{
					op_id_t->PKID_Receive_Temp[Uart_Receive_Count] = UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count];
				}

				switch (UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count])
				{
					case 0x3B:          //receive ";"
						Uart_Receive_Type = Uart_Receive_Type5;
						Uart_Receive_Count = 0;
						break;
					default:
						Uart_Receive_Count++;
						break;
				}
			}
			else
			{
				Uart_Receive_Type = Uart_Receive_Type_Failure;
			}
		}
		else if(Uart_Receive_Type == Uart_Receive_Type5)
		{
			if(Uart_Receive_Count <= DSID)  	// ���յ����к�bit����С�� 32
			{
				if(UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count] != 0x3B)
				{
					op_id_t->DSID_Receive_Temp[Uart_Receive_Count] = UART0_Fram_Record.Data_RX_BUF[Uart_Receive_All_Count];
				}

				if(Uart_Receive_Count >= DSID)
				{
					// ���ճɹ�
					Uart_Receive_Type = Uart_Receive_Type_Succeed;
				}
				else
				{
					Uart_Receive_Count++;
				}
			}
			else
			{
				Uart_Receive_Type = Uart_Receive_Type_Failure;
			}
		}
		else 
		{
			// ����ʧ��
			Uart_Receive_Type = Uart_Receive_Type_Failure;
		}

		// ���յ����м���
		Uart_Receive_All_Count++;
	}


	if(Uart_Receive_Type == Uart_Receive_Type_Succeed)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// ��¼ID���̹���
void system_input_ID_manage(void)
{
	if(_system_paygo_state.paygo_id != TRUE  && UART0_Fram_Record.InfBit.FramFinishFlag == 1 \
										&& UART0_Fram_Record.InfBit.FramLength >= 10 \
										&& UART0_Fram_Record.Data_RX_BUF != NULL)
	{
		RED_SATE_OPEN();
		GREEN_SATE_OPEN();

		struct OpenpayGo_ID openpaygo_id_t;
		memset(&openpaygo_id_t, 0, sizeof(struct OpenpayGo_ID));

		_Bool flag_t = judge_uart_receive_id(&openpaygo_id_t);

		if(flag_t == TRUE)
		{
			// ���ؽ��յ���ID
			returns_the_uart_receive_id(&openpaygo_id_t, TRUE);

			uint32_t scid = array_to_int32_bit(&openpaygo_id_t.SCID_Receive_Temp[0], SCID);
			
			uint32_t suid = array_to_int32_bit(&openpaygo_id_t.SUID_Receive_Temp[0], SUID);

			// ����ID, ����ʼ���ڴ�
			op_manage_set_flash_id(&scid, &suid, &openpaygo_id_t.SKID_Receive_Temp[0], &openpaygo_id_t.PKID_Receive_Temp[0], &openpaygo_id_t.DSID_Receive_Temp[0]);

			RED_SATE_CLOSE();
			GREEN_SATE_CLOSE();

			// �ر�UART0���ܿ�
			system_uart0_config_off();

			// ��ʼ���ڴ��Լ�������Ե�PAYGO
			op_manage_init_all_PAYGO_value();

			// ���е���ʾ״̬��ʱ�����
    		op_manage_run_display_state_and_time();
		}
		else
		{
			// ���ؽ���ʧ��
			returns_the_uart_receive_id(NULL, FALSE);

			memset(&UART0_Fram_Record.Data_RX_BUF[0], 0, UART0_Fram_Record.InfBit.FramLength);
			UART0_Fram_Record.InfBit.FramLength = 0;
			UART0_Fram_Record.InfBit.FramFinishFlag = 0;
		}
	}
}


/********************************************************************
 ** \brief  ���ؽ��յ���ID
 ** @param  struct OpenpayGo_IDָ��
 ** @param  _Bool �Ƿ���ճɹ�����
 ** \retval ��
********************************************************************/
void returns_the_uart_receive_id(void * pointer, _Bool RECEIVE_DONE)
{
	char UART_SCID[] = {0X53, 0X43, 0X49, 0X44, 0X3A};
	char UART_SUID[] = {0X53, 0X55, 0X49, 0X44, 0X3A};
	char UART_SKID[] = {0X53, 0X4B, 0X49, 0X44, 0X3A};
	char UART_PKID[] = {0X50, 0X4B, 0X49, 0X44, 0X3A};
	char UART_DSID[] = {0X44, 0X53, 0X49, 0X44, 0X3A};

	char R_N[] = {0x0D, 0x0A};	// \r\n

	if(RECEIVE_DONE == TRUE)
	{
		struct OpenpayGo_ID * OPG_ID_data = (struct OpenpayGo_ID *)pointer;

		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));

		// SCID����
		system_ble_uart0_send_string(&UART_SCID[0], sizeof(UART_SCID));
		system_ble_uart0_send_string(&OPG_ID_data->SCID_Receive_Temp[0], sizeof(OPG_ID_data->SCID_Receive_Temp));
		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));

		// SUID����
		system_ble_uart0_send_string(&UART_SUID[0], sizeof(UART_SUID));
		system_ble_uart0_send_string(&OPG_ID_data->SUID_Receive_Temp[0], sizeof(OPG_ID_data->SUID_Receive_Temp));
		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));

		// SKID����
		system_ble_uart0_send_string(&UART_SKID[0], sizeof(UART_SKID));
		system_ble_uart0_send_string(&OPG_ID_data->SKID_Receive_Temp[0], sizeof(OPG_ID_data->SKID_Receive_Temp));
		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));

		// PKID����
		system_ble_uart0_send_string(&UART_PKID[0], sizeof(UART_PKID));
		system_ble_uart0_send_string(&OPG_ID_data->PKID_Receive_Temp[0], sizeof(OPG_ID_data->PKID_Receive_Temp));
		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));

		// DSID����
		system_ble_uart0_send_string(&UART_DSID[0], sizeof(UART_DSID));
		system_ble_uart0_send_string(&OPG_ID_data->DSID_Receive_Temp[0], sizeof(OPG_ID_data->DSID_Receive_Temp));
		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));
	}
	else
	{
		char failure_reply[] = {'F', 'a', 'i', 'l', 'e', 'd', ',', ' ', 'p', 'l', 'e', 'a', 's', 'e', ' ', 'r', 'e', 's', 'e', 'n', 'd', '!'};
		// ����ʧ��
		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));
		system_ble_uart0_send_string(&failure_reply[0], sizeof(failure_reply));
		system_ble_uart0_send_string(&R_N[0], sizeof(R_N));
	}
}


/********************************************************************
 ** \brief  �������ÿ��λת��Ϊ1��HEX
 ** @param  char ����ָ�룬���յ�HEX����
 ** @param  uint8_t ���鳤�ȣ����յ�HEX����
 ** @param  char Դ���鳤�ȣ�Դ����
 ** \retval ��
********************************************************************/
void array_to_hex_bit(char * Array_data, uint8_t Array_len, char * temp_skid)
{
	uint8_t i = 0;
	uint8_t j = 0;
 	uint8_t temp_skid_byte = 0;

    // ��32��hexת��Ϊ16��hex
    for( i = 0; i < Array_len; i++)
    {
        for(j = 0; j < 2; j++)
        {
            if(*temp_skid >= '0' && *temp_skid <= '9')
            {
                *temp_skid -= 0x30;
            }
            else if(*temp_skid >= 'a' && *temp_skid <= 'z')
            {
                *temp_skid -= 0x57;
            }
            else if(*temp_skid >= 'A' && *temp_skid <= 'Z')
            {
                *temp_skid -= 0x37;
            }

            if(j == 0)
            {
                temp_skid_byte = *temp_skid * 0x10;
            }
            else
            {
                temp_skid_byte += *temp_skid;
            }

            temp_skid++;
        }

        *Array_data = temp_skid_byte;
		Array_data++;
    }
}


/********************************************************************
 ** \brief  ������ת��Ϊһ������
 ** @param  char����ָ��
 ** @param  char���鳤��
 ** \retval ��
********************************************************************/
uint64_t array_to_int32_bit(char * Array_data, uint8_t Array_len)
{
	uint8_t i;
	uint64_t result = 0;

	for ( i = 0; i < Array_len; i++)
	{
		if (*Array_data <= 9) 
		{
            result = result * 10 + *Array_data;
		}
		else if(*Array_data >= '0' && *Array_data <= '9') 
		{
			result = result * 10 + (*Array_data - '0');
		}

		Array_data++;
    }

	return result;
}


/********************************************************************
 ** \brief  ��ȡ�������ֵ
 ** \retval ��
********************************************************************/
int get_key_pressed(void) 
{
	system_martix_key_scan();		// ɨ�����ѽ

	unsigned char this_char = matrix_value.matrix_scan_value;	// �õ�����ɨ������Ľ��

	switch (this_char) {
        case '*':
					return STAR_KEY;

        case '#':
					return HASH_KEY;

        case 0x00 :
					return INVALID_KEY;

        default:
					return ((char) (this_char - '0')); // this_char is now an int
	}
}


/********************************************************************
 ** \brief  �ȴ���������
 ** \retval ��
********************************************************************/
uint64_t wait_for_token_entry(void)			
{
    // ��ü�������ֵ�Լ����ص���Կ
    int last_key = 0;
	uint64_t temp_token = NO_ENTER_TOKEN_VALUE;            
	                       
    // ���޼�������ֵ�жϣ���������ʱ������
    _Bool no_pressed_token = 0;
    uint8_t keypad_free_time_count = 0; 

    // ��������ĸ�������������ֵ��¼
    uint8_t keypad_pressed_num = 0;
    char input_open_paygo_token[TOKEN_LENGTH] = {0};

	// ���ڱ�־�Ƿ��������
	_Bool keypad_pressed_input_sure_token = FALSE;

	if(op_manage_lock_control(TRUE) && ((STAR_KEY == get_key_pressed()) && (no_pressed_token == FALSE)))
	{
		no_pressed_token = TRUE;
	}
	else if((STAR_KEY == get_key_pressed()) && !op_manage_lock_control(TRUE))		//�Ƿ�����������
	{
		open_paygo_display_startup(DISPLAY_KEYBOARD_LOCK);
		display_state_manage.display_state_flag = TRUE;

        if(_system_paygo_state.paygo_enabled && !_system_paygo_state.paygo_unlock)
        {
            display_state_manage.display_state_activity = TRUE;     // ��ʾ���ֱ��
        }

		matrix_value.matrix_scan_value = 0;
	}
	else if(HASH_KEY != get_key_pressed() && (STAR_KEY != get_key_pressed()) && (0x00 != matrix_value.matrix_scan_value))
	{
        irq_time.lcd_light_up_time = 0;
		matrix_value.matrix_scan_value = 0;
	}

    // ֻҪ����*�ţ���һֱ�����治�ϵĻ�ȡɨ������ֱ������#�����߳�ʱ
	while(no_pressed_token)			
	{
		last_key = get_key_pressed();

		switch (last_key)
		{
			case STAR_KEY:

				if(keypad_pressed_keycode_allow)
				{
					GREEN_SATE_OPEN();

					keypad_pressed_keycode_allow = FALSE;

					// �û�����ʱ����㣬׼����ʱ5���ӣ�û�м�������ͽ���
					keypad_free_time_count = 0;		
					irq_time.lcd_light_up_time = 0;
					matrix_value.matrix_scan_value = 0;		// ��վ���ɨ����

					// �����������Լ��洢token������
					keypad_pressed_num = 0;
					memset(input_open_paygo_token, 0x00, sizeof(input_open_paygo_token));

					// ��ʾ
					open_paygo_token_data_display('*', keypad_pressed_num);

					GREEN_SATE_CLOSE();
				}
				break;

			case HASH_KEY:

				GREEN_SATE_OPEN();

				keypad_pressed_keycode_allow = FALSE;
				no_pressed_token = FALSE;
				keypad_pressed_input_sure_token = TRUE;
				keypad_free_time_count = 0;
				matrix_value.matrix_scan_value = 0;	

				open_paygo_token_data_display('#', keypad_pressed_num);
				delay1ms(1000);
				
				GREEN_SATE_CLOSE();

				break;

			case INVALID_KEY:

				matrix_value.matrix_scan_value = 0;
				break;
			
			default:

				// �ǲ����������ź�
				if(keypad_pressed_keycode_allow)
				{
					GREEN_SATE_OPEN();

					// ����û�����ʱ�����
					keypad_free_time_count = 0;
					// �������ʱ�䣬������
					irq_time.lcd_light_up_time = 0;

					// �õ����������
					input_open_paygo_token[keypad_pressed_num] = get_key_pressed();

					open_paygo_token_data_display(input_open_paygo_token[keypad_pressed_num], keypad_pressed_num);
					
					keypad_pressed_num++;

					matrix_value.matrix_scan_value = 0;
					keypad_pressed_keycode_allow = FALSE;

					GREEN_SATE_CLOSE();
				}
				break;
		}

		// ��Ʋ���
		RED_SATE_CLOSE();

		if(irq_time.lptimer_1s_flag)
		{
			keypad_free_time_count++;

			// ���밴����������5����˳�����״̬
			if(keypad_free_time_count > 5)			
			{
				no_pressed_token = FALSE;					// �������볬ʱ���˳�ѭ��

				irq_time.lcd_light_up_time = 0;				// �������ʱ��
				keypad_free_time_count = 0;					// ������볬ʱ��ʱ

				matrix_value.matrix_scan_value = 0;
				keypad_pressed_input_sure_token = TRUE;		// ����������
				//temp_token = NO_ENTER_TOKEN_VALUE;
			}
			irq_time.lptimer_1s_flag = FALSE;
		}

		// �ػ��ͺ�Ǵ��ڼ䲻������
		if(__sleep_mode.device_power_on_state == FALSE || __sleep_mode.tamper_open == TRUE)
		{
			break;
		}
	}

	// ������Կ���
	//	��һ����Ȼ����Ƕ�����棬�ø��򵥵ķ�ʽʵ�֣�Ϊ�˷����Ķ���Ԥ�������������ͷ�Χ�����Է����ɡ�
	if(keypad_pressed_input_sure_token == TRUE)	 
	{
		// �õ��������Կ
		temp_token = array_to_int32_bit(&input_open_paygo_token[0], TOKEN_LENGTH);
		return temp_token;
	}

	return temp_token;
}


/********************************************************************
 ** \brief  ���п���
 ** \retval ��
********************************************************************/
void system_console_run_manage(void)
{
	// �ػ��ͺ�Ǵ��ڼ䲻������
	if(__sleep_mode.device_power_on_state != FALSE || __sleep_mode.tamper_open != TRUE)
	{
		uint32_t input_token;

		input_token = wait_for_token_entry();

		op_manage_recognition_input_token(input_token);
	}

	// ��ʾʱ������־λ�͹���
	op_manage_run_display_state_and_time();

	// PEYGO ״̬����ʹ洢
	op_manager_paygo_credit_and_workqueue();

	// ��¼ID����
	system_input_ID_manage();
}



