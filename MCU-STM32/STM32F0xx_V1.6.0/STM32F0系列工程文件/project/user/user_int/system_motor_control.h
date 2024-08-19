#ifndef __SYSTEM_MOTOR_CONTROL_H_
#define __SYSTEM_MOTOR_CONTROL_H_

#include "main_file_define.h"

// ������
#define SYSTEM_REFRIGERATOR_ENABLE()	Gpio_SetIO(REFRIGERATOR_PORT,REFRIGERATOR_PIN)
// �ر���
#define SYSTEM_REFRIGERATOR_DISABLE() Gpio_ClrIO(REFRIGERATOR_PORT,REFRIGERATOR_PIN)

// ��ʼ���������
void system_refrigerator_control_init(void);

// �򿪱���
void system_refrigerator_control_open(void);

// �رձ���
void system_refrigerator_control_shut(void);


#endif
