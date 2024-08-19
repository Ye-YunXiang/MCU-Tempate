#ifndef _SYSTEM_ADC_H_
#define _SYSTEM_ADC_H_

#include "main_file_define.h"

// adc�ĵ͵�ѹ��
#define ADC_LEW_V       9100
// adc�ĸߵ�ѹ��
#define ADC_HIGH_V      9800

//v2.0.2 20124-3-7 �Լ�֮ǰ��
// #define BAT_ADC_OPEN()              Gpio_ClrIO(BAT_ADC_EN_PORT, BAT_ADC_EN_PIN)
// #define BAT_ADC_CLOSE()             Gpio_SetIO(BAT_ADC_EN_PORT, BAT_ADC_EN_PIN)
//v2.0.2 20124-3-22 �Լ�֮���
#define BAT_ADC_OPEN()              Gpio_SetIO(BAT_ADC_EN_PORT, BAT_ADC_EN_PIN)
#define BAT_ADC_CLOSE()             Gpio_ClrIO(BAT_ADC_EN_PORT, BAT_ADC_EN_PIN)

// �ɼ���ѹ
#define ADC_START()                 Adc_SQR_Start()

// ��ȡ�ⲿ����ĵ�ƽ״̬
#define ADV_5V_GET_STATE()          Gpio_GetInputIO(ADC_5V_PORT, ADC_5V_PIN)
// �ػ��жϼ�⣬�͵�ƽ
#define ADC_OPEN_5V_OFF_TEST_IRQ()  Gpio_DisableIrq(ADC_5V_PORT, ADC_5V_PIN, GpioIrqHigh); Gpio_EnableIrq(ADC_5V_PORT, ADC_5V_PIN, GpioIrqLow)
// �رչػ��жϼ�⣬�͵�ƽ
#define ADC_CLOSE_5V_OFF_TEST_IRQ() Gpio_DisableIrq(ADC_5V_PORT, ADC_5V_PIN, GpioIrqLow)
// �򿪿����жϼ�⣬�ߵ�ƽ
#define ADC_OPEN_5V_ON_TEST_IRQ()   Gpio_EnableIrq(ADC_5V_PORT, ADC_5V_PIN, GpioIrqHigh)

// ��ȡ�ⲿ����ĵ�ƽ״̬
#define ADV_TAMPER_GET_STATE()      Gpio_GetInputIO(ADC_TAMPER_PORT, ADC_TAMPER_PIN)
// ��ǹ����жϼ�⣬�͵�ƽ
#define ADC_TAMPER_OFF_TEST_IRQ()   Gpio_DisableIrq(ADC_TAMPER_PORT, ADC_TAMPER_PIN, GpioIrqHigh); Gpio_EnableIrq(ADC_TAMPER_PORT, ADC_TAMPER_PIN, GpioIrqLow)
// ��Ǵ��жϼ�⣬�ߵ�ƽ
#define ADC_TAMPER_ON_TEST_IRQ()    Gpio_DisableIrq(ADC_TAMPER_PORT, ADC_TAMPER_PIN, GpioIrqLow); Gpio_EnableIrq(ADC_TAMPER_PORT, ADC_TAMPER_PIN, GpioIrqHigh)
// �رպ�ǹ����жϼ��
#define ADC_TAMPER_CLOSE_TEST_IRQ() Gpio_DisableIrq(ADC_TAMPER_PORT, ADC_TAMPER_PIN, GpioIrqHigh);Gpio_DisableIrq(ADC_TAMPER_PORT, ADC_TAMPER_PIN, GpioIrqLow)

enum DEVICE_VOLTAGE
{
    NULL_VOLTAGE,       // ��ʼ��   
    LOW_VOLTAGE,        // < 9.1V         24V��< 21V
    MEDIUM_VOLTAGE,     // 9.1 - 9.8V     24V: 21V - 22.6V
    HIGH_VOLTAGE        // > 9.8V         24V: >22.6   
};

struct ADC_SAMPLE_STRUCT
{
    // �豸��ѹ
    uint8_t device_voltage;
	// �ɼ�������ʶ��
    _Bool ADC_sample_flag;
    // 2s�ź�
    uint8_t ADC_2s_flg;
    // �������ݣ� ��0��Ϊ��ص�ѹ����1��Ϊ�ⲿ��Դ��ѹ����2���¶�
    uint32_t ADC_sample_data[3];   
};

extern struct ADC_SAMPLE_STRUCT	__ADC;

// ����ⲿ��ƽ���ų�ʼ����5V����ǹ���
void system_adc_input_check_port_init(void);

// �����ADC�������ų�ʼ��
void system_adc_bat_control_port_init(void);

///< ADC �����˿ڳ�ʼ��
void system_adc_port_init(void);

// �����ADC��ʼ��
void system_adc_run_init(void);

// �ر�ADC���������ʱ��
void system_adc_sleep(void);

// // ADC�жϼ���ص�ѹ
// void Adc_IRQHandler(void);

// �򿪵��ADC���
void system_adc_sample_on(void);

// �رյ��ADC���
void system_adc_sample_off(void);

// ����ADC���ָ���õ������ĵ��¶�
uint32_t system_adc_get_kelvin_temperature(uint32_t u32AdcRestult);

#endif
