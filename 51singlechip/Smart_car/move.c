#include "move.h"
#include <reg52.h>

//不能够直接P1^0=1操作
//因为P1^0默认是一个常量，常量不能被赋值
sbit P1_0 = P1^0;
sbit P1_1 = P1^1;
sbit P1_2 = P1^2;
sbit P1_3 = P1^3;
sbit P1_4 = P1^4;
sbit P1_5 = P1^5;
sbit P1_6 = P1^6;
sbit P1_7 = P1^7;

void Left_go()
{
	P1_0=1;P1_1=0;P1_2=1;P1_3=0;
}

void Left_back()
{
	P1_0=0;P1_1=1;P1_2=0;P1_3=1;
}

void Left_stop()
{
	P1_0=0;P1_1=0;P1_2=0;P1_3=0;
}

void Right_go()
{
	P1_4=1;P1_5=0;P1_6=1;P1_7=0;
}

void Right_back()
{
	P1_4=0;P1_5=1;P1_6=0;P1_7=1;
}

void Right_stop()
{
	P1_4=0;P1_5=0;P1_6=0;P1_7=0;
}

void run(void)
{
	Left_go() ;   
	Right_go() ;
}

void backrun(void)
{    
	Left_back() ; 
	Right_back() ;
}

void leftrun(void)
{
	Left_back();
	Right_go();
}

void rightrun(void)
{
	 Left_go() ; 
	 Right_back();
}

void stop(void) {
	Left_stop();
	Right_stop();
}
