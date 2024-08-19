/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	open_paygo_flash.c 
* Description:	Manage OpenPAYGO flash 
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes:        
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#include "open_paygo_flash.h"
#include <math.h>

/********************************************************************
 ** @brief  ѡ������ڴ�ҳ��
 ** @param  enum close_flash_flag ѡ��Ҫ�����������Ӧ��ҳ
 ** @retval ��
********************************************************************/
void open_paygo_flash_close_flash(enum close_flash_flag flag)
{
    switch(flag)
    {
        case CLOSE_FLASH_LOCK :
            system_write_flash_lock_close();
            break;
        case CLOSE_FLASH_DATE_ONE :
            system_write_flash_date_one_close();
            break;
        case CLOSE_FLASH_DATE_TOW :
            system_write_flash_date_tow_close();
            break;
        case CLOSE_FLASH_PRODUCT_TIME :
            system_write_flash_product_time_close();
            break;
        case CLOSE_FLASH_PRODUCT_ID :
            system_write_flash_product_id_close();
            break;
    }
}


/********************************************************************
 ** @brief  ѡ������ڴ�ҳ��
 ** @param  _Bool TRUE��������ʱ�䣬FALSE�洢�������
 ** @retval ��
********************************************************************/
void open_paygo_flash_write_lock_state(__flash_lock * data, _Bool temp_flag)
{
    if(temp_flag == TRUE)
	{
        uint8_t flash_write_sate = 1; 
            
        __flash_lock temp_data;

        do{
            system_write_flash_lock_close();

            system_write_flash_lock_state(data);

            system_read_flash_lock_state(&temp_data);

            flash_write_sate = memcmp(data, &temp_data, sizeof(__flash_lock));
        }
        while(flash_write_sate != 0);
        
	}
	else
	{
		system_write_flash_lock_error(&data->lock_error);
	}
}

/********************************************************************
 ** @brief  ��ȡ����ʱ�䣬�����ݾ͸�ֵ������Ľṹ�壬û�����ݾͲ���ֵ
 ** @param  __flash_lock ������Ϣ�Ľṹ��ָ��
 ** @retval ���ݲ�Ϊ0xff����1���񷵻�0
********************************************************************/
_Bool open_paygo_flash_read_lock_sate(__flash_lock * data)
{
    uint8_t flash_read_sate = 1; 

    __flash_lock compare_data;
    // ʹ�� memset �������ṹ����ڴ涼��Ϊ1
    memset(&compare_data, 0xFF, sizeof(compare_data));
    
    __flash_lock temp_data;

    do{
        __flash_lock temp_data_1;

        system_read_flash_lock_state(&temp_data);

        system_read_flash_lock_state(&temp_data_1);

        flash_read_sate = memcmp(&temp_data, &temp_data_1, sizeof(__flash_lock));
    }
    while(flash_read_sate != 0);

    if(memcmp(&temp_data, &compare_data, sizeof(__flash_lock)) == 0)
    {
        return FALSE;
    }
    else
    {
        memcpy(data, &temp_data, sizeof(__flash_lock));

        return TRUE;
    }
}


/********************************************************************
 ** @brief  ��flash����ID
 ** @param  __flash_product_id �ṹ��ָ��
 ** @retval ��
********************************************************************/
void open_paygo_flash_write_product_id(__flash_product_id * data)
{
    uint8_t flash_write_sate = 1; 
    
    __flash_product_id temp_data;

    do{
        system_write_flash_product_id_close();

        system_write_flash_product_id_data(data);

        system_read_flash_product_id_data(&temp_data);

        flash_write_sate = memcmp(data, &temp_data, sizeof(__flash_product_id));
    }
    while(flash_write_sate != 0);
}

/********************************************************************
 ** @brief  ��ȡ ID �������ݾ͸�ֵ������Ľṹ�壬û�����ݾͲ���ֵ
 ** @param  __flash_product_id �ṹ��ָ��
 ** @retval ���ݲ�Ϊ0xff����1���񷵻�0
********************************************************************/
_Bool open_paygo_flash_read_product_id(__flash_product_id * data)
{
    uint8_t flash_read_sate = 1; 

    __flash_product_id compare_data;
    memset(&compare_data, 0xff, sizeof(__flash_product_id));
	
    __flash_product_id temp_data;

    do{
        __flash_product_id temp_data_1;
		
		system_read_flash_product_id_data(&temp_data_1);

        system_read_flash_product_id_data(&temp_data);

        flash_read_sate = memcmp(&temp_data, &temp_data_1, sizeof(__flash_product_id));
    }
    while(flash_read_sate != 0);

    if(memcmp(&temp_data, &compare_data, sizeof(__flash_product_id)) == 0 \
            || temp_data.unit_number_code == 0 \
            || temp_data.unit_number_code == (pow(16,2*sizeof(temp_data.unit_number_code))-1))
    {
        return FALSE;
    }
    else
    {
        memcpy(data, &temp_data, sizeof(__flash_product_id));

        return TRUE;
    }
}


/********************************************************************
 ** @brief  ��flash�������ʱ�䣬�ܿ���ʱ��
 ** @param  __flash_date_one �ṹ��ָ��
 ** @retval ��
********************************************************************/
void open_paygo_flash_write_date_one(__flash_date_one * data)
{
    uint8_t flash_write_sate = 1; 
    
    __flash_date_one temp_data;

    do{
        system_write_flash_date_one_close();

        system_write_flash_date_one(data);

        system_read_flash_date_one(&temp_data);

        flash_write_sate = memcmp(data, &temp_data, sizeof(__flash_date_one));
    }
    while(flash_write_sate != 0);
}

// �������ϱ��汾�ű�־
void open_paygo_flash_write_product_id_have_reported(uint8_t *data)
{
    system_write_flash_product_id_have_reported(data);
}

/********************************************************************
 ** @brief  ��ȡ����ʱ�䡢�ܿ���ʱ�� �������ݾ͸�ֵ������Ľṹ�壬û�����ݾͲ���ֵ
 ** @param  __flash_date_one �ṹ��ָ��
 ** @retval ���ݲ�Ϊ0xff����1���񷵻�0
********************************************************************/
_Bool open_paygo_flash_read_date_one(__flash_date_one * data)
{
    uint8_t flash_read_sate = 1; 
    
    __flash_date_one temp_data;

    do{
        __flash_date_one temp_data_1;

        system_read_flash_date_one(&temp_data);

        system_read_flash_date_one(&temp_data_1);

        flash_read_sate = memcmp(&temp_data, &temp_data_1, sizeof(__flash_date_one));
    }
    while(flash_read_sate != 0);

    if(temp_data.available_time == (pow(16,2*sizeof(temp_data.available_time))-1) \
                    && temp_data.final_date == (pow(16,2*sizeof(temp_data.final_date))-1) )
    {
        data->available_time = 0;

        data->final_date = 0;

        return FALSE;
    }
    else
    {
        memcpy(data, &temp_data, sizeof(__flash_date_one));

        return TRUE;
    }
}



/********************************************************************
 ** @brief  �洢���ʱ��
 ** @param  __flash_date_tow �ṹ��ָ��
 ** @retval ��
********************************************************************/
void open_paygo_flash_write_date_tow(__flash_date_tow * data)
{
    uint8_t i;

    uint8_t flash_write_sate = 1; 
    
    __flash_date_tow temp_data;

    do{

        // ����洢λ�ã����λΪָʾλ����Ϊ0�������Ѿ�������
        for(i=0; i<7; i++)  // 0~6����Ӧ�ڴ��д洢ʱ��λ�õ�7����ַ
        {
            if(data->location_date[i] == 255 && i == 0)
            {
                data->location_date[i] = data->location_date[i] << 1;

                system_write_flash_date_tow_close();

                break;
            }
            else if(data->location_date[i] > 128) 
            {
                data->location_date[i] = data->location_date[i] << 1;

                break;
            }
            else if(data->location_date[i] == 128 && i != 6)
            {
                data->location_date[i] = data->location_date[i] << 1;
                i++;
                data->location_date[i] = data->location_date[i] << 1;

                break;
            }
            else if(data->location_date[i] == 128 && i == 6)
            {
                memset(data->location_date, 255, sizeof(data->location_date));     // ##############################

                data->location_date[0] = data->location_date[0] << 1;

                system_write_flash_date_tow_close();
                break;
            }
        }

        system_write_flash_date_tow(data);

        system_read_flash_date_tow(&temp_data, 0);

        flash_write_sate = memcmp(data, &temp_data, sizeof(__flash_date_tow));
    }
    while(flash_write_sate != 0);

    // ����û����ı�־λ
    system_write_flash_date_tow_flag(&data->location_date[0]);
}


/********************************************************************
 ** @brief  ��ȡ���ʱ��
 ** @param  __flash_date_tow �ṹ��ָ��
 ** @retval ��
********************************************************************/
void open_paygo_flash_read_date_tow(__flash_date_tow * data)
{
    uint8_t flash_read_sate = 1; 
    
    __flash_date_tow temp_data;

    do{
        __flash_date_tow temp_data_1;

        system_read_flash_date_tow(&temp_data, 1);

        system_read_flash_date_tow(&temp_data_1, 1);

        flash_read_sate = memcmp(&temp_data, &temp_data_1, sizeof(__flash_date_tow));
    }
    while(flash_read_sate != 0);

    memcpy(data, &temp_data, sizeof(__flash_date_tow));
}


/********************************************************************
 ** @brief  �����ۼ�ʱ�䡢count
 ** @param  __flash_product_time �ṹ��ָ��
 ** @retval ��
********************************************************************/
void open_paygo_flash_write_product_time(__flash_product_time * data)
{
    uint8_t flash_write_sate = 1; 
    
    __flash_product_time temp_data;

    do{
        system_write_flash_product_time_close();

        system_write_flash_product_time_data(data);

        system_read_flash_product_time_data(&temp_data);

        flash_write_sate = memcmp(data, &temp_data, sizeof(__flash_product_time));
    }
    while(flash_write_sate != 0);
}

/********************************************************************
 ** @brief  ��ȡ����ʱ�䡢count �������ݾ͸�ֵ������Ľṹ�壬û�����ݾͲ���ֵ
 ** @param  __flash_product_time �ṹ��ָ��
 ** @retval ���ݲ�Ϊ0xff����1���񷵻�0
********************************************************************/
_Bool open_paygo_flash_read_product_time(__flash_product_time * data)
{
    uint8_t flash_read_sate = 1; 

    __flash_product_time compare_data;
    // ʹ�� memset �������ṹ����ڴ涼��Ϊ1
    memset(&compare_data, 0xFF, sizeof(compare_data));
    
    __flash_product_time temp_data;

    do{
        __flash_product_time temp_data_1;

        system_read_flash_product_time_data(&temp_data);

        system_read_flash_product_time_data(&temp_data_1);

        flash_read_sate = memcmp(&temp_data, &temp_data_1, sizeof(__flash_product_time));
    }
    while(flash_read_sate != 0);

    if(memcmp(&temp_data, &compare_data, sizeof(__flash_product_time)) == 0)
    {
        data->add_time = 0;

        data->token_count = 0;

        return FALSE;
    }
    else
    {
        memcpy(data, &temp_data, sizeof(__flash_product_time));

        return TRUE;
    }
}
