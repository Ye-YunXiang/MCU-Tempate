/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_seve_flash.c 
* Description:	initialize FLASH
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#include "system_seve_flash.h"


/********************************************************************
 ** \brief  (����Ϊuint8��һ���ֽ�)���ָ��ҳ
 ** @param  �����ַ
 ** \retval ��
********************************************************************/
void system_write_flash_close(volatile uint32_t addr)
{
    while(Ok != Flash_SectorErase(addr)){};  
}

/********************************************************************
 ** \brief  (����Ϊuint8��һ���ֽ�)�洢��������
 ** @param  �洢��ַ
 ** @param  Դ���ݵ�ַ
 ** @param  ���鳤��
 ** \retval ��
********************************************************************/
void system_flash_write_array_data(volatile uint32_t addr, volatile uint8_t * data, uint16_t len)
{
    uint16_t i;
    volatile uint32_t flash_addr = addr;
    
    for(i=0; i< len; i++)
    {
        Flash_WriteByte(flash_addr, *data);
        flash_addr += 0x01;
        data++;
    }
}


/********************************************************************
 ** \brief  (����Ϊuint8��һ���ֽ�)��ȡ��������
 ** @param  ��ȡ��ַ
 ** @param  �洢���ݵ�ַ
 ** @param  ���鳤��
 ** \retval ��
********************************************************************/
void system_flash_read_array_data(volatile uint32_t addr, volatile uint8_t * data, uint16_t len)  
{
    uint16_t i;
    volatile uint32_t flash_addr = addr;
    
    for(i=0; i< len; i++)
    {
        *data = *((uint8_t *)flash_addr);
        flash_addr += 0x01;
        data++;
    }
}
 

/********************************************************************
 ** \brief  �������洢ʱ���ַ��
 ** @param  _flash_date_tow�Ĵ洢ʱ������ָ��
 ** \retval ����Ĵ洢��ַ
********************************************************************/
uint32_t calculate_interval_date_addr(volatile uint8_t * data)
{
    uint8_t i;
    uint8_t sum = 0;
    volatile uint32_t addr_interval_date = FLASH_ADDR_DATE_INTERVAL_DATE;

     // ����洢�洢�߼����ڵ�ַ�߼�
    for(i=0; i<7; i++)
    {
        if((*data == 0x00) && (i != 6))
        {
            sum += 7u;
            data++;
        }
        else
        {
            int j;
            for(j=0; j<8; j++)
            {
                uint8_t bit_j = (*data >> j) & 0x01;

                if( bit_j != 0x00)
                {
                    break;
                }
                sum += 1u;
            }
            break;
        }
    }
	
	sum -= 1;
    addr_interval_date = addr_interval_date + (sum * 8);

    return addr_interval_date;
}

/********************************************************************************
 ** lock  ҵ���߼�   
 *******************************************************************************/

/********************************************************************
 ** \brief  ��ȡ�Ƿ���������������������������ʱ��
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_write_flash_lock_close(void)
{
    while(Ok != Flash_SectorErase(FLASH_ADDR_LOCK_STATE)){};
}


/********************************************************************
 ** \brief  д��flash lock�������
 ** @param  _flash_lock��errorָ��
 ** \retval ��
********************************************************************/
void system_write_flash_lock_error(uint8_t * data)
{
    uint8_t temp_data = 0xff;
    temp_data = temp_data << *data;

    Flash_WriteByte(FLASH_ADDR_LOCK_ERROR, temp_data);
}


/********************************************************************
 ** \brief  д��flash����_flash_lock
 ** @param  _flash_lockָ��
 ** \retval ��
********************************************************************/
void system_write_flash_lock_state(struct flash_lock * data)
{
    uint8_t temp_data = 0xff;

    temp_data = temp_data << data->lock_error;
    Flash_WriteByte(FLASH_ADDR_LOCK_ERROR, temp_data);

    Flash_WriteHalfWord(FLASH_ADDR_LOCK_DATE, data->lock_date);
}


/********************************************************************
 ** \brief  ��ȡflash����_flash_lock
 ** @param  _flash_lockָ��
 ** \retval ��
********************************************************************/
void system_read_flash_lock_state(struct flash_lock * data)
{
    uint8_t temp_data = 0;
    uint8_t i = 0;

    temp_data = *((uint8_t *)FLASH_ADDR_LOCK_ERROR);

    do{
        if(temp_data == 0)
        {
            i = 0;
            break;
        }
		
		uint8_t temp_bit = temp_data&0x01;
        if( temp_bit == 0)
        {
            i++;
            temp_data = temp_data >> 1;
        }
        else
        {
            break;
        }
    }
    while(1);
    
    data->lock_error = i;
    data->lock_date  = *((uint16_t *)FLASH_ADDR_LOCK_DATE); 

    if(data->lock_date == 0 || data->lock_date >= 4000)
    { 
        data->lock_date = 0;
    }
}

/********************************************************************************
 ** date  ҵ���߼�   
 *******************************************************************************/

/********************************************************************
 ** \brief  ���ҳ252_flash_date_one
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_write_flash_date_one_close(void)
{
    while(Ok != Flash_SectorErase(FLASH_ADDR_DATE_FINAL_DATE)){};      // 252
}


/********************************************************************
 ** \brief  �洢flash����ʱ�䡢�ܿ���ʱ��
 ** @param  _flash_date_oneָ��
 ** \retval ��
********************************************************************/
void system_write_flash_date_one(struct flash_date_one * data)
{
    // д��һ���֣�ʱ���
    Flash_WriteWord(FLASH_ADDR_DATE_FINAL_DATE, data->final_date);

    // д��һ���֣��ܿ���ʱ��
    Flash_WriteWord(FLASH_ADDR_DATE_SUM_AVAILABLE_TIME, data->available_time);
}


/********************************************************************
 ** \brief  ��ȡflash�ڴ��н���ʱ�䡢�ܿ���ʱ��
 ** @param  _flash_date_oneָ��
 ** \retval ��
********************************************************************/
void system_read_flash_date_one(struct flash_date_one * data)
{
    // ��ȡʱ���
    data->final_date = *((uint32_t *)FLASH_ADDR_DATE_FINAL_DATE);

    // ��ȡ�����
    data->available_time = *((uint32_t *)FLASH_ADDR_DATE_SUM_AVAILABLE_TIME);
}


/********************************************************************
 ** \brief  ���ҳ253
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_write_flash_date_tow_close(void)
{
    while(Ok != Flash_SectorErase(FLASH_ADDR_DATE_LOCATION_DATE)){};   // 253
}


/********************************************************************
 ** \brief  �洢��Ӵ洢ʱ���־λ
 ** @param  1
 ** \retval ��
********************************************************************/
void system_write_flash_date_tow_flag(uint8_t * add)
{
    volatile uint32_t addr_interval_date = 0;

    addr_interval_date = calculate_interval_date_addr(add);
    addr_interval_date += 4u;

    Flash_WriteByte(addr_interval_date, 0x01);
}


/********************************************************************
 ** \brief  �洢��Ӵ洢��ַ����Ӵ洢ʱ��
 ** @param  _flash_date_towָ��
 ** \retval ��
********************************************************************/
void system_write_flash_date_tow(struct flash_date_tow * data)
{
    volatile uint32_t addr_interval_date = 0;

    system_flash_write_array_data(FLASH_ADDR_DATE_LOCATION_DATE,\
                                    &data->location_date[0], sizeof(data->location_date));

    addr_interval_date = calculate_interval_date_addr(data->location_date);

    // д��ʱ���
    Flash_WriteWord(addr_interval_date, data->interval_date);
}


/********************************************************************
 ** \brief  ��ȡ��Ӵ洢��ַ����Ӵ洢ʱ��
 ** @param  _flash_date_towָ��
 ** @param  �Ƿ���֤�б�־λTRUE/FALSE
 ** \retval ��
********************************************************************/
void system_read_flash_date_tow(struct flash_date_tow * data, _Bool temp_flag)
{
    uint8_t flash_read_state = 1;
    uint8_t interval_date_flag;
    volatile uint32_t addr_interval_date = 0;
    volatile uint32_t addr_interval_date_temp;

    system_flash_read_array_data(FLASH_ADDR_DATE_LOCATION_DATE,\
                                    &data->location_date[0], sizeof(data->location_date));

    addr_interval_date = calculate_interval_date_addr(data->location_date);

    // ��ȡʱ�䣬�Ƿ���֤��־λ�����û�б�־λ��������
    if(temp_flag)
    {
        do{ 
            addr_interval_date_temp = addr_interval_date + 4u;
            interval_date_flag = *((uint8_t *)addr_interval_date_temp);

            if( interval_date_flag == 0xff )
            {
                if(addr_interval_date != FLASH_ADDR_DATE_INTERVAL_DATE)
                {
                    addr_interval_date -= 8; 
                }
                else
                {
                    data->interval_date = *((uint32_t *)addr_interval_date);
                    flash_read_state = 0;
                }
            }
            else
            {
                data->interval_date = *((uint32_t *)addr_interval_date);
                flash_read_state = 0;
            }
        }
        while(flash_read_state != 0);
    }
    else
    {
        data->interval_date = *((uint32_t *)addr_interval_date);
    }
}

/********************************************************************************
 ** product time  ҵ���߼�   
 *******************************************************************************/

/********************************************************************
 ** \brief  ���ҳ254
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_write_flash_product_time_close()
{
    while(Ok != Flash_SectorErase(FLASH_ADDR_PRODUCT_TIME_ADD_TIME)){};
}


/********************************************************************
 ** \brief  һ��д���ۼ�ʱ���count����
 ** @param  _flash_product_timeָ��
 ** \retval ��
********************************************************************/
void system_write_flash_product_time_data(struct flash_product_time * data)
{
    // ���ݶ��ǰ��ֳ��ȵ�
    Flash_WriteHalfWord(FLASH_ADDR_PRODUCT_TIME_ADD_TIME, data->add_time);

    Flash_WriteHalfWord(FLASH_ADDR_PRODUCT_TIME_TOKEN_COUNT, data->token_count);
}


/********************************************************************
 ** \brief  һ�ζ�ȡ�ۼ�ʱ���count����
 ** @param  _flash_product_timeָ��
 ** \retval ��
********************************************************************/
void system_read_flash_product_time_data(struct flash_product_time * data)
{
    data->add_time = *((uint16_t *)FLASH_ADDR_PRODUCT_TIME_ADD_TIME);
    data->token_count = *((uint16_t *)FLASH_ADDR_PRODUCT_TIME_TOKEN_COUNT);
}

/********************************************************************************
 ** product id  ҵ���߼�   
 *******************************************************************************/
 
/********************************************************************
 ** \brief  ���ҳ255
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_write_flash_product_id_close(void)
{
    while(Ok != Flash_SectorErase(FLASH_ADDR_PRODUCT_ID_STARTING_CODE)){};
}


/********************************************************************
 ** \brief  һ��д��ID����
 ** @param  _flash_product_idָ��
 ** \retval ��
********************************************************************/
void system_write_flash_product_id_data(struct flash_product_id * data)
{
    system_flash_write_array_data(FLASH_ADDR_PRODUCT_ID_SECRET_KEY, \
                                    &data->secret_key[0], sizeof(data->secret_key));

    Flash_WriteWord(FLASH_ADDR_PRODUCT_ID_STARTING_CODE, data->starting_code);

    Flash_WriteWord(FLASH_ADDR_PRODUCT_ID_UNIT_NUMBER_CODE, data->unit_number_code);

    // д��MQTT
    system_flash_write_array_data(FLASH_ADDR_PRODUCT_ID_DEVICE_SECRET, \
                                    &data->device_secret[0], sizeof(data->device_secret));

    system_flash_write_array_data(FLASH_ADDR_PRODUCT_ID_PRODUCT_KEY, \
                                    &data->product_key[0], sizeof(data->product_key));

    // д��OTA��ص�
    Flash_WriteByte(FLASH_ADDR_PRODUCT_ID_HAVE_REPORTED, data->have_reported);
    system_flash_write_array_data(FLASH_ADDR_PRODUCT_ID_VERSION, \
                                    &data->version[0], sizeof(data->version));
}


/********************************************************************
 ** \brief  һ�ζ�ȡID����
 ** @param  _flash_product_idָ��
 ** \retval ��
********************************************************************/
void system_read_flash_product_id_data(struct flash_product_id * data)
{
    system_flash_read_array_data(FLASH_ADDR_PRODUCT_ID_SECRET_KEY,\
                                    &data->secret_key[0], sizeof(data->secret_key));

    data->starting_code = *((uint32_t *)FLASH_ADDR_PRODUCT_ID_STARTING_CODE);
    data->unit_number_code = *((uint32_t *)FLASH_ADDR_PRODUCT_ID_UNIT_NUMBER_CODE);

    // ����MQTT
    system_flash_read_array_data(FLASH_ADDR_PRODUCT_ID_DEVICE_SECRET, \
                                    &data->device_secret[0], sizeof(data->device_secret));

    system_flash_read_array_data(FLASH_ADDR_PRODUCT_ID_PRODUCT_KEY, \
                                    &data->product_key[0], sizeof(data->product_key));

    // ��ȡOTA���
    data->have_reported = *((uint8_t *)FLASH_ADDR_PRODUCT_ID_HAVE_REPORTED);
    system_flash_read_array_data(FLASH_ADDR_PRODUCT_ID_VERSION, \
                                    &data->version[0], sizeof(data->version));
}

/********************************************************************
 ** \brief  д�����ϱ�����
 ** @param  uint8_t 1��0
 ** \retval ��
********************************************************************/
void system_write_flash_product_id_have_reported(uint8_t *data)
{
    Flash_WriteByte(FLASH_ADDR_PRODUCT_ID_HAVE_REPORTED, *data);
}
