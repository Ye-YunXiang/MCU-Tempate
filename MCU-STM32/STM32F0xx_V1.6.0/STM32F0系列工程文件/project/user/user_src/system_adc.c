#include "system_adc.h"


#define T25 298.15    // 25���µĿ������¶ȣ�Ҳ����298.15K
#define R25 10        // ���������������ֵ
#define B	3435      // �������豾��ϵ��


// �豸״̬���豸��ѹ���豸�ɼ���������flash�е��¶�У׼ֵ��ADC�ɼ����ݣ���ص�ѹ���ⲿ��Դ��ѹ�������¶ȣ�
struct ADC_SAMPLE_STRUCT __ADC = {0, 0, 0, {0,0,0}};


// ����ⲿ��ƽ���ų�ʼ����5V����ǹ���
void system_adc_input_check_port_init(void)
{
    ///< ��������ʱ�� --------------------------------------------
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
    stc_gpio_cfg_t 	stcGpioCfg;

    stcGpioCfg.enDir         = GpioDirIn;
    stcGpioCfg.enDrv         = GpioDrvL;
    stcGpioCfg.enPu          = GpioPuDisable;
    stcGpioCfg.enPd          = GpioPdDisable;
    stcGpioCfg.enOD          = GpioOdDisable;
    stcGpioCfg.enCtrlMode    = GpioAHB;

    // ��ʼ������ⲿ����5V---------------------------------------------------	
    Gpio_Init(ADC_5V_PORT, ADC_5V_PIN, &stcGpioCfg); 
    ADC_OPEN_5V_OFF_TEST_IRQ(); ///< �򿪲����õ͵�ƽ�ж�

    // ��ʼ������Ǵ�����---------------------------------------------------	
    Gpio_Init(ADC_TAMPER_PORT, ADC_TAMPER_PIN, &stcGpioCfg); 
    ADC_TAMPER_CLOSE_TEST_IRQ(); ///< �رչ����жϼ��

    // ʹ��IO���ж�---------------------------------------------------
    EnableNvic(PORTA_IRQn, IrqLevel2, TRUE);               ///< ʹ�ܶ˿�PORTAϵͳ�ж�
}

// �����ADC�������ų�ʼ��
void system_adc_bat_control_port_init(void)
{
    ///< ����GPIO����ʱ�� --------------------------------------------
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
    // ��ʼ�����Ƽ���ؿ���---------------------------------------------------
	stc_gpio_cfg_t 	stc_gpio_cfg_init;
	
	stc_gpio_cfg_init.enDir			= GpioDirOut;
	stc_gpio_cfg_init.enDrv			= GpioDrvL;
	stc_gpio_cfg_init.enPu 			= GpioPuDisable;
	stc_gpio_cfg_init.enPd 			= GpioPdDisable;
	stc_gpio_cfg_init.enCtrlMode		= GpioAHB;
	
	Gpio_Init(BAT_ADC_EN_PORT, BAT_ADC_EN_PIN, &stc_gpio_cfg_init);	
    BAT_ADC_CLOSE();     // �رռ��
}

///< ADC �����˿ڳ�ʼ��
void system_adc_port_init(void)
{    
    ///< ��������ʱ�� --------------------------------------------
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
    stc_gpio_cfg_t 	stcGpioCfg;

    stcGpioCfg.enDir         = GpioDirIn;
    stcGpioCfg.enDrv         = GpioDrvL;
    stcGpioCfg.enPu          = GpioPuDisable;
    stcGpioCfg.enPd          = GpioPdDisable;
    stcGpioCfg.enOD          = GpioOdDisable;
    stcGpioCfg.enCtrlMode    = GpioAHB;

    Gpio_Init(ADC_BAT_PORT, ADC_BAT_PIN, &stcGpioCfg); 
    Gpio_Init(ADC_24V_PORT, ADC_24V_PIN, &stcGpioCfg);
    Gpio_Init(ADC_TEMP_PORT, ADC_TAMPER_PIN, &stcGpioCfg);

    Gpio_SetAnalogMode(ADC_BAT_PORT, ADC_BAT_PIN);           //PB14 (AIN21)
    Gpio_SetAnalogMode(ADC_24V_PORT, ADC_24V_PIN);           //PA00 (AIN0)
    Gpio_SetAnalogMode(ADC_TEMP_PORT, ADC_TAMPER_PIN);       //PB15 (AIN22)
}


// �����ADC��ʼ��
void system_adc_run_init(void)
{
    ///< ADCģ�� ��ʼ�� -------------------------------------------------------
    stc_adc_cfg_t              stcAdcCfg;
    DDL_ZERO_STRUCT(stcAdcCfg);
    
    // ///< ����ADC/BGR����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);
    Bgr_BgrEnable();                                        ///< ����BGR
	
    // Bgr_TempSensorEnable(); // ʹ���ڲ��¶ȴ�����
    
    ///< ADC ��ʼ������
    stcAdcCfg.enAdcMode         = AdcScanMode;               ///<����ģʽ-ɨ��
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv8;            ///<������Ƶ-8
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle12Clk;     ///<����������-12
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelInBgr2p5; 	///<�ο���ѹѡ��-�ڲ�2.5
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;          ///<OP BUF����-��
    stcAdcCfg.enInRef           = AdcMskInRefEnable;        ///<�ڲ��ο���ѹʹ��-��
    stcAdcCfg.enAdcAlign        = AdcAlignRight;            ///<ת��������뷽ʽ-��
    Adc_Init(&stcAdcCfg);
	
    // ADC ˳�����ģʽ ����------------------------------------------------
    stc_adc_sqr_cfg_t          stcAdcSqrCfg;
    
    DDL_ZERO_STRUCT(stcAdcSqrCfg);
        
    ///< ˳��ɨ��ģʽ���ܼ�ͨ������
    stcAdcSqrCfg.bSqrDmaTrig = FALSE;
    stcAdcSqrCfg.enResultAcc = AdcResultAccDisable;
    stcAdcSqrCfg.u8SqrCnt    = 4;
    Adc_SqrModeCfg(&stcAdcSqrCfg);

    Adc_CfgSqrChannel(AdcSQRCH0MUX, AdcExInputCH21);    // С���
    Adc_CfgSqrChannel(AdcSQRCH1MUX, AdcExInputCH0);     // �ⲿ��Դ
    Adc_CfgSqrChannel(AdcSQRCH2MUX, AdcExInputCH22);    // �����¶�
 
    // ����жϱ�־λ
    Adc_ClrIrqStatus(AdcMskIrqSqr);
    ///< ADC �ж�ʹ��
    Adc_EnableIrq();
    EnableNvic(ADC_DAC_IRQn, IrqLevel3, TRUE);

    __ADC.ADC_sample_flag = 0;
    
    // ///< ����˳��ɨ�����
    //  Adc_SQR_Start();
    //	BAT_ADC_OPEN();
}

// �ر�ADC���������ʱ��
void system_adc_sleep(void)
{
    // ����жϱ�־λ
    Adc_ClrIrqStatus(AdcMskIrqSqr);
    ///< ADC �ж�ʹ��
    Adc_DisableIrq();
    EnableNvic(ADC_DAC_IRQn, IrqLevel3, FALSE);
    // �رղ���
    Adc_SQR_Stop();
    // �ر�ADC����
    BAT_ADC_CLOSE();
    Bgr_BgrDisable();            ///< �ر�BGR
	
	// ���³�ʼ��ADC
	M0P_ADC->CR0 = 0x27F0;
	
	// �ر�ADCʱ��
	Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, FALSE);
}

// �򿪵��ADC���
void system_adc_sample_on(void)
{
    // �򿪵��ADC����
    BAT_ADC_OPEN();
	
	delay1ms(5);

    __ADC.ADC_sample_flag = 0; 	

    // ����жϱ�־λ
    Adc_ClrIrqStatus(AdcMskIrqSqr);
    ///< ����˳��ɨ�����
    Adc_SQR_Start();                    
}

// �رյ��ADC���
void system_adc_sample_off(void)
{
    // �رղ���
    Adc_SQR_Stop();
    // �ر�ADC����
    BAT_ADC_CLOSE();
}



// �¶���ء���������������������������������������������������������
//�������ܣ�

//B����һ��ϵ�������������豾��ϵ���йأ��ٶ�˵����ɳ����ṩ����3435�������Ҽ��ɣ�
//R25�������������ֵ��
//Rntc������������������������ֵ
//T25��25���µĿ������¶ȣ�Ҳ����298.15K
//Tn����ʱ����õ���ʵ�ʿ������¶ȣ�Ҫ�õ����϶� C=Tn-273.15 ��

//ģ����ת����
float temp_Get_R(uint32_t u32AdcRestult)
{
	//2.5�ο���ѹ���������ѹ
	float v1 = (float)(u32AdcRestult * 2.5) / 4096;//ntc�ϵĵ�ѹ
	float v2 = 2.5 - v1;
	float r = (v1 / v2) * 10;
	
	return r;
}

//ʵ��ln(x) �ļ���
double myln(double a)
{
   int N = 15;//ȡ��ǰ15+1��������
   int k, nk;
   double x, xx, y;
   x = (a - 1) / (a + 1);
   xx = x * x;
   nk = 2 * N + 1;
   y = 1.0 / nk;
   for(k = N; k > 0; k--)
   {
     nk = nk - 2;
     y = 1.0 / nk + xx * y;
   }

   return 2.0 * x * y;
}

// ����ADC���ָ���õ������ĵ��¶�
uint32_t system_adc_get_kelvin_temperature(uint32_t u32AdcRestult)
{
	float N1, N2, N3, N4;
	float Rntc = temp_Get_R(u32AdcRestult);
	N1 = (myln(R25) - myln(Rntc)) / B;
	N2 = 1 / T25 - N1;
	N3 = 1 / N2;
	N4 = N3 - 273.15;//������ת���϶�
	
    // �Ŵ��¶�1000��
	return (uint32_t)(N4 * 1000);
}
