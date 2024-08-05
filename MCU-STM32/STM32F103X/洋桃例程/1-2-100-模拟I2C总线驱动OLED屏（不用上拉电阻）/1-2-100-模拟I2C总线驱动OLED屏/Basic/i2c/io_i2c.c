

/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1-201708202309 ������


*/

//ģ��I2C������������//

#include "delay.h"
#include "io_i2c.h"



//I2C��ʼ��
void I2C_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;  //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //50Mhz�ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);//PA7,PA6 �����	

}

//����IIC��ʼ�ź�
void VL2_IIC_Start(void)
{
	VL2_SDA_OUT();//sda�����
	VL2_IIC_SDA=1;	  	  
	VL2_IIC_SCL=1;
	delay_us(VL2_SPEED*2);
 	VL2_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(VL2_SPEED*2);
	VL2_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
void VL2_IIC_Stop(void)
{
	VL2_SDA_OUT();//sda�����
	VL2_IIC_SCL=0;
	VL2_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(VL2_SPEED*2);
	VL2_IIC_SCL=1; 
	VL2_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(VL2_SPEED*2);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 VL2_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	VL2_SDA_IN();  //SDA����Ϊ����  
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
	VL2_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}

//����ACKӦ��
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

//������ACKӦ��		    
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

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void VL2_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	VL2_SDA_OUT(); 	    
    VL2_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 VL2_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	VL2_SDA_IN();//SDA����Ϊ����
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
		VL2_IIC_NAck();//����nACK
	else
		VL2_IIC_Ack(); //����ACK   
	return receive;
}

//IICдһ���ֽ�����
u8 VL2_IIC_Write_1Byte(u8 SlaveAddress, u8 REG_Address,u8 REG_data)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);
	if(VL2_IIC_Wait_Ack())
	{
		VL2_IIC_Stop();//�ͷ�����
		return 1;//ûӦ�����˳�

	}
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();	
	VL2_IIC_Send_Byte(REG_data);
	VL2_IIC_Wait_Ack();	
	VL2_IIC_Stop();

	return 0;
}

//IIC��һ���ֽ�����
u8 VL2_IIC_Read_1Byte(u8 SlaveAddress, u8 REG_Address,u8 *REG_data)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);//��д����
	if(VL2_IIC_Wait_Ack())
	{
		 VL2_IIC_Stop();//�ͷ�����
		 return 1;//ûӦ�����˳�
	}		
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();
	VL2_IIC_Start(); 
	VL2_IIC_Send_Byte(SlaveAddress|0x01);//��������
	VL2_IIC_Wait_Ack();
	*REG_data = VL2_IIC_Read_Byte(0);
	VL2_IIC_Stop();

	return 0;
}

//IICдn�ֽ�����
u8 VL2_IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address,u16 len, u8 *buf)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);//��д����
	if(VL2_IIC_Wait_Ack()) 
	{
		VL2_IIC_Stop();//�ͷ�����
		return 1;//ûӦ�����˳�
	}
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();
	while(len--)
	{
		VL2_IIC_Send_Byte(*buf++);//����buff������
		VL2_IIC_Wait_Ack();	
	}
	VL2_IIC_Stop();//�ͷ�����

	return 0;
	
}

//IIC��n�ֽ�����
u8 VL2_IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address,u16 len,u8 *buf)
{
	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress);//��д����
	if(VL2_IIC_Wait_Ack()) 
	{
		VL2_IIC_Stop();//�ͷ�����
		return 1;//ûӦ�����˳�
	}
	VL2_IIC_Send_Byte(REG_Address);
	VL2_IIC_Wait_Ack();

	VL2_IIC_Start();
	VL2_IIC_Send_Byte(SlaveAddress|0x01);//��������
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
	VL2_IIC_Stop();//�ͷ�����

	return 0;
	
}

/**************************************������Ӧ�ò���õĺ���******************************************************/


//д�������
//address:��ַ
//index:ƫ�Ƶ�ַ
//pdata:����ָ��
//count:���� ���65535
u8 I2C_SAND_BUFFER(u8 address, u8 index,u8 *pdata,u16 count)
{
	u8 status = STATUS_OK;

	if(VL2_IIC_Write_nByte(address,index,count,pdata))
	{
	   status  = STATUS_FAIL;

	}

	return status;
}


//���������
//address:��ַ
//index:ƫ�Ƶ�ַ
//pdata:����ָ��
//count:���� ���65535
u8 I2C_READ_BUFFER(u8 address,u8 index,u8 *pdata,u16 count)
{
	u8 status = STATUS_OK;

	if(VL2_IIC_Read_nByte(address,index,count,pdata))
	{
	  status  = STATUS_FAIL;
	}

	return status;
}

//д1������(���ֽ�)
//address:��ַ
//index:ƫ�Ƶ�ַ
//data:����(8λ)
u8 I2C_SAND_BYTE(u8 address,u8 index,u8 data)
{
	u8 status = STATUS_OK;

	status = I2C_SAND_BUFFER(address,index,&data,1);

	return status;
}
 
/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/


/*
ѡ��IO�ӿڹ�����ʽ��
GPIO_Mode_AIN ģ������
GPIO_Mode_IN_FLOATING ��������
GPIO_Mode_IPD ��������
GPIO_Mode_IPU ��������
GPIO_Mode_Out_PP �������
GPIO_Mode_Out_OD ��©���
GPIO_Mode_AF_PP �����������
GPIO_Mode_AF_OD ���ÿ�©���
*/
