/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_internal_rtc.h
* Description:	Initialize RTC
* 
* Author:		Poetry Cloud
* Date: 	 	2023/12/25
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Cloud create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/
#ifndef __SYSTEM_INTERNAL_RTC_H
#define __SYSTEM_INTERNAL_RTC_H

#include "main_file_define.h"

#define	rtc_cmd_open()			    Rtc_Cmd(TRUE)
#define	rtc_cmd_close()			    Rtc_Cmd(FALSE)

#define YEAR_T 	    0x23
#define MON_T		0x01
#define DAY_T		0x01
#define HOUR_T	    0x00
#define MIN_T		0x00
#define SEC_T		0x00
#define	WEEK_T	    0x01

#pragma pack(1)
struct rtc_time
{
    uint8_t  sec;				//ʱ�䣺��    
    uint8_t  min;				//ʱ�䣺��
    uint8_t  hour;    	        //ʱ�䣺ʱ
    uint8_t  day;				//ʱ�䣺��    
    uint8_t  mon;               //ʱ�䣺��
    uint8_t year;			    //ʱ�䣺��
};
#pragma pack()

// ��ʼ��RTC
void system_internal_rtc_init(void);

// ��������
void rtc_alarm_irq_open(void);

// �ر�����
void rtc_alarm_irq_close(void);

// ����RTCʱ��
uint8_t system_internal_rtc_set_time(struct rtc_time *rtc_time);
// ��ȡRTCʱ��
void system_internal_rtc_get_time(struct rtc_time *get_rtc);

#endif
