/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_manage.h
* Description:	Manage the Open PAYGO logic
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#ifndef __OPEN_PAYGO_MANAGE_H_
#define __OPEN_PAYGO_MANAGE_H_

#include "main_struct_typedef.h"
#include "main_file_define.h"

// openPAYGO �����ļ�
#include "opaygo_core_extended.h"
#include "opaygo_decoder_extended.h"
#include "opaygo_core.h"
#include "siphash.h"
#include "opaygo_decoder.h"
#include "opaygo_value_utils.h"
#include "restricted_digit_set_mode.h"


#define NO_ENTER_TOKEN_VALUE			0xFFFFFFFFFFFFFFFF

#define LOCKING_TIME   60


// �����ڲ����еĽṹ��
typedef struct
{
    __flash_lock * _this_lock_pointer;
    // ��һ��ʱ�䣬������ʱ��
    __flash_date_one * _this_date_one_pointer;
    // ����洢ʱ��λ�ã�����洢ʱ���źţ�����洢ʱ��
    __flash_date_tow * _this_date_tow_pointer;
    // �ۼ�ʱ�䣬count
    __flash_product_time * _this_product_time_pointer;
    // ��Կ��starting code, ID
    __flash_product_id * _this_product_id_pointer;
}__this;

extern __this _this_pointer;

typedef struct
{
    _Bool paygo_id;           // ��û��PAYGO  ID
    _Bool paygo_enabled;      // ��û���ڼ�ʱ
    _Bool paygo_unlock;       // PAYGO ��û�� unlock
}__system_paygo_state;

extern __system_paygo_state _system_paygo_state;


//  ��ʼ������PAYGO�洢�ṹ���Լ��ڴ�
void op_manage_init_all_PAYGO_value(void);


// ��������1�������������д洢��flash�еı���
void op_manage_load_activation_variables(void);

// ��������2����������У׼ paygo ʱ��
void op_manage_rouse_calibration_time_and_display(uint8_t flow);
// ��������2.1 ��������RTC��������������ж�
void op_manage_rtc_error_manage(void);
// ��������2.2 ��������ʼ���ж�ʱ��
void op_manage_rouse_irq_time_init(void);

// ��������3 ��������֤�汾��
void op_manage_verify_version_numbe(const uint8_t *temp_version_number, _Bool temp_falg);

// ���е���ʾ״̬��ʱ�����
void op_manage_run_display_state_and_time(void);
// ��֤Token������Ƿ�Ϊ���������Ҫ����
void op_manage_recognition_input_token(uint32_t Input_Token_Data);

// PAYGO������״̬����
void op_manager_paygo_credit_and_workqueue(void);

// ������ʱ��
// �������ڼ�ʱ״̬������û��unlock������²��ܴ�
void op_manage_write_date_tow(void * struct_data);

// ����ID
void op_manage_set_flash_id(uint32_t * temp_scid, uint32_t * temp_suid, char * temp_skid, char * temp_pkid, char * temp_dsid);

// ��Կ����
int op_manage_product_get_openpaygo_activation_value(uint64_t input_token, uint16_t * temp_count);

// �Խ�����������Կ�����ж�
void op_manage_product_opepaygo_token_judge_result(int temp_credit_remaining, uint16_t temp_count);

// �������
void op_manage_add_activation_time(int activation_value);

// ���ø�������
void op_manage_set_activation_time(int activation_value);

// �ж�PAYGO���޶�ȣ����ڴ�M25����Ϣ
_Bool op_manage_PAYGO_do_odd_times(void);

// ��Ǵ���ʾ����
void op_manage_tamper_open_manage(void);


// ���������ɹ�����1��ʧ�ܷ���0 
// flag��0��Ϊ�������trueΪ��֤�Ƿ�����
_Bool op_manage_lock_control(_Bool temp_flag);


// ����MQTT������
void op_manage_get_mqtt_param(void * temp_param);

#endif
