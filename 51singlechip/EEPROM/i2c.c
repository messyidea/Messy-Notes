#include <intrins.h>
#include <reg52.h>
#include "i2c.h"
#include "common_func.h"

sbit SCL = P2^1;
sbit SDA = P2^0;

void I2C_Init()
{
	SCL = 1;
	_nop_();		//等待一个机器周期
	SDA = 1;
	_nop_();
}

void I2C_Start()
{
	SCL = 1;
	delay5us();
	SDA = 1;
	_nop_();
	SDA = 0;
	delay5us();
	
}

void I2C_Stop()
{
	SCL = 1;
	delay5us();
	SDA = 0;
	_nop_();
	SDA = 1;
	delay5us();
}

void I2C_Send_Byte(unsigned char dat)
{
	unsigned char i;
	
	for(i = 0;i < 8; i++) 
	{
		SCL = 0;		//允许sda变化
		_nop_();
		if(dat&0x80)
		{
			SDA = 1;
			_nop_();
		}
		else
		{
			SDA = 0;
			_nop_();
		}
		SCL = 1;
		_nop_();
		dat <<= 1;
	}
	SCL = 0;
	_nop_();
	
	SDA = 1;
	_nop_();
	
	
}


unsigned char I2C_Read_Byte()
{
	unsigned char a ,dat = 0;
	
	for(a = 0;a < 8; a++)
	{
		SCL = 1;
		_nop_();
		dat <<= 1;
		if(SDA)
		{
			dat = dat | 0x01;
		}
		else
		{
			dat = dat & 0xfe;
		}
		_nop_();
		SCL = 0;
		_nop_();
		
	}
	return (dat);
}

bit Check_Ack()
{
	SCL = 1;
	delay5us();
	
	if(SDA) 	//1的话非应答
	{
		SCL = 0;
		_nop_();
		I2C_Stop();
		return (0);			//转换成我们容易理解的逻辑
	}
	else
	{
		SCL = 0;
		_nop_();
		return (1);
	}
}

void Master_Ack(bit i)
{
	SCL = 0;
	_nop_();
	
	if(i)
	{
		SDA = 0;
	}
	else
	{
		SDA = 1;
	}
	_nop_();
	
	SCL = 1;
	delay5us();
	SCL = 0;
	_nop_();
	
	SDA = 1;	//释放总线
	_nop_();
}