/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_set_clock.h
* Description:	initialize clock and Timer
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/

#ifndef __SYSTEM_CLOCK_SET_H
#define __SYSTEM_CLOCK_SET_H

#include "main_file_define.h"

// ���ﶨ5ms
#define u16ArrValue_time0               30000    // ��ʱ��
// ���ﶨ1s
#define u16ArrValue_lptime_rum          32768    // ����״̬��ʱ����ʹ�� RCL  ,��Ƶ256
// ���ﶨ��128Ϊһ�룩
#define u16ArrValue_lptime_sleep        0    // ˯��״̬��ʱ����ʹ�� RCL  ,��Ƶ256

#define TIMER_5ms_ON()                    Bt_M0_Run(TIM0)
#define TIMER_5ms_OFF()                   Bt_M0_Stop(TIM0)


void system_clock_configuration_init(void);
// ϵͳ����ʱʱ������
void system_clock_sleep_configuration(void);
// ϵͳ˯��ʱʱ������
void system_clock_sleep_configuration(void);

void system_timer0_configuration(void);            // 1s

// ��timer0
void system_timer0_open(void);
// �ر�timer0
void system_timer0_close(void);

void system_run_lptimer_configuration(void);       // 1s
// ��lpTime0�ж�
void system_run_lptimer_open(void);
// �ر�lpTime0�ж�
void system_run_lptimer_close(void);

void system_time_clock_config(void);


#endif //__SYSTEM_CLOCK_SET_H
