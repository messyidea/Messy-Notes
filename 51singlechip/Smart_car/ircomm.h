/*----------------------------------------
ircomm
-----------------------------------------*/

#ifndef _IRCOMM_H_
#define _IRCOMM_H_

#define uchar unsigned char
#define uint unsigned int

//初始化IRCOMM
void IR_init();
//一位一位接收数据
void IRcordpro();
//判断接收到的是什么，保存在DataPort
void IRword();
//延时
void delay(uint z);
//通过红外控制小车
void control();

#endif