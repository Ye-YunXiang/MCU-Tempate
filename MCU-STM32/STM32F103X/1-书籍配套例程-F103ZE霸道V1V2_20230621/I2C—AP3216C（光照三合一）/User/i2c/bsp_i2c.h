#ifndef __I2C_H
#define	__I2C_H


#include "stm32f10x.h"


/**************************I2C�������壬I2C1��I2C2********************************/
#define             I2Cx                                I2C1
#define             I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             I2C_CLK                             RCC_APB1Periph_I2C1
#define             I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             I2C_SCL_PORT                        GPIOB   
#define             I2C_SCL_PIN                         GPIO_Pin_6
#define             I2C_SDA_PORT                        GPIOB 
#define             I2C_SDA_PIN                         GPIO_Pin_7



/* STM32 I2C ����ģʽ */
#define I2C_Speed              400000  //*
/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
#define I2Cx_OWN_ADDRESS7      0X0A   

/*�ȴ���ʱʱ��*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


void I2C_Init_Config(void);
uint32_t I2C_Mem_Write(uint16_t DevAddress,u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
uint32_t I2C_Mem_Read(uint16_t DevAddress,u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
#endif /* __I2C_EE_H */
