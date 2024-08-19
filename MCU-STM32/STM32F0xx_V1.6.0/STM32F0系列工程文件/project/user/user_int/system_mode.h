#ifndef _SYSTEM_MODE_H_
#define _SYSTEM_MODE_H_

#include "main_file_define.h"

#include "lpm.h"

enum SLEEP_MODE_ENUM
{
    _SYSTEM_BOOT_SCREEN,    // ϵͳ������Ļ
    _SYSTEM_BOOT_RTC,       // ϵͳ����RTC
    _SYSTEM_BOOT_VERIFY,    // ϵͳ������֤
    _SYSTEM_RUN,            // ϵͳ����
    _SYSTEM_SLEEP,          // ϵͳ����
    _SYSTEM_SLEEP_ROUSE,     // ϵͳ˯�߻���

    _SYSTEM_TAMPER_OPEN     // ��Ǳ���
};

struct SLEEP_MODE_STRUCT
{
    // �豸״̬:TRUE �ϵ磬FALSE û�ϵ�
    _Bool device_power_on_state;
    // ����״̬
    uint8_t run_state;
	// ��ƽ���״̬��ת
	_Bool state_flip_5v;
    // ��Ǳ���   1 ��Ǳ��򿪡�
    _Bool tamper_open;
    // MQTT�Ƿ��ϱ�
    _Bool mqtt_tamper_open;

};

extern struct SLEEP_MODE_STRUCT	__sleep_mode;


// ϵͳ���������ʼ��
void system_mode_peripherals_init(void);

// id��¼���Ƿ��
void system_mode_ID_uart_select_open(void);

// ���Ѻ�m25�ϱ���ʼ��
void system_mode_rouse_reported_init(void);

// ϵͳ���ѻ�˯������,TRUEΪ�������̣�FALSEΪȫ��״̬
void system_mode_wakeUP_or_sleep_prrocess(_Bool state);

//  ������ʼ����������
void system_mode_IO_init(void);

//  ����˯����������
void system_mode_sleep_IO_init(void);

// �ػ�����
void system_mode_power_off_set(void);

// �жϽ������˯��
void system_mode_judge_depp_sleep_open(void);

#endif
