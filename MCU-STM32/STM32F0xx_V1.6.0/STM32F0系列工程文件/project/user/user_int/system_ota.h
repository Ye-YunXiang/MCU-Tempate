#ifndef __SYSTEM_OTA_H_
#define __SYSTEM_OTA_H_

#include "main_file_define.h"


/*--------------------------------------------------------------------
 *---CRC16 У���㷨
*---------------------------------------------------------------------*/
#define CRC_16_XMODEM  ota_bin_crc_check

// CRCУ���õ����ļ�
// ����: ��ʼֵ������ʽ��������ֵ�����뷴ת�������ת����ʼָ�롢���ݳ���
// ������ǰ���Ҫ��������������ݲ����Լ�����
// ������ʹ��XMODEM��׼��ota_bin_crc_check(0,0x1021,0,false,false,puchMsg,usDataLen)
uint16_t ota_bin_crc_check(uint16_t wCRCin, uint16_t wCPoly, uint16_t wResultXOR, \
                            _Bool input_invert, _Bool ouput_invert, const uint8_t *puchMsg, uint32_t usDataLen);

/*--------------------------------------------------------------------
 *---ϵͳ��λ
*---------------------------------------------------------------------*/
void __OTA_SYSTEM_RESTART(void);

/*--------------------------------------------------------------------
 *---��ʼ������
*---------------------------------------------------------------------*/
// ��ȡ bootloader����
void bootloader_read(void);
// д��bootloader����
void bootloader_write(void);
// ���� bootloader�ڵ���Ϣ
void bootloader_manage(void);

// ������п��ܻᷢ���ڴ�й©��ȫ�ֶ�̬��
void ota_close_global_interrupt(void);


/*--------------------------------------------------------------------
 *---OTA �ļ���֤����
*---------------------------------------------------------------------*/
//@@@@@ У���ļ����л����� @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void ota_bin_checkout(void);

// ����bin�ļ�
void ota_write_bin_data(uint8_t * data, uint16_t len, uint32_t addr_move);

// ����bin�ļ�У����
void ota_set_bin_crc(uint8_t * bin_head, uint8_t crc_length);

// ��ȡbin�ļ�
void ota_set_bin_file(uint8_t * bin_head);

// �������ص�bin�ļ���С
void ota_set_gsm_bin_size(uint32_t size);

/*---------------------------------------------------------------
 *--get end set bootloader ������
*-------------------------------------------------------------*/
// �õ�ƫ����
uint32_t ota_get_ota_bin_move(void);

// �õ��ļ���С
uint32_t ota_get_ota_bin_size(void);

// ����Ƿ��Ѿ��л�����
uint8_t bootloader_get_handover_performed(void);

// ����Ƿ����ʧ��
uint8_t bootloader_get_fail(void);

// �������ʧ�ܱ�־
void bootloader_clear_fail(void);

#endif
