/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_gsm_m25.c
* Description:	
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/
#include "system_gsm_m25.h"
#include "system_ota.h"

_Bool GSM_EN_STATE = 0;

/* ***********************************************************************
 * ��Ҫ��ס�ļ�������Token
 * �ϱ����ԣ� /sys/${ProductKey}/${deviceName}/thing/event/property/post
 * �ϱ��豸�汾��/ota/device/inform/${ProductKey}/${deviceName}
 *      {
            "id": "123",
            "params": {
                "version": "1.0.1"
            }
        }
 * ������ȡOTA���ģ� /sys/${ProductKey}/${deviceName}/thing/ota/firmware/get_reply
 * ������ȡOTA��Ϣ�� /sys/${ProductKey}/${deviceName}/thing/ota/firmware/get
 *      {
            "id": "123",
            "version": "1.0",
            "params": {
            },
            "method": "thing.ota.firmware.get"
        }
 *
**/

// MQTTѭ�����͵���ʱװ�أ� 1��Ϊ1s    
#define DEFINE_GSM_POWER_SEND_TIME_COUNT     600         // Ŀǰװ��10����
// ��ȡУ����λ��
#define READ_GSM_BIN_CRC            2
// ÿ�ζ�ȡbin�ļ���С
#define READ_GSM_BIN_SIZE           512

#define AT                        "AT\r\n"                                        // 1��AT��OK
#define ATE0                      "ATE0\r\n"                                      // 2���رջ��� OK
#define AT_QICSGP                 "AT+QICSGP=1,\"iot-eu.aer.net\",\"\",\"\"\r\n"  // 3��������������APN��վλ�á� OK
#define AT_CGATT                  "AT+CGATT?\r\n"                                 // 4������Ƿ��ųɹ���վ
#define AT_QIREGAPP               "AT+QIREGAPP\r\n"                               // 5��ִ�н��� OK
#define AT_CGDCONT                "AT+CGDCONT=1,\"IP\",\"\"\r\n"                  // 6������PDP�����ġ� OK
#define AT_CGACT                  "AT+CGACT=1,1\r\n"                              // 7������PDP������  OK
#define AT_QIACT                  "AT+QIACT\r\n"                                  // 8�������ƶ�����  OK
#define AT_QILOCIP                "AT+QILOCIP\r\n"                                // 9����ȡ��ַ
#define AT_QISTAT                 "AT+QISTAT\r\n"                                 // 10���鿴����״̬
#define AT_QNITZ                  "AT+QNITZ=1\r\n" 
#define AT_QLTS                   "AT+QLTS\r\n" 
// OTA��� ----------------
#define AT_QHTTPGET               "AT+QHTTPGET=60\r\n"                            // �������ӵ��ļ�
// ע�⣺��һ���᷵���ļ���С
#define AT_QHTTPDL                "AT+QHTTPDL=\"RAM:bin.BIN\",204800\r\n"         // ���ļ�������200k��С��RAM��
// ע�⣺��һ���᷵���ļ�ָ��
#define AT_QFOPEN                 "AT+QFOPEN=\"RAM:bin.BIN\"\r\n"
// MQTT���----------------------
#define AT_QMTCFG_VERSIN          "AT+QMTCFG=\"VERSION\",0,1\r\n"                 // ����汾�š� OK
#define AT_QMTCFG_KEEPALIVE       "AT+QMTCFG=\"KEEPALIVE\",0,60\r\n"              // ��������ʱ�䡣 OK
#define AT_QMTCLOSE               "AT+QMTCLOSE=0\r\n"                             // �˳�MQTT��TCP���ӡ�
#define AT_QMTDISC                "AT+QMTDISC=0\r\n"                                // �Ͽ�MQTT�ͻ��˺ͷ��������


#define AT_CSQ                    "AT+CSQ\r\n"                                    // ���źš�


#define AT_REPLY_OK               "OK"  
// TCP���ӻظ�
#define AT_REPLY_QMTOPEN_OK       "0,0" // "\r\n+QMTOPEN: 0,0"  
// ����MQTT�������ظ�
#define AT_REPLY_QMTCONN_OK       "0,0,0" // "\r\n+QMTCONN: 0,0,0" 
// �ϱ����Իظ�
#define AT_REPLY_QMTPUB_OK        "0,3441,0" // "\r\n+QMTPUB: 0,3441,0"  
// �鿴�Ƿ��ϻ�վ                 
#define AT_REPLY_CGATT_OK         "1"
// ��ѯ��ǰ����״̬
#define AT_REPLY_QISTAT_OK       "STATUS"
// ��ѯIP��ַ
#define AT_REPLY_QILOCIP_OK       "."

// �������ӻظ�
// #define AT_REPLY_QHTTPURL         "CONNECT"  // �յ�����ظ����Է�����
// �����ļ��ظ�
#define AT_REPLY_QHTTPDL          "+QHTTPDL: " // ��λ���ļ���С��λ��
// ��ȡ�ļ�
#define AT_REPLY_QFOPEN           "+QFOPEN: "  // ��λ����ȡ�ļ���ָ��λ��
// ��ȡ�ظ���ȡ�˼����ֽ�
#define AT_REPLY_QFREAD           "CONNECT "    // ��λ����ȡ�˼����ֽڣ�ע��connect������һ���ո�
// ���ĳɹ��Ļظ�
#define AT_REPLY_QMTSUB            "0,1,0"     // ��0��TCP����Ϣ��ʶ��Ϊ1�����������յ���Ϣ
// ������ȡOTA��Ϣ�����Ļظ�
#define AT_REPLY_QMTSUB_OTA                 "+QMTRECV:"
// ������ȡOTA��Ϣ�����Ļظ�����������---ͷ
#define AT_REPLY_QMTSUB_OTA_HEAD                 "\"url\":\""
// ������ȡOTA��Ϣ�����Ļظ�����������---β
#define AT_REPLY_QMTSUB_OTA_TAIL                 "\",\"md5\":\""
// ��ȡ�ź�
#define AT_REPLY_CSQ                    "+CSQ: "        //+CSQ: 28,0

#define AT_REPLY_QLTS                   "+QLTS:"       // +QLTS: "24/03/19,03:53:17+32,0"

// ��ȡ�汾��
#define AT_REPLY_VERSION              "\"version\":\""

// �ػ��ظ�
#define AT_REPLY_POWER_OFF_OK          "NORMAL POWER DOWN" 

// �ź�ǿ��
uint8_t csq_value = 0;

// M25�������ݽṹ��
struct  STRUCT_LPUSART0_Fram strM25_Fram_Record = { 0 };
// MQTT״̬����
struct  STRUCT_MQTT_STATE mqtt_state = { 0 };
// GSM��������ʱ
uint32_t gsm_power_restart_time = 0;
// GSM���ͼ�ʱ���Ͷ���ϴ�����
uint32_t gsm_power_sending_time_count = 0;

// ָ������������ָ��ģ�û�õ�ʱ����ͷŵ�
char *ota_bin_url = NULL;
// ��ŷ��ص��ļ�ָ��
uint32_t ota_bin_pointer = 0;
// ÿ�ζ�ȡ�ļ���С
uint16_t ota_bin_read_size = READ_GSM_BIN_SIZE;
// ͬ��ʱ��
uint8_t mqtt_lock_in_time[30]; 


// ��ʼ�����нṹ��ͱ���
void system_gsm_value(void)
{
    // �ź�ǿ��
    csq_value = 0;

    // M25�������ݽṹ��
    memset(&strM25_Fram_Record, 0, sizeof(struct  STRUCT_LPUSART0_Fram));
    // MQTT״̬����
    memset(&mqtt_state, 0, sizeof(struct  STRUCT_MQTT_STATE));

    // GSM��������ʱ
    gsm_power_restart_time = 0;
    // GSM���ͼ�ʱ���Ͷ���ϴ�����
    gsm_power_sending_time_count = 0;

    // ָ������������ָ��ģ�û�õ�ʱ����ͷŵ�
    if(ota_bin_url != NULL)
    {
        ota_bin_url = NULL;
    }

    // ��ŷ��ص��ļ�ָ��
    ota_bin_pointer = 0;
    // ÿ�ζ�ȡ�ļ���С
    ota_bin_read_size = READ_GSM_BIN_SIZE;
    // ͬ��ʱ��
    memset(&mqtt_lock_in_time[0], 0, sizeof(mqtt_lock_in_time));
}


/********************************************************************
 ** \brief  MQTT����ƴ�ӣ�������Ҫƴ�ӵ���Ϣ��ͨ������
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_gsm_mqtt_send_splicing(enum MQTT_TYPE temp_mqtt_type)
{
    // �е���ջ��������Զ�̬����
    char *temp_mqtt_str = (char *)malloc(400);
    // �ϱ�ָ��
    char *send_at_qmtpub_token = (char *)malloc(150);

    // ���»�ȡproduct key ��� '\0'
	uint8_t temp_product_key[sizeof(_this_pointer._this_product_id_pointer->product_key)+1] = {0};
	memcpy(temp_product_key, _this_pointer._this_product_id_pointer->product_key, \
									sizeof(_this_pointer._this_product_id_pointer->product_key));
	// ���»�ȡdevice_secretx ��� '\0'
	uint8_t temp_device_secret[sizeof(_this_pointer._this_product_id_pointer->device_secret)+1] = {0};
	memcpy(temp_device_secret, _this_pointer._this_product_id_pointer->device_secret,\
									sizeof(_this_pointer._this_product_id_pointer->device_secret));

    switch (temp_mqtt_type)
    {
        case MQTT_TYPE_ALIAUTH:  // �����Ʋ���
            sprintf(temp_mqtt_str, "AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%d\",\"%s\"\r\n", \
                                temp_product_key, \
                                _this_pointer._this_product_id_pointer->unit_number_code, \
                                temp_device_secret);
            break;

        case MQTT_TYPE_QMTOPEN:  // TCP����
            sprintf(temp_mqtt_str, "AT+QMTOPEN=0,\"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n", \
                                temp_product_key);
            break;

        case MQTT_TYPE_QMTCONN:  // MQTT����
            sprintf(temp_mqtt_str, "AT+QMTCONN=0,\"%d\"\r\n", \
                                _this_pointer._this_product_id_pointer->unit_number_code);
            break;

        case MQTT_TYPE_QMTPUB:  // �ϱ�����
			if(temp_mqtt_type == MQTT_TYPE_QMTPUB)
			{
				struct GSM_MQTT_PARAM temp_param;
				op_manage_get_mqtt_param((void *)(&temp_param)); 

				// �ϱ�������-----------------------------------------------
                // ��ͷ
                sprintf(temp_mqtt_str, "{\"method\":\"thing.service.property.set\",\"id\":\"%d\",\
                                            \"params\":{\"count\":%d,\"time_remaining\":{\"hour\":%d,\"min\":%d},\
                                            \"CSQ\":%d,\"BAT_VDC\":%.3f,\"DC_VDC\":%.3f,\"power_state\":%d", \
                                            _this_pointer._this_product_id_pointer->unit_number_code,\
                                            temp_param.count, \
                                            temp_param.time_remaining_hour,temp_param.time_remaining_min,\
                                            csq_value,\
                                            temp_param.BAT_VDC,\
                                            temp_param.DC_VDC,\
                                            temp_param.power_state);
                                            
                // ����Ƿ�򿪣��ϱ���ʱ��    
                if(__sleep_mode.mqtt_tamper_open == TRUE)
                {
                    sprintf(temp_mqtt_str, "%s,\"tamper_open\":\"%s\"", temp_mqtt_str, mqtt_lock_in_time);
                    __sleep_mode.mqtt_tamper_open = FALSE;
                }

                // �Ƿ�Ϊ����״̬���ϱ��¶�
                if(__sleep_mode.device_power_on_state == TRUE)
                {
                    sprintf(temp_mqtt_str, "%s,\"refrigerator_thermometer\":%d",\
                                            temp_mqtt_str,\
                                            temp_param.refrigerator_thermometer);
					
					__NOP();
                }

                // �ϱ���β---------------------------------------------------------
                sprintf(temp_mqtt_str, "%s},\"version\":\"1.0.0\"}", temp_mqtt_str);

				// �ϱ�Token ��Ҫ���͵������ֽ�
				sprintf(send_at_qmtpub_token, "AT+QMTPUB=0,3441,1,0,\
										\"/sys/%s/%d/thing/event/property/post\",%d\r\n",\
										temp_product_key, \
										_this_pointer._this_product_id_pointer->unit_number_code,\
										strlen(temp_mqtt_str));

				system_ble_lpuart0_send_string(send_at_qmtpub_token, strlen(send_at_qmtpub_token));

				delay1ms(500);
			}
            break;

        // ����OTA��ȡ --------------------------------------------------------------------
        case MQTT_TYPE_QMTSUB:           // ����OTA������ȡ
            sprintf(temp_mqtt_str, "AT+QMTSUB=0,1,\"/sys/%s/%d/thing/ota/firmware/get_reply\",2\r\n",\
                                    temp_product_key, \
                                    _this_pointer._this_product_id_pointer->unit_number_code);
            break;

        case MQTT_TYPE_QMTPUB_VERSION:   // �ϱ��汾
            // �ϱ�������
            sprintf(temp_mqtt_str, "{\"id\":\"%d\",\"params\":{\"version\": \"%s\"}}", \
                                    _this_pointer._this_product_id_pointer->unit_number_code,\
                                    _this_pointer._this_product_id_pointer->version);
            // �ϱ�Token ��Ҫ���͵������ֽ�
            sprintf(send_at_qmtpub_token, "AT+QMTPUB=0,3441,1,0,\
                                    \"/ota/device/inform/%s/%d\",%d\r\n",\
                                    temp_product_key, \
                                    _this_pointer._this_product_id_pointer->unit_number_code,\
                                    strlen(temp_mqtt_str));

            system_ble_lpuart0_send_string(send_at_qmtpub_token, strlen(send_at_qmtpub_token));

            delay1ms(500);
            break;

        case MQTT_TYPE_QMTPUB_OTA:       // OTA��ȡ
            // �ϱ�������
            sprintf(temp_mqtt_str, "{\"id\":\"123\",\"version\":\"1.0\",\
                                    \"params\":{},\"method\":\"thing.ota.firmware.get\"}");
            // �ϱ�Token ��Ҫ���͵������ֽ�
            sprintf(send_at_qmtpub_token, "AT+QMTPUB=0,3441,1,0,\
                                    \"/sys/%s/%d/thing/ota/firmware/get\",%d\r\n",\
                                    temp_product_key, \
                                    _this_pointer._this_product_id_pointer->unit_number_code,\
                                    strlen(temp_mqtt_str));
            system_ble_lpuart0_send_string(send_at_qmtpub_token, strlen(send_at_qmtpub_token));

            delay1ms(500);
            break;

        // ����HTTP--------------------------------------------------------------------
        case MQTT_TYPE_QHTTPURL:     // �ϴ���������
            sprintf(temp_mqtt_str, "AT+QHTTPURL=%d,30\r\n",strlen(ota_bin_url));
            system_ble_lpuart0_send_string(temp_mqtt_str, strlen(temp_mqtt_str));

            delay1ms(500);

            sprintf(temp_mqtt_str, "%s",ota_bin_url);
            break; 

        case MQTT_TYPE_QFREAD:       // ͨ��ָ���ȡ�ļ�
            // ���ö�ȡ�ļ�����
            if(temp_mqtt_type == MQTT_TYPE_QFREAD)
            {
				uint32_t temp_size = ota_get_ota_bin_size();
				uint32_t temp_move = ota_get_ota_bin_move();
                uint32_t temp_bin_move = temp_size - READ_GSM_BIN_CRC - temp_move;
                
                if( temp_bin_move >= READ_GSM_BIN_SIZE) // �ļ����ȴ���������ȡ�ļ����ȣ�����һ�ζ�ȡһҳ���ļ�
                {
                    ota_bin_read_size = READ_GSM_BIN_SIZE;
                }
                else if(temp_bin_move < READ_GSM_BIN_SIZE && temp_bin_move != 0)    // �ļ�����С��һҳ������ʣ�೤��
                {
                    ota_bin_read_size = temp_bin_move;      // ʣ��Ķ�ȡ����
                }
                else    // �ļ���ȡ��ϣ�����0�ˣ�����ȡУ����
                {
                    ota_bin_read_size = READ_GSM_BIN_CRC;
                }

				sprintf(temp_mqtt_str, "AT+QFREAD=%d,%d\r\n",ota_bin_pointer, ota_bin_read_size);
			}
            break;

        default:
            break;
    }

    strM25_Fram_Record.InfBit.FramLength = 0;
    strM25_Fram_Record.InfBit.FramFinishFlag = 0;
	
	__NOP();
    system_ble_lpuart0_send_string(temp_mqtt_str, strlen(temp_mqtt_str));

    // һ��Ҫ�ͷ�
    free(temp_mqtt_str);
    temp_mqtt_str = NULL;
    free(send_at_qmtpub_token);
    send_at_qmtpub_token = NULL;
}


/********************************************************************
 ** \brief  ��ʼ��GSM�ĵ�Դʹ�����źͿ���ʹ������
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_gsm_port_init(void)
{
    // ��ʼ�����нṹ��ͱ���***********************************
    system_gsm_value();

    stc_gpio_cfg_t 	stc_out_led_init;
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
	stc_out_led_init.enDir		= GpioDirOut;
	stc_out_led_init.enDrv		= GpioDrvL;
	stc_out_led_init.enPu 		= GpioPuDisable;
	stc_out_led_init.enPd 		= GpioPdDisable;
	stc_out_led_init.enCtrlMode	= GpioAHB;
	
	Gpio_Init(GSM_EN_PORT, GSM_EN_PIN, &stc_out_led_init);
	Gpio_Init(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, &stc_out_led_init);

    Gpio_ClrIO(GSM_EN_PORT, GSM_EN_PIN);
    Gpio_ClrIO(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN);
}


/********************************************************************
 ** \brief  GSM����
 ** @param  ��
 ** \retval 1Ϊ�ɹ���0����ִ��
********************************************************************/
_Bool system_gsm_enabled_power_on(void)
{
    if(GSM_GET_EN() == FALSE)
    {
        TIMER_5ms_ON();     // ��5ms��ʱ�� Ϊ���Ŀ���
		
		// LPuart0 ���ų�ʼ��
		system_lpuart0_port_init();
		
        GSM_SET_EN();
        mqtt_state.mqtt_task_state_count_down = 20; //����ʱ100ms
    }
    else if(GSM_GET_PWRKEY() == FALSE)
    {
        GSM_SET_PWRKEY();
        mqtt_state.mqtt_task_state_count_down = 400; //����ʱ2S
    }
    else
    {
        GSM_CLR_PWRKEY();
        mqtt_state.mqtt_at_type = MQTT_TYPE_AT;
        mqtt_state.mqtt_task_state = MQTT_TYPE_SEND;
		
		mqtt_state.mqtt_task_state_count_down = 800; //����ʱ4S
        return 1;
    }
    
    return 0;
}

/********************************************************************
 ** \brief  GSM�ػ����������￨
 ** @param  ��
 ** \retval 1Ϊ�ɹ���0����ִ��
********************************************************************/
_Bool system_gsm_enabled_power_off(void)
{
	if(GSM_GET_EN() == TRUE && GSM_GET_PWRKEY() == FALSE)
	{	
        GSM_SET_PWRKEY();
		mqtt_state.mqtt_task_state_count_down = 400; //����ʱ2S
	}
	else if(GSM_GET_EN() == TRUE)
	{
        GSM_CLR_PWRKEY();
        GSM_CLR_EN();
		
		// �ر� LPuart0 �˿�
		system_lpuart0_port_close();

		mqtt_state.mqtt_task_state_count_down = 400; //����ʱ2S
	}
    else
    {
        mqtt_state.mqtt_at_type = MQTT_TYPE_NULL;
		mqtt_state.mqtt_task_state = MQTT_TYPE_NULL;

		TIMER_5ms_OFF();    // �ر�5ms��ʱ��  Ϊ���Ŀ���
        return 1;
    }
	
	return 0;
}


/********************************************************************
 ** \brief  GSM����/�ػ�/���� ����
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_gsm_enabled_state(void)
{
    // ֻҪ������������һ�Σ�Ȼ�������������Ǳ�����ϵ��ֱ�ӹػ����´��ڷ��ˡ�
    if(mqtt_state.mqtt_unsent_count != 0 && (mqtt_state.mqtt_unsent_count%3 == 0) \
                                    && mqtt_state.mqtt_task_state == MQTT_TYPE_POWER_RESTART)
    {
        mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_OFF;
        gsm_power_restart_time = 0;
        mqtt_state.mqtt_unsent_count = 0;

        // Ψһ��һ��ȫ�ֶ�̬�ڴ棬һ��Ҫע����û���ͷ�Ŷ
        // �ڴ�й© no wey no wey������������
        if(ota_bin_url != NULL)
        {
            free(ota_bin_url);
            ota_bin_url = NULL;
        }
    }

    // ���񵹼�ʱ�� һ����Ϊ 5mS�� ��һ����������ã�
    if(mqtt_state.mqtt_task_state_count_down == 0)
    {
        // ����MQTT������״̬
        switch (mqtt_state.mqtt_task_state)
        {
            case MQTT_TYPE_POWER_ON:
                if(system_gsm_enabled_power_on())
                {
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                break;
            
            case MQTT_TYPE_POWER_OFF:
                system_gsm_enabled_power_off();
                break;
            case MQTT_TYPE_POWER_RESTART:
                if(system_gsm_enabled_power_off())
                {
                    mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_ON;
                }
                break;

            default:
                break;
        }
    }
}

// ��ȡOTA �� URL
void system_gsm_mqtt_get_uart(void)
{
    if((_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTSUB_OTA_HEAD) == TRUE \
                && (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_VERSION) == TRUE)
    {
        uint8_t *ota_version_head = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_VERSION); 
        ota_version_head = ota_version_head + strlen(AT_REPLY_VERSION);
        
        uint8_t temp_version[sizeof(_this_pointer._this_product_id_pointer->version)];
        do{
            sprintf((char *)temp_version,"%s%c", temp_version, *ota_version_head);
            ota_version_head++;
        } while (*ota_version_head != 0x22);

        // �汾�Ƿ�һ��
        if( memcmp(&_this_pointer._this_product_id_pointer->version, temp_version, \
                                sizeof(_this_pointer._this_product_id_pointer->version)) != 0) 
        {
            uint8_t *ota_head = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTSUB_OTA_HEAD); 
            ota_head = ota_head + strlen(AT_REPLY_QMTSUB_OTA_HEAD);
            uint8_t *ota_tail = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTSUB_OTA_TAIL); 
            // ����url��ֵ��������
            uint16_t temp_bin_url_count = 0;

            // ����Ƿ�Ҫ���ͷ��ڴ�
            if(ota_bin_url != NULL)
            {
                free(ota_bin_url);
                ota_bin_url = NULL;
            }
            ota_bin_url = (char *)malloc(ota_tail - ota_head + 1);

            do
            {
                ota_bin_url[temp_bin_url_count] = *ota_head;
                temp_bin_url_count++;
                ota_head++;
            } while (ota_head != ota_tail);

            ota_bin_url[temp_bin_url_count] = '\0';

            
            // ���ý�����������
            system_gsm_ready_to_restart();
            mqtt_state.mqtt_ota_state = 1;

        }
        else
        {
            // ����ػ�����
            system_gsm_ready_to_shut_down();
        }
    }
    else
    {
        // ����ػ�����
        system_gsm_ready_to_shut_down();
    }                    
}


/********************************************************************
 ** \brief  MQTT���մ���
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_gsm_mqtt_receive(void)
{
    if(strM25_Fram_Record.InfBit.FramFinishFlag == 1 && mqtt_state.mqtt_task_state == MQTT_TYPE_WAIT_RX \
                                                                && strM25_Fram_Record.InfBit.FramLength < 4)
    {
        strM25_Fram_Record.InfBit.FramFinishFlag = 0;
    }
    else if(strM25_Fram_Record.InfBit.FramFinishFlag == 1 && mqtt_state.mqtt_task_state == MQTT_TYPE_WAIT_RX)// ��� lpuart0 ���յ�����  ���� ����״̬ҲΪ����״̬
    {
        // Ԥ������Ϊ����״̬����û�ý��յ���Ϣ������Ϊ��ǰ״̬
        mqtt_state.mqtt_task_state = MQTT_TYPE_SEND;
        // �ȴ��ظ���ʱ�����������ʱ�ļ�ʱ��
        mqtt_state.mqtt_wait_receive_count = 0;
        // ���񵹼�ʱ����һ�����մ�����¼���ͬʱ���û�лظ�Ҳ��Ҫ��ը��̫�죬���ǿ���һ���͵�Ƶ�ʣ�һ��Ϊ5ms
        mqtt_state.mqtt_task_state_count_down = 20;

        // ִ����������
        switch (mqtt_state.mqtt_at_type)
        {
            // ��������----------------
            case MQTT_TYPE_AT:
            case MQTT_TYPE_ATE0:  
            case MQTT_TYPE_QICSGP:
            case MQTT_TYPE_CGDCONT:
            case MQTT_TYPE_QIREGAPP:
            case MQTT_TYPE_CGACT:
            case MQTT_TYPE_QIACT:
            // ͬ������ʱ��
            case MQTT_TYPE_QNITZ:
            // MQTT����---------------
            case MQTT_TYPE_VERSIN:
            case MQTT_TYPE_KEEPALIVE:
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_at_type++;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                break;
            
            case MQTT_TYPE_ALIAUTH: 
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_CSQ;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
				break;

            // ͬ������ʱ��
            case MQTT_TYPE_QLTS: 
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QLTS) == TRUE )
                {
                    memset(mqtt_lock_in_time, 0, sizeof(mqtt_lock_in_time));
                    uint8_t *q = &mqtt_lock_in_time[0];
                    uint8_t *p = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QLTS); 
                    p = p + strlen(AT_REPLY_QLTS);
                    
                    _Bool temp_start = 0;
                    uint8_t temp_count = 0;
                    do{
                        if(*p != '"' && temp_start == TRUE)
                        {
                            *q = *p;
                            q++;
                        }
                        else if(*p == '"' && temp_start != TRUE)
                        {
                            temp_start = TRUE;
                            temp_count = 0;
                        }
                        else if(*p == '"' && temp_start == TRUE)
                        {
                            *q = '\0';
                            break;
                        }

                        if(temp_count > 28)
                        {
                            *q = '\0';
                            break;
                        }
                        p++;
                        temp_count++;
                    } while (1);

                    mqtt_state.mqtt_at_type = MQTT_TYPE_CGDCONT;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
				break;

            // ��ȡһ���ź�
            case MQTT_TYPE_CSQ: 
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_CSQ) == TRUE )
                {
                    csq_value = 0;
                    uint8_t *p = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_CSQ); 
                    p = p + strlen(AT_REPLY_CSQ);

                    do{
                        if(*p >= 0x30 && *p <= 0x39)
                        {
                            csq_value = csq_value * 10 + (*p - '0');
                        }
                        p++;
                    } while (*p >= 0x30 && *p <= 0x39);

                    mqtt_state.mqtt_at_type = MQTT_TYPE_QMTOPEN;
                    // ׼���������������
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME*2;
                }
                break;
            
            // ���ڸ��Ż�վ����
            case MQTT_TYPE_CGATT:        // ��û�и��Ż�վ
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_CGATT_OK) == TRUE )
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QIREGAPP;     // ִ�н�������
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                else
                {
                    mqtt_state.mqtt_task_state_count_down = 200;       // ����ȴ� 1S
                }
                break;

            case MQTT_TYPE_QILOCIP:        // �鿴IP��ַ
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QILOCIP_OK) == TRUE )
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QISTAT;     // ִ�н�������
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                else
                {
                    mqtt_state.mqtt_task_state_count_down = 200;       // ����ȴ� 1S
                }
                break;

            case MQTT_TYPE_QISTAT:        // �鿴����״̬
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QISTAT_OK) == TRUE )
                {
                    if( mqtt_state.mqtt_ota_state == OTA_TYPE_UPGRADE_TASK)
                    {
                        mqtt_state.mqtt_at_type = MQTT_TYPE_QHTTPURL;     // �����ļ�
                    }
                    else
                    {
                        mqtt_state.mqtt_at_type = MQTT_TYPE_VERSIN;     // �ϱ�����
                    }

                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                else
                {
                    mqtt_state.mqtt_task_state_count_down = 200;       // ����ȴ� 1S
                }
                break;

            // ��������������� ---------------------------------------------------------------------------------------------------

            // ����������MQTT�Ĺ��� --------------------------------------------------------
            case MQTT_TYPE_QMTOPEN:     // ����TCP
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;
                }
                else if( (bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTOPEN_OK) == TRUE )
                {              
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QMTCONN;
                }
                break;

            case MQTT_TYPE_QMTCONN:     // ����MQTT����
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;
                }
                else if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTCONN_OK) == TRUE )
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QMTPUB;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                else
                {
                    system_ble_lpuart0_send_string(AT_QMTDISC, strlen(AT_QMTDISC));
                    system_ble_lpuart0_send_string(AT_QMTCLOSE, strlen(AT_QMTCLOSE));
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QMTOPEN;
                }
                break;

            case MQTT_TYPE_QMTPUB:       // �ϴ�����
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;
                }
                else if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTPUB_OK) == TRUE )
                {
                    if(_this_pointer._this_product_id_pointer->have_reported == 0 \
                                                    || bootloader_get_fail() == TRUE) // ���û���ϱ����ϱ�/������û�и���ʧ��
                    {
                        mqtt_state.mqtt_at_type = MQTT_TYPE_QMTSUB;
                    }
                    else    // �ϱ����Ͳ�ѯһ����û�и���
                    {
                        mqtt_state.mqtt_at_type = MQTT_TYPE_QMTPUB_OTA;
                    }
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                break;

            // ��ȡOTA��� ------------------------------------------------------------------
            case MQTT_TYPE_QMTSUB:          // ����OTA������ȡ
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;
                }
                else if((_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTSUB) == TRUE)
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QMTPUB_VERSION;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                break;

            case MQTT_TYPE_QMTPUB_VERSION:   // �ϱ��汾
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;
                }
                else if((_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTPUB_OK) == TRUE)
                {
                    // �ϱ��ɹ�������flash
                    _this_pointer._this_product_id_pointer->have_reported = 1;
                    // ���´洢��Ϣ
                    op_manage_verify_version_numbe(NULL, TRUE);

                    if(bootloader_get_fail() != TRUE)
                    {
                        mqtt_state.mqtt_at_type = MQTT_TYPE_QMTPUB_OTA;
                        gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                    }
                    else
                    {
                        // �������ʧ�ܱ�־
                        bootloader_clear_fail();
                        // ����ػ�����
                        system_gsm_ready_to_shut_down();
                    }
                }
                break;

            case MQTT_TYPE_QMTPUB_OTA:       // OTA��ȡ
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE  \
                        || (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTPUB_OK) == TRUE)
                {
                    mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;

                    // �ж���û��������url����
                    if((_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTSUB_OTA) == TRUE)
                    {
                        // ��ȡURL����
                        system_gsm_mqtt_get_uart();              
                    }
                }
                else if((_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QMTSUB_OTA) == TRUE)
                {
                    // ��ȡURL����
                    system_gsm_mqtt_get_uart();              
                }
                break;

            // ������HTTP��������------------------------------------------------------------
            case MQTT_TYPE_QHTTPURL: 
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QHTTPGET;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME * 2;
                }
                break;

            case MQTT_TYPE_QHTTPGET: 
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QHTTPDL;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME;
                }
                break;

            // �����ļ����᷵���ļ���С
            case MQTT_TYPE_QHTTPDL:  
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
					if((_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QHTTPDL) == TRUE)
					{
						uint8_t *p = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QHTTPDL);
						p = p + strlen(AT_REPLY_QHTTPDL);    // �ƶ�������ִ�����
						
						uint32_t temp_ota_bin_size = 0;
						do{
							// �����ڿո�\r��\n��','
							if(*p >= 0x30 && *p <= 0x39)
							{
								temp_ota_bin_size = temp_ota_bin_size * 10 + (*p - '0');
							}
							p++;
						} while (*p >= 0x30 && *p <= 0x39);

						// ����OTA���ļ���С
						ota_set_gsm_bin_size(temp_ota_bin_size);
						
						mqtt_state.mqtt_at_type = MQTT_TYPE_QFOPEN;
						gsm_power_restart_time = GSM_POWER_RESTART_TIME;
					}
					else
					{
						mqtt_state.mqtt_at_type = MQTT_TYPE_QHTTPDL;
						mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;
						gsm_power_restart_time = GSM_POWER_RESTART_TIME * 2;
					}
                }
				else if((_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QHTTPDL) == TRUE)
				{
					uint8_t *p = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QHTTPDL);
					p = p + strlen(AT_REPLY_QHTTPDL);    // �ƶ�������ִ�����
					
					uint32_t temp_ota_bin_size = 0;
					do{
						// �����ڿո�\r��\n��','
						if(*p >= 0x30 && *p <= 0x39)
						{
							temp_ota_bin_size = temp_ota_bin_size * 10 + (*p - '0');
						}
						p++;
					} while (*p >= 0x30 && *p <= 0x39);

					// ����OTA���ļ���С
					ota_set_gsm_bin_size(temp_ota_bin_size);
					
					mqtt_state.mqtt_at_type = MQTT_TYPE_QFOPEN;
					gsm_power_restart_time = GSM_POWER_RESTART_TIME;
				}
                break;

            case MQTT_TYPE_QFOPEN:    
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_OK) == TRUE )
                {
                    // ��ȡ�ļ�λ��ָ��
                    uint8_t *p = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QFOPEN);
                    p = p + strlen(AT_REPLY_QFOPEN);    // �ƶ�������ִ�����
                    
                    ota_bin_pointer = 0;
                    do{
                        if(*p >= 0x30 && *p <= 0x39)
                        {
                            ota_bin_pointer = ota_bin_pointer * 10 + (*p - '0');
                        }
                        p++;
                    } while (*p >= 0x30 && *p <= 0x39);

                    mqtt_state.mqtt_at_type = MQTT_TYPE_QFREAD;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME * 2;

                    // �ȶ�ȡ�ļ�
                    ota_bin_read_size = READ_GSM_BIN_SIZE;
                }
                else if(ota_bin_pointer != 0)
                {
                    mqtt_state.mqtt_at_type = MQTT_TYPE_QFREAD;
                    gsm_power_restart_time = GSM_POWER_RESTART_TIME * 2;

                    // �ȶ�ȡ�ļ�
                    ota_bin_read_size = READ_GSM_BIN_SIZE;
                }

                break;

            case MQTT_TYPE_QFREAD:   
                if( (_Bool)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QFREAD) == TRUE )
                {
                    uint8_t *bin_head = (uint8_t *)strstr(strM25_Fram_Record.Data_RX_BUF, AT_REPLY_QFREAD); 
                    bin_head = bin_head + strlen(AT_REPLY_QFREAD);  // λ����8λ��������ͷ��0x20
                    
                    // ��ʼ��ȡУ���룡������
                    if(ota_bin_read_size == READ_GSM_BIN_CRC)
                    {
                        // ��ȡУ�����������
                        ota_set_bin_crc(bin_head, READ_GSM_BIN_CRC);
                    }
                    else   // ��bin�ļ�
                    {
                        // ��ȡbin�ļ�
                        ota_set_bin_file(bin_head);
                    }
                }

                if(ota_bin_url != NULL)
                {
                    free(ota_bin_url);
                    ota_bin_url = NULL;
                }    
                break;

            default:
                break;
        }

        // ������ջ����Լ���ʱ
        strM25_Fram_Record.InfBit.FramLength = 0;
        strM25_Fram_Record.InfBit.FramFinishFlag = 0;
        irq_time.timer_5_ms_count = 0;
    }

}


/********************************************************************
 ** \brief  MQTT���ʹ���
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_gsm_mqtt_send(void)
{
    if(mqtt_state.mqtt_at_type != MQTT_TYPE_NULL && mqtt_state.mqtt_task_state == MQTT_TYPE_SEND \
                                                    && mqtt_state.mqtt_task_state_count_down == 0)
    {
        // �����һ�½��յĺ���
        strM25_Fram_Record.InfBit.FramLength = 0;
        strM25_Fram_Record.InfBit.FramFinishFlag = 0;
        irq_time.timer_5_ms_count = 0;

        switch (mqtt_state.mqtt_at_type)
        {
            case MQTT_TYPE_AT: 
                system_ble_lpuart0_send_string(AT, strlen(AT));
                break;

            case MQTT_TYPE_ATE0:
                system_ble_lpuart0_send_string(ATE0, strlen(ATE0));
                break;

            case MQTT_TYPE_QICSGP:  // APN����
                system_ble_lpuart0_send_string(AT_QICSGP, strlen(AT_QICSGP));
                break;

            case MQTT_TYPE_CGATT:  // �Ƿ񸽻�վ
                system_ble_lpuart0_send_string(AT_CGATT, strlen(AT_CGATT));
                break;

            case MQTT_TYPE_QIREGAPP:  // ִ�н�������
                system_ble_lpuart0_send_string(AT_QIREGAPP, strlen(AT_QIREGAPP));
                break;

            case MQTT_TYPE_QNITZ:  // ͬ������ʱ��
                system_ble_lpuart0_send_string(AT_QNITZ, strlen(AT_QNITZ));
                break;

            case MQTT_TYPE_QLTS:  // ��ȡ����ʱ��
                system_ble_lpuart0_send_string(AT_QLTS, strlen(AT_QLTS));
                break;

            case MQTT_TYPE_CGDCONT: // PDP
                system_ble_lpuart0_send_string(AT_CGDCONT, strlen(AT_CGDCONT));
                break;

            case MQTT_TYPE_CGACT:  // ����PDP
                system_ble_lpuart0_send_string(AT_CGACT, strlen(AT_CGACT));
                break;

            case MQTT_TYPE_QIACT:  // �����ƶ�����
                system_ble_lpuart0_send_string(AT_QIACT, strlen(AT_QIACT));
                break;

            case MQTT_TYPE_QILOCIP:  // �����ƶ�����
                system_ble_lpuart0_send_string(AT_QILOCIP, strlen(AT_QILOCIP));
                break;

            case MQTT_TYPE_QISTAT:  // �鿴��������״̬
                system_ble_lpuart0_send_string(AT_QISTAT, strlen(AT_QISTAT));
                break;

            case MQTT_TYPE_CSQ:  // ��ȡģ���ź�
                system_ble_lpuart0_send_string(AT_CSQ, strlen(AT_CSQ));
                break;

            case MQTT_TYPE_VERSIN:  // MQTT�汾����
                system_ble_lpuart0_send_string(AT_QMTCFG_VERSIN, strlen(AT_QMTCFG_VERSIN));
                break;

            case MQTT_TYPE_KEEPALIVE:  // MQTT����
                system_ble_lpuart0_send_string(AT_QMTCFG_KEEPALIVE, strlen(AT_QMTCFG_KEEPALIVE));
                break;

            case MQTT_TYPE_ALIAUTH:   // MQTT�����Ʋ���
            case MQTT_TYPE_QMTOPEN: // TCP����
            case MQTT_TYPE_QMTCONN: // MQTT ����
            case MQTT_TYPE_QMTPUB: // MQTT������Ϣ
            case MQTT_TYPE_QMTPUB_VERSION:   // �ϱ��汾
            case MQTT_TYPE_QMTSUB:           // ����OTA������ȡ
            case MQTT_TYPE_QMTPUB_OTA:       // OTA��ȡ
                system_gsm_mqtt_send_splicing((enum MQTT_TYPE)mqtt_state.mqtt_at_type);
                break;

            // OTA��� -----------------------------------------------------------------
            case MQTT_TYPE_QHTTPURL: // ��������
                system_gsm_mqtt_send_splicing(MQTT_TYPE_QHTTPURL);
                break;

            case MQTT_TYPE_QHTTPGET:  // ��ȡ������Դ
                system_ble_lpuart0_send_string(AT_QHTTPGET, strlen(AT_QHTTPGET));
                break;

            case MQTT_TYPE_QHTTPDL: // ���浽�ڴ���
                system_ble_lpuart0_send_string(AT_QHTTPDL, strlen(AT_QHTTPDL));
                break;

            case MQTT_TYPE_QFOPEN: // �򿪸ձ����bin   
                system_ble_lpuart0_send_string(AT_QFOPEN, strlen(AT_QFOPEN));
                break;

            case MQTT_TYPE_QFREAD: // ��ȡ�򿪵��ļ�
                system_gsm_mqtt_send_splicing(MQTT_TYPE_QFREAD);
                break;

            default:
                break;
        }

        // ���淢���������Ϊ����ģʽ
        mqtt_state.mqtt_task_state = MQTT_TYPE_WAIT_RX;
    }
}


// ���ý���ػ�����
void system_gsm_ready_to_shut_down(void)
{
    mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_OFF;  // �ػ�

    system_ble_lpuart0_send_string(AT_QMTDISC, strlen(AT_QMTDISC));
    system_ble_lpuart0_send_string(AT_QMTCLOSE, strlen(AT_QMTCLOSE));

    gsm_power_restart_time = 0;
    mqtt_state.mqtt_unsent_count = 0;
}

// ���ý�����������
void system_gsm_ready_to_restart(void)
{
    mqtt_state.mqtt_task_state = MQTT_TYPE_POWER_RESTART;  // ����

    system_ble_lpuart0_send_string(AT_QMTDISC, strlen(AT_QMTDISC));
    system_ble_lpuart0_send_string(AT_QMTCLOSE, strlen(AT_QMTCLOSE));

    mqtt_state.mqtt_unsent_count = 0;
}


/********************************************************************
 ** \brief  MQTT�����̿���
 ** @param  ��
 ** \retval ��
********************************************************************/
void system_gsm_mqtt_manage(void)
{
    // ����/�ػ�
    system_gsm_enabled_state();

    // ���տ���
    system_gsm_mqtt_receive();
    
    // MQTT���Ϳ���
    system_gsm_mqtt_send();

    // ���͵���ʱ ��ʱȡ��ѭ�����͵ı�־λ����Ϊ��û��PG��Ҫ����  mqtt_state.mqtt_cyclic_send == TRUE
    if(mqtt_state.mqtt_task_state == MQTT_TYPE_NULL && gsm_power_sending_time_count == 0 && _system_paygo_state.paygo_id != FALSE)
    {
        // ��װMQTT�����¼�
        gsm_power_sending_time_count = DEFINE_GSM_POWER_SEND_TIME_COUNT;
   }
}


