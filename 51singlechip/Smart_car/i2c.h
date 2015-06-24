/* -----------------------------
I2C
 ------------------------------------ */
#ifndef _I2C_H_
#define _I2C_H_

void delay5us();
void I2C_Init();  //iic初始化
void I2C_Start(); //iic起始信号
void I2C_Stop();  //iic停止信号
void I2C_Send_Byte(unsigned char dat);  //发送一个字节的数据
unsigned char I2C_Read_Byte();  //读取一个字节的数据

bit Check_ACK();  //检查从机应答
void Master_ACK(bit i);  //主机应答

unsigned char I2C_ReceiveData(unsigned char ADDR);
bit I2C_TransmitData(unsigned char ADDR, DAT);
#endif