

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1-201708202309 创建。


*/

//模拟I2C总线驱动程序//

#include "delay.h"
#include "io_i2c.h"



//I2C初始化
void I2C_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;  //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //50Mhz速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);//PA7,PA6 输出高	

}

//产生IIC起始信号
void VL2_IIC_Start(void)
{
	VL2_SDA_OUT();//sda线输出
	VL2_IIC_SDA=1;	  	  
	VL2_IIC_SCL=1;
	delay_us(VL2_SPEED*2);
 	VL2_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(VL2_SPEED*2);
	VL2_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void VL2_IIC_Stop(void)
{
	VL2_SDA_OUT();//sda线输出
	VL2_IIC_SCL=0;
	VL2_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(VL2_SPEED*2);
	VL2_IIC_SCL=1; 
	VL2_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(VL2_SPEED*2);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 VL2_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	VL2_SDA_IN();  //SDA设置为输入  
	VL2_IIC_SDA=1;delay_us(1);	   
	VL2_IIC_SCL=1;delay_us(1);	 
	while(VL2_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			VL2_IIC_Stop();
			return 1;
		}
	}
	VL2_IIC_SCL=0;//时钟输出0 	   
	return 0;  
}

//产生ACK应答
void VL2_IIC_Ack(void)
{
	VL2_IIC_SCL=0;
	VL2_SDA_OUT();
	VL2_IIC_SDA=0;
	delay_us(VL2_SPEED);
	VL2_IIC_SCL=1;
	delay_us(VL2_SPEED);
	VL2_IIC_SCL=0;
}

//不产生ACK应答		    
void VL2_IIC_NAck(void)
{
	VL2_IIC_SCL=0;
	VL2_SDA_OUT();
	VL2_IIC_SDA=1;
	delay_us(VL2_SPEED);
	VL2_IIC_SCL=1;
	delay_us(VL2_SPEED);
	VL2_IIC_SCL=0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void VL2_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	VL2_SDA_OUT(); 	    
    VL2_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			VL2_IIC_SDA=1;
		else
			VL2_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(VL2_SPEED);  
		VL2_IIC_SCL=1;
		delay_us(VL2_SPEED); 
		VL2_IIC_SCL=0;	
		delay_us(VL2_SPEED);
    }	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 VL2_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	VL2_SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		VL2_IIC_SCL=0; 
		delay_us(VL2_SPEED*2);
	  VL2_IIC_SCL=1;
		receive<<=1;
		if(VL2_READ_SDA)receive++;   
	  delay_us(VL2_SPEED*2); //1
	}					 
	if (!ack)
		VL2_IIC_NAck();//发送nACK
	else
		VL2_IIC_Ack(); //发送ACK   
	return receive;
}

//IIC写一个字节数据
u8 VL2_IIC_Write_1Byte(u8 SlaveAddress, u8 REG_Address,u8 REG_data)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);
	if(VL2_IIC_Wait_Ack())
	{
		VL2_IIC_Stop();//释放总线
		return 1;//没应答则退出

	}
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();	
	VL2_IIC_Send_Byte(REG_data);
	VL2_IIC_Wait_Ack();	
	VL2_IIC_Stop();

	return 0;
}

//IIC读一个字节数据
u8 VL2_IIC_Read_1Byte(u8 SlaveAddress, u8 REG_Address,u8 *REG_data)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL2_IIC_Wait_Ack())
	{
		 VL2_IIC_Stop();//释放总线
		 return 1;//没应答则退出
	}		
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();
	VL2_IIC_Start(); 
	VL2_IIC_Send_Byte(SlaveAddress|0x01);//发读命令
	VL2_IIC_Wait_Ack();
	*REG_data = VL2_IIC_Read_Byte(0);
	VL2_IIC_Stop();

	return 0;
}

//IIC写n字节数据
u8 VL2_IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address,u16 len, u8 *buf)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL2_IIC_Wait_Ack()) 
	{
		VL2_IIC_Stop();//释放总线
		return 1;//没应答则退出
	}
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();
	while(len--)
	{
		VL2_IIC_Send_Byte(*buf++);//发送buff的数据
		VL2_IIC_Wait_Ack();	
	}
	VL2_IIC_Stop();//释放总线

	return 0;
	
}

//IIC读n字节数据
u8 VL2_IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address,u16 len,u8 *buf)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL2_IIC_Wait_Ack()) 
	{
		VL2_IIC_Stop();//释放总线
		return 1;//没应答则退出
	}
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();

	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress|0x01);//发读命令
	VL2_IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
		{
			*buf = VL2_IIC_Read_Byte(0);
		}
		else
		{
			*buf = VL2_IIC_Read_Byte(1);
		}
		buf++;
		len--;
	}
	VL2_IIC_Stop();//释放总线

	return 0;
	
}

/**************************************以下是应用层调用的函数******************************************************/


//写多个数据
//address:地址
//index:偏移地址
//pdata:数据指针
//count:长度 最大65535
u8 I2C_SAND_BUFFER(u8 address, u8 index,u8 *pdata,u16 count)
{
	u8 status = STATUS_OK;

	if(VL2_IIC_Write_nByte(address,index,count,pdata))
	{
	   status  = STATUS_FAIL;

	}

	return status;
}


//读多个数据
//address:地址
//index:偏移地址
//pdata:数据指针
//count:长度 最大65535
u8 I2C_READ_BUFFER(u8 address,u8 index,u8 *pdata,u16 count)
{
	u8 status = STATUS_OK;

	if(VL2_IIC_Read_nByte(address,index,count,pdata))
	{
	  status  = STATUS_FAIL;
	}

	return status;
}

//写1个数据(单字节)
//address:地址
//index:偏移地址
//data:数据(8位)
u8 I2C_SAND_BYTE(u8 address,u8 index,u8 data)
{
	u8 status = STATUS_OK;

	status = I2C_SAND_BUFFER(address,index,&data,1);

	return status;
}
 
/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/


/*
选择IO接口工作方式：
GPIO_Mode_AIN 模拟输入
GPIO_Mode_IN_FLOATING 浮空输入
GPIO_Mode_IPD 下拉输入
GPIO_Mode_IPU 上拉输入
GPIO_Mode_Out_PP 推挽输出
GPIO_Mode_Out_OD 开漏输出
GPIO_Mode_AF_PP 复用推挽输出
GPIO_Mode_AF_OD 复用开漏输出
*/
