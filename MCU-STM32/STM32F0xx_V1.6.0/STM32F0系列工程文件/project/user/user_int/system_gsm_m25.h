/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_gsm_m25.h
* Description:	
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/
#ifndef __SYSTEM_GSM_M25_H_
#define __SYSTEM_GSM_M25_H_

#include "main_struct_typedef.h"
#include "main_file_define.h"



#define GSM_GET_PWRKEY()  Gpio_GetInputIO(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN)
#define GSM_GET_EN()      Gpio_GetInputIO(GSM_EN_PORT, GSM_EN_PIN)
//#define GSM_GET_EN()      GSM_EN_STATE

#define GSM_SET_EN()      Gpio_SetIO(GSM_EN_PORT, GSM_EN_PIN);
#define GSM_CLR_EN()      Gpio_ClrIO(GSM_EN_PORT, GSM_EN_PIN);
//#define GSM_SET_EN()      GSM_EN_STATE = 1
//#define GSM_CLR_EN()      GSM_EN_STATE = 0
#define GSM_SET_PWRKEY()  Gpio_SetIO(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN)
#define GSM_CLR_PWRKEY()  Gpio_ClrIO(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN)

#define GSM_POWER_RESTART_TIME  24000           // ģ�鳬ʱ����ʱ��     

//�����ջ����ֽ���
#define RX_BUF_MAX_LEN     640//1024

//��������֡�Ĵ���ṹ��
extern struct  STRUCT_LPUSART0_Fram
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
    union 
    {
        __IO uint16_t InfAll;
        struct 
        {
            __IO uint16_t FramLength       :15;
            __IO uint16_t FramFinishFlag   :1;
        } InfBit;
    }; 
	
} strM25_Fram_Record;

enum MQTT_TYPE
{
    MQTT_TYPE_NULL,         //                      0
    MQTT_TYPE_POWER_ON,     // ����                 1
    MQTT_TYPE_POWER_OFF,     // �ػ�                2
    MQTT_TYPE_POWER_RESTART,     // ����            3

    MQTT_TYPE_SEND,         // AT ����״̬          4
    MQTT_TYPE_WAIT_RX,       // AT ����״̬         5

    // ��������
    MQTT_TYPE_AT,           //                     6
    MQTT_TYPE_ATE0,         //                     7
    MQTT_TYPE_QICSGP,       // APN                  8
    MQTT_TYPE_CGATT,        // # �Ƿ񸽻�վ         9
    MQTT_TYPE_QIREGAPP,     // # ִ�н���           10
    MQTT_TYPE_QNITZ,        // ͬ������ʱ��
    MQTT_TYPE_QLTS,         // ��ȡͬ��������ʱ��
    MQTT_TYPE_CGDCONT,      // PDP                  11
    MQTT_TYPE_CGACT,        // # ����PDP            12
    MQTT_TYPE_QIACT,        // # �����ƶ�����       13
    MQTT_TYPE_QILOCIP,      // # ��ȡIP��ַ         14
    MQTT_TYPE_QISTAT,       // # �鿴����״̬       15
    
    // MQTT���Ӻͷ��͹���
    MQTT_TYPE_VERSIN,       // MQTT�汾             16
    MQTT_TYPE_KEEPALIVE,    // MQTT����             17
    MQTT_TYPE_ALIAUTH,      // MQTT�����Ʋ���       18
    MQTT_TYPE_QMTOPEN,      // TCP                  19
    MQTT_TYPE_QMTCONN,      // MQTT����             20
    MQTT_TYPE_QMTPUB,        // ������Ϣ            21

    MQTT_TYPE_QMTSUB,           // ����OTA������ȡ  22
    MQTT_TYPE_QMTPUB_VERSION,   // �ϱ��汾         23
    MQTT_TYPE_QMTPUB_OTA,       // OTA��ȡ          24

    // OTA��������
    MQTT_TYPE_QHTTPURL,      // ��������            25
    MQTT_TYPE_QHTTPGET,      // ��ȡ������Դ        26
    MQTT_TYPE_QHTTPDL,       // ���浽�ڴ���        27
    MQTT_TYPE_QFOPEN,        // �򿪸ձ����bin     28
    MQTT_TYPE_QFREAD,        // ��ȡ�򿪵��ļ�      29

    // ��ѯ�ź�
    MQTT_TYPE_CSQ          // �ź�                  30
};

enum OTA_TYPE
{
    OTA_TYPE_NULL,          // ������
    OTA_TYPE_UPGRADE_TASK,  // ����������
    OTA_TYPE_BEING_UPDATED  // ������������ֹ����
};

//��������֡�Ĵ���ṹ��
extern struct STRUCT_MQTT_STATE
{
    // 0�������� 1����������, 2��������
    uint8_t mqtt_ota_state;     // ota״̬����

	uint8_t mqtt_at_type;       // ����mqtt��������  ����ָ�������
    uint8_t mqtt_task_state;    // ����mqtt����״̬  �������ػ������͡����ա�������
    uint8_t mqtt_unsent_count;    // ������δ���ʹ�����3����û���ӻ�վ�ɹ���������TCPʧ�ܾ�����������3�ξ͹ػ���
    uint16_t mqtt_task_state_count_down;    // ר����������ʱ��ʱʱ�䣬һ������Ϊ5ms
    uint16_t mqtt_wait_receive_count;    // �ȴ��ظ���ʱʱ��

    _Bool mqtt_cyclic_send;  // �Ƿ��GSM�ķ��ͣ��ر�����������û�ж�Ҫѭ�����ͣ���ʱ������
} mqtt_state;


struct GSM_MQTT_PARAM           // ����mqqt���Ͳ���
{
    uint16_t count;             
    uint16_t time_remaining_hour;
    uint16_t time_remaining_min;
    float    BAT_VDC;
    float    DC_VDC;
    int    refrigerator_thermometer;      //�����¶�
    _Bool    power_state;   // ��ǰ״̬
};

extern uint32_t gsm_power_restart_time;

extern uint32_t gsm_power_sending_time_count;

extern _Bool GSM_EN_STATE;


// MQTT����ƴ�ӣ�������Ҫƴ�ӵ���Ϣ��ͨ������
void system_gsm_mqtt_send_splicing(enum MQTT_TYPE temp_mqtt_type);

// ���ػ��͹������ų�ʼ��
void system_gsm_port_init(void);
// ����/�ػ�����
void system_gsm_enabled_state(void);
// ����
_Bool system_gsm_enabled_power_on(void);
// �ػ�
_Bool system_gsm_enabled_power_off(void);

// ���ý���ػ�����
void system_gsm_ready_to_shut_down(void);
// ���ý�����������
void system_gsm_ready_to_restart(void);

// ��ȡOTA �� URL
void system_gsm_mqtt_get_uart(void);
// MQTT���ʹ���
void system_gsm_mqtt_send(void);
// MQTT���մ���
void system_gsm_mqtt_receive(void);

// MQTT�����̿���
void system_gsm_mqtt_manage(void);

#endif
