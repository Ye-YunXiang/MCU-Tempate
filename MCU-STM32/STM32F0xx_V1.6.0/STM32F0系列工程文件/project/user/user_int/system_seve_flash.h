/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_seve_flash.h
* Description:	initialize FLASH
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#ifndef __SYSTEM_SEVE_FLASH_H
#define __SYSTEM_SEVE_FLASH_H

#include "main_file_define.h"

// ҳ251��1F600 - 1F7FF     ҳ252��1F800 - 1F9FF
// ҳ253��1FA00 - 1FBFF     ҳ254��1FC00 - 1FDFF    ҳ255��1FE00 - 1FFFF

// ������251ҳ,�洢���Ƿ�������0Ϊ��������������������������������ʱ��
#define FLASH_ADDR_LOCK_STATE                           (0x0001F600)
#define FLASH_ADDR_LOCK_ERROR                           (0x0001F604)
#define FLASH_ADDR_LOCK_DATE                            (0x0001F608)

// ����ʱ��252ҳ���洢����һ�γ�ֵʱ�䡢�ܿ���ʱ�䡣
#define FLASH_ADDR_DATE_FINAL_DATE                      (0x0001F800)
#define FLASH_ADDR_DATE_SUM_AVAILABLE_TIME              (0x0001F808)

// ����ʱ��253ҳ���洢��ʱ��λ�ã�7����ַ����30���Ӵ洢��ʱ�䣨8����ַ����
#define FLASH_ADDR_DATE_LOCATION_DATE                   (0x0001FA00)
#define FLASH_ADDR_DATE_INTERVAL_DATE                   (0x0001FA08)      // 8����ַ8����ַ�����ϼ�

// ��ֵ�ۼƣ�254ҳ���洢���ۼ�ʱ�䣨1����ַ����countֵ��1����ַ����
#define FLASH_ADDR_PRODUCT_TIME_ADD_TIME                (0x0001FC00)
#define FLASH_ADDR_PRODUCT_TIME_TOKEN_COUNT             (0x0001FC04)

// �洢ID��255ҳ�����10����ַ��
// ÿ���ĳ��ȶ��ǹ̶��ġ�
#define FLASH_ADDR_PRODUCT_ID_STARTING_CODE             (0x0001FE00)      // Starting Code
#define FLASH_ADDR_PRODUCT_ID_UNIT_NUMBER_CODE          (0x0001FE10)      // Unit Number Code
#define FLASH_ADDR_PRODUCT_ID_SECRET_KEY                (0x0001FE20)      // Secret Key       16��ַ
#define FLASH_ADDR_PRODUCT_ID_DEVICE_SECRET             (0x0001FE40)
#define FLASH_ADDR_PRODUCT_ID_PRODUCT_KEY               (0x0001FE70)
#define FLASH_ADDR_PRODUCT_ID_HAVE_REPORTED             (0x0001FE90)      // �Ƿ��ϱ����汾��
#define FLASH_ADDR_PRODUCT_ID_VERSION                   (0x0001FEA0)      // �洢�汾��

// ����ṹ����ý����͵ķ�ʽ�洢���ڴ�
#pragma pack(1)

// �洢�Ƿ������������                ���������޸ģ�
struct flash_lock
{
	uint16_t lock_date;           // ��������ʱ�䣺������Զ���ΪĬ������30���ӣ���ǰʱ�����30���ӵ���
    uint8_t lock_error;             // ����������,�洢�ľ��������֣�ÿ�δ洢ÿ�β���
   
};

// �洢����ʱ�ı�������һ�δ洢ʱ��      ���������޸ģ�
struct flash_date_one
{
    uint32_t available_time;        // ����ʹ��ʱ�䣬�����
    uint32_t final_date;         // ����ʱ���������ʱ���

};

// �洢����30���Ӵ洢һ�ε�ʱ��     ���������޸ģ�
struct flash_date_tow
{
    uint32_t  interval_date;      // ���洢ʱ��ʱ���
    uint8_t  location_date[7];      // ��Ӵ洢ʱ���ַ,ע���ʼ��������ʱ��ȫ����ֵΪ255
};

// �洢ϵͳ��صģ����۳�ֵʱ�䡢countֵ    ���������޸ģ�
struct flash_product_time
{
    uint16_t add_time;              // �ۼƳ�ֵʱ��
    uint16_t token_count;           // Token Count

};

// ��ϵͳ��صģ�����IDֵ   ���������޸ģ�
struct flash_product_id
{
    uint8_t  secret_key[16];        // ��Կ
    uint32_t starting_code;         // ������
    uint32_t unit_number_code;      // ID
    uint8_t  device_secret[32];
    uint8_t  product_key[11];
    
    // ����OTA���
    uint8_t  have_reported;         // �Ƿ��ϱ����汾�ţ�0Ϊ��
    uint8_t  version[8];            // �汾��
};


// ֻ����������
#pragma pack()

/********************************************************************************
 ** ����   
 *******************************************************************************/

// (����Ϊuint8��һ���ֽ�)�洢�������ݣ������洢��ַ��Դ���ݵ�ַ�� ���鳤��
void system_flash_write_array_data(volatile uint32_t addr, volatile uint8_t * data,  uint16_t len);  
// (����Ϊuint8��һ���ֽ�)��ȡ��������, ������ȡ��ַ���洢���ݵ�ַ�����鳤��
void system_flash_read_array_data(volatile uint32_t addr, volatile uint8_t * data, uint16_t len);       

// �����������洢ʱ��ĵ�ַ�м���0,ǰ6����ַ���λΪ0�������ﶼ�����ˣ�������ĵ�ַ��
// �洢��ַƫ����Ϊ8�������м����㣬��ƫ�Ƽ���8��ַ����Ϊ����洢ʱ��ĵ�ַ��
uint32_t calculate_interval_date_addr(volatile uint8_t * data);

// ���ָ��ҳ
void system_write_flash_close(volatile uint32_t addr);
/********************************************************************************
 ** ҵ���߼�   
 *******************************************************************************/


void system_write_flash_lock_close(void);
// д����������������
void system_write_flash_lock_error(uint8_t * data);
// д������ʱ�䣬�Լ�����״̬
void system_write_flash_lock_state(struct flash_lock * data);
// ��ȡ�������
void system_read_flash_lock_state(struct flash_lock * data);

// ���ҳ252
void system_write_flash_date_one_close(void);
// �洢����ʱ�䡢�ܿ���ʱ��
void system_write_flash_date_one(struct flash_date_one * data);
// ��ȡ����ʱ�䡢�ܿ���ʱ��
void system_read_flash_date_one(struct flash_date_one * data);

// ���ҳ253
void system_write_flash_date_tow_close(void);
// �洢��Ӵ洢��ַ����Ӵ洢ʱ��
void system_write_flash_date_tow(struct flash_date_tow * data);
// �洢��Ӵ洢ʱ���־λ
void system_write_flash_date_tow_flag(uint8_t * add);
// ��ȡ��Ӵ洢��ַ����Ӵ洢ʱ��,temp_flagΪ1����У���־λ��Ϊ0ֱ�Ӷ�ȡ
void system_read_flash_date_tow(struct flash_date_tow * data, _Bool temp_flag);

// ���ҳ254
void system_write_flash_product_time_close(void);
// һ��д������
void system_write_flash_product_time_data(struct flash_product_time * data);
// һ�ζ�ȡ����
void system_read_flash_product_time_data(struct flash_product_time * data);

// ���ҳ255
void system_write_flash_product_id_close(void);
// һ��д������
void system_write_flash_product_id_data(struct flash_product_id * data);
// һ�ζ�ȡ����
void system_read_flash_product_id_data(struct flash_product_id * data);
// д�����ϱ�����
void system_write_flash_product_id_have_reported(uint8_t *data);




#endif
