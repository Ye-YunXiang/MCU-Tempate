#ifndef __IO_I2C_H
#define __IO_I2C_H	 
#include "sys.h"

//ģ��I2C������������//

//I2C�����ٶ�����
#define VL2_SPEED		2  //ȡֵ1~255��ֵԽС�ٶ�Խ�죨�ȶ���Խ���Ĭ��ֵΪ2


//IO��������
#define VL2_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define VL2_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO��������	 
#define VL2_IIC_SCL    PBout(6) 		//SCL
#define VL2_IIC_SDA    PBout(7) 		//SDA	 
#define VL2_READ_SDA   PBin(7) 		    //����SDA 

//״̬
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01

//IIC��������
void I2C_Configuration(void);//��ʼ��IIC��IO��

u8 I2C_SAND_BYTE(u8 address,u8 index,u8 data);              //IICдһ��8λ����
u8 I2C_SAND_BUFFER(u8 address, u8 index,u8 *pdata,u16 count);//IIC����д

u8 I2C_READ_BYTE(u8 address,u8 index,u8 *pdata);             //IIC��һ��8λ����
u8 I2C_READ_BUFFER(u8 address,u8 index,u8 *pdata,u16 count);  //IIC������

#endif
