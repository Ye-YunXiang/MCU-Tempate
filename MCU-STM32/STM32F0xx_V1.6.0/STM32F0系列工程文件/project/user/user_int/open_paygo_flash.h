/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_flash.h
* Description:	Manage OpenPAYGO flash 
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes:        
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

// open_paygo_flash.h�ļ�
#ifndef __OPEN_PAYGO_FLASH_H_
#define __OPEN_PAYGO_FLASH_H_

#include "main_struct_typedef.h"
#include "main_file_define.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "system_seve_flash.h"


enum close_flash_flag
{
    CLOSE_FLASH_LOCK,
    CLOSE_FLASH_DATE_ONE,
    CLOSE_FLASH_DATE_TOW,
    CLOSE_FLASH_PRODUCT_TIME,
    CLOSE_FLASH_PRODUCT_ID
};


// ����ڴ�ҳ��
void open_paygo_flash_close_flash(enum close_flash_flag flag);

// �����������Լ���������
void open_paygo_flash_write_lock_state(__flash_lock * data, _Bool temp_flag);
// ��ȡ������Ϣ
_Bool open_paygo_flash_read_lock_sate(__flash_lock * data);

// ����ID
void open_paygo_flash_write_product_id(__flash_product_id * data);
// ��ȡID
_Bool open_paygo_flash_read_product_id(__flash_product_id * data);
// �������ϱ��汾�ű�־
void open_paygo_flash_write_product_id_have_reported(uint8_t * data);

// �������ʱ�䣬�ܿ���ʱ��
void open_paygo_flash_write_date_one(__flash_date_one * data);
// ��ȡ����ʱ�䡢�ܿ���ʱ��
_Bool open_paygo_flash_read_date_one(__flash_date_one * data);

// �洢���ʱ��
void open_paygo_flash_write_date_tow(__flash_date_tow * data);
// ��ȡ���ʱ��
void open_paygo_flash_read_date_tow(__flash_date_tow * data);

// �����ۼ�ʱ�䡢count
void open_paygo_flash_write_product_time(__flash_product_time * data);
// ��ȡ����ʱ�䡢count
_Bool open_paygo_flash_read_product_time(__flash_product_time * data);

#endif
