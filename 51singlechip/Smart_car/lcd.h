/*----------------------------------------
LCD控制
unsigned char str[] = "U R MySunshine";

InitLcd1602();
LcdShowStr(2, 0, str);
LcdShowStr(0, 1, "My Only Sunshine");
------------------------------------------*/
#ifndef _LCD_H_
#define _LCD_H_

/* 等待液晶准备好 */
void LcdWaitReady();
/* 向LCD1602液晶写入一字节命令，cmd-待写入命令值 */
void LcdWriteCmd(unsigned char cmd);
/* 向LCD1602液晶写入一字节数据，dat-待写入数据值 */
void LcdWriteDat(unsigned char dat);
/* 设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
void LcdSetCursor(unsigned char x, unsigned char y);
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
/* 区域清除，清除从(x,y)坐标起始的len个字符位 */
void LcdAreaClear(unsigned char x, unsigned char y, unsigned char len);
/* 初始化1602液晶 */
void InitLcd1602();
//清空屏幕
void LcdAllClear();



#endif