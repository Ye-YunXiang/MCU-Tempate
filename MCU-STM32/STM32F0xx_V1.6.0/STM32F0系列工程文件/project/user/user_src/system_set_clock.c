/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_set_clock.c 
* Description:	initialize clock and Timer
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#include "system_set_clock.h"

/********************************************************************
 ** \brief  ��ʼ����״̬��ϵͳʱ��
 ** \retval ��
********************************************************************/
void system_clock_configuration_init(void)
{
    stc_sysctrl_clk_cfg_t	StcClkCfg;
	
	///< RCHʱ�Ӳ�ͬƵ�ʵ��л�����Ҫ�Ƚ�ʱ���л���RCL
	// �����л�Ϊ�ⲿRCL��˳��ʹ���ⲿRCL
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    Sysctrl_SetRCLStableTime(SysctrlRclStableCycle64);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
	Sysctrl_ClkSourceEnable(SysctrlClkXTL, TRUE);
	//Sysctrl_SysClkSwitch(SysctrlClkXTL);
    
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    
    ///< HCLK������24M���˴�����FLASH���ȴ�����Ϊ0 cycle
    Flash_WaitCycle(FlashWaitCycle0);
	
	
    /** �޸�ϵͳʱ�ӷ�Ƶ *************************************/
	StcClkCfg.enClkSrc    = SysctrlClkRCH;
    StcClkCfg.enHClkDiv   = SysctrlHclkDiv1;
    StcClkCfg.enPClkDiv   = SysctrlPclkDiv1;
 
    Sysctrl_ClkInit(&StcClkCfg);

    Flash_Init(6, TRUE);
}


/********************************************************************
 ** \brief  ����ʱ��Ƶ��Ϊ����״̬
 ** \retval ��
********************************************************************/
 void system_clock_sleep_configuration(void)
 {
     stc_sysctrl_clk_cfg_t	StcClkCfg;
	
 	///< RCHʱ�Ӳ�ͬƵ�ʵ��л�����Ҫ�Ƚ�ʱ���л���RCL
 	// �����л�Ϊ�ⲿRCL
     Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
     Sysctrl_SetRCLStableTime(SysctrlRclStableCycle64);
     Sysctrl_ClkSourceEnable(SysctrlClkXTL, TRUE);
     
     Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);

     Sysctrl_ClkSourceEnable(SysctrlClkRCH, FALSE);
	 
	 Sysctrl_SysClkSwitch(SysctrlClkXTL);
    
     Flash_WaitCycle(FlashWaitCycle0);
	
     /** �޸�ϵͳʱ�ӷ�Ƶ *************************************/
 	StcClkCfg.enClkSrc    = SysctrlClkXTL;
     StcClkCfg.enHClkDiv   = SysctrlHclkDiv1;
     StcClkCfg.enPClkDiv   = SysctrlPclkDiv1;
 
     Sysctrl_ClkInit(&StcClkCfg);

     Flash_Init(1, TRUE);
 }


/********************************************************************
 ** \brief  ��ʼ����ʱ��timer0����ʱ5ms
 ** \retval ��
********************************************************************/
void system_timer0_configuration(void)
{
    uint16_t u16ArrValue;

	uint16_t                   u16CntValue;
//    uint8_t                    u8ValidPeriod;
    stc_bt_mode0_cfg_t     stcBtBaseCfg;
	
	DDL_ZERO_STRUCT(stcBtBaseCfg);				

	Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE);
	
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //��ʱ��ģʽ
    stcBtBaseCfg.enCT       = BtTimer;                      //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv4;                 //PCLK
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //�Զ�����16λ������/��ʱ��
    stcBtBaseCfg.bEnTog     = FALSE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                     //TIM0 ��ģʽ0���ܳ�ʼ��
    
    u16ArrValue = 0x10000 - u16ArrValue_time0;
    Bt_M0_ARRSet(TIM0, u16ArrValue);                        //��������ֵ(ARR = 0x10000 - ����)
    
    u16CntValue = 0x10000 - u16ArrValue_time0;
    Bt_M0_Cnt16Set(TIM0, u16CntValue);                      //���ü�����ֵ
    
    // Bt_ClearIntFlag(TIM0,BtUevIrq);                         //���жϱ�־   
    // Bt_Mode0_EnableIrq(TIM0);                               //ʹ��TIM0�ж�(ģʽ0ʱֻ��һ���ж�)
    // EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);                 //TIM0�ж�ʹ��
    // // Bt_M0_Run(TIM0);
    // Bt_M0_Stop(TIM0);  // Ϊ���Ŀ���
}


// ��timer0�ж�
void system_timer0_open(void)
{
	// ��ʱ��
//	Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE);
	
    Bt_ClearIntFlag(TIM0,BtUevIrq);                         //���жϱ�־   
    Bt_Mode0_EnableIrq(TIM0);                               //ʹ��TIM0�ж�(ģʽ0ʱֻ��һ���ж�)
    EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);                 //TIM0�ж�ʹ��
    //Bt_M0_Run(TIM0);
    Bt_M0_Stop(TIM0);  // Ϊ���Ŀ���
}


// �ر�timer0�ж�
void system_timer0_close(void)
{	
    Bt_M0_Stop(TIM0);  // Ϊ���Ŀ���
    Bt_ClearIntFlag(TIM0,BtUevIrq);                         //���жϱ�־   
    Bt_Mode0_DisableIrq(TIM0);                               //ʹ��TIM0�ж�(ģʽ0ʱֻ��һ���ж�)
    EnableNvic(TIM0_IRQn, IrqLevel3, FALSE);                 //TIM0�ж�ʹ��
	
//	Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, FALSE);
}


/********************************************************************
 ** \brief  ��ʼ������״̬��LPTim0��ʹ�� RCL,��Ƶ256��1S
 ** \retval ��
********************************************************************/
void system_run_lptimer_configuration(void)
{
	stc_lptim_cfg_t    stcLptCfg;    
    DDL_ZERO_STRUCT(stcLptCfg);

    ///< ʹ��LPTIM0 ����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpTim0, TRUE);
    
    stcLptCfg.enPrs	   = LptimPrsDiv1;
    stcLptCfg.enGate   = LptimGateLow;
    stcLptCfg.enGatep  = LptimGatePLow;
	stcLptCfg.enTcksel = LptimXtl;   // ʹ���ⲿRCL
    // stcLptCfg.enTcksel = LptimRcl;   // ʹ���ڲ�RCL
    // stcLptCfg.enTogen  = LptimTogEnLow;
    stcLptCfg.enCt     = LptimTimerFun;           //����������
    stcLptCfg.enMd     = LptimMode2;            //����ģʽΪģʽ2���Զ���װ��16λ������/��ʱ��
    stcLptCfg.u16Arr   = u16ArrValue_lptime_rum;                 //Ԥװ�ؼĴ���ֵ��������ֵ 
    Lptim_Init(M0P_LPTIMER0, &stcLptCfg);

    // ����ṹ��rtc_state and irq_time
	open_paygo_struct_init();    // * ��λ�����ʱ�Ľṹ�� ************************************************
    
    Lptim_ClrItStatus(M0P_LPTIMER0);            //����жϱ�־λ
    Lptim_ConfIt(M0P_LPTIMER0, TRUE);           //����LPTIMER�ж�    
    EnableNvic(LPTIM_0_1_IRQn, IrqLevel3, TRUE); 
    Lptim_Cmd(M0P_LPTIMER0, TRUE);   ///< ����lptimer0����
}

// ��lpTime0�ж�
void system_run_lptimer_open(void)
{
	 ///< ʹ��LPTIM0 ����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpTim0, TRUE);
	
    Lptim_ClrItStatus(M0P_LPTIMER0);            //����жϱ�־λ
    Lptim_ConfIt(M0P_LPTIMER0, TRUE);           //����LPTIMER�ж�    
    EnableNvic(LPTIM_0_1_IRQn, IrqLevel3, TRUE); 
    Lptim_Cmd(M0P_LPTIMER0, TRUE);   ///< ����lptimer0����
}

// �ر�lpTime0�ж�
void system_run_lptimer_close(void)
{
    Lptim_Cmd(M0P_LPTIMER0, FALSE);   ///< �ر�lptimer0����
    Lptim_ClrItStatus(M0P_LPTIMER0);            //����жϱ�־λ
    EnableNvic(LPTIM_0_1_IRQn, IrqLevel3, FALSE); 
    Lptim_ConfIt(M0P_LPTIMER0, FALSE);           

    // ����ṹ��rtc_state and irq_time
	open_paygo_struct_init();    // * ��λ�����ʱ�Ľṹ�� ************************************************
    irq_time.sleep_open = TRUE;             // ��˯��ģʽ
	
	 ///< ʹ��LPTIM0 ����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpTim0, TRUE);
}


/********************************************************************
 ** \brief  δ��
 ** \retval ��
********************************************************************/
// void system_time_clock_config(void)
// {
// 	en_flash_waitcycle_t      enWaitCycle; 
//     stc_sysctrl_pll_cfg_t     stcPLLCfg;
    
//     DDL_ZERO_STRUCT(stcPLLCfg);								
    
//     enWaitCycle = FlashWaitCycle1;
//     Flash_WaitCycle(enWaitCycle);
    
//     stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     //RCH 4MHz
//     stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  //PLL ���48MHz
//     stcPLLCfg.enPllClkSrc = SysctrlPllRch;              //����ʱ��Դѡ��RCH
//     stcPLLCfg.enPllMul    = SysctrlPllMul2;            //4MHz x 12 = 48MHz
//     Sysctrl_SetPLLFreq(&stcPLLCfg);
//     Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
//     Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
    
//     Sysctrl_SysClkSwitch(SysctrlClkPLL);  				//ʱ���л�
// }
