#include <reg52.h>

#define LCD1602_DB  P0

sbit LCD1602_RS = P3^5;
sbit LCD1602_RW = P3^6;
sbit LCD1602_E = P3^4;

void InitLcd1602();
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
void main()
{
	unsigned char str[] = "U R MySunshine";

	InitLcd1602();
	LcdShowStr(2, 0, str);
	LcdShowStr(0, 1, "My Only Sunshine");
	while(1);		
}

void LcdWaitReady()
{
	unsigned char sta;

	LCD1602_DB = 0xFF;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do	{
		LCD1602_E = 1;
		sta = LCD1602_DB;
		LCD1602_E = 0;
	}while(sta & 0x80);
}

void LcdWriteCmd(unsigned char cmd)
{
	LcdWaitReady();
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 1;
	LCD1602_E = 0;
}
void LcdWriteDat(unsigned char dat)
{
	LcdWaitReady();
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_E = 1;
	LCD1602_E = 0;
}
void LcdSetCursor(unsigned char x, unsigned char y)
{
	unsigned char addr;
	if(y == 0)
		addr = 0x00 + x;
	else
		addr = 0x40 + x;
	LcdWriteCmd(addr | 0x80);
}
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str)
{
	LcdSetCursor(x, y);
	while(*str != '\0')
	{
		LcdWriteDat(*str++);   
	}
}
void InitLcd1602()
{
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0C);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x01);
}