#include <reg52.h>
#include "i2c.h"
#include "common_func.h"
#include "lcd1602.h"

void main()
{
	bit SendFlag;
	P1 = 0XFF;
	I2C_Init();
	//写一个字节数据到EEPROM的08位置，写入的内容为0x0f;
	I2C_Start();
	I2C_Send_Byte(0xA0 + 0);
	if(Check_Ack())
	{
		SendFlag = 1;
	}
	I2C_Send_Byte(0x08);
	if(Check_Ack())
	{
		SendFlag = 1;
	}
	I2C_Send_Byte(0x0f);
	if(Check_Ack())
	{
		SendFlag = 1;
	}
	I2C_Stop();
	
	//把eeprom的数据读出来
	I2C_Start();
	I2C_Send_Byte(0xA0 + 0);
	if(Check_Ack())
	{
		SendFlag = 1;
	}
	I2C_Send_Byte(0x08);
	if(Check_Ack())
	{
		SendFlag = 1;
	}	
	
	I2C_Start();
	I2C_Send_Byte(0xA0 + 1);
	if(Check_Ack())
	{
		SendFlag = 1;
	}
	P1 = I2C_Read_Byte();
	Master_Ack(0);
	I2C_Stop();
	
	while(1);
}