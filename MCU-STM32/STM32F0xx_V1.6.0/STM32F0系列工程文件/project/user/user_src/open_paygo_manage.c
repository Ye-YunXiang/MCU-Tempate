/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_manage.c 
* Description:	Manage the Open PAYGO logic
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#include "open_paygo_manage.h"
#include "system_ota.h"

__flash_date_tow INIT_THIS_DATE_TOW = {0,{255,255,255,255,255,255,255}};


// PAYGO״̬��ʼ��
__system_paygo_state _system_paygo_state;

// �Ƿ������Ľṹ��
__flash_lock _this_lock = {0, 0};
// ��һ��ʱ�䣬������ʱ��
__flash_date_one _this_date_one = {0, 0};
// ����洢ʱ��λ�ã�����洢ʱ���źţ�����洢ʱ��
__flash_date_tow _this_date_tow = {0, {255,255,255,255,255,255,255}};
// �ۼ�ʱ�䣬count
__flash_product_time _this_product_time = {0,0};

// ��Կ��starting code, ID
__flash_product_id _this_product_id;



__this _this_pointer = {  &_this_lock,\
                                &_this_date_one,\
                                &_this_date_tow,\
                                &_this_product_time,\
                                &_this_product_id};

const uint64_t Openpaygo_Test_Token[5] = {800100000,800200000,800300000,888880000,888890000};


/********************************************************************
 ** @brief  ��ʼ������PAYGO�洢�ṹ���Լ��ڴ�
 ** @param  ��
********************************************************************/
void op_manage_init_all_PAYGO_value(void)
{
    // ��ʼ���ṹ��
	memset(&_this_lock, 0, sizeof(__flash_lock));
	memset(&_this_date_one, 0, sizeof(__flash_date_one));
	
	_this_date_tow.interval_date = 0;
	memset(&_this_date_tow.location_date[0], 255, sizeof(_this_date_tow.location_date));
	
	memset(&_this_product_time, 0, sizeof(__flash_product_time));
	
    // ��ʼ���ڴ�
    open_paygo_flash_close_flash(CLOSE_FLASH_LOCK);
    open_paygo_flash_close_flash(CLOSE_FLASH_DATE_ONE);
    open_paygo_flash_close_flash(CLOSE_FLASH_DATE_TOW);
    open_paygo_flash_close_flash(CLOSE_FLASH_PRODUCT_TIME);

    // ��ʼ��ʾ����ṹ��
    open_paygo_display_struct();		
}


// ��������1��������������������
/********************************************************************
 ** @brief  ����flash�����м������
 ** @param  ��
 ** @retval ��
********************************************************************/
void op_manage_load_activation_variables(void) 
{
	memset(&_this_product_id, 0, sizeof(__flash_product_time));
	
    open_paygo_flash_read_lock_sate(&_this_lock);

    if(open_paygo_flash_read_date_one(&_this_date_one) != FALSE)
    {
        open_paygo_flash_read_date_tow(&_this_date_tow);

        // _system_paygo_state.paygo_enabled = TRUE;
    }

    open_paygo_flash_read_product_time(&_this_product_time);

    if(_this_product_time.add_time >= 998)
    {
        _system_paygo_state.paygo_unlock = TRUE;   // unlock

        _system_paygo_state.paygo_enabled = FALSE;  // ֹͣ��ʱ
    }
    else
    {
        _system_paygo_state.paygo_unlock = FALSE;
    }

    if(open_paygo_flash_read_product_id(&_this_product_id) != FALSE)
    {
        _system_paygo_state.paygo_id = TRUE;
    }
    else
    {
        _system_paygo_state.paygo_id = FALSE;

        // _system_paygo_state.paygo_enabled = FALSE;

        _system_paygo_state.paygo_unlock = FALSE;
    }
}

// ��������2����������
/********************************************************************
 ** @brief  ���Ѻ��ʼ��ʱ�䣬����֤RTC�Ƿ��������Լ���ʾ��Ļ
 ** @param  ��
 ** @retval ��
********************************************************************/
void op_manage_rouse_calibration_time_and_display(uint8_t flow)
{
    uint8_t i ;
    uint8_t temp_rtc_state;

    if(flow == 0)   // ������Ļ
    {
        rtc_state.rtc_starting_test = TRUE;

        open_paygo_display_startup(DISPLAY_SLOGAN);

        // PAYGO ���ж��
        if(op_manage_PAYGO_do_odd_times() == TRUE)
        {
            uint32_t temp_time;

            uint8_t time_sate = starting_run_examine_get_sec(&temp_time, FALSE);

            if(!time_sate || temp_time < _this_date_tow.interval_date)
            {
                rtc_time_t timer_date_temp;

                second_conversion_date(&timer_date_temp, _this_date_tow.interval_date);

                example_set_rtc_time(&timer_date_temp);
            }

            // ����MQTT����ѭ��������Ϣ
            //mqtt_state.mqtt_cyclic_send = TRUE;
            // ������Ϣ,��GSM
            if(mqtt_state.mqtt_task_state == MQTT_TYPE_NULL || gsm_power_sending_time_count == 1 || gsm_power_sending_time_count == 0)
            {
                mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_ON;
            }
        }
            
        irq_time.lptimer_2s_flag = 0;
    }
    else if(flow == 1)  // ����RTC
    {
        open_paygo_display_startup(DISPLAY_UP_LOADING);

        // uart��ʼ����ר�ã��Ͳ�Ҫ�ظ���ʼ��
        if(UART0_Fram_Record.set_up != TRUE || __sleep_mode.device_power_on_state == FALSE)
        {
            TIMER_5ms_ON(); // ��5ms��ʱ�� Ϊ���Ŀ���
        }

        i = 0;
        temp_rtc_state = 0;

        // i ÿ�μ�һ������250ms�� �������� 18 * 0.25s = 4.5s
        while(i !=  18)
        {
            if(temp_rtc_state == 0 && !_system_paygo_state.paygo_unlock)
            {
                temp_rtc_state = open_paygo_time_starting_run_examine();            // ��֤�ɹ��Ļ��ͻ��lptime������ֵ
            }

            if(i == 0 || i ==1 || i == 4 || i == 5 || i == 8 || i == 9\
                                        || i == 12 || i == 14 || i == 16)
            {
                GREEN_SATE_OPEN();
            }
            else
            {
                GREEN_SATE_CLOSE();
            }

            if(irq_time.timer_250ms_flag == 1)
            {
                i++;

                irq_time.timer_250ms_flag = 0;
                irq_time.timer_5_ms_count = 0;
            }
			
			if(__sleep_mode.device_power_on_state != TRUE && __sleep_mode.run_state != _SYSTEM_SLEEP_ROUSE)
			{
				break;
			}
        }
        
        // uart��ʼ����ר�ã��Ͳ�Ҫ�ر�
        if(UART0_Fram_Record.set_up != TRUE || __sleep_mode.device_power_on_state == FALSE)
        {
            TIMER_5ms_OFF();     // Ϊ���Ŀ���
        }
    }
    else if(flow == 2)  // �������
    {
        // ��������2.1 ������ �ж�RTC״̬�������Ƿ����������
        op_manage_rtc_error_manage();
        
        // ��������2.2 ��������ʼ���ж���֤RTC���ߴ洢�ı�־λʱ��
        op_manage_rouse_irq_time_init();  

        // ���е���ʾ״̬��ʱ�����
        op_manage_run_display_state_and_time();
    }

    // ˯��ģʽ�²��ܽ���
	if(__sleep_mode.run_state != _SYSTEM_SLEEP_ROUSE)
    {
		// ϵͳ���ѻ�˯������
		system_mode_wakeUP_or_sleep_prrocess(TRUE);
    }
}

// ��������2.1 ����
/********************************************************************
 ** @brief  ��RTC�Ƿ񻵵�����������ж�
 ** @param  ��
 ** @retval ��
********************************************************************/
void op_manage_rtc_error_manage(void)
{
    // RTC״̬�ǻ��ģ�����PAYGO״̬������unlock
    if(rtc_state.rtc_run_state == RTC_ERROR && _system_paygo_state.paygo_unlock != TRUE)
    {
        if(_system_paygo_state.paygo_enabled == TRUE)
        {
            // �����еĹ�����RTC������
            if(irq_time.lcd_light_up_time >= 4)
            {
                open_paygo_display_rtc_error_run_manage();

                irq_time.lcd_light_up_time = 0;
            }
        }
        else
        {
            GREEN_SATE_OPEN();

            RED_SATE_OPEN();
                
            open_paygo_display_startup(DISPLAY_RTC_ERROR);

            // RTC ������
            while(1)
            {
                if(__sleep_mode.device_power_on_state == FALSE)
                {
                    break;
                }   
            }
        }
    }
}

// ��������2.2 ������
/********************************************************************
 ** @brief  ��ʼ���ж�ʱ�䣬���жϼ��RTC and �洢��־λ
 ** @param  ��
 ** @retval ��
********************************************************************/
void op_manage_rouse_irq_time_init(void)
{
    if(op_manage_PAYGO_do_odd_times() == TRUE)
    {
        uint32_t temp_time_data;

        _system_paygo_state.paygo_enabled = TRUE;

        // �õ��������ʣ������
        temp_time_data = _this_date_one.final_date - irq_time.lptimer_time_count;

        if(temp_time_data >= 1800)          // ���ڻ����30����
        {
            irq_time.irq_10_min = (temp_time_data % 600) + irq_time.lptimer_time_count;
            irq_time.irq_30_min = (temp_time_data % 1800) + irq_time.lptimer_time_count;

            irq_time.lptimer_test_flag = 0;
            irq_time.lptimer_flash_flag = 0;
        }
        else if(temp_time_data >= 600)      // ���ڻ����10����
        {
            irq_time.irq_10_min = (temp_time_data % 600) + irq_time.lptimer_time_count;
            irq_time.irq_30_min = _this_date_one.final_date;

            irq_time.lptimer_test_flag = 0;
            irq_time.lptimer_flash_flag = 0;
        }
        else                                // С��10���ӻ��߼�ʱ���
        {
            irq_time.irq_10_min = _this_date_one.final_date;
            irq_time.irq_30_min = _this_date_one.final_date;

            irq_time.lptimer_test_flag = 0;
            irq_time.lptimer_flash_flag = 0;
        }
    }
    else
    {
        _system_paygo_state.paygo_enabled = FALSE;

        // ����10����
        irq_time.irq_10_min = irq_time.lptimer_time_count + 600;

        irq_time.lptimer_test_flag = 0;
        irq_time.lptimer_flash_flag = 0;
    }
}

// ��������3 ��������֤�汾��
/********************************************************************
 ** @brief  ��֤�Լ��滻�汾��
 ** @param  �汾��
 ** @param  bool �Ƿ����´��룬0Ϊֻ����֤�汾�ţ�1Ϊ���´�����Ϣ
 ** @retval ��
********************************************************************/
void op_manage_verify_version_numbe(const uint8_t *temp_version_number, _Bool temp_falg)
{
    if(temp_falg != TRUE)
    {
        if( memcmp(&_this_product_id.version, temp_version_number, sizeof(_this_product_id.version)) != 0) 
        {
            memcpy(&_this_product_id.version, temp_version_number, sizeof(_this_product_id.version));

            _this_product_id.have_reported = 0;
            
            open_paygo_flash_write_product_id(&_this_product_id);   // ���´���ID

        }
        else if(bootloader_get_handover_performed() == TRUE)
        {
            _this_product_id.have_reported = 0;
            
            open_paygo_flash_write_product_id(&_this_product_id);   // ���´���ID
        }
    }
    else
    {
        open_paygo_flash_write_product_id(&_this_product_id);   // ���´���ID
    }
}

/********************************************************************
 ** @brief  ��flash����uart��ȡ��ID
 ** @param  uint32_t starting code
 ** @param  uint32_t unit number code
 ** @param  char secret key
 ** @retval ��
********************************************************************/
void op_manage_set_flash_id(uint32_t * temp_scid, uint32_t * temp_suid, char * temp_skid, char * temp_pkid, char * temp_dsid)
{
    _this_product_id.starting_code = *temp_scid;

    _this_product_id.unit_number_code = *temp_suid;

    // ��32��hexת��Ϊ16��hex
    array_to_hex_bit((char *)&_this_product_id.secret_key[0], sizeof(_this_product_id.secret_key), temp_skid);

    memcpy(&_this_product_id.product_key, temp_pkid, sizeof(_this_product_id.product_key));

    memcpy(&_this_product_id.device_secret, temp_dsid, sizeof(_this_product_id.device_secret));

    open_paygo_flash_write_product_id(&_this_product_id);

    // PAYGO״̬��ʼ��
    memset(&_system_paygo_state, 0, sizeof(__system_paygo_state));

    _system_paygo_state.paygo_id = TRUE;
}


/********************************************************************
 ** @brief  ���е���ʾ״̬����ʾʱ�����
 ** @param  ��
 ** @retval ��
********************************************************************/
void op_manage_run_display_state_and_time(void)
{
    if(_system_paygo_state.paygo_unlock && display_state_manage.display_state_enum != DISPLAY_UNLOCK)
    {
        display_state_manage.display_state_flag = TRUE;

        display_state_manage.display_state_enum = DISPLAY_UNLOCK;
    }
    else if(!_system_paygo_state.paygo_unlock && _system_paygo_state.paygo_enabled && _this_date_one.available_time != 0)
    {

        uint32_t temp_value = _this_date_one.final_date - irq_time.lptimer_time_count;

        uint32_t temp_display_day = temp_value / 86400;

        uint32_t temp_display_hour = (temp_value - temp_display_day * 86400) / 3600;

        if(display_state_manage.display_state_enum != DISPLAY_ACTIVATED_TIME)
        {
            display_state_manage.display_state_flag = TRUE;

            display_state_manage.display_state_activity = TRUE;

            display_state_manage.display_state_enum = DISPLAY_ACTIVATED_TIME;
        }

        if(display_state_manage.display_hour != temp_display_hour || display_state_manage.display_day != temp_display_day)
        {
            display_state_manage.display_state_activity = TRUE;

            display_state_manage.display_day = temp_display_day;

            display_state_manage.display_hour = temp_display_hour;

            // display_state_manage.all_display_min = temp_value / 60;
        }
    }
    else if(!_system_paygo_state.paygo_unlock && _system_paygo_state.paygo_id && display_state_manage.display_state_enum != DISPLAY_NO_PAYGO)   // û��PYAYGO
    {
        display_state_manage.display_state_activity = FALSE;
        display_state_manage.display_state_flag = TRUE;

        display_state_manage.display_state_enum = DISPLAY_NO_PAYGO;
    }
    else if(!_system_paygo_state.paygo_unlock && !_system_paygo_state.paygo_id && display_state_manage.display_state_enum != DISPLAY_NO_ID)    // û��ID
    {
        display_state_manage.display_state_activity = FALSE;
        display_state_manage.display_state_flag = TRUE;

        display_state_manage.display_state_enum = DISPLAY_NO_ID;
    }
}


/********************************************************************
 ** @brief  PAYGO������״̬����
 ** @param  ��
 ** @retval ��
********************************************************************/
void    op_manager_paygo_credit_and_workqueue(void)
{
    // unlock�ж�
    if(_this_product_time.add_time >= 998 && _system_paygo_state.paygo_enabled)
    {
        _system_paygo_state.paygo_enabled = FALSE;
        _system_paygo_state.paygo_unlock = TRUE;

        memset(&_this_date_one, 0, sizeof(_this_date_one));
		memcpy(&_this_date_tow, &INIT_THIS_DATE_TOW, sizeof(_this_date_tow));

        open_paygo_flash_close_flash(CLOSE_FLASH_DATE_ONE);
        open_paygo_flash_close_flash(CLOSE_FLASH_DATE_TOW);

        GREEN_SATE_CLOSE();
        RED_SATE_CLOSE();

        //mqtt_state.mqtt_cyclic_send = TRUE;
    }

    // ��ʱ�����ж�
    if(_this_date_one.final_date <= irq_time.lptimer_time_count && _system_paygo_state.paygo_enabled \
                                                                    && !_system_paygo_state.paygo_unlock)
    {
        _system_paygo_state.paygo_enabled = FALSE;

        memset(&_this_date_one, 0, sizeof(_this_date_one));
		memcpy(&_this_date_tow, &INIT_THIS_DATE_TOW, sizeof(_this_date_tow));

        // ���
        open_paygo_flash_close_flash(CLOSE_FLASH_DATE_ONE);
        open_paygo_flash_close_flash(CLOSE_FLASH_DATE_TOW);

        GREEN_SATE_CLOSE();
        RED_SATE_CLOSE();

        //mqtt_state.mqtt_cyclic_send = FALSE;
    }

    // ������ʱ�����������ڴ��л�����������̴洢һ�Ρ�
    if(((irq_time.lptimer_time_count - _this_date_tow.interval_date) > 3600) && _system_paygo_state.paygo_enabled \
                                                                                && !_system_paygo_state.paygo_unlock)
    {
        // �����ʱ״̬�µļ��ʱ��
        op_manage_write_date_tow(NULL);

        //mqtt_state.mqtt_cyclic_send = TRUE;
    }

    // �Ƿ��������ж�
    if((_system_paygo_state.paygo_unlock || _system_paygo_state.paygo_enabled) && __sleep_mode.tamper_open != TRUE)
    {
        // �򿪱���
        system_refrigerator_control_open();
    }
    else
    {
        // �رձ���
        system_refrigerator_control_shut();
    }

    // ��֤ʱ��
	open_paygo_time_run_examine();
    // �����֤ʱ��ʧ�ܿ�ʼ��֤RTC��û�л���
    open_paygo_time_starting_run_examine();
}


// ������ʱ��
// �������ڼ�ʱ״̬������û��unlock������²��ܴ�
void op_manage_write_date_tow(void * struct_data)
{
    if(struct_data == NULL)
    {
        if(_system_paygo_state.paygo_enabled && !_system_paygo_state.paygo_unlock)
        {
            _this_date_tow.interval_date = irq_time.lptimer_time_count;

            // ������ʱ��
            open_paygo_flash_write_date_tow(&_this_date_tow);

            //mqtt_state.mqtt_cyclic_send = TRUE;
        }
    }
    else
    {
        // ������ʱ��
        open_paygo_flash_write_date_tow( (__flash_date_tow *)struct_data);
    }
}

 
/********************************************************************
 ** @brief  ��֤Token������Ƿ�Ϊ���������Ҫ����
 ** @param  uint32_t ���������ȡ��token
 ** @retval ��
********************************************************************/
void op_manage_recognition_input_token(uint32_t input_token_data)
{
//	uint32_t Read_Openpaygo_Unit_Number;
    
    // ����������
	if(input_token_data != (uint32_t)NO_ENTER_TOKEN_VALUE)
	{
        if(Openpaygo_Test_Token[4] == input_token_data)	    //three hour test code   88889 ��countֵ
		{
            display_state_manage.display_state_token_enum = DISPLAY_TOKEN_COUNT;
        }
 		else if(Openpaygo_Test_Token[3] == input_token_data)			//research device id	 88888 ������ID��
		{
            display_state_manage.display_state_token_enum = DISPLAY_TOKEN_ID;
		}
		else if(Openpaygo_Test_Token[2] == input_token_data)	//three hour test code   8003 ����ID״̬�³�ֵ��ֵ3��
		{
            // û��ID������¿�����
            if(!_system_paygo_state.paygo_id)
            {
                uint32_t rtc_green_time;

                starting_run_examine_get_sec(&rtc_green_time, FALSE);

                _this_date_one.final_date = rtc_green_time + (3*24*60*60);
                _this_date_one.available_time = (3*24*60*60);

                _this_date_tow.interval_date = rtc_green_time;

                // ��ʼ���ж�ʱ��
                op_manage_rouse_irq_time_init();

                // ����
                open_paygo_flash_write_date_one(&_this_date_one);
                op_manage_write_date_tow(&_this_date_tow);

                // �򿪼�ʱ��־
                _system_paygo_state.paygo_enabled = TRUE;
            }
            else
            {
                display_state_manage.display_state_token_enum = DISPLAY_NO_TOKEN;
            }
		}
        else if(Openpaygo_Test_Token[1] == input_token_data)	//three hour test code   8002 ����ID״̬�³�ֵ��ֵ3Сʱ
		{
            // û��ID������¿�����
            if(!_system_paygo_state.paygo_id)
            {
                uint32_t rtc_green_time;

                starting_run_examine_get_sec(&rtc_green_time, FALSE);

                _this_date_one.final_date = rtc_green_time + (3*60*60);
                _this_date_one.available_time = (3*60*60);

                _this_date_tow.interval_date = rtc_green_time;

                // ��ʼ���ж�ʱ��
                op_manage_rouse_irq_time_init();

                // ����
                open_paygo_flash_write_date_one(&_this_date_one);
                op_manage_write_date_tow(&_this_date_tow);

                // �򿪼�ʱ��־
                _system_paygo_state.paygo_enabled = TRUE;
            }
            else
            {
                display_state_manage.display_state_token_enum = DISPLAY_NO_TOKEN;
            }
		}
		// ��ʽҪע�� ################################################################################
		else if(Openpaygo_Test_Token[0] == input_token_data)			//research device id	 8001 ���count��##########################################################
		{
            display_state_manage.display_state_token_enum = DISPLAY_TOKEN_COUNT_CLOSE;

            _this_product_time.token_count = 0;

            open_paygo_flash_write_product_time(&_this_product_time); 
		}
		else 													//user input openpaygo token  �û�����OpenPAYGO�Ǻ�
		{
            int get_credit_remaining = 0;
            uint16_t temp_token_count = _this_product_time.token_count; 				// Token Count

			get_credit_remaining = op_manage_product_get_openpaygo_activation_value(input_token_data, &temp_token_count);

			// �Խ�����������Կ�����ж�
            op_manage_product_opepaygo_token_judge_result(get_credit_remaining, temp_token_count);
		}
		
        // ����ʾ���
		display_state_manage.display_state_flag = TRUE;

        if(_system_paygo_state.paygo_enabled && !_system_paygo_state.paygo_unlock)
        {
            display_state_manage.display_state_activity = TRUE;     // ��ʾ���ֱ��
        }
	}
}


/********************************************************************
 ** @brief  �Լ��̻�ȡ����Կ���н��ܣ�����ܳɹ��͸�ֵcount
 ** @param  uint64_t �������Կ
 ** @param  uint16_t open paygo �� count
 ** @retval ���ܳ�����������-1Ϊ��Ч��
********************************************************************/
int op_manage_product_get_openpaygo_activation_value(uint64_t input_token, uint16_t * temp_count)
{
	int credit_data = 0;

	uint8_t i = 0;

	uint32_t openpaygo_id_starting_code = _this_product_id.starting_code;		// Starting Code
	
	uint8_t openpaygo_id_secret_key[sizeof(_this_product_id.secret_key)];		// Secret Key

    for(i = 0; i < sizeof(_this_product_id.secret_key); i++)
	{
		openpaygo_id_secret_key[i] = _this_product_id.secret_key[i];
	}
	
	__NOP();
	
	// ֱ�Ӽ���OpenPAYGO�Ľ��룬������ʱ��	
	credit_data = GetActivationValueFromToken(input_token, temp_count, openpaygo_id_starting_code, (unsigned char *)openpaygo_id_secret_key);
	
	__NOP();
	
	return credit_data;
}


/********************************************************************
 ** @brief  �Խ�����������Կ�����ж�
 ** @param  int ���ܳ���������
 ** @param  uint16_t ���ܳ�����count
 ** @retval ��
********************************************************************/
void op_manage_product_opepaygo_token_judge_result(int temp_credit_remaining, uint16_t temp_count)
{
    switch (temp_credit_remaining)
    {
        case INVALID_TOKEN_VALUE:   // -1
        case PAYGO_ID_VALUE:        // 996
        case PAYGO_COUNT_VALUE:     // 997
        case COUNTER_SYNC_VALUE:    // 999

            display_state_manage.display_state_token_enum = DISPLAY_NO_TOKEN;

            op_manage_lock_control(FALSE);

            break;

        case PAYGO_DISABLE_VALUE:   // 0
           
            display_state_manage.display_state_token_enum = DISPLAY_TOKEN_DISABLE;

            // ���ýṹ��
            memset(&_this_date_one, 0, sizeof(_this_date_one));
		    memcpy(&_this_date_tow, &INIT_THIS_DATE_TOW, sizeof(_this_date_tow));

            // ���flash
            open_paygo_flash_close_flash(CLOSE_FLASH_DATE_ONE);
            open_paygo_flash_close_flash(CLOSE_FLASH_DATE_TOW);

            // ����ۼƵĶ���������countֵ
            _this_product_time.add_time = 0;
            _this_product_time.token_count = temp_count;
            open_paygo_flash_write_product_time(&_this_product_time);

            // �����豸״̬
            _system_paygo_state.paygo_enabled = TRUE;
            _system_paygo_state.paygo_unlock = FALSE;

            // ������֤ʱ���־
            rtc_state.rtc_starting_test = TRUE;

            break;

        case PAYGO_UNLOCK_VALUE:    // 998

            _system_paygo_state.paygo_unlock = TRUE;

            _this_product_time.add_time = 998;

            _this_product_time.token_count = temp_count;

            open_paygo_flash_write_product_time(&_this_product_time);           
            
            break;

        default:                    // ɶҲ����

            if(temp_credit_remaining > PAYGO_DISABLE_VALUE && temp_credit_remaining <= MAX_ACTIVATION_VALUE \
									&& _system_paygo_state.paygo_unlock != TRUE && rtc_state.rtc_run_state == RTC_NORMAL)
            {				
                display_state_manage.display_state_token_enum = DISPLAY_TOKEN_EFFICACIOUS;

                _this_product_time.token_count = temp_count;

                if( _this_product_time.token_count % 2)
                {
                    op_manage_set_activation_time(temp_credit_remaining);       // ����Ϊ����
                }
                else
                {
                    op_manage_add_activation_time(temp_credit_remaining);       // ż��Ϊ���
                }

                 _system_paygo_state.paygo_enabled = TRUE;
            }
            else
            {
                display_state_manage.display_state_token_enum = DISPLAY_NO_TOKEN;
            }

            break;
    }

    if(temp_credit_remaining != -1 && _this_lock.lock_error != 0)
    {
        _this_lock.lock_date = 0;

        _this_lock.lock_error = 0;

        open_paygo_flash_close_flash(CLOSE_FLASH_LOCK);
    }
    else if(temp_credit_remaining != -1 && temp_credit_remaining != 996 && \
            temp_credit_remaining != 997 && mqtt_state.mqtt_task_state == MQTT_TYPE_NULL)
    {
        mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_ON;
    }
}


/********************************************************************
 ** @brief  Open PAYGO ��������
 ** @param  int Ҫ��ӵ�����
 ** @retval ��
********************************************************************/
void op_manage_add_activation_time(int activation_value)
{	
    _this_date_one.available_time += activation_value * 86400;

    _this_date_one.final_date = (_this_date_one.final_date == 0) ? \
		(irq_time.lptimer_time_count + (activation_value * 86400)) : (_this_date_one.final_date + (activation_value * 86400));

    _this_product_time.add_time = ((_this_product_time.add_time + activation_value) > 998) ? \
                                                998 : (_this_product_time.add_time + activation_value);

    open_paygo_flash_write_date_one(&_this_date_one);

    open_paygo_flash_write_product_time(&_this_product_time);
}


/********************************************************************
 ** @brief  Open PAYGO ����ø�������
 ** @param  int Ҫ���ǵ�����
 ** @retval ��
********************************************************************/
void op_manage_set_activation_time(int activation_value)
{
    _this_date_one.available_time = activation_value * 86400;

    _this_date_one.final_date = irq_time.lptimer_time_count + (activation_value * 86400);

    // �ۼ���������

    open_paygo_flash_write_date_one(&_this_date_one);

    open_paygo_flash_write_product_time(&_this_product_time);
}


// ��Ǵ���ʾ����
void op_manage_tamper_open_manage(void)
{
    // �򿪺��
    if(__sleep_mode.tamper_open == TRUE)
    {
        system_lcd1602_display_clear_dispaly();		// �����Ļ
        System_LCD_Light_Disable();		// �ر���ʾ
        RED_SATE_CLOSE();				// �رպ��
        GREEN_SATE_CLOSE();				// �ر��̵�

        system_refrigerator_control_shut();     // �رձ���

        display_state_manage.display_state_token_enum = DISPLAY_TAMPER;	// ��Ǵ�

        // __sleep_mode.mqtt_tamper_open = 1; // �ϱ��򿪱�־

        __sleep_mode.run_state = _SYSTEM_BOOT_SCREEN;   // ����ϵͳ

        if(mqtt_state.mqtt_task_state == MQTT_TYPE_NULL || gsm_power_sending_time_count <= 1)
        {
            mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_ON;
        }
        
        while(__sleep_mode.tamper_open == TRUE)
        {
            // M25 �ڷ���
            system_gsm_mqtt_manage();		
        }

        // ��ʾʱ������־λ�͹���
        //op_manage_run_display_state_and_time();
    }
}


// �ж�PAYGO���޶�ȣ����ڴ�M25����Ϣ
_Bool op_manage_PAYGO_do_odd_times(void)
{
    if(_this_date_one.available_time != 0 && _this_date_one.available_time != 0xffffffff \
										&& _system_paygo_state.paygo_unlock == FALSE)
	{
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************************************
** ��������
 ******************************************************************************/

/********************************************************************
 ** @brief  ������Կ������������
 ** @param  _Bool trueΪ�Ѿ���������֤��falseΪ��û������Ӵ������
 ** @retval δ��������1������������0
********************************************************************/
_Bool op_manage_lock_control(_Bool temp_flag)
{
    if(temp_flag)
    {
        if(_this_lock.lock_date != 0)
        {
            if(irq_time.lptimer_2s_flag  == 2)
            {
                _this_lock.lock_date -= 2;

                irq_time.lptimer_2s_flag = 0;
            }

            return 0;
        }
        else if(_this_lock.lock_date == 0 && _this_lock.lock_error >= 6)
        {
            _this_lock.lock_date = 0;

            _this_lock.lock_error = 0;

            open_paygo_flash_close_flash(CLOSE_FLASH_LOCK);
        }
    }
    else
    {
        if(_this_lock.lock_error >= 5)       // ��5��
        {
            irq_time.lptimer_2s_flag = 0;

            _this_lock.lock_error = 6;

            _this_lock.lock_date = LOCKING_TIME;

            open_paygo_flash_write_lock_state(&_this_lock, TRUE);

            return 0;
        }
        else
        {
            _this_lock.lock_error++;
            
            open_paygo_flash_write_lock_state(&_this_lock, FALSE);
        } 
    }

    return 1;
}

/*******************************************************************************
** MQTT���
 ******************************************************************************/

/********************************************************************
 ** @brief  ���MQTT������
 ** @param  struct GSM_MQTT_PARAM 
 ** @retval ��
********************************************************************/
void op_manage_get_mqtt_param(void * temp_param)
{
    system_adc_sample_on(); // ���ADC��ѹ

	delay100us(5);

	struct GSM_MQTT_PARAM * _mqtt_param = (struct GSM_MQTT_PARAM *)(temp_param);

    _mqtt_param->count = _this_product_time.token_count;

    
    // ��䷢��ʱ��
    if(!_system_paygo_state.paygo_unlock && _system_paygo_state.paygo_enabled && _this_date_one.available_time != 0)
    {
        _mqtt_param->time_remaining_hour = (_this_date_one.final_date - irq_time.lptimer_time_count) / 60 /60;

        _mqtt_param->time_remaining_min = ((_this_date_one.final_date - irq_time.lptimer_time_count) /60 ) % 60;
    }
    else if(_system_paygo_state.paygo_unlock == TRUE)
    {
        _mqtt_param->time_remaining_hour = 9999;

        _mqtt_param->time_remaining_min = 9999;
    }
    else {
        _mqtt_param->time_remaining_hour = 0;

        _mqtt_param->time_remaining_min = 0;
	}

    // ת����ص�ѹ
    _mqtt_param->BAT_VDC = (float)__ADC.ADC_sample_data[0] / 1000;
    // ת���ⲿ�����ѹ
    _mqtt_param->DC_VDC = (float)__ADC.ADC_sample_data[1] / 1000;


    // ת��Ϊ���������¶�
    _mqtt_param->refrigerator_thermometer = __ADC.ADC_sample_data[2] / 1000;
    if(_mqtt_param->refrigerator_thermometer > (100.0) || _mqtt_param->refrigerator_thermometer < (-50.0))
    {
        _mqtt_param->refrigerator_thermometer = 999.0;
    }

    _mqtt_param->power_state = __sleep_mode.device_power_on_state;
}
