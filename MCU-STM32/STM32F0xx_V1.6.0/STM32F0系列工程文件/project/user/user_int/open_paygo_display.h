/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_display.c 
* Description:	Manage the Open PAYGO state display
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#ifndef __OPEN_PAYGO_DISABLE_H_
#define __OPEN_PAYGO_DISABLE_H_

#include "main_file_define.h"

#define UI_DISPLAY_TIMEE	20

enum DISPLAY_STATE{
    DISPLAY_FALSE,              // ɶҲû��
    DISPLAY_SLOGAN,             // �������
    DISPLAY_UP_LOADING,         // �����ȴ�
    DISPLAY_NO_ID,              // û��id��ʱ�򣬲���û�����
    DISPLAY_NO_PAYGO,           // ��id����û������ʱ��
    DISPLAY_RTC_ERROR,           // RTC������

    DISPLAY_ACTIVATED_TIME,      // ���ʱ
    DISPLAY_WAIT,               // �ȴ�
    DISPLAY_UNLOCK,              // unlok

    DISPLAY_NO_TOKEN,           // �����Tokenû��
    DISPLAY_TOKEN_DISABLE,      // ������disable��
    DISPLAY_TOKEN_ID,           // �����˲鿴ID
    DISPLAY_TOKEN_COUNT,        // ����鿴count
    DISPLAY_TOKEN_COUNT_CLOSE,   // ���������count

    DISPLAY_TOKEN_EFFICACIOUS,   // TOKEN������Ч

    DISPLAY_KEYBOARD_LOCK,        // ��������

    DISPLAY_TAMPER          // ��Ǳ���
};


typedef struct display_state
{
    uint16_t display_day;
    uint8_t display_hour;
    // uint16_t all_display_min;
    
    _Bool display_state_flag;
    _Bool display_state_activity;
    enum  DISPLAY_STATE display_state_token_enum;
    enum  DISPLAY_STATE display_state_enum;
}display_state_t;

extern display_state_t display_state_manage;

// ��ʼ��ʾ����ṹ��
void open_paygo_display_struct(void);

// ������ʾ
void open_paygo_display_startup(enum DISPLAY_STATE state);

// �����й�����ʾ
void open_paygo_display_running_manage(void);

// ���й�����RTC���ˣ���ʾ
void open_paygo_display_rtc_error_run_manage(void);

// LCD����������
void open_paygo_display_light_time_manage(void);

// led����˸����
void open_paygo_display_led_activity_blink_manage(void);

// ������ʾ����
void open_paygo_token_data_display(char temp_last_key,uint8_t temp_num);


#endif

