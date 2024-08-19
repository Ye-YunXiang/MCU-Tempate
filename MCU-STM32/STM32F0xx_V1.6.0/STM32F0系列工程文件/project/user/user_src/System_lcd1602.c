/******************************************************************************
* ProjectName: 	GSM koolboks battery controler version1 projection
* FileName: 	system_lcd1602.c 
* Description:	Initialize LCD1602
* 
* Author:		Soild_Alasong
* Date: 	 	2022/09/01
* Notes: 
* -----------------------------------------------------------------
* Histroy: V1.0 2023/12/25, Alasong create this file
* CPU:HC32L176KATA-LQFP64  FLASH:64K,128ҳÿҳ512�ֽ�
******************************************************************************/
#include "system_lcd1602.h"


/********************************************************************
 ** \brief  lcd��ʾ�ַ�
 ** @param  ��Ҫ��ʾ���ַ�
 ** @param  ��ʾ����Ļλ��
 ** \retval ��
********************************************************************/
static void LCD_Dispaly_Write_CMD_OR_Data(uint8_t Cmdordata,uint8_t Writetype)
{
	if(Writetype==0)
	{
		Gpio_ClrIO(LCDRS_PORT,LCDRS_PIN);		//write command
	}
	else
	{
		Gpio_SetIO(LCDRS_PORT,LCDRS_PIN);			//write data
	}
//	PORT_SetBits(PortA,cmdordata&0x00ff);								// 8 ������
//	PORT_ResetBits(PortA,(~cmdordata)&0x00ff);					// 8 ������
	Gpio_SetPort(GpioPortA,(Cmdordata&0xf0)&0x00f0);			// 4 ������
	Gpio_ClrPort(GpioPortA,((~Cmdordata)&0xf0)&0x00f0);		// 4 ������
	delay1ms(5);
	Gpio_SetIO(LCDEN_PORT,LCDEN_PIN);
	delay1ms(5);
	Gpio_ClrIO(LCDEN_PORT,LCDEN_PIN);
		
	Gpio_SetPort(GpioPortA,((Cmdordata&0x0f)<<4)&0x00f0);			// 4 ������
	Gpio_ClrPort(GpioPortA,(((~Cmdordata)&0x0f)<<4)&0x00f0);	// 4 ������
	delay1ms(5);
	Gpio_SetIO(LCDEN_PORT,LCDEN_PIN);
	delay1ms(5);
	Gpio_ClrIO(LCDEN_PORT,LCDEN_PIN);
}


/********************************************************************
 ** \brief  lcd���ų�ʼ��
 ** \retval ��
********************************************************************/
void system_lcd1602_display_port_init(void)
{
	stc_gpio_cfg_t Stc_LCD_Port_Init;
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
		Stc_LCD_Port_Init.enDir = GpioDirOut;
//		Stc_LCD_Port_Init.enDrv = GpioDrvL;
		Stc_LCD_Port_Init.enPu = GpioPuDisable;
		Stc_LCD_Port_Init.enPd = GpioPdDisable;
//		Stc_LCD_Port_Init.enOD = GpioOdDisable;
//		Stc_LCD_Port_Init.enCtrlMode = GpioAHB;
	

  	Gpio_Init(LCDRS_PORT, LCDRS_PIN, &Stc_LCD_Port_Init);
	Gpio_Init(LCDEN_PORT, LCDEN_PIN, &Stc_LCD_Port_Init);		
	Gpio_Init(LCDRW_PORT, LCDRW_PIN, &Stc_LCD_Port_Init);
	
	Gpio_Init(LCDD4_PORT, LCDD4_PIN, &Stc_LCD_Port_Init);
	Gpio_Init(LCDD5_PORT, LCDD5_PIN, &Stc_LCD_Port_Init);
	Gpio_Init(LCDD6_PORT, LCDD6_PIN, &Stc_LCD_Port_Init);
	Gpio_Init(LCDD7_PORT, LCDD7_PIN, &Stc_LCD_Port_Init);

	Gpio_ClrIO(LCDD4_PORT,LCDD4_PIN);
	Gpio_ClrIO(LCDD5_PORT,LCDD5_PIN);
	Gpio_ClrIO(LCDD6_PORT,LCDD6_PIN);
	Gpio_ClrIO(LCDD7_PORT,LCDD7_PIN);
		
	Gpio_Init(LCD_LIGHT_PORT,LCD_LIGHT_PIN ,&Stc_LCD_Port_Init);
	System_LCD_Light_Disable();

}


/********************************************************************
 ** \brief  LCD1602��ʼ��
 ** \retval ��
********************************************************************/
void system_lcd1602_dispaly_config_init(void)
{
	//delay1ms(200);
	delay1ms(5);
	LCD_Dispaly_Write_CMD_OR_Data(0x30,0);
	delay1ms(5);
	LCD_Dispaly_Write_CMD_OR_Data(0x30,0);
	delay1ms(5);
	LCD_Dispaly_Write_CMD_OR_Data(0x30,0);
	delay1ms(5);
	LCD_Dispaly_Write_CMD_OR_Data(0x28,0);//����16*2��ʾ��5*7����4λ���ݽӿ�
	delay1ms(5);
	Gpio_SetIO(LCDEN_PORT,LCDEN_PIN);
	delay1ms(5);
	Gpio_ClrIO(LCDEN_PORT,LCDEN_PIN);
	delay1ms(5);
	LCD_Dispaly_Write_CMD_OR_Data(0x28,0);//����16*2��ʾ��5*7����4λ���ݽӿ�
	delay1ms(5);
	LCD_Dispaly_Write_CMD_OR_Data(0x0C,0);//���ÿ���ʾ������ʾ���
	LCD_Dispaly_Write_CMD_OR_Data(0x06,0);//д�ַ����ַ�Զ���1
	LCD_Dispaly_Write_CMD_OR_Data(0x01,0);//��ʾ��0������ָ����0
	delay1ms(5);
}


/********************************************************************
 ** \brief	lcd��ʾ�ַ���
 ** @param  �ַ���ָ��
 ** @param	�ַ�������
 ** @param	��ʾ����ʾ��ʼλ��
 ** \retval ��
********************************************************************/
void system_lcd1602_string_dispaly(char *Lcd_string,uint8_t String_len, uint8_t Row_num)
{
	LCD_Dispaly_Write_CMD_OR_Data(Row_num,0);
	for(uint8_t i = 0; i < String_len; i++)
	{
		LCD_Dispaly_Write_CMD_OR_Data(*Lcd_string,1);
		Lcd_string++;
		delay1ms(20);
	}
}

/********************************************************************
 ** \brief  ��������ʱ��ʾ
 ** @param  char�ַ�ָ��
 ** @param  ��ʾ�ڵڼ�λ
 ** @param	��ʾ���ͣ�0x00,0x23,0x2A
 ** \retval ��
********************************************************************/
void system_lcd1602_token_data_display(char * Openpaygo_Token, uint8_t * Token_Num, uint8_t Dispalay_Type)
{
	if(Dispalay_Type == 0x00)
	{
		LCD_Dispaly_Write_CMD_OR_Data(0x81 + *Token_Num, 0);				
		LCD_Dispaly_Write_CMD_OR_Data(*Openpaygo_Token + 0x30, 1);
	}
	else if(Dispalay_Type == 0x23)
	{
		LCD_Dispaly_Write_CMD_OR_Data(0x81 + *Token_Num, 0);				
		LCD_Dispaly_Write_CMD_OR_Data(0x23, 1);
	}
	else if(Dispalay_Type == 0x2A)
	{
		LCD_Dispaly_Write_CMD_OR_Data(0x80 + *Token_Num, 0);				
		LCD_Dispaly_Write_CMD_OR_Data(0x2A, 1);
	}
	else 
	{;}
}


/********************************************************************
 ** \brief  �����Ļ��ʾ
 ** \retval ��
********************************************************************/
void system_lcd1602_display_clear_dispaly(void)
{
	LCD_Dispaly_Write_CMD_OR_Data(0x01,0);//��ʾ��0������ָ����0
}
