/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_time.c 
* Description:	Manage OpenPAYGO time calculations
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#include "open_paygo_time.h"


// ��ʱ��ͬ������ʱ���
uint8_t rtc_error_log = 0;
// �жϼ���
volatile irq_time_t irq_time = {0};
// rtc״̬
volatile rtc_state_t rtc_state;

// ��¼��һ�ε�RTCʱ���
uint32_t starting_run_examine_time;

// ����ṹ��rtc_state and irq_time
void open_paygo_struct_init(void)
{
    rtc_state.rtc_run_state = 0;          // RTC����״̬
    rtc_state.rtc_starting_test = 0;      // RTC��������ź�
    rtc_state.rtc_run_test_flag = 0;      // ����ʱ�����ź�
    rtc_state.rtc_read_error_cont = 0;    // RTC����״̬������

    irq_time.lptimer_time_count = 0;      // ÿ���ʱ
    irq_time.irq_10_min = 0;
    irq_time.irq_30_min = 0;
    irq_time.lptimer_test_flag = 0;       // �ﵽ������Ի��ߴ洢���ź�
    irq_time.lptimer_flash_flag = 0;         // �ﵽ������Ի��ߴ洢���ź�

    irq_time.lptimer_led_1s_flag = 0;         // LED��ÿ���ź�
    irq_time.lptimer_1s_flag = 0;         // rtc��ÿ���ź�
    irq_time.lptimer_2s_flag = 0;         // rtc��2���ź�
    irq_time.timer_250ms_flag = 0;
    irq_time.lcd_light_up_time = 0;       // lcd�Ƶ�����ʱ��

    irq_time.timer_5_ms_count = 0;       // 5ms��ʱ

    irq_time.sleep_open = 0;            // ��ʱ��״̬ ��˯��ģʽ��������ģʽ
    irq_time.lp_sleep_count = 0;          // ���Ѵ���
}


/********************************************************************
 ** \brief  ������� rtc_time_t �ṹ���Աһ��һ�������1s���С�
 ** @param  ����rtc_time_t�ṹ��ָ��
 ** \retval ���������1�������ⷵ��0,������ֵ����
********************************************************************/
_Bool compare_struct_rtc_time_t(const rtc_time_t *s1, const rtc_time_t *s2) 
{
    if ((s1->year == s2->year && s1->mon == s2->mon \
            && s1->day == s2->day && s1->hour == s1->hour && s1->min == s2->min)\
            && ( s1->sec == s2->sec || s1->sec+1 == s2->sec || s1->sec == s2->sec+1 ))
    {
        return 1;
    }

    return 0;
}


/********************************************************************
 ** \brief  ��� rtc_time_t ��֯��Ա�Ƿ��зǷ�ֵ
 ** @param  ��Ҫ����rtc_time_t�ṹ��ָ��
 ** \retval ���������1�������ⷵ��0
********************************************************************/
bool struct_rtc_time_invalid_value_examine(const rtc_time_t *s1) 
{
    if (s1->year < 70 && s1->mon <= 12 && s1->day <= 31 && s1->hour <= 24 && s1->min <= 60 && s1->sec <= 60)
    {
        return 1;
    }

    return 0;
}


/********************************************************************
 ** \brief  ��ȡRTC��������֤�����ֵ���ṹ��
 ** @param  ��Ҫ��ֵ��rtc_time_t�ṹ��ָ��
 ** \retval ���������1�������ⷵ��0
********************************************************************/
bool open_paygo_time_read_tow_examine( rtc_time_t * date )
{
    rtc_time_t temp_date_t;

    uint8_t i = 0;

    do{
        system_internal_rtc_get_time(date);

        system_internal_rtc_get_time(&temp_date_t);

        if(compare_struct_rtc_time_t(date, &temp_date_t) && struct_rtc_time_invalid_value_examine(date))
        {
            return 1;
        }
        i++;
    }
    while(i <= 5);

    return 0;
}

/********************************************************************
 ** \brief  �������ʱ�䵽���ڵ�����
 ** @param  rtc_time_t�ṹ��
 ** \retval ������������֮ǰ�����
********************************************************************/
uint32_t date_conversion_second(rtc_time_t *s1 ) 
{
		struct tm tm1;
		time_t timep;
	
		//�ȸ�ֵ,ע�������һ�ĵط�
		tm1.tm_year = s1->year + 2000 - 1900;
		tm1.tm_mon = s1->mon - 1;
		tm1.tm_mday = s1->day;
		tm1.tm_hour = s1->hour;
		tm1.tm_min = s1->min;
		tm1.tm_sec = s1->sec;
		
		timep=mktime(&tm1);
		
		//��һ�¾���������
		return timep;
}

/********************************************************************
 ** \brief  ����ʱ����������Ϊ����
 ** @param  rtc_time_t��ֵ�ṹ��
 ** @param  ����ʱ������
 ** \retval ��
********************************************************************/
void second_conversion_date( rtc_time_t * dt, time_t timestamp)
{
    uint32_t days = timestamp / 86400;  // һ���� 86400 ��
    uint32_t seconds = timestamp % 86400;

    dt->sec = (uint8_t)(seconds % 60);
    dt->min = (uint8_t)((seconds / 60) % 60);
    dt->hour = (uint8_t)((seconds / 3600) % 24);

    uint16_t year = 1970;
    uint8_t month = 1;
    uint8_t day = 1;

    while (days > 365) 
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            if (days >= 366)
            {
                days -= 366;
                year++;
            } 
            else 
            {
                break;
            }
        } 
        else 
        {
            days -= 365;
            year++;
        }
    }

    while (days > 0) {

        uint8_t daysInMonth;

        switch (month) 
        {
            case 4:
            case 6:
            case 9:
            case 11:
                daysInMonth = 30;
                break;
            case 2:
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
                    daysInMonth = 29;
                else 
                    daysInMonth = 28;
                break;
            default:
                daysInMonth = 31;
                break;
        }

        if (days >= daysInMonth) 
        {
            days -= daysInMonth;
            month++;
        } 
        else 
        {
            day += days;
            break;
        }
    }

	if(day>= 32)
	{
	    month += 1;
	    day = 1;
    }
	
	if(month>=13)
	{
        year += 1;
		month = 1;
	}
	
    dt->year = year - 2000;
    dt->mon = month;
    dt->day = day;
}

/********************************************************************
 ** \brief  ���¸�ֵRTCʱ�����̣�һ������û�����û����
 ** @param  rtc_time_t��ֵ�ṹ��
 ** \retval 0Ϊ������1Ϊ�쳣
********************************************************************/
void example_set_rtc_time(rtc_time_t * date)
{
    uint8_t assigned_state;

    assigned_state = system_internal_rtc_set_time(date);

    if(assigned_state != 0)
    {
        rtc_alarm_irq_close();
        rtc_cmd_close(); 
        Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,FALSE);

        system_internal_rtc_init();

        assigned_state = system_internal_rtc_set_time(date);
    }

    if(assigned_state == Ok)        // ��ö�٣� OK = 0
    {
        rtc_state.rtc_run_state = RTC_WARNING;

        rtc_state.rtc_starting_test = TRUE;
    }
    else
    {
        rtc_state.rtc_run_state = RTC_ERROR;

        rtc_state.rtc_starting_test = FALSE;
    }
}

/********************************************************************
 ** \brief  ��ȡֱ�ӻ�ȡRTC������
 ** @param  uint32_t�����ջ�ȡ��������
 ** @param  �Ƿ��ʧ���ö�ʱ������rtc��TRUEΪ�ǣ�FALSEΪ��
 ** \retval 1Ϊ������0Ϊ�쳣
********************************************************************/
_Bool starting_run_examine_get_sec(uint32_t * data, _Bool temp_flag)
{
    rtc_time_t temp_date = {0};

    if(!open_paygo_time_read_tow_examine(&temp_date))
    {
        if(temp_flag == TRUE)
        {
            rtc_time_t timer_date_temp;

            second_conversion_date(&timer_date_temp, irq_time.lptimer_time_count);

            example_set_rtc_time(&timer_date_temp);
        }

        rtc_error_log++;    // ������¼�����������������ʹ�� @@@@@@@@@@@@@@@@@@@@@@@@

        rtc_state.rtc_starting_test = TRUE;
        
        return 0;
    }

    *data = date_conversion_second(&temp_date);

    return 1;
}


/********************************************************************
 ** \brief  ���� ��֤ʱ��
 ** \retval 1Ϊ������0Ϊ�쳣
********************************************************************/
void open_paygo_time_run_examine(void)
{
    // ��֤RTCʱ��
    if( irq_time.lptimer_test_flag == TRUE && rtc_state.rtc_starting_test == FALSE && !_system_paygo_state.paygo_unlock)
    {
        uint32_t rtc_green_time;

        uint32_t temp_time_data;
        
        _Bool temp_flag = (rtc_state.rtc_run_state==RTC_NORMAL) ? TRUE : FALSE;

        _Bool temp_examine_flag = starting_run_examine_get_sec(&rtc_green_time, temp_flag);

        // ��ȡ����ʱ��û���⣬û�зǷ�ֵ������
        if(temp_examine_flag && rtc_state.rtc_run_state == RTC_NORMAL)
        {   
            int64_t temp_TDOA = (int64_t)rtc_green_time - (int64_t)irq_time.lptimer_time_count;

            // ��ʱ����RTC��ʱ�����10������
            if(temp_TDOA > 10 || temp_TDOA < -10)
            {
                rtc_time_t timer_date_temp;

                second_conversion_date(&timer_date_temp, irq_time.lptimer_time_count);

                example_set_rtc_time(&timer_date_temp);

                rtc_error_log++;    // ������¼�����������������ʹ�� @@@@@@@@@@@@@@@@@@@@@@@@

                rtc_state.rtc_starting_test = TRUE;

                return;
            }
            else
            {
                irq_time.lptimer_time_count = rtc_green_time;

                // �õ��������ʣ������
                temp_time_data = _this_pointer._this_date_one_pointer->final_date - irq_time.lptimer_time_count;

                if(temp_time_data >= 600 && _system_paygo_state.paygo_enabled)      // ���ڻ����10����
                {
                    irq_time.irq_10_min = (temp_time_data % 600) + irq_time.lptimer_time_count;
                }
                else if(_system_paygo_state.paygo_enabled)        // С��10���ӻ��߼�ʱ���
                {
                    irq_time.irq_10_min = _this_pointer._this_date_one_pointer->final_date;
                }
                else
                {
                    irq_time.irq_10_min = irq_time.lptimer_time_count + 600;
                }
            }
        }
        else if(rtc_state.rtc_run_state == RTC_NORMAL)
        {   
            return;
        }

        if( irq_time.lptimer_flash_flag == 1 && _system_paygo_state.paygo_enabled)
        {
            _this_pointer._this_date_tow_pointer->interval_date = irq_time.lptimer_time_count;

            // ������ʱ��
            open_paygo_flash_write_date_tow(_this_pointer._this_date_tow_pointer);

            temp_time_data = _this_pointer._this_date_one_pointer->final_date - irq_time.lptimer_time_count;

            if(temp_time_data >= 1800)          // ���ڻ����30����
            {
                irq_time.irq_30_min = (temp_time_data % 1800) + irq_time.lptimer_time_count;
            }
            else
            {
                irq_time.irq_30_min = _this_pointer._this_date_one_pointer->final_date;
            }

            irq_time.lptimer_flash_flag = 0;
        }

        irq_time.lptimer_test_flag = 0;
    }

    return;
}


/********************************************************************
 ** \brief  ������֤RTC�Ƿ���������,����Ҫ��֤3��
 ** @param  ��
 ** \retval 0Ϊ����ѭ����1Ϊ����������2Ϊ�������
********************************************************************/
uint8_t open_paygo_time_starting_run_examine(void)
{
    if(rtc_state.rtc_starting_test == TRUE && irq_time.lptimer_1s_flag == 1)
    {
        if(rtc_state.rtc_read_error_cont == 0)
        {
            if(starting_run_examine_get_sec(&starting_run_examine_time, FALSE) == 0)
            {
                rtc_state.rtc_run_state = RTC_ERROR;

                rtc_state.rtc_starting_test = FALSE;
            
                starting_run_examine_time = 0;

                rtc_state.rtc_read_error_cont = 0;

                return 2;
            }

            rtc_state.rtc_read_error_cont++;
        }
        else if(rtc_state.rtc_read_error_cont == 3)
        {
            uint32_t temp_time;

            if(starting_run_examine_get_sec(&temp_time, FALSE) == 0)
            {

                rtc_state.rtc_run_state = RTC_ERROR;

                rtc_state.rtc_starting_test = FALSE;
            
                starting_run_examine_time = 0;

                rtc_state.rtc_read_error_cont = 0;

                return 2;
            }

            if(temp_time - starting_run_examine_time <= 1 \
                    && temp_time - starting_run_examine_time >= 5)
            {
                rtc_state.rtc_run_state = RTC_ERROR;

                rtc_state.rtc_starting_test = FALSE;

                starting_run_examine_time = 0;

                rtc_state.rtc_read_error_cont = 0;

                return 2;
            }

            rtc_state.rtc_run_state = RTC_NORMAL;

            rtc_state.rtc_starting_test = FALSE;

            starting_run_examine_time = 0;

            rtc_state.rtc_read_error_cont = 0;

            irq_time.lptimer_time_count = temp_time;      // lptimeͬ����ʱ��ֵ��

            return 1;

        }
        else
        {
            rtc_state.rtc_read_error_cont ++;
        }

        irq_time.lptimer_1s_flag = 0;
    }

    return 0;
}
