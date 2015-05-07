/*--------------------------------------------
这是软件模拟IIC总线通讯的函数库
时间：2015-5-7 19:46:06
包括：
	初始化： I2C_Init
	起始信号： I2C_Start
---------------------------------------------*/
#ifndef _I2C_H_
#define _I2C_H_

void I2C_Init();
void I2C_Start();
void I2C_Stop();
void I2C_Send_Byte(unsigned char dat);	//发送一个字节
unsigned char I2C_Read_Byte();		//接收一个字节

bit Check_Ack();	//检查从机应答
void Master_Ack(bit i);		//主机应答

#endif