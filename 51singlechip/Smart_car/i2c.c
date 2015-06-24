#include <reg52.h>
#include <intrins.h>
#include "i2c.h"

#define AT24C02_ADDR 0XA0
sbit SCL = P2^1;
sbit SDA = P2^0;

void delay5us()
{
	_nop_();
	_nop_();
}


/*I2C初始化*/
void I2C_Init()	
{
	SDA = 1;
	_nop_();
	SCL = 1;
	_nop_();
}

/*I2C起始信号*/
void I2C_Start()  
{
	SCL = 1;
	_nop_();
	SDA = 1;
	delay5us();
	SDA = 0;
	delay5us();
	SCL = 0;
}

/*I2C终止信号*/
void I2C_Stop()
{
	SDA = 0;
	_nop_();
	SCL = 1;
	delay5us();
	SDA = 1;
	delay5us();
}

/*主机发送应答*/
void Master_ACK(bit i)		
{
	SCL = 0; // 拉低时钟总线允许SDA数据总线上的数据变化
	_nop_(); // 让总线稳定
	if (i)	 //如果i = 1 那么拉低数据总线 表示主机应答
	{
		SDA = 0;
	}
	else	 
	{
		SDA = 1;	 //发送非应答
	}
	_nop_();//让总线稳定
	SCL = 1;//拉高时钟总线 让从机从SDA线上读走 主机的应答信号
	delay5us();
	SCL = 0;//拉低时钟总线， 占用总线继续通信
	_nop_();
	SDA = 1;//释放SDA数据总线。
	_nop_();
}

/*检测从机应答*/
bit Check_ACK()
{
	SCL = 1;
	delay5us();
	if (SDA)
	{
		SCL = 0;
		_nop_();
		I2C_Stop();
		return(0);
	}
	else
	{
		SCL = 0;
		_nop_();
		return(1);
	}
}

/*发送一个字节*/
void I2C_Send_byte(unsigned char dat)
{
	unsigned char i;
	for(i = 0 ; i < 8 ; i++)
	{
		SCL = 0;
		_nop_();
		if (dat & 0x80)
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
		dat= dat<<1;	//  
	}
	SCL = 0;
	_nop_();
	SDA = 1;
	_nop_();
}


/*I2C 读一字节*/
unsigned char I2C_Read_Byte()
{
	unsigned char dat,i;
	SCL = 0;
	_nop_();
	SDA = 1;
	_nop_();
	for(i = 0 ; i < 8 ; i++)
	{
		dat = dat<<1;
		SCL = 1;
		_nop_();
		if (SDA)			    
		{
			 dat |= 0x01; 
		}
		else
		{
			 dat &=  0xfe;	
		}
		_nop_();
		SCL = 0 ;
		_nop_();
		/*if(i < 7)
		{
			dat = dat << 1;	
		}*/
	}
	return(dat);
}


/*I2C发送数据,将上面的几个函数进一步封装*/
bit I2C_TransmitData(unsigned char ADDR, DAT)
{
	I2C_Start();
	I2C_Send_Byte(AT24C02_ADDR+0);
	if (!Check_ACK())
	{
		return(0);
	}
	I2C_Send_Byte(ADDR); 
	if (!Check_ACK())
	{
		return(0);
	}
	I2C_Send_Byte(DAT);
	if (!Check_ACK())
	{
		return(0);
	}
	I2C_Stop();
	return(1);	
}

/*I2C接收数据*/
unsigned char I2C_ReceiveData(unsigned char ADDR)
{
	unsigned char DAT;
	I2C_Start();
	I2C_Send_Byte(AT24C02_ADDR+0);
	if (!Check_ACK())
	{
		return(0);
	}
	I2C_Send_Byte(ADDR);
	Master_ACK(0);
	I2C_Start();
	I2C_Send_Byte(AT24C02_ADDR+1);
	if (!Check_ACK())
	{
		return(0);
	}
	DAT = I2C_Read_Byte();
	Master_ACK(0);
	I2C_Stop();
	return(DAT);	
}