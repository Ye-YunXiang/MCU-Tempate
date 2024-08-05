#ifndef __IO_I2C_H
#define __IO_I2C_H	 
#include "sys.h"

//模拟I2C总线驱动程序//

//I2C总线速度设置
#define VL2_SPEED		2  //取值1~255，值越小速度越快（稳定性越差），默认值为2


//IO方向设置
#define VL2_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define VL2_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO操作函数	 
#define VL2_IIC_SCL    PBout(6) 		//SCL
#define VL2_IIC_SDA    PBout(7) 		//SDA	 
#define VL2_READ_SDA   PBin(7) 		    //输入SDA 

//状态
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01

//IIC操作函数
void I2C_Configuration(void);//初始化IIC的IO口

u8 I2C_SAND_BYTE(u8 address,u8 index,u8 data);              //IIC写一个8位数据
u8 I2C_SAND_BUFFER(u8 address, u8 index,u8 *pdata,u16 count);//IIC连续写

u8 I2C_READ_BYTE(u8 address,u8 index,u8 *pdata);             //IIC读一个8位数据
u8 I2C_READ_BUFFER(u8 address,u8 index,u8 *pdata,u16 count);  //IIC连续读

#endif
