/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_time.h
* Description:	Manage OpenPAYGO time calculations
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/
#ifndef __OPEN_PAYGO_TIME_H_
#define __OPEN_PAYGO_TIME_H_

#include "main_struct_typedef.h"
#include "main_file_define.h"


#define GREEN_TIME_2023_1_1_0_0_0       1672502400
#define GREEN_TIME_2070_1_1_0_0_0       3155731200

enum RTC_STATE
{
	RTC_NORMAL,
	RTC_WARNING,
	RTC_ERROR
};

typedef struct
{
    uint8_t     rtc_run_state;          // RTC����״̬
    _Bool       rtc_starting_test;      // RTC��������ź�
    _Bool       rtc_run_test_flag;      // ����ʱ�����ź�
    uint8_t     rtc_read_error_cont;    // RTC����״̬������
} rtc_state_t;

// 0 δ������ 1 ����
typedef struct
{
    uint32_t    lptimer_time_count;      // ÿ���ʱ
    uint32_t    irq_10_min;
    uint32_t    irq_30_min;
    

    _Bool       lptimer_test_flag;       // �ﵽ������Ի��ߴ洢���ź�
    _Bool       lptimer_flash_flag;         // �ﵽ������Ի��ߴ洢���ź�

    _Bool       lptimer_led_1s_flag;         // LED��ÿ���ź�
    _Bool       lptimer_1s_flag;         // rtc��ÿ���ź�
    uint8_t     lptimer_2s_flag;         // rtc��2���ź�
    _Bool       timer_250ms_flag;
    uint8_t     lcd_light_up_time;       // lcd�Ƶ�����ʱ��

    uint16_t    timer_5_ms_count;       // 5ms��ʱ

    // * ˯����� **********************************************
    _Bool       sleep_open;            // ��ʱ��״̬ ��˯��ģʽ��������ģʽ
    uint8_t     lp_sleep_count;          // ���Ѵ���
} irq_time_t;


// �жϼ���
extern volatile irq_time_t irq_time;
// rtc״̬
extern volatile rtc_state_t rtc_state;

extern uint8_t rtc_error_log;


// ����ṹ��rtc_state and irq_time
void open_paygo_struct_init(void);

// �Ƚϴ洢ʱ���rtc_time_t�ṹ���Ա�Ƿ�һ��
_Bool compare_struct_rtc_time_t(const rtc_time_t *s1, const rtc_time_t *s2);
// ��� rtc_time_t ��֯��Ա�Ƿ��зǷ�ֵ
_Bool struct_rtc_time_invalid_value_examine(const rtc_time_t *s1);
// �������ʱ�䵽���ڵ�����
uint32_t date_conversion_second(rtc_time_t *s1 );
// ����ʱ����������Ϊ����
void second_conversion_date(rtc_time_t * date, time_t timestamp);
// ���¸�ֵRTCʱ��
void example_set_rtc_time(rtc_time_t * date);
// ��ȡֱ�ӻ�ȡRTC����������֤RTC�Ƿ�����������
_Bool starting_run_examine_get_sec(uint32_t * data, _Bool temp_flag);
// ��ȡRTC��������֤�����ֵ���ṹ��
_Bool open_paygo_time_read_tow_examine(rtc_time_t * date );

// ���� ��֤ʱ��
void open_paygo_time_run_examine(void);
// ��֤RTC�Ƿ���������
uint8_t open_paygo_time_starting_run_examine(void);


#endif
