#include <reg52.h>
#include "move.h"
#include "ircomm.h"
#include "lcd.h"
#include "speed.h"
#include "i2c.h"

//现在的模式
unsigned char now;
//之前的模式
unsigned char pre;
void main()
{
	//delay(1000000);
	//T2定时器初始化,有时候会初始化缓慢
	T2Init();
	//LCD初始化
	InitLcd1602();
	//I2C初始化
	I2C_Init();
	pre = 0; now = 0;
	I2C_TransmitData(0x08,0);
	//开始红外控制
	control();
}