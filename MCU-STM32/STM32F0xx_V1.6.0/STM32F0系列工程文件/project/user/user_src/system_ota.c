#include "system_ota.h"

/*************************************************************************
 * �����ַ��,�ݶ�
 * bootloader 9.5k sector0~29     0x00 ~ 0x25FF
 * bootloader data                0x2600 ~ 0x27FF   // �����洢����OTA������
 * APP1       40k sector20~99     0x2800 ~ 0xC7FF
 * APP2       40k sector100~179   0xC800 ~ 0x167FF
**/

// bootloader ͷ�ļ���ַ
#define BOOTLOADER_ADDR                         (0x0)
#define BOOTLOADER_SIZE                         9728   // 9.5k
#define BOOTLOADER_DATA_ADDR                    (0x2600)
#define BOOTLOADER_DATA_SIZE                    512    // 512byte

#define BOOTLOADER_DATA_APP_ADDR                (0x2610)   // APP   0/0xFF ΪAPP1��1 ΪAPP2
// ���µĶ���׼��������
// #define BOOTLOADER_DATA_SWITCHOVER_ADDR         (0x2611)   // �Ƿ��л�APP  0/0xFF ���л�, 1 Ϊ�л�
// #define BOOTLOADER_DATA_HANDOVER_PERFORMED_ADDR (0x2612)   // �Ƿ��Ѿ��л�APP  0/0xFF û���л�, 1 Ϊ�Ѿ��л�
// #define BOOTLOADER_DATA_FAIL_ADDR               (0x2613)   // �Ƿ����ʧ��  0/0xFF û����, 1 Ϊʧ��

// APP1 ͷ�ļ���ַ  
#define APP_1_ADDR                              (0x2800)
#define APP_1_SIZE                              40960   // 40k
// APP���ػ�����
#define APP_2_ADDR                              (0xC800)
#define APP_2_SIZE                              40960   // 40k

// ����ṹ����ý����͵ķ�ʽ�洢���ڴ�
#pragma pack(1)
struct SYSTEM_OTA
{
	uint8_t app;						// ûɶ�ã�������Ϊ�ṹ��ͷ
    uint8_t switchover;                 // �Ƿ��л�
    uint8_t handover_performed;         // �Ƿ��Ѿ��л�
    uint8_t fail;                       // ����ʧ�ܱ�־λ  
};
#pragma pack()
// bootloader�洢����Ϣ
struct SYSTEM_OTA __system_ota = {0};

struct OTA_UPGRADE_VALUE
{
    // ���ص��ļ���С
    uint32_t ota_bin_size ;
    // bin�ļ��洢��ַƫ����
    uint32_t ota_bin_move;
    // bin�ļ�У����
    uint16_t ota_bin_crc;
};
struct OTA_UPGRADE_VALUE __ota_upgrade = {0};


/*--------------------------------------------------------------------
 *---ϵͳ��λ
*---------------------------------------------------------------------*/
void __OTA_SYSTEM_RESTART(void)
{
    NVIC_SystemReset();

    while(1)
    {
        __NOP();
    }
}

/*---------------------------------------------------------------------------------------------------
 *---��ʼ������
*--------------------------------------------------------------------------------------------------*/
// ��ȡ bootloader���� ,�ṹ��һ��Ҫ�ֽڶ��룡����
void bootloader_read(void)
{
    uint8_t flash_read_sate = 1; 
    
    struct SYSTEM_OTA temp_data;

    do{
        struct SYSTEM_OTA temp_data_1;

        system_flash_read_array_data(BOOTLOADER_DATA_APP_ADDR, &temp_data_1.app, sizeof(struct SYSTEM_OTA));

        system_flash_read_array_data(BOOTLOADER_DATA_APP_ADDR, &temp_data.app, sizeof(struct SYSTEM_OTA));

        flash_read_sate = memcmp(&temp_data.app, &temp_data_1.app, sizeof(struct SYSTEM_OTA));
    }
    while(flash_read_sate != 0);

    memcpy(&__system_ota, &temp_data, sizeof(struct SYSTEM_OTA));
}

// д��bootloader����  �ṹ��һ��Ҫ�ֽڶ��룡����
void bootloader_write(void)
{
    uint8_t flash_write_sate = 1; 
    
    struct SYSTEM_OTA temp_data;

    do{
        system_write_flash_close(BOOTLOADER_DATA_APP_ADDR);

        system_flash_write_array_data(BOOTLOADER_DATA_APP_ADDR, &__system_ota.app, sizeof(struct SYSTEM_OTA));

        system_flash_read_array_data(BOOTLOADER_DATA_APP_ADDR, &temp_data.app, sizeof(struct SYSTEM_OTA));  

        flash_write_sate = memcmp(&__system_ota, &temp_data, sizeof(struct SYSTEM_OTA));
    }
    while(flash_write_sate != 0);
}

// ���� bootloader�ڵ���Ϣ
// �������Ҫ�ж��Ƿ����������Ƿ����л��³���ȵ�
void bootloader_manage(void)
{
    if(__system_ota.switchover == TRUE && __system_ota.handover_performed == TRUE)
    {
        // ����л��ɹ��������־λ�����߻ص�bootloader���ֱ�־λ�����ǻ�ص���
        __system_ota.switchover = FALSE;
        __system_ota.handover_performed = FALSE;

        bootloader_write();
    }
}

// ����о�û��Ҫ����Ϊ�Ͳ���������������Է���һ�����Űɡ�
// ������п��ܻᷢ���ڴ�й©��ȫ�ֶ�̬��
void ota_close_global_interrupt(void)
{
	system_uart0_config_off();

    // �����ʱ״̬�µļ��ʱ��
    op_manage_write_date_tow(NULL);

}

/*---------------------------------------------------------------------------------------------------
 *---OTA �ļ���֤����
*-------------------------------------------------------------------------------------------------*/
/********************************************************************
 ** \brief  (����Ϊuint8��һ���ֽ�)����bin�ļ�������
 ** @param  Դ���ݵ�ַ
 ** @param  ���鳤��
 ** @param  ��ַƫ��
 ** \retval ��
********************************************************************/
void ota_write_bin_data(uint8_t * data, uint16_t len, uint32_t addr_move)
{
    // ֱ�ӽ�������
    uint32_t write_addr = APP_2_ADDR + addr_move;

    uint8_t flash_write_sate = 1; 

    do{
        system_write_flash_close(write_addr);

        system_flash_write_array_data(write_addr, data, len);

        flash_write_sate = memcmp(data, ((uint8_t *)write_addr), len);
    }
    while(flash_write_sate != 0);
}


/********************************************************************
 ** \brief  У��bin�ļ�
 ** \retval ��
********************************************************************/
//@@@@@ У���ļ����л����� @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void ota_bin_checkout(void)
{
    // ֱ����֤������
    volatile uint32_t write_addr = APP_2_ADDR;

    // ��֤
    uint16_t crc = CRC_16_XMODEM(0,0x1021,0,false,false, ((uint8_t *)write_addr), __ota_upgrade.ota_bin_size);

   if(crc == __ota_upgrade.ota_bin_crc && __ota_upgrade.ota_bin_size > 1024)     // У��ͨ��
   {
        __system_ota.switchover = TRUE;

        bootloader_write();
	   
	    // �ر�����ʹ�õ��жϵķ���
	 	ota_close_global_interrupt();

        // �������
        __OTA_SYSTEM_RESTART();
   }
   else   // У��ʧ��
   {
        __system_ota.fail = TRUE;
        bootloader_write();

        // ���ý�����������
        system_gsm_ready_to_restart();
   }
}


// ����bin�ļ�У����
void ota_set_bin_crc(uint8_t * bin_head, uint8_t crc_length)
{
    uint8_t i = 0;
    // ָ�����������������ݲ�ͬģ��ظ�����Ϣ��������
    do{
        bin_head++;
    } while (*bin_head != 0x0a && *bin_head != 0x0d && *bin_head != 0x20);
    // ����\r\n
    bin_head = bin_head + 2;

    __ota_upgrade.ota_bin_crc = 0;

    // ��ʼ��ȡУ���룡������
    for(i = 0; i < crc_length; i++)
    {
        __ota_upgrade.ota_bin_crc = (__ota_upgrade.ota_bin_crc * 0x100) + *bin_head;
        bin_head++;
    }

    // ʵ��bin�ļ�ȥ��У�����С
    __ota_upgrade.ota_bin_size = __ota_upgrade.ota_bin_size - crc_length;

    // ��ʼУ������
    ota_bin_checkout();
}


// ��ȡbin�ļ�
void ota_set_bin_file(uint8_t * bin_head)
{
    uint16_t temp_bin_size = 0;
    do{
        // �����ڿո�\r��\n
        if(*bin_head != 0x0a && *bin_head != 0x0d && *bin_head != 0x20)
        {
            temp_bin_size = temp_bin_size * 10 + (*bin_head - '0');
        }
        bin_head++;
    } while (*bin_head != 0x0a && *bin_head != 0x0d && *bin_head != 0x20);
    // ����\r\n
    bin_head = bin_head + 2;

    if(temp_bin_size != 0)   // ����bin�ļ�
    {
        // ��ʼ��ȡbin�ļ�  // ����һ���ͻ�ȡ�ļ�һ����Ķ�̬�ռ�
        uint8_t *temp_bin_file = (uint8_t *)malloc(temp_bin_size);
        memcpy(temp_bin_file, bin_head, temp_bin_size);
        
        ota_write_bin_data(temp_bin_file, temp_bin_size, __ota_upgrade.ota_bin_move);
        __ota_upgrade.ota_bin_move += temp_bin_size;
        
        free(temp_bin_file);
    }
    else   // ������bin�ļ�
    {
        ota_bin_checkout();
    }
}

// CRCУ���õ����ļ�
// ����: ��ʼֵ������ʽ��������ֵ�����뷴ת�������ת����ʼָ�롢���ݳ���
// ������ǰ���Ҫ��������������ݲ����Լ�����
// ����ʹ�õ���XMODEM�ı�׼��ota_bin_crc_check(0,0x1021,0,false,false,puchMsg,usDataLen)
uint16_t ota_bin_crc_check(uint16_t wCRCin, uint16_t wCPoly, uint16_t wResultXOR, \
                            _Bool input_invert, _Bool ouput_invert, const uint8_t *puchMsg, uint32_t usDataLen)
{
    uint8_t wChar = 0;
    while (usDataLen--)
    {
        wChar = *(puchMsg++);
        if(input_invert)//����ֵ��ת
        {
            uint8_t temp_char = wChar;
            wChar = 0;
            for(int i = 0; i < 8; ++i)
            {
                if(temp_char&0x01)
                    wChar |= 0x01<<(7-i);
                temp_char>>=1;
            }
        }
        wCRCin ^= (wChar << 8);
        for (int i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
                wCRCin = (wCRCin << 1) ^ wCPoly;
            else
                wCRCin = wCRCin << 1;
        }
    }
    if(ouput_invert)
    {
        uint16_t temp_short = wCRCin;
        wCRCin=0;
        for(int i=0;i<16;++i)
        {
            if(temp_short&0x01)
                wCRCin|=0x01<<(15-i);
            temp_short>>=1;
        }
    }
    return (wCRCin^wResultXOR);
}

// �������ص�bin�ļ���С
void ota_set_gsm_bin_size(uint32_t size)
{
    __ota_upgrade.ota_bin_size = size;
    // ƫ����ָ��
    __ota_upgrade.ota_bin_move = 0;
}

/*---------------------------------------------------------------------------------------------
 *--get end set bootloader ������
*--------------------------------------------------------------------------------------------*/
// �õ�ƫ����
uint32_t ota_get_ota_bin_move(void)
{
    return __ota_upgrade.ota_bin_move;
}

// �õ��ļ���С
uint32_t ota_get_ota_bin_size(void)
{
    return __ota_upgrade.ota_bin_size;
}

// ����Ƿ��Ѿ��л�����
uint8_t bootloader_get_handover_performed(void)
{
    return __system_ota.handover_performed;
}

// ����Ƿ����ʧ��
uint8_t bootloader_get_fail(void)
{
    return __system_ota.fail;
}

// �������ʧ�ܱ�־
void bootloader_clear_fail(void)
{
    __system_ota.fail = FALSE;

    bootloader_write();
}
