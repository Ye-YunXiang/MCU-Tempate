/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName:     system_run_manage.h
* Description:  Managing system running status
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#ifndef __SYSTEM_RUN_MANAGE_H_
#define __SYSTEM_RUN_MANAGE_H_

#include "main_struct_typedef.h"
#include "main_file_define.h"

// ��¼ID���̹���
void system_input_ID_manage(void);

// ���ؽ��յ���ID
void returns_the_uart_receive_id(void * pointer, _Bool RECEIVE_DONE);

// �������ÿ��λת��Ϊ1��HEX
void array_to_hex_bit(char * Array_data, uint8_t Array_len, char * temp_skid);

// ���ַ�����ת��Ϊһ������
uint64_t array_to_int32_bit(char *Array_data, unsigned char Array_len);

// ��ȡ�������ֵ
int get_key_pressed(void);

// �ȴ���������
uint64_t wait_for_token_entry(void);

// ���п���
void system_console_run_manage(void);

#endif
